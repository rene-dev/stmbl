//
//  setup.c
//  test
//
//  Created by Rene on 09/12/13.
//  Copyright (c) 2013 Rene Hopf. All rights reserved.
//

#include "setup.h"

void setup_led(){
	RCC_AHB1PeriphClockCmd(LED_R_IO_RCC, ENABLE);
	RCC_AHB1PeriphClockCmd(LED_Y_IO_RCC, ENABLE);
	RCC_AHB1PeriphClockCmd(LED_G_IO_RCC, ENABLE);
	
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

    GPIO_InitStructure.GPIO_Pin   = LED_R_PIN;
    GPIO_Init(LED_R_PORT, &GPIO_InitStructure);
	
    GPIO_InitStructure.GPIO_Pin   = LED_Y_PIN;
    GPIO_Init(LED_Y_PORT, &GPIO_InitStructure);
	
    GPIO_InitStructure.GPIO_Pin   = LED_G_PIN;
    GPIO_Init(LED_G_PORT, &GPIO_InitStructure);
}

void setup(){

    //PC6 PB5
    //UB_ENCODER_TIM3_Init(ENC_T3_MODE_4AB, ENC_T3_A, 2000);
    //TIM4:PWM
    //TIM2:int

	 //messpin
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	 GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //int timer
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//wird in UB_USB_CDC_Init() nochmal gesetzt!

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

	setup_usart();
   setup_adc();
   setup_pid_timer();
	setup_led();

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

void setup_usart(){
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	RCC_APB1PeriphClockCmd(UART_DRV_RCC, ENABLE);
	RCC_AHB1PeriphClockCmd(UART_DRV_RX_IO_RCC, ENABLE);
	RCC_AHB1PeriphClockCmd(UART_DRV_TX_IO_RCC, ENABLE);

	//USART TX
	GPIO_PinAFConfig(UART_DRV_TX_PORT, UART_DRV_TX_PIN_SOURCE, UART_DRV_TX_AF_SOURCE);
	GPIO_InitStruct.GPIO_Pin = UART_DRV_TX_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(UART_DRV_TX_PORT, &GPIO_InitStruct);

	//USART RX
	GPIO_PinAFConfig(UART_DRV_RX_PORT, UART_DRV_RX_PIN_SOURCE, UART_DRV_RX_AF_SOURCE);
	GPIO_InitStruct.GPIO_Pin = UART_DRV_RX_PIN;
	GPIO_Init(UART_DRV_RX_PORT, &GPIO_InitStruct);

	USART_InitStruct.USART_BaudRate = 2000000;//2000000
	USART_InitStruct.USART_WordLength = USART_WordLength_9b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(UART_DRV, &USART_InitStruct);
	/* Enable the USART */
	USART_Cmd(UART_DRV, ENABLE);
	
    // Clock Enable
    RCC_AHB1PeriphClockCmd(UART_DRV_TX_DMA_RCC, ENABLE);

    // DMA-Disable
    DMA_Cmd(UART_DRV_TX_DMA, DISABLE);
    DMA_DeInit(UART_DRV_TX_DMA);

    // DMA2-Config
    DMA_InitStructure.DMA_Channel = UART_DRV_TX_DMA_CHAN;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(UART_DRV->DR);
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&UART_DMA_Buffer;
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_BufferSize = DATALENGTH*2+1;//uint32_t + start
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(UART_DRV_TX_DMA, &DMA_InitStructure);

    DMA_Cmd(UART_DRV_TX_DMA, ENABLE);

	USART_DMACmd(UART_DRV, USART_DMAReq_Tx, ENABLE);
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

// Setup ADC
void setup_adc(){
    RCC_AHB1PeriphClockCmd(SIN_IO_RCC, ENABLE);
    RCC_AHB1PeriphClockCmd(COS_IO_RCC, ENABLE);
    /* ADC clock enable */
    RCC_APB2PeriphClockCmd(SIN_ADC_RCC | COS_ADC_RCC, ENABLE);

    //Analog pin configuration
    GPIO_InitStructure.GPIO_Pin = SIN_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(SIN_PORT,&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = COS_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(COS_PORT,&GPIO_InitStructure);

    //ADC structure configuration
    ADC_DeInit();

    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//data converted will be shifted to right
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//Input voltage is converted into a 12bit number giving a maximum value of 4096
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; //the conversion is continuous, the input data is converted more than once
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_TRGO;//trigger on rising edge of TIM2
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
	 
    // Clock Enable
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

    // DMA-Disable
    DMA_Cmd(DMA2_Stream0, DISABLE);
    DMA_DeInit(DMA2_Stream0);

    // DMA2-Config
    DMA_InitStructure.DMA_Channel = DMA_Channel_0;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC->CDR;
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADC_DMA_Buffer;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
    DMA_InitStructure.DMA_BufferSize = ADC_ANZ;
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

    NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    DMA_Cmd(DMA2_Stream0, ENABLE);

    DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);
 }

void SysTick_Handler(void)
{
  time++;
}
