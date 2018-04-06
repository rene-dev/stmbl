#include "stm32f10x_conf.h"
#include "common_f1.h"
#include "defines.h"
#include "version.h"
#include <math.h>

#define ARES 4096.0  // analog resolution, 12 bit
#define AREF 3.3     // analog reference voltage

//iramx v3.1-v3.3 hardware
#define RCUR 0.0181    //shunt
#define TPULLUP 10000  //iramx temperature pullup
#define R10 10000
#define R11 180

#define VDIVUP 36000.0  //HV div pullup R1,R12
#define VDIVDOWN 280.0  //HV div pulldown R2,R9
#define PWM_U TIM1->CCR1
#define PWM_V TIM1->CCR2
#define PWM_W TIM1->CCR3

#define AMP(a) (((a)*AREF / ARES - AREF / (R10 + R11) * R11) / (RCUR * R10) * (R10 + R11))
#define TEMP(a) (log10f((a)*AREF / ARES * TPULLUP / (AREF - a * AREF / ARES)) * (-53) + 290)

#define VOLT(a) ((a) / ARES * AREF / VDIVDOWN * (VDIVUP + VDIVDOWN))

volatile uint16_t ADCConvertedValue[100];  //DMA buffer for ADC
volatile uint8_t rxbuf[50];                //DMA buffer for UART RX
uint32_t rxpos = 0;                        //UART rx buffer position

volatile uint32_t u_cmd = 0;
volatile uint32_t v_cmd = 0;
volatile uint32_t w_cmd = 0;

uint32_t u_error = 0;
uint32_t v_error = 0;
uint32_t w_error = 0;

#define TOFIXEDU32(a) ((uint32_t)((a)*65536))
#define TOFLOAT32(a) ((uint32_t)((a) / 65536))

#define TEMP_RES 32
#define TEMP_SCALE (TEMP_RES / ARES)
int16_t temp_buf[TEMP_RES];

float tempb(float i) {
  unsigned int x = (int)(i * TEMP_SCALE);
  float a        = TOFLOAT(temp_buf[x]);
  float b        = TOFLOAT(temp_buf[x + 1]);
  return (a + (b - a) * (i * TEMP_SCALE - x));
}

volatile uint32_t timeout       = 99999;
volatile uint16_t temp_raw      = 0;
volatile uint8_t hv_fault       = 0;
volatile uint8_t hv_enabled     = 0;
volatile int32_t hv_fault_count = 0;

float volt = 0;
float amp  = 0;
float temp = 0;

volatile unsigned int systime = 0;
volatile float u, v, w;
volatile int uartsend = 0;

packet_to_hv_t packet_to_hv;
packet_from_hv_t packet_from_hv;
int32_t datapos = -1;

TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
TIM_OCInitTypeDef TIM_OCInitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef USART_InitStruct;
ADC_InitTypeDef ADC_InitStructure;
DMA_InitTypeDef DMA_InitStructure;
DMA_InitTypeDef DMA_InitStructuretx;

void Wait(unsigned int ms) {  //TODO: systick is not used
  volatile unsigned int t = systime + ms;
  while(t >= systime) {
  }
}

void SysTick_Handler(void) {
  systime++;
}

void RCC_Configuration(void) {
  RCC_ClocksTypeDef RCC_Clocks;
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000 - 1);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
}

void hv_enable() {
  GPIO_SetBits(GPIOB, GPIO_Pin_6);
  hv_enabled = 1;
}

void hv_disable() {
  GPIO_ResetBits(GPIOB, GPIO_Pin_6);
  hv_enabled = 0;
}

void GPIO_Configuration(void) {
  //LED init
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  //Enable output
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  //Fault in
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  //PA5,6,7 sv2
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  hv_disable();
}

void tim2_init() {
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  TIM_TimeBaseStructure.TIM_Period            = 480;  // 72000000 / 480 = 150kHz
  TIM_TimeBaseStructure.TIM_Prescaler         = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision     = 0;
  TIM_TimeBaseStructure.TIM_CounterMode       = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 1;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  TIM_OCInitStructure.TIM_OCMode       = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState  = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
  TIM_OCInitStructure.TIM_Pulse        = 240;
  TIM_OCInitStructure.TIM_OCPolarity   = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OCNPolarity  = TIM_OCNPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState  = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

  TIM_OC2Init(TIM2, &TIM_OCInitStructure);

  TIM_Cmd(TIM2, ENABLE);

  TIM_CtrlPWMOutputs(TIM2, ENABLE);
}

