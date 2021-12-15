//
//  setup.c
//  test
//
//  Created by Rene on 09/12/13.
//  Copyright (c) 2013 Rene Hopf. All rights reserved.
//

#include "setup.h"
#include "usbd_cdc_if.h"
#include "defines.h"

RCC_ClocksTypeDef RCC_Clocks;
volatile uint32_t ADC_DMA_Buffer0[ADC_SAMPLES_IN_RT];  //240
volatile uint32_t ADC_DMA_Buffer1[ADC_SAMPLES_IN_RT];

void setup() {
  //Enable clocks
  //TODO: small f4 does not have GPIOE
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_DMA1 | RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_CRC, ENABLE);

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

  setup_res();
  usb_init();

  GPIO_InitTypeDef GPIO_InitStructure;
  // messpin
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  
  //fan
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_0;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  // systick timer
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);
  //systick prio

  NVIC_SetPriority(SysTick_IRQn, 14);
}

// Setup Resolver Interface
// master timer triggers ADC1,ADC2 via OC, and slave timer via TRGO at 1.2MHz
// slave timer OC generates resolver reference signal at 10kHz, phase can be adjusted by oc value
// DMA2 moves ADC_ANZ samples to memory, generates transfer complete interrupt at 5kHz
void setup_res() {
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;
  //master timer
  RCC_APB1PeriphClockCmd(TIM_MASTER_RCC, ENABLE);
  TIM_TimeBaseStructure.TIM_ClockDivision     = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode       = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period            = ADC_TIMER_FREQ / ADC_TRIGGER_FREQ - 1;  //70 1.2MHz
  TIM_TimeBaseStructure.TIM_Prescaler         = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM_MASTER, &TIM_TimeBaseStructure);
  TIM_ARRPreloadConfig(TIM_MASTER, ENABLE);
  TIM_SelectOutputTrigger(TIM_MASTER, TIM_TRGOSource_Update);  // trigger ADC

  //oc for adc trigger
  TIM_OCInitStructure.TIM_OCMode       = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState  = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
  TIM_OCInitStructure.TIM_Pulse        = 1;
  TIM_OCInitStructure.TIM_OCPolarity   = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState  = TIM_OCIdleState_Set;
  //ADC trigger OC depends on timer
  TIM_MASTER_ADC_OC_INIT(TIM_MASTER, &TIM_OCInitStructure);
  TIM_MASTER_ADC_OC_PRELOAD(TIM_MASTER, TIM_OCPreload_Enable);
  TIM_CtrlPWMOutputs(TIM_MASTER, ENABLE);

  //slave timer triggers frt
  RCC_APB1PeriphClockCmd(TIM_SLAVE_RCC, ENABLE);
  TIM_TimeBaseStructure.TIM_ClockDivision     = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode       = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period            = ADC_TRIGGER_FREQ / FRT_FREQ - 1;  //60 20kHz
  TIM_TimeBaseStructure.TIM_Prescaler         = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM_SLAVE, &TIM_TimeBaseStructure);
  TIM_SelectSlaveMode(TIM_SLAVE, TIM_SlaveMode_External1);  //Rising edges of the selected trigger (TRGI) clock the counter
  TIM_ITRxExternalClockConfig(TIM_SLAVE, TIM_SLAVE_ITR);    // clk = TIM_MASTER trigger out
  TIM_ARRPreloadConfig(TIM_SLAVE, ENABLE);
  TIM_SLAVE->CNT = (TIM_SLAVE->ARR + 1) / 2;
  TIM_Cmd(TIM_SLAVE, ENABLE);

  /* ADC clock enable */
  RCC_APB2PeriphClockCmd(FB0_SIN_ADC_RCC | FB0_COS_ADC_RCC, ENABLE);

#ifdef FB1
  RCC_APB2PeriphClockCmd(FB1_SIN_ADC_RCC | FB1_COS_ADC_RCC, ENABLE);
