//
//  setup.c
//  test
//
//  Created by Rene on 09/12/13.
//  Copyright (c) 2013 Rene Hopf. All rights reserved.
//

#include "setup.h"
//#include "usb_cdc.h"

void setup(){
   //Enable clocks
   //RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_DMA1 | RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_CRC, ENABLE);
   rcc_periph_clock_enable(RCC_GPIOA);
   rcc_periph_clock_enable(RCC_GPIOB);
   rcc_periph_clock_enable(RCC_GPIOC);
   rcc_periph_clock_enable(RCC_GPIOD);
   rcc_periph_clock_enable(RCC_DMA1);
   rcc_periph_clock_enable(RCC_DMA2);
   rcc_periph_clock_enable(RCC_CRC);
   
	//messpin
   // GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_9;
 //    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
 //   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
 //   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
 //   GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
 //   GPIO_Init(GPIOB, &GPIO_InitStructure);
	gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO3 | GPIO4 | GPIO5 | GPIO8 | GPIO9);
   //gpio_set_output_options(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, GPIO3 | GPIO4 | GPIO5 | GPIO8 | GPIO9);
   
   //foo leds
	gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT,GPIO_PUPD_NONE, GPIO10 | GPIO11);
   gpio_set(GPIOC, GPIO11);
   gpio_clear(GPIOC, GPIO10);
   //foo

   //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
   //scb_set_priority_grouping();//TODO

   setup_res();
   //usb_init();
   cdc_init();
	// systick timer
	//RCC_GetClocksFreq(&RCC_Clocks);
	//SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);
   systick_set_reload(rcc_ahb_frequency / 1000);
   systick_interrupt_enable();
   systick_counter_enable();//??
   //systick prio

   //NVIC_SetPriority(SysTick_IRQn, 14);
   nvic_set_priority(NVIC_SYSTICK_IRQ,14);
   nvic_enable_irq(NVIC_SYSTICK_IRQ);
}

// Setup Resolver Interface
// TIM2 triggers ADC1 and 2 at 20kHz
// TIM2 OC1 generates resolver reference signal at 10kHz
// DMA2 moves 4 samples to memory, generates transfer complete interrupt at 5kHz

