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
   
	//messpin
   GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_9;
 	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
   GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
   GPIO_Init(GPIOB, &GPIO_InitStructure);

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
// TIM2 triggers ADC1 and 2 at 20kHz
// TIM2 OC1 generates resolver reference signal at 10kHz
// DMA2 moves 4 samples to memory, generates transfer complete interrupt at 5kHz
void setup_res(){
    //resolver timer
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 420;//20kHz
    TIM_TimeBaseStructure.TIM_Prescaler = 9;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_ARRPreloadConfig(TIM2,ENABLE);

    TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);//trigger ADC

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
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_TRGO;//trigger on rising edge of TIM8
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;
    ADC_InitStructure.ADC_NbrOfConversion = ADC_ANZ;//I think this one is clear :p
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

    for(int i = 1;i<=ADC_ANZ;i++){
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
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADC_DMA_Buffer;
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
    DMA_Init(DMA2_Stream0, &DMA_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    DMA_Cmd(DMA2_Stream0, ENABLE);

    DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);
 }