void tim1_init() {
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

  //TIM1
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  //TIM1N
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* Channel 1, 2 and 3 Configuration in PWM mode */
  TIM_TimeBaseStructure.TIM_Period            = PWM_RES;
  TIM_TimeBaseStructure.TIM_Prescaler         = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision     = 0;
  TIM_TimeBaseStructure.TIM_CounterMode       = TIM_CounterMode_CenterAligned3;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 1;
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
  TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);

  /* int NVIC setup */
  NVIC_InitStructure.NVIC_IRQChannel                   = TIM1_UP_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;

  NVIC_Init(&NVIC_InitStructure);

  TIM_OCInitStructure.TIM_OCMode       = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState  = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_Pulse        = 0;
  TIM_OCInitStructure.TIM_OCPolarity   = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OCNPolarity  = TIM_OCNPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState  = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

  TIM_OC1Init(TIM1, &TIM_OCInitStructure);
  TIM_OC2Init(TIM1, &TIM_OCInitStructure);
  TIM_OC3Init(TIM1, &TIM_OCInitStructure);

  TIM_Cmd(TIM1, ENABLE);

  TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

void usart_init() {
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

  //DMA is configured every tx cycle
  DMA_InitStructuretx.DMA_PeripheralBaseAddr = (uint32_t)&USART2->DR;
  DMA_InitStructuretx.DMA_MemoryBaseAddr     = (uint32_t)&packet_from_hv;
  DMA_InitStructuretx.DMA_DIR                = DMA_DIR_PeripheralDST;
  DMA_InitStructuretx.DMA_BufferSize         = sizeof(packet_from_hv_t);
  DMA_InitStructuretx.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
  DMA_InitStructuretx.DMA_MemoryInc          = DMA_MemoryInc_Enable;
  DMA_InitStructuretx.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructuretx.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
  DMA_InitStructuretx.DMA_Mode               = DMA_Mode_Normal;
  DMA_InitStructuretx.DMA_Priority           = DMA_Priority_High;
  DMA_InitStructuretx.DMA_M2M                = DMA_M2M_Disable;

  //RX DMA
  DMA_DeInit(DMA1_Channel6);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART2->DR;
  DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t)&rxbuf;
  DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize         = sizeof(rxbuf);
  DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority           = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel6, &DMA_InitStructure);
  DMA_Cmd(DMA1_Channel6, ENABLE);

  //USART TX
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  //USART RX
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  USART_InitStruct.USART_BaudRate            = DATABAUD;
  USART_InitStruct.USART_WordLength          = USART_WordLength_8b;
  USART_InitStruct.USART_StopBits            = USART_StopBits_1;
  USART_InitStruct.USART_Parity              = USART_Parity_No;
  USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStruct.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(USART2, &USART_InitStruct);
  USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);
  USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
  USART_Cmd(USART2, ENABLE);
}

