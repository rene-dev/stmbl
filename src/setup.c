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
    
    //res erreger
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    
    //messpin
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
      
    //enable
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

  
    /* int set up, TIM2*/
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 420;//20kHz
    TIM_TimeBaseStructure.TIM_Prescaler = 9;
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
    
    setup_pwm();
    setup_adc();
    setup_pid_timer();

	// systick timer
	time = 0;
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);
	SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);
    //systick prio
    //NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1); 
    
    #ifdef USBTERM
    UB_USB_CDC_Init();
    #endif
}

//setup PWM using TIM4
void setup_pwm(){
    /* TIM4 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    /* GPIOD clock enable */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4);

    /* pwm set up, TIM4*/
    /* Compute the prescaler value */       // 168MHz/2         / pwm frq / pwm res - 1
    uint16_t PrescalerValue = (uint16_t) ((SystemCoreClock /2) / 10000 / mag_res) - 1; // = 4

    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = mag_res;
    TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    
    /* PWM1 Mode configuration: Channel1 */
    TIM_OC1Init(TIM4, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

    /* PWM1 Mode configuration: Channel2 */
    TIM_OC2Init(TIM4, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);

    /* PWM1 Mode configuration: Channel4 */
    TIM_OC4Init(TIM4, &TIM_OCInitStructure);
    TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
}

void setup_pid_timer(){
    /* TIM5 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

    /* pid int set up, TIM5*/
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 420;//20kHz
    TIM_TimeBaseStructure.TIM_Prescaler = 9;
    //TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
    TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
    
    /* int NVIC setup */
    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    NVIC_Init(&NVIC_InitStructure);    
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
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//no trigger for conversion
    ADC_InitStructure.ADC_NbrOfConversion = 1;//I think this one is clear :p
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;//The scan is configured in one channel
    ADC_Init(ADC1,&ADC_InitStructure);//Initialize ADC with the previous configuration
    ADC_Init(ADC2,&ADC_InitStructure);//Initialize ADC with the previous configuration

    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
    ADC_CommonInit(&ADC_CommonInitStructure);

    //Enable ADC conversion
    ADC_Cmd(ADC1,ENABLE);
    ADC_Cmd(ADC2,ENABLE);

    //Select the channel to be read from
    ADC_RegularChannelConfig(ADC1,RES_SIN_CHANNEL,1,RES_SampleTime);
    ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);
    ADC_EOCOnEachRegularChannelCmd(ADC1,ENABLE);

    ADC_RegularChannelConfig(ADC2,RES_COS_CHANNEL,1,RES_SampleTime);
    ADC_ITConfig(ADC2,ADC_IT_EOC,DISABLE);
    ADC_EOCOnEachRegularChannelCmd(ADC2,DISABLE);

    // analog NVIC
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void SysTick_Handler(void)
{
  time++;
}
