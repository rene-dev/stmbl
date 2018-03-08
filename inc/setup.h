//
//  setup.h
//  test
//
//  Created by Rene on 09/12/13.
//  Copyright (c) 2013 Rene Hopf. All rights reserved.
//

#pragma once

#include "stm32f4xx_conf.h"
//#include "misc.h"
#include "version.h"
#include "common.h"
#include "hw/hw.h"
#define U_ID ((__IO uint32_t *)0x1FFF7A10)

void setup(void);
void setup_res(void);

extern volatile uint32_t ADC_DMA_Buffer0[ADC_TR_COUNT * PID_WAVES * (ADC_OVER_FB0 + ADC_OVER_FB1)];
extern volatile uint32_t ADC_DMA_Buffer1[ADC_TR_COUNT * PID_WAVES * (ADC_OVER_FB0 + ADC_OVER_FB1)];

RCC_ClocksTypeDef RCC_Clocks;
