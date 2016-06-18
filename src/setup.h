//
//  setup.h
//  test
//
//  Created by Rene on 09/12/13.
//  Copyright (c) 2013 Rene Hopf. All rights reserved.
//

#pragma once

#include "stm32f4xx_hal.h"
#include "misc.h"
#include "version.h"
#include "common.h"
// #include "defines_res.h"

//states

// state -10 = reset state, -6 = fb error, -5 = sat error, -4 = over current, -3 = pos_error, -2 = over temperature, -1 = over voltage, 0 = disabled, 1 = enabled, 2 = phasing
#define STATE_RESET -10
#define STATE_FB_ERROR -6
#define STATE_SAT_ERROR -5
#define STATE_OVR_CURR -4
#define STATE_POS_ERROR -3
#define STATE_OVR_TEMP -2
#define STATE_OVR_VOLT -1
#define STATE_DISABLED 0
#define STATE_ENABLED 1
#define STATE_PHASING 2

#define U_ID ((__IO uint32_t *)0x1FFF7A10)


//#define mag_res 5250
#define mag_res 8400

//sample times for F4: 3,15,28,56,84,112,144,480
#define RES_SampleTime ADC_SampleTime_28Cycles

#define  ADC_ANZ 16
#define  PID_WAVES 4

void setup(void);
void setup_rt_time(void);

volatile uint32_t ADC_DMA_Buffer[ADC_ANZ * PID_WAVES];

//TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
// TIM_OCInitTypeDef TIM_OCInitStructure;
// NVIC_InitTypeDef NVIC_InitStructure;
// GPIO_InitTypeDef GPIO_InitStructure;
// DMA_InitTypeDef DMA_InitStructure;
// ADC_InitTypeDef ADC_InitStructure;
// RCC_ClocksTypeDef RCC_Clocks;
