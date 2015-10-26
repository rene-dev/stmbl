#include "stm32f10x_conf.h"
#include "common.h"
#include <math.h>

//iramx v31 hardware
#define AREF 3.3// analog reference voltage
#define ARES 4096.0// analog resolution, 12 bit
#define RCUR 0.0181//shunt
#define TPULLUP 10000//iramx temperature pullup
#define R10 10000
#define R11 180

#ifdef TROLLER

#define ADC_channels 4
#define VDIVUP 1000000.0
#define VDIVDOWN 4990.0
#define PWM_U TIM1->CCR3
#define PWM_V TIM1->CCR2
#define PWM_W TIM1->CCR1
#define ACS_VPA 0.066 // ACS712 V/A
#define ACS_UP 2000.0
#define ACS_DOWN 3000.0
#define ACS_OFFSET 2.5
#define MIN(a, b)  (((a) < (b)) ? (a) : (b))

#define AMP(a) ((a * AREF / ARES * (ACS_DOWN + ACS_UP) / ACS_DOWN - ACS_OFFSET) / ACS_VPA)

#else

#define ADC_channels 3
#define VDIVUP 36000.0//HV div pullup R1,R12
#define VDIVDOWN 280.0//HV div pulldown R2,R9
#define PWM_U TIM1->CCR1
#define PWM_V TIM1->CCR2
#define PWM_W TIM1->CCR3

#define AMP(a) ((a * AREF / ARES - AREF / (R10 + R11) * R11) / (RCUR * R10) * (R10 + R11))

#endif

#define VOLT(a) (a / ARES * AREF / VDIVDOWN * (VDIVUP + VDIVDOWN))
#define TEMP(a) (log10f(a * AREF / ARES * TPULLUP / (AREF - a * AREF / ARES)) * (-53) + 290)

__IO uint16_t ADCConvertedValue[ADC_channels];//DMA buffer for ADC

#define SQRT3 1.732050808

#ifndef M_PI
#define M_PI		3.14159265358979323846
#define M_TWOPI         (M_PI * 2.0)
#endif

#define TEMP_RES 32
#define SCALE (TEMP_RES / ARES)
int16_t temp_buf[TEMP_RES];

float tempb(float i){
   unsigned int x = (int)(i * SCALE);
   float a = TOFLOAT(temp_buf[x]);
   float b = TOFLOAT(temp_buf[x + 1]);
   return(a + (b - a) * (i * SCALE - x));
}

volatile uint32_t timeout = 99999;
volatile uint16_t volt_raw = 0;
volatile uint16_t amp_raw = 0;
volatile uint16_t temp_raw = 0;

float volt = 0;
float amp = 0;
float temp = 0;

volatile unsigned int systime = 0;
volatile float u,v,w;
volatile int uartsend = 0;

uint16_t buf = 0x0000;
packet_to_hv_t packet_to_hv;
packet_from_hv_t packet_from_hv;
int32_t datapos = -1;

TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef USART_InitStruct;
ADC_InitTypeDef ADC_InitStructure;
DMA_InitTypeDef DMA_InitStructure;

void Wait(unsigned int ms){
   volatile unsigned int t = systime + ms;
   while(t >= systime){
   }
}

void SysTick_Handler(void)
{
   systime++;
}

void RCC_Configuration(void)
{
   ErrorStatus HSEStartUpStatus;
   RCC_DeInit();
   RCC_HSEConfig(RCC_HSE_ON);
   HSEStartUpStatus = RCC_WaitForHSEStartUp();

   if(HSEStartUpStatus == SUCCESS){
      FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
      FLASH_SetLatency(FLASH_Latency_2);
      RCC_HCLKConfig(RCC_SYSCLK_Div1);
      RCC_PCLK2Config(RCC_HCLK_Div1);
      RCC_PCLK1Config(RCC_HCLK_Div2);
      RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
      RCC_PLLCmd(ENABLE);
      while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
      RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
      while(RCC_GetSYSCLKSource() != 0x08){}
   }

   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
}

