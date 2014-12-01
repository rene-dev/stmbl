//
//  setup.c
//  test
//
//  Created by Rene on 09/12/13.
//  Copyright (c) 2013 Rene Hopf. All rights reserved.
//

#include "setup.h"

void setup(){

    //PC6 PB5
    //UB_ENCODER_TIM3_Init(ENC_T3_MODE_4AB, ENC_T3_A, 2000);
    //TIM4:PWM
    //TIM2:int


    //int timer
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//wird in UB_USB_CDC_Init() nochmal gesetzt!
    //res erreger


    //messpin
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* int set up, TIM2*/
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 420;//20kHz
    TIM_TimeBaseStructure.TIM_Prescaler = 9;
    //TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);

    /* int NVIC setup */
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;

    NVIC_Init(&NVIC_InitStructure);

    setup_adc();
    //setup_dma();
    setup_pid_timer();

	// systick timer
	time = 0;

	RCC_GetClocksFreq(&RCC_Clocks);
	SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);
    //systick prio

    NVIC_SetPriority(SysTick_IRQn, 14);

    #ifdef USBTERM
    UB_USB_CDC_Init();
    #endif
}


void setup_pid_timer(){
    /* TIM5 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

    /* pid int set up, TIM5*/
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 1680;//5kHz
    TIM_TimeBaseStructure.TIM_Prescaler = 9;
    //TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
    TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);

    /* int NVIC setup */
    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 12;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStructure);
}

// Setup DMA
void setup_dma(){
    // Clock Enable
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

    // DMA-Disable
    DMA_Cmd(DMA2_Stream2, DISABLE);
    DMA_DeInit(DMA2_Stream2);

    // DMA2-Config
    DMA_InitStructure.DMA_Channel = DMA_Channel_1;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC2->DR;
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADC2_DMA_Buffer;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
    DMA_InitStructure.DMA_BufferSize = ADC2d_ANZ;
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
    DMA_Init(DMA2_Stream2, &DMA_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 15;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    DMA_Cmd(DMA2_Stream2, ENABLE);

    ADC_DMARequestAfterLastTransferCmd(ADC2, ENABLE);
    ADC_DMACmd(ADC2, ENABLE);
    DMA_ITConfig(DMA2_Stream2, DMA_IT_TC, ENABLE);
}

// Setup ADC
void setup_adc(){
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    /* ADC clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_ADC2, ENABLE);

    //Analog pin configuration
    GPIO_InitStructure.GPIO_Pin = RES_SIN_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(RES_SIN_PORT,&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = RES_COS_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(RES_COS_PORT,&GPIO_InitStructure);

    //ADC structure configuration
    ADC_DeInit();

    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//data converted will be shifted to right
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//Input voltage is converted into a 12bit number giving a maximum value of 4096
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; //the conversion is continuous, the input data is converted more than once
    //ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//trigger on rising edge of TIM2
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_NbrOfConversion = 1;//I think this one is clear :p
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;//The scan is configured in one channel
    ADC_Init(ADC1,&ADC_InitStructure);//Initialize ADC with the previous configuration
    ADC_Init(ADC2,&ADC_InitStructure);//Initialize ADC with the previous configuration

    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    ADC_CommonInitStructure.ADC_Mode = ADC_DualMode_InjecSimult;
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
    ADC_CommonInit(&ADC_CommonInitStructure);

    ADC_InjectedSequencerLengthConfig(ADC1, 4);
    ADC_InjectedChannelConfig(ADC1, RES_SIN_CHANNEL, 1, RES_SampleTime);
    ADC_InjectedChannelConfig(ADC1, RES_SIN_CHANNEL, 2, RES_SampleTime);
    ADC_InjectedChannelConfig(ADC1, RES_SIN_CHANNEL, 3, RES_SampleTime);
    ADC_InjectedChannelConfig(ADC1, RES_SIN_CHANNEL, 4, RES_SampleTime);
    ADC_ExternalTrigInjectedConvConfig(ADC1, ADC_ExternalTrigInjecConv_T2_TRGO);
    ADC_ExternalTrigInjectedConvEdgeConfig(ADC1, ADC_ExternalTrigInjecConvEdge_Rising);
    ADC_InjectedDiscModeCmd(ADC1, DISABLE);
    ADC_ITConfig(ADC1, ADC_IT_JEOC, ENABLE);

    ADC_InjectedSequencerLengthConfig(ADC2, 4);
    ADC_InjectedChannelConfig(ADC2, RES_COS_CHANNEL, 1, RES_SampleTime);
    ADC_InjectedChannelConfig(ADC2, RES_COS_CHANNEL, 2, RES_SampleTime);
    ADC_InjectedChannelConfig(ADC2, RES_COS_CHANNEL, 3, RES_SampleTime);
    ADC_InjectedChannelConfig(ADC2, RES_COS_CHANNEL, 4, RES_SampleTime);
    //ADC_ExternalTrigInjectedConvConfig(ADC2, ADC_ExternalTrigInjecConv_T2_TRGO);
    //ADC_ExternalTrigInjectedConvEdgeConfig(ADC2, ADC_ExternalTrigInjecConvEdge_Rising);
    ADC_InjectedDiscModeCmd(ADC2, DISABLE);
    ADC_ITConfig(ADC2, ADC_IT_JEOC, DISABLE);

    //Enable ADC conversion
    ADC_Cmd(ADC1,ENABLE);
    ADC_Cmd(ADC2,ENABLE);

    // analog NVIC
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void SysTick_Handler(void)
{
  time++;
}
