//
//  setup.h
//  test
//
//  Created by Rene on 09/12/13.
//  Copyright (c) 2013 Rene Hopf. All rights reserved.
//

#ifndef test_setup_h
#define test_setup_h

#include <stm32f4_discovery.h>
#include <stm32f4xx_conf.h>
#include "stm32_ub_dac_dma.h"
#include "stm32_ub_encoder_tim3.h"

//#define mag_res 5250
#define mag_res 8400

void setup();
void SysTick_Handler(void);

volatile int time;

TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
TIM_OCInitTypeDef TIM_OCInitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
GPIO_InitTypeDef GPIO_InitStructure;
DAC_InitTypeDef DAC_InitStructure;
DMA_InitTypeDef DMA_InitStructure;
ADC_InitTypeDef ADC_InitStructure;

#endif