void GPIO_Configuration(void)
{
   //LED init
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
   GPIO_Init(GPIOC, &GPIO_InitStructure);

#ifdef TROLLER
   //shutdown pins
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
   GPIO_Init(GPIOB, &GPIO_InitStructure);
   GPIO_SetBits(GPIOB,GPIO_Pin_1);
   GPIO_SetBits(GPIOB,GPIO_Pin_2);
   GPIO_SetBits(GPIOB,GPIO_Pin_3);
   // GPIO_ResetBits(GPIOB,GPIO_Pin_1);
   // GPIO_ResetBits(GPIOB,GPIO_Pin_2);
   // GPIO_ResetBits(GPIOB,GPIO_Pin_3);
   //GPIO_ResetBits(GPIOC,GPIO_Pin_2);//greep led off
#else
   //Enable output
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
   GPIO_Init(GPIOB, &GPIO_InitStructure);
   //Fault in
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
   GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif
}

void tim1_init(){
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

   //TIM1
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOA, &GPIO_InitStructure);
   //TIM1 N
	// not needed for troller
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOB, &GPIO_InitStructure);

   /* Channel 1, 2 and 3 Configuration in PWM mode */
   TIM_TimeBaseStructure.TIM_Period = PWM_RES;
   TIM_TimeBaseStructure.TIM_Prescaler = 0;
   TIM_TimeBaseStructure.TIM_ClockDivision = 0;
   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned3;
   TIM_TimeBaseStructure.TIM_RepetitionCounter = 1;
   TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
   TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
   //TIM_SelectOutputTrigger(TIM1, TIM_TRGOSource_Update);

   /* int NVIC setup */
   NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;

   NVIC_Init(&NVIC_InitStructure);

   TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	//not needed for troller
   TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
   TIM_OCInitStructure.TIM_Pulse = 0;
   TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
   TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
   TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
   TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

   TIM_OC1Init(TIM1, &TIM_OCInitStructure);
   TIM_OC2Init(TIM1, &TIM_OCInitStructure);
   TIM_OC3Init(TIM1, &TIM_OCInitStructure);

   TIM_Cmd(TIM1, ENABLE);

   TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

void usart_init(){
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
   RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

   DMA_DeInit(DMA1_Channel7);
   DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART2->DR;
   DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&packet_from_hv;
   DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
   DMA_InitStructure.DMA_BufferSize = sizeof(packet_from_hv_t);
   DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
   DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
   DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
   DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
   DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
   DMA_InitStructure.DMA_Priority = DMA_Priority_High;
   DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
   DMA_Init(DMA1_Channel7, &DMA_InitStructure);

   //USART TX
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
   GPIO_Init(GPIOA, &GPIO_InitStructure);

   //USART RX
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
   GPIO_Init(GPIOA, &GPIO_InitStructure);

   USART_InitStruct.USART_BaudRate = DATABAUD;
   USART_InitStruct.USART_WordLength = USART_WordLength_8b;
   USART_InitStruct.USART_StopBits = USART_StopBits_1;
   USART_InitStruct.USART_Parity = USART_Parity_No;
   USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
   USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

   USART_Init(USART2, &USART_InitStruct);
   USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);
   USART_Cmd(USART2, ENABLE);

   USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//Enable USART RX not empty interrupt

   NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);

   /* Enable the USART2 */
}

// Setup ADC
void setup_adc(){
   RCC_ADCCLKConfig(RCC_PCLK2_Div4);
   /* Enable DMA1 clock */
   RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
   /* Enable ADC1 and GPIOC clock */
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);

   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;

#ifdef TROLLER
	//PINA0 IN0 DC link
	//PINA6 IN6 iu
	//PINA7 IN7 iv
	//PINB0 IN8 iw

	//PINA5 IN5 uu
	//PINA4 IN4 uv
	//PINA1 IN1 uw
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_6 | GPIO_Pin_7;
   GPIO_Init(GPIOA, &GPIO_InitStructure);
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
   GPIO_Init(GPIOB, &GPIO_InitStructure);
