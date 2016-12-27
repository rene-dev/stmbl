//
//  setup.c
//  test
//
//  Created by Rene on 09/12/13.
//  Copyright (c) 2013 Rene Hopf. All rights reserved.
//

#include "setup.h"
#include "usb_cdc.h"

void setup(){
   //Enable clocks
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_DMA1 | RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_CRC, ENABLE);

   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

   setup_res();
   usb_init();

	// systick timer
	RCC_GetClocksFreq(&RCC_Clocks);
	SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);
   //systick prio

   NVIC_SetPriority(SysTick_IRQn, 14);
}

// Setup Resolver Interface
// TIM4 triggers ADC1,ADC2(OC4) and TIM2(TRGO) at 1.2MHz
// TIM2 OC1 generates resolver reference signal at 10kHz
// DMA2 moves 60 samples to memory, generates transfer complete interrupt at 5kHz
void setup_res(){
   //master timer
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
   TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
   TIM_TimeBaseStructure.TIM_Period = 70 - 1;// 84e6 / 70 = 1.2MHz
   TIM_TimeBaseStructure.TIM_Prescaler = 0;
   TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
   TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
   TIM_ARRPreloadConfig(TIM4,ENABLE);
   TIM_SelectOutputTrigger(TIM4, TIM_TRGOSource_Update);// trigger ADC
   
   // debug tim4
   // GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
   // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
   // GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   // GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   // GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
   // GPIO_Init(GPIOB, &GPIO_InitStructure);
   // GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_TIM4);

   //oc4 for adc trigger
   TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
   TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
   TIM_OCInitStructure.TIM_Pulse = 1;
   TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
   TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;

   TIM_OC4Init(TIM4, &TIM_OCInitStructure);
   TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
   TIM_CtrlPWMOutputs(TIM4, ENABLE);
      
   //slave timer
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
   TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
   TIM_TimeBaseStructure.TIM_Period = 60 - 1;// 1.2e6 / 60 = 20kHz
   TIM_TimeBaseStructure.TIM_Prescaler = 0;
   TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
   TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
   TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_External1);//Rising edges of the selected trigger (TRGI) clock the counter
   TIM_ITRxExternalClockConfig(TIM2, TIM_TS_ITR3);// clk = TIM4 trigger out
   TIM_ARRPreloadConfig(TIM2,ENABLE);
   
   // resolver reference signal
   //TODO: move to res.comp
   TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;
   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
   TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
   TIM_OCInitStructure.TIM_Pulse = 5;
   TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
   TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
   TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
   TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
   
   TIM_OC3Init(TIM2, &TIM_OCInitStructure);
   TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
   TIM_CtrlPWMOutputs(TIM2, ENABLE);
   
   TIM_Cmd(TIM2, ENABLE);
   
    /* ADC clock enable */
    RCC_APB2PeriphClockCmd(SIN_ADC_RCC | COS_ADC_RCC, ENABLE);

    //Analog pin configuration
    GPIO_InitStructure.GPIO_Pin = SIN_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(SIN_PORT,&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = COS_PIN;
    GPIO_Init(COS_PORT,&GPIO_InitStructure);

    //ADC structure configuration
    ADC_DeInit();

    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//data converted will be shifted to right
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//Input voltage is converted into a 12bit number giving a maximum value of 4096
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; //the conversion is continuous, the input data is converted more than once
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T4_CC4;//trigger on rising edge of TIM4 cc4
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;
    ADC_InitStructure.ADC_NbrOfConversion = 1;//ADC_ANZ;//I think this one is clear :p
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;//The scan is configured in one channel
    ADC_Init(SIN_ADC, &ADC_InitStructure);//Initialize ADC with the previous configuration
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    ADC_Init(COS_ADC, &ADC_InitStructure);//Initialize ADC with the previous configuration

    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    ADC_CommonInitStructure.ADC_Mode = ADC_DualMode_RegSimult;
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_2;
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
    ADC_CommonInit(&ADC_CommonInitStructure);

    for(int i = 1;i<=1;i++){
        ADC_RegularChannelConfig(SIN_ADC, SIN_ADC_CHAN, i, RES_SampleTime);
        ADC_RegularChannelConfig(COS_ADC, COS_ADC_CHAN, i, RES_SampleTime);
    }

    ADC_MultiModeDMARequestAfterLastTransferCmd(ENABLE);

    //Enable ADC conversion
    ADC_Cmd(SIN_ADC,ENABLE);
    ADC_Cmd(COS_ADC,ENABLE);

    // DMA-Disable
    DMA_Cmd(DMA2_Stream0, DISABLE);
    DMA_DeInit(DMA2_Stream0);

    // DMA2-Config
    DMA_InitStructure.DMA_Channel = DMA_Channel_0;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC->CDR;
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADC_DMA_Buffer0;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
    DMA_InitStructure.DMA_BufferSize = ADC_ANZ * PID_WAVES;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    //TODO: use double buffer
    //DMA_DoubleBufferModeConfig(DMA2_Stream0, (uint32_t)ADC_DMA_Buffer1, DMA_Memory_0);
    //DMA_DoubleBufferModeCmd(DMA2_Stream0, ENABLE);
    DMA_Init(DMA2_Stream0, &DMA_InitStructure);

    //HAL Fast realtime irq 20kHz
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    //HAL Realtime irq 5kHz
    NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    DMA_Cmd(DMA2_Stream0, ENABLE);

    DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);
 }
