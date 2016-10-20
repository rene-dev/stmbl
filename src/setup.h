//
//  setup.h
//  test
//
//  Created by Rene on 09/12/13.
//  Copyright (c) 2013 Rene Hopf. All rights reserved.
//

#pragma once

#include "stm32f4xx_conf.h"
#include "misc.h"
#include "version.h"
#include "common.h"
#include "defines_res.h"
#define U_ID ((__IO uint32_t *)0x1FFF7A10)


//#define mag_res 5250
#define mag_res 8400

//sample times for F4: 3,15,28,56,84,112,144,480
#define RES_SampleTime ADC_SampleTime_28Cycles

#define  ADC_ANZ 16
#define  PID_WAVES 4

void setup(void);
void setup_res(void);

volatile uint32_t ADC_DMA_Buffer[ADC_ANZ * PID_WAVES];

TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
TIM_OCInitTypeDef TIM_OCInitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
GPIO_InitTypeDef GPIO_InitStructure;
DMA_InitTypeDef DMA_InitStructure;
ADC_InitTypeDef ADC_InitStructure;
RCC_ClocksTypeDef RCC_Clocks;
