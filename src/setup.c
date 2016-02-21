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
   //RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_DMA1 | RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_CRC, ENABLE);
   
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