#else
	//PINC5 IN15 DC link
	//PINC4 IN14 AMP
	//PINB0 IN8 temperature
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
   GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif

   DMA_DeInit(DMA1_Channel1);
   DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
   DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADCConvertedValue;
   DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
   DMA_InitStructure.DMA_BufferSize = ADC_channels;
   DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
   DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
   DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
   DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
   DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
   DMA_InitStructure.DMA_Priority = DMA_Priority_High;
   DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
   DMA_Init(DMA1_Channel1, &DMA_InitStructure);

   NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);

   /* Enable DMA1 channel1 */
   DMA_Cmd(DMA1_Channel1, ENABLE);
   DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);

   /* ADC1 configuration ------------------------------------------------------*/
   ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
   ADC_InitStructure.ADC_ScanConvMode = ENABLE;
   ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
   ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
   ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
   ADC_InitStructure.ADC_NbrOfChannel = ADC_channels;
   ADC_Init(ADC1, &ADC_InitStructure);

   ADC_TempSensorVrefintCmd(ENABLE);

#ifdef TROLLER
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_13Cycles5); //volt
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 2, ADC_SampleTime_13Cycles5); //iu
	ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 3, ADC_SampleTime_13Cycles5); //iv
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 4, ADC_SampleTime_13Cycles5); //iw
#else
	ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 1, ADC_SampleTime_13Cycles5); //volt
   ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 2, ADC_SampleTime_13Cycles5); //amp
   ADC_RegularChannelConfig(ADC1, ADC_Channel_8 , 3, ADC_SampleTime_13Cycles5); //temp
#endif

   /* Enable ADC1 DMA */
   ADC_DMACmd(ADC1, ENABLE);

   /* Enable ADC1 */
   ADC_Cmd(ADC1, ENABLE);

   /* Enable ADC1 reset calibration register */
   ADC_ResetCalibration(ADC1);
   /* Check the end of ADC1 reset calibration register */
   while(ADC_GetResetCalibrationStatus(ADC1));

   /* Start ADC1 calibration */
   ADC_StartCalibration(ADC1);
   /* Check the end of ADC1 calibration */
   while(ADC_GetCalibrationStatus(ADC1));
}

//TIM1 update interrupt, every PWM cycle
void TIM1_UP_IRQHandler(){
   TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
   ADC_SoftwareStartConvCmd(ADC1, ENABLE);   //trigger ADC
   if(timeout > 30){
      GPIO_ResetBits(GPIOB,GPIO_Pin_6);   //disable driver
      //GPIO_SetBits(GPIOC,GPIO_Pin_1);  //yellow led on
      //GPIO_ResetBits(GPIOC,GPIO_Pin_2);   //green led off
   }else{
      GPIO_SetBits(GPIOB,GPIO_Pin_6);  //Enable driver
      //GPIO_SetBits(GPIOC,GPIO_Pin_2);//green led on
      //GPIO_ResetBits(GPIOC,GPIO_Pin_1);   //yellow led off
      timeout ++;
   }
   //GPIO_SetBits(GPIOB,GPIO_Pin_12);
}

//DMA transfer complete interrupt, every PWM cycle, when ADC conversion is complete
void DMA1_Channel1_IRQHandler(){
   DMA_ClearITPendingBit(DMA1_IT_TC1);

   //TODO: hadrware limits for current, temperature and voltage

   //fault test
   if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)){
      //GPIO_ResetBits(GPIOC,GPIO_Pin_0);//red led off
   }else{
      //GPIO_SetBits(GPIOC,GPIO_Pin_0);//red led on
   }

   volt_raw = ADCConvertedValue[0];
   amp_raw = ADCConvertedValue[1];
   temp_raw = ADCConvertedValue[2];
}