#endif

  //Analog pin configuration
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin  = FB0_SIN_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(FB0_SIN_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = FB0_COS_PIN;
  GPIO_Init(FB0_COS_PORT, &GPIO_InitStructure);

#ifdef FB1
  GPIO_InitStructure.GPIO_Pin = FB1_SIN_PIN;
  GPIO_Init(FB1_SIN_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = FB1_COS_PIN;
  GPIO_Init(FB1_COS_PORT, &GPIO_InitStructure);
#endif

  //ADC structure configuration
  ADC_DeInit();
  ADC_InitTypeDef ADC_InitStructure;
  ADC_InitStructure.ADC_DataAlign            = ADC_DataAlign_Right;  //data converted will be shifted to right
  ADC_InitStructure.ADC_Resolution           = ADC_Resolution_12b;   //Input voltage is converted into a 12bit number giving a maximum value of 4096
  ADC_InitStructure.ADC_ContinuousConvMode   = DISABLE;              //the conversion is continuous, the input data is converted more than once
  ADC_InitStructure.ADC_ExternalTrigConv     = TIM_MASTER_ADC;       //trigger on rising edge of TIM_MASTER oc
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;
  ADC_InitStructure.ADC_NbrOfConversion      = ADC_OVER_FB0 + ADC_OVER_FB1;  //I think this one is clear :p
  ADC_InitStructure.ADC_ScanConvMode         = ENABLE;                       //The scan is configured in one channel
  ADC_Init(FB0_SIN_ADC, &ADC_InitStructure);                                 //Initialize ADC with the previous configuration
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_Init(FB0_COS_ADC, &ADC_InitStructure);  //Initialize ADC with the previous configuration

  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  ADC_CommonInitStructure.ADC_Mode             = ADC_DualMode_RegSimult;
  ADC_CommonInitStructure.ADC_Prescaler        = ADC_Prescaler_Div4;
  ADC_CommonInitStructure.ADC_DMAAccessMode    = ADC_DMAAccessMode_2;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInit(&ADC_CommonInitStructure);

  for(int i = 1; i <= ADC_OVER_FB0; i++) {
    ADC_RegularChannelConfig(FB0_SIN_ADC, FB0_SIN_ADC_CHAN, i, RES_SampleTime);
    ADC_RegularChannelConfig(FB0_COS_ADC, FB0_COS_ADC_CHAN, i, RES_SampleTime);
  }

#ifdef FB1
  for(int i = ADC_OVER_FB0 + 1; i <= ADC_OVER_FB0 + ADC_OVER_FB1; i++) {
    ADC_RegularChannelConfig(FB1_SIN_ADC, FB1_SIN_ADC_CHAN, i, RES_SampleTime);
    ADC_RegularChannelConfig(FB1_COS_ADC, FB1_COS_ADC_CHAN, i, RES_SampleTime);
  }
#endif


  ADC_DiscModeChannelCountConfig(ADC1, 1);
  ADC_DiscModeChannelCountConfig(ADC2, 1);
  ADC_DiscModeCmd(ADC1, ENABLE);
  ADC_DiscModeCmd(ADC2, ENABLE);

  ADC_MultiModeDMARequestAfterLastTransferCmd(ENABLE);

  //Enable ADC conversion
  ADC_Cmd(FB0_SIN_ADC, ENABLE);
  ADC_Cmd(FB0_COS_ADC, ENABLE);

  // DMA-Disable
  DMA_Cmd(DMA2_Stream0, DISABLE);
  DMA_DeInit(DMA2_Stream0);

  // DMA2-Config
  DMA_InitTypeDef DMA_InitStructure;
  DMA_InitStructure.DMA_Channel            = DMA_Channel_0;
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC->CDR;
  DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)ADC_DMA_Buffer0;
  DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize         = ARRAY_SIZE(ADC_DMA_Buffer0);
  DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Word;
  DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority           = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode           = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold      = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst        = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;

  DMA_DoubleBufferModeConfig(DMA2_Stream0, (uint32_t)ADC_DMA_Buffer1, DMA_Memory_0);
  DMA_DoubleBufferModeCmd(DMA2_Stream0, ENABLE);
  DMA_Init(DMA2_Stream0, &DMA_InitStructure);

  NVIC_InitTypeDef NVIC_InitStructure;
  //HAL Fast realtime irq 20kHz
  NVIC_InitStructure.NVIC_IRQChannel                   = TIM_SLAVE_IRQ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  //HAL Realtime irq 5kHz
  NVIC_InitStructure.NVIC_IRQChannel                   = DMA2_Stream0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  DMA_Cmd(DMA2_Stream0, ENABLE);

  DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);
}