void setup_res(){
    //resolver timer
    //RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
   rcc_periph_clock_enable(RCC_TIM2);

    // TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    // TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    // TIM_TimeBaseStructure.TIM_Period = 420;//20kHz
    // TIM_TimeBaseStructure.TIM_Prescaler = 9;
    // TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    // TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    //TIM_ARRPreloadConfig(TIM2,ENABLE);

    //timer_set_mode(TIM2,TIM_CR1_CKD_CK_INT,TIM_CR1_CMS_EDGE,TIM_CR1_DIR_UP);
    timer_reset(TIM2);
    timer_set_mode(TIM2,TIM_CR1_CKD_CK_INT,TIM_CR1_CMS_EDGE,TIM_CR1_DIR_UP);
    timer_set_clock_division(TIM2,TIM_CR1_CKD_CK_INT);
    timer_direction_up(TIM2);
    timer_set_repetition_counter(TIM2,0);
    //timer_enable_preload(TIM2);
    timer_set_period(TIM2,420);
    timer_set_prescaler(TIM2,9);
    timer_enable_update_event(TIM2);
    //hal does this:
    //timer_enable_irq(TIM2,TIM_DIER_UIE);
    //timer_enable_counter(TIM2);

    //TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);//trigger ADC
    timer_set_master_mode(TIM2,TIM_CR2_MMS_UPDATE);

    /* ADC clock enable */
    //RCC_APB2PeriphClockCmd(SIN_ADC_RCC | COS_ADC_RCC, ENABLE);
    rcc_periph_clock_enable(RCC_ADC1);
    rcc_periph_clock_enable(RCC_ADC2);

    //Analog pin configuration
    // GPIO_InitStructure.GPIO_Pin = SIN_PIN;
    // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    // GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    // GPIO_Init(SIN_PORT,&GPIO_InitStructure);

    // GPIO_InitStructure.GPIO_Pin = COS_PIN;
    // GPIO_Init(COS_PORT,&GPIO_InitStructure);
    gpio_mode_setup(GPIOC, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO4);
    gpio_mode_setup(GPIOC, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO5);

    //ADC structure configuration
    //ADC_DeInit();
    adc_off(ADC1);
    adc_off(ADC2);

    // ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//data converted will be shifted to right
    adc_set_right_aligned(ADC1);
    adc_set_right_aligned(ADC2);
    // ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//Input voltage is converted into a 12bit number giving a maximum value of 4096
    adc_set_resolution(ADC1,ADC_CR1_RES_12BIT);
    adc_set_resolution(ADC2,ADC_CR1_RES_12BIT);
    // ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; //the conversion is continuous, the input data is converted more than once
    adc_enable_discontinuous_mode_regular(ADC1,ADC_ANZ);
    adc_enable_discontinuous_mode_regular(ADC2,ADC_ANZ);
    adc_disable_external_trigger_injected(ADC1);
    adc_disable_external_trigger_injected(ADC2);
    // ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_TRGO;//trigger on rising edge of TIM8
    // ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;
    adc_enable_external_trigger_regular(ADC1,ADC_CR2_EXTSEL_TIM2_TRGO,ADC_CR2_EXTEN_RISING_EDGE);//ADC2 trigger via dual mode
    // ADC_InitStructure.ADC_NbrOfConversion = ADC_ANZ;//I think this one is clear :p
    // ADC_InitStructure.ADC_ScanConvMode = ENABLE;//The scan is configured in one channel
    adc_enable_scan_mode(ADC1);
    adc_enable_scan_mode(ADC2);
    // ADC_Init(SIN_ADC, &ADC_InitStructure);//Initialize ADC with the previous configuration
    // ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    // ADC_Init(COS_ADC, &ADC_InitStructure);//Initialize ADC with the previous configuration
    //
    // ADC_CommonInitTypeDef ADC_CommonInitStructure;
    // ADC_CommonInitStructure.ADC_Mode = ADC_DualMode_RegSimult;
    // ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
    adc_set_clk_prescale(ADC_CCR_ADCPRE_BY4);
    // ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_2;
    //TODO: set this bit: ADC_CCR_DMA_MODE_2
    ADC_CCR |= ADC_CCR_DMA_MODE_2;
    ADC_CCR |= ADC_CCR_DDS;
    // ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
    // ADC_CommonInit(&ADC_CommonInitStructure);
    // for(int i = 1;i<=ADC_ANZ;i++){16
    //     ADC_RegularChannelConfig(SIN_ADC, SIN_ADC_CHAN, i, RES_SampleTime);14
    //     ADC_RegularChannelConfig(COS_ADC, COS_ADC_CHAN, i, RES_SampleTime);15
    // }
    //TODO: sample time?
    adc_set_sample_time_on_all_channels(ADC1,ADC_SMPR_SMP_28CYC);
    adc_set_sample_time_on_all_channels(ADC2,ADC_SMPR_SMP_28CYC);
    uint8_t sin_ch[] = {14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14};
    uint8_t cos_ch[] = {15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15};
    adc_set_regular_sequence(ADC1, 16, sin_ch);
    adc_set_regular_sequence(ADC2, 16, cos_ch);
    
    adc_enable_dma(ADC1);
    adc_enable_dma(ADC2);
    //
    // ADC_MultiModeDMARequestAfterLastTransferCmd(ENABLE);
    adc_set_multi_mode(ADC_CCR_MULTI_DUAL_REGULAR_SIMUL);

    // //Enable ADC conversion
    // ADC_Cmd(SIN_ADC,ENABLE);
    adc_power_on(ADC1);
    // ADC_Cmd(COS_ADC,ENABLE);
    adc_power_on(ADC2);
    //
    // // DMA-Disable
    // DMA_Cmd(DMA2_Stream0, DISABLE);
    // DMA_DeInit(DMA2_Stream0);
    dma_stream_reset(DMA2, DMA_STREAM0);
    //
    // // DMA2-Config
    // DMA_InitStructure.DMA_Channel = DMA_Channel_0;
    dma_channel_select(DMA2, DMA_STREAM0, DMA_SxCR_CHSEL_0);
    // DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC->CDR;
    dma_set_peripheral_address(DMA2, DMA_STREAM0, (uint32_t)&ADC_CDR);
    // DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADC_DMA_Buffer;
    dma_set_memory_address(DMA2, DMA_STREAM0, (uint32_t)&ADC_DMA_Buffer);
    // DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
    dma_set_transfer_mode(DMA2, DMA_STREAM0, DMA_SxCR_DIR_PERIPHERAL_TO_MEM);
    // DMA_InitStructure.DMA_BufferSize = ADC_ANZ * PID_WAVES;
    dma_set_number_of_data(DMA2, DMA_STREAM0, ADC_ANZ * PID_WAVES);
    // DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    // DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    dma_enable_memory_increment_mode(DMA2, DMA_STREAM0);
    // DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
    dma_set_peripheral_size(DMA2, DMA_STREAM0, DMA_SxCR_PSIZE_32BIT);
    // DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
    dma_set_memory_size(DMA2, DMA_STREAM0, DMA_SxCR_MSIZE_32BIT);
    // DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    dma_enable_circular_mode(DMA2, DMA_STREAM0);
    // DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    dma_set_priority(DMA2, DMA_STREAM0, DMA_SxCR_PL_HIGH);
    // DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    // DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
    // DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    // DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    // DMA_Init(DMA2_Stream0, &DMA_InitStructure);

    // NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    // NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    // NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    // NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    // NVIC_Init(&NVIC_InitStructure);
    nvic_enable_irq(NVIC_TIM2_IRQ);
    nvic_set_priority(NVIC_TIM2_IRQ,0);
    //
    // NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;
    // NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    // NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    // NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    // NVIC_Init(&NVIC_InitStructure);
    nvic_enable_irq(NVIC_DMA2_STREAM0_IRQ);
    nvic_set_priority(NVIC_DMA2_STREAM0_IRQ,2);
    //
    // DMA_Cmd(DMA2_Stream0, ENABLE);
    dma_enable_stream(DMA2, DMA_STREAM0);
    //
    // DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);
    dma_enable_transfer_complete_interrupt(DMA2, DMA_STREAM0);
 }