//UART RX not empty interrupt
//adds data to the struct, until struct is full. Then calculates uvw, and sets pwm
//TODO: chekcsum
void USART2_IRQHandler(){
   USART_ClearITPendingBit(USART2, USART_IT_RXNE);
   buf = USART_ReceiveData(USART2);
   if(buf == 255){ //start condition
      datapos = 0;
      ((uint8_t*)&packet_to_hv)[datapos++] = (uint8_t)buf;
      uartsend = 1;
      //GPIOC->BSRR = (GPIOC->ODR ^ GPIO_Pin_2) | (GPIO_Pin_2 << 16);//green
   }else if(datapos >= 0 && datapos < sizeof(packet_to_hv_t)){
      ((uint8_t*)&packet_to_hv)[datapos++] = (uint8_t)buf;
   }
   if(datapos == sizeof(packet_to_hv_t)){//all data received
      datapos = -1;
      unbuff_packet((packet_header_t*)&packet_to_hv, sizeof(to_hv_t));

      float ua = TOFLOAT(packet_to_hv.data.a);
      float ub = TOFLOAT(packet_to_hv.data.b);

      float u = ua; // inverse clarke
      float v = - ua / 2.0 + ub / 2.0 * SQRT3;
      float w = - ua / 2.0 - ub / 2.0 * SQRT3;

      //TODO: SVM, clamping

      u += volt / 2.0;
      v += volt / 2.0;
      w += volt / 2.0;

      if(u < v){
         if(u < w){
            v -= u;
            w -= u;
            u = 0.0;
         }
         else{
            u -= w;
            v -= w;
            w = 0.0;
         }
      }
      else{
         if(v < w){
            u -= v;
            w -= v;
            v = 0.0;
         }
         else{
            u -= w;
            v -= w;
            w = 0.0;
         }
      }

      PWM_U = CLAMP(u / volt * PWM_RES, 0, PWM_RES * 0.95);
      PWM_V = CLAMP(v / volt * PWM_RES, 0, PWM_RES * 0.95);
      PWM_W = CLAMP(w / volt * PWM_RES, 0, PWM_RES * 0.95);

      timeout = 0; //reset timeout
      //GPIOC->BSRR = (GPIOC->ODR ^ GPIO_Pin_0) | (GPIO_Pin_0 << 16);//toggle red led
   }
}

int main(void)
{
   RCC_Configuration();
   GPIO_Configuration();

   RCC_ClocksTypeDef RCC_Clocks;
   RCC_GetClocksFreq(&RCC_Clocks);
   SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000 - 1);

   setup_adc();
   tim1_init();
   usart_init();

   PWM_U = 0;
   PWM_V = 0;
   PWM_W = 0;

   packet_from_hv.head.start = 255;
   packet_from_hv.head.key = 0;

   for(int i = 0; i < TEMP_RES; i++){
      temp_buf[i] = TOFIXED(TEMP(i / SCALE));
   }

   while(1){
      if(uartsend == 1){
         amp = AMP(amp_raw);
         volt = VOLT(volt_raw);

         packet_from_hv.data.dc_volt = TOFIXED(volt);
         packet_from_hv.data.dc_cur =  TOFIXED(amp);
         packet_from_hv.data.hv_temp = TOFIXED(temp);
#ifdef TROLLER
         packet_from_hv.data.dc_cur =  TOFIXED(0);
         packet_from_hv.data.hv_temp = TOFIXED(0);
         packet_from_hv.data.a = TOFIXED(AMP(ADCConvertedValue[1]));
         packet_from_hv.data.b = TOFIXED(AMP(ADCConvertedValue[2]));
         packet_from_hv.data.c = TOFIXED(AMP(ADCConvertedValue[3]));
#endif
         buff_packet((packet_header_t*)&packet_from_hv, sizeof(from_hv_t));
         uartsend = 0;
         //while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);

         // DMA_Cmd(DMA1_Channel7, DISABLE);
         // DMA_ClearFlag(DMA1_FLAG_TC7);
         // DMA_Cmd(DMA1_Channel7, ENABLE);
         //
         //
         // while(DMA_GetFlagStatus(DMA1_FLAG_TC7) == RESET){
         //    GPIO_SetBits(GPIOC,GPIO_Pin_0);
         // }
         // GPIO_ResetBits(GPIOC,GPIO_Pin_0);
         //GPIO_SetBits(GPIOC,GPIO_Pin_2);


         for(int j = 0;j<sizeof(packet_from_hv_t);j++){
            while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
            USART_SendData(USART2, ((uint8_t*)&packet_from_hv)[j]);
         }

         if(temp_raw < ARES && temp_raw > 0){
            temp = tempb(temp_raw);
         }
      }
      //GPIOA->BSRR = (GPIOA->ODR ^ GPIO_Pin_2) | (GPIO_Pin_2 << 16);//toggle red led
   }
}