// Setup ADC
void setup_adc() {
  RCC_ADCCLKConfig(RCC_PCLK2_Div6);  // 12MHz
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_ADC2 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;

  //PINC5 IN15 DC link
  //PINC4 IN14 AMP
  //PINB0 IN8 temperature
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
  DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t)ADCConvertedValue;
  DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize         = ARRAY_SIZE(ADCConvertedValue);
  DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority           = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);

  /* Enable DMA1 channel1 */
  DMA_Cmd(DMA1_Channel1, ENABLE);

  /* ADC1 configuration ------------------------------------------------------*/
  ADC_InitStructure.ADC_Mode               = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode       = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_T2_CC2;
  ADC_InitStructure.ADC_DataAlign          = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel       = 1;
  ADC_Init(ADC1, &ADC_InitStructure);

  ADC_ExternalTrigConvCmd(ADC1, ENABLE);

  ADC_TempSensorVrefintCmd(ENABLE);

  ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_13Cycles5);  //amp

  /* Enable ADC1 DMA */
  ADC_DMACmd(ADC1, ENABLE);

  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);

  /* Enable ADC1 reset calibration register */
  ADC_ResetCalibration(ADC1);
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1))
    ;

  /* Start ADC1 calibration */
  ADC_StartCalibration(ADC1);
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1))
    ;

  //ADC2, injected mode for voltage and temperatue
  ADC_InitStructure.ADC_Mode               = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode       = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign          = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel       = 2;
  ADC_Init(ADC2, &ADC_InitStructure);

  ADC_InjectedSequencerLengthConfig(ADC2, 2);
  /* ADC1 injected channel Configuration */
  ADC_InjectedChannelConfig(ADC2, ADC_Channel_15, 1, ADC_SampleTime_71Cycles5);
  ADC_InjectedChannelConfig(ADC2, ADC_Channel_8, 2, ADC_SampleTime_71Cycles5);
  /* ADC1 injected external trigger configuration */
  ADC_ExternalTrigInjectedConvConfig(ADC2, ADC_ExternalTrigInjecConv_None);
  ADC_Cmd(ADC2, ENABLE);
  /* Enable ADC1 reset calibration register */
  ADC_ResetCalibration(ADC2);
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC2))
    ;

  /* Start ADC1 calibration */
  ADC_StartCalibration(ADC2);
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC2))
    ;
  ADC_SoftwareStartInjectedConvCmd(ADC2, ENABLE);
}

//TIM1 update interrupt, every PWM cycle
void TIM1_UP_IRQHandler() {
  TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
  if(timeout > 30) {  //disable driver
    hv_disable();
    GPIO_SetBits(GPIOC, GPIO_Pin_1);    //yellow led on
    GPIO_ResetBits(GPIOC, GPIO_Pin_2);  //green led off

    PWM_U = 0;
    PWM_V = 0;
    PWM_W = 0;

    u_cmd   = TOFIXEDU32(0.0);
    u_error = TOFIXEDU32(0.0);
    v_cmd   = TOFIXEDU32(0.0);
    v_error = TOFIXEDU32(0.0);
    w_cmd   = TOFIXEDU32(0.0);
    w_error = TOFIXEDU32(0.0);
  } else {
    GPIO_SetBits(GPIOC, GPIO_Pin_2);    //green led on
    GPIO_ResetBits(GPIOC, GPIO_Pin_1);  //yellow led off
    timeout++;

    u_error += u_cmd;
    uint32_t u = TOFLOAT32(u_error);
    u_error    = MAX(0, u_error - TOFIXEDU32(u));
    PWM_U      = u;

    v_error += v_cmd;
    uint32_t v = TOFLOAT32(v_error);
    v_error    = MAX(0, v_error - TOFIXEDU32(v));
    PWM_V      = v;

    w_error += w_cmd;
    uint32_t w = TOFLOAT32(w_error);
    w_error    = MAX(0, w_error - TOFIXEDU32(w));
    PWM_W      = w;
  }
}

