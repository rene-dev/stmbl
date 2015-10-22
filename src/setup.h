//
//  setup.h
//  test
//
//  Created by Rene on 09/12/13.
//  Copyright (c) 2013 Rene Hopf. All rights reserved.
//

#ifndef test_setup_h
#define test_setup_h

#include "stm32f4_discovery.h"
#include "stm32f4xx_conf.h"
#include "misc.h"
#include "../../stm32f103/inc/common.h"
#include "defines_res.h"

#include "stm32_ub_usb_cdc.h"

//#define mag_res 5250
#define mag_res 8400

//sample times for F4: 3,15,28,56,84,112,144,480
#define RES_SampleTime ADC_SampleTime_28Cycles

#define  ADC_ANZ 16
#define  PID_WAVES 4

void setup();
void setup_res();
void SysTick_Handler(void);

volatile unsigned long systime;

volatile uint32_t ADC_DMA_Buffer[ADC_ANZ * PID_WAVES];

TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
TIM_OCInitTypeDef TIM_OCInitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
GPIO_InitTypeDef GPIO_InitStructure;
DAC_InitTypeDef DAC_InitStructure;
DMA_InitTypeDef DMA_InitStructure;
ADC_InitTypeDef ADC_InitStructure;
RCC_ClocksTypeDef RCC_Clocks;



#endif
