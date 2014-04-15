//
//  setup.c
//  test
//
//  Created by Rene on 09/12/13.
//  Copyright (c) 2013 Rene Hopf. All rights reserved.
//

#include "setup.h"

void setup(){
    //TIM7: DAC krams, auf PA5
    UB_DAC_DMA_Init(SINGLE_DAC2_DMA);
    UB_DAC_DMA_SetWaveform2(DAC_WAVE1_SINUS);
    UB_DAC_DMA_SetFrq2(0,4199);

    //PC6 PB5
    UB_ENCODER_TIM3_Init(ENC_T3_MODE_4AB, ENC_T3_A, 2000);


    //TIM4:PWM
    //TIM2:int

    /* TIM config */
    /* TIM4 clock enable */
    //PWM timer
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    //int timer
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    //DAC enable
    //RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
    /* LEDs are on GPIOD */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_ADC2 | RCC_APB2Periph_ADC3, ENABLE);

    /*
     //DMA
     RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
     //GPIO A an
     RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
     GPIO_Init(GPIOA, &GPIO_InitStructure);
     */

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14; // V, U, W
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11 | GPIO_Pin_15; // time, enable
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7; //neuer res erreger
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);

    //Analog pin configuration
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;//The channel 10 is connected to PC0
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN; //The PC0 pin is configured in analog mode
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //We don't need any pull up or pull down
    GPIO_Init(GPIOA,&GPIO_InitStructure);//Affecting the port with the initialization structure configuration

    //ADC structure configuration
    ADC_DeInit();
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//data converted will be shifted to right
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//Input voltage is converted into a 12bit number giving a maximum value of 4096
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; //the conversion is continuous, the input data is converted more than once
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//no trigger for conversion
    ADC_InitStructure.ADC_NbrOfConversion = 1;//I think this one is clear :p
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;//The scan is configured in one channel
    //ADC_InitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
    ADC_Init(ADC1,&ADC_InitStructure);//Initialize ADC with the previous configuration
    ADC_Init(ADC2,&ADC_InitStructure);//Initialize ADC with the previous configuration
    ADC_Init(ADC3,&ADC_InitStructure);//Initialize ADC with the previous configuration


    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
    ADC_CommonInit(&ADC_CommonInitStructure);


    //Enable ADC conversion
    ADC_Cmd(ADC1,ENABLE);
    ADC_Cmd(ADC2,ENABLE);
    ADC_Cmd(ADC3,ENABLE);

    //Select the channel to be read from
    ADC_RegularChannelConfig(ADC1,ADC_Channel_1,1,ADC_SampleTime_480Cycles);
    ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);
    ADC_EOCOnEachRegularChannelCmd(ADC1,ENABLE);

    ADC_RegularChannelConfig(ADC2,ADC_Channel_2,1,ADC_SampleTime_480Cycles);
    ADC_ITConfig(ADC2,ADC_IT_EOC,DISABLE);
    ADC_EOCOnEachRegularChannelCmd(ADC2,DISABLE);

    ADC_RegularChannelConfig(ADC3,ADC_Channel_3,1,ADC_SampleTime_84Cycles);
    ADC_ITConfig(ADC3,ADC_IT_EOC,DISABLE);
    ADC_EOCOnEachRegularChannelCmd(ADC3,DISABLE);

    /* pwm set up */

    /* Compute the prescaler value */       // 168MHz/2         / pwm frq / pwm res - 1
    uint16_t PrescalerValue = (uint16_t) ((SystemCoreClock /2) / 20000 / mag_res) - 1; // = 4

    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = mag_res;
    TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    /* PWM1 Mode configuration: Channel1 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;


    TIM_OC1Init(TIM4, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

    /* PWM1 Mode configuration: Channel2 */
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC2Init(TIM4, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);

    /* PWM1 Mode configuration: Channel3 */
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC3Init(TIM4, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);

    /* int set up */
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 8400;//10kHz
    TIM_TimeBaseStructure.TIM_Prescaler = 0;
    //TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    //TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);//DAC?

    /* int NVIC setup */
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    NVIC_Init(&NVIC_InitStructure);

    // analog NVIC
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);


    //int for DAC timer, TIM7
    TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    NVIC_Init(&NVIC_InitStructure);
    /*
     //DAC setup
     DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
     DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
     DAC_InitStructure.DAC_Trigger = DAC_Trigger_T2_TRGO;
     DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
     DAC_Init(DAC_Channel_1, &DAC_InitStructure);
     DAC_Init(DAC_Channel_2, &DAC_InitStructure);

     //DMA setup
     DMA_InitStructure.DMA_Channel = DMA_Channel_7;
     DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(DAC->DHR12RD);
     DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&sinCosTable;
     DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
     DMA_InitStructure.DMA_BufferSize = 32;
     DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
     DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
     DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
     DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
     DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
     DMA_InitStructure.DMA_Priority = DMA_Priority_High;
     //DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
     DMA_Init(DMA1_Stream5, &DMA_InitStructure);

     DMA_Cmd(DMA1_Stream1, ENABLE);
     DAC_Cmd(DAC_Channel_1, ENABLE);
     DAC_Cmd(DAC_Channel_2, ENABLE);
     DAC_DMACmd(DAC_Channel_1, ENABLE);
     */

	// systick timer
	time = 0;
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);
	SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);
  UB_USB_CDC_Init();
}

void SysTick_Handler(void)
{
  time++;
}