int main(void) {
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
  RCC_Configuration();
  GPIO_Configuration();

  setup_adc();
  tim1_init();
  tim2_init();
  usart_init();

  PWM_U = 0;
  PWM_V = 0;
  PWM_W = 0;

  packet_from_hv.head.start = 255;
  packet_from_hv.head.key   = 0;

  for(int i = 0; i < TEMP_RES; i++) {
    temp_buf[i] = TOFIXED(TEMP(i / TEMP_SCALE));
  }

  while(1) {
    //next received packet will be written to bufferpos
    uint32_t bufferpos = sizeof(rxbuf) - DMA_GetCurrDataCounter(DMA1_Channel6);
    //how many packets we have the the rx buffer for processing
    uint32_t available = (bufferpos - rxpos + sizeof(rxbuf)) % sizeof(rxbuf);

    for(int i = 0; i < available; i++) {
      uint16_t buf = rxbuf[(rxpos) % sizeof(rxbuf)];
      if(buf == 255) {  //start condition
        datapos                               = 0;
        ((uint8_t *)&packet_to_hv)[datapos++] = (uint8_t)buf;
        uartsend                              = 1;
      } else if(datapos >= 0 && datapos < sizeof(packet_to_hv_t)) {
        ((uint8_t *)&packet_to_hv)[datapos++] = (uint8_t)buf;
      }
      if(datapos == sizeof(packet_to_hv_t)) {  //all data received
        datapos = -1;
        unbuff_packet((packet_header_t *)&packet_to_hv, sizeof(to_hv_t));
        if(packet_to_hv.data.enable == 1) {
          hv_enable();
        } else {
          hv_disable();
        }
        float ua = packet_to_hv.data.a;
        float ub = packet_to_hv.data.b;

        float u = 0.0;
        float v = 0.0;
        float w = 0.0;

        if(packet_to_hv.data.mode == 0) {  //a,b voltages
          u = ua;                          // inverse clarke
          v = -ua / 2.0 + ub / 2.0 * M_SQRT3;
          w = -ua / 2.0 - ub / 2.0 * M_SQRT3;
        } else if(packet_to_hv.data.mode == 1) {  //DC, a: -dclink ... +dclink
          u = ua / 2.0;
          v = -ua / 2.0;
          w = 0;
        } else if(packet_to_hv.data.mode == 2) {  //2phase, a,b: -dclink/2 ... +dclink/2
          u = ua;
          v = 0;
          w = ub;
        }

        u += volt / 2.0;
        v += volt / 2.0;
        w += volt / 2.0;

        if(u < v) {
          if(u < w) {
            v -= u;
            w -= u;
            u = 0.0;
          } else {
            u -= w;
            v -= w;
            w = 0.0;
          }
        } else {
          if(v < w) {
            u -= v;
            w -= v;
            v = 0.0;
          } else {
            u -= w;
            v -= w;
            w = 0.0;
          }
        }

        u_cmd = TOFIXEDU32(CLAMP(u / volt * PWM_RES, 0, PWM_RES * 0.95));
        v_cmd = TOFIXEDU32(CLAMP(v / volt * PWM_RES, 0, PWM_RES * 0.95));
        w_cmd = TOFIXEDU32(CLAMP(w / volt * PWM_RES, 0, PWM_RES * 0.95));

        timeout = 0;  //reset timeout
      }

      rxpos++;
      rxpos = rxpos % sizeof(rxbuf);
    }

    if(uartsend == 1) {
      DMA_DeInit(DMA1_Channel7);
      DMA_Init(DMA1_Channel7, &DMA_InitStructuretx);
      DMA_Cmd(DMA1_Channel7, ENABLE);
      int adcbufferpos;
      uint32_t cur_sum = 0;
      //next received packet will be written to bufferpos
      adcbufferpos = ARRAY_SIZE(ADCConvertedValue) - DMA_GetCurrDataCounter(DMA1_Channel1);
      //bufferpos-1 .. bufferpos-1-samples
      int samples = 30;
      for(int i = 0; i < samples; i++) {
        if(adcbufferpos + i >= ARRAY_SIZE(ADCConvertedValue)) {
          adcbufferpos = 0;
        }
        cur_sum += ADCConvertedValue[adcbufferpos + i];
      }

      amp  = AMP((float)cur_sum / (float)samples);
      volt = VOLT(ADC_GetInjectedConversionValue(ADC2, ADC_InjectedChannel_1));

      packet_from_hv.data.dc_volt = TOFIXED(volt);
      packet_from_hv.data.dc_cur  = TOFIXED(amp);
      packet_from_hv.data.hv_temp = TOFIXED(temp);
      int hv_fault_limit          = 10;
      if(hv_fault == 1 && hv_enabled == 1) {
        if(hv_fault_count < hv_fault_limit) {
          hv_fault_count++;
        }
      } else {
        hv_fault_count = 0;
      }
      if(hv_fault_count >= hv_fault_limit) {
        packet_from_hv.data.hv_fault = 1;
      } else {
        packet_from_hv.data.hv_fault = 0;
      }

      buff_packet((packet_header_t *)&packet_from_hv, sizeof(from_hv_t));
      uartsend = 0;
      temp_raw = ADC_GetInjectedConversionValue(ADC2, ADC_InjectedChannel_2);
      if(temp_raw < ARES && temp_raw > 0) {
        temp = tempb(temp_raw);
      }
      ADC_SoftwareStartInjectedConvCmd(ADC2, ENABLE);
    }
  }
}
