//
//  setup.h
//  test
//
//  Created by Rene on 09/12/13.
//  Copyright (c) 2013 Rene Hopf. All rights reserved.
//

#pragma once

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/dma.h>
#include <libopencm3/stm32/adc.h>
#include <libopencm3/stm32/crc.h>
#include <libopencm3/usb/usbd.h>
#include <libopencm3/usb/cdc.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/cm3/scb.h>

#include "misc.h"
#include "version.h"
#include "common.h"
#include "defines_res.h"
#include "cdc.h"

#define U_ID ((volatile uint32_t *)0x1FFF7A10)//TODO: should be in opencm3


//#define mag_res 5250
#define mag_res 8400

//sample times for F4: 3,15,28,56,84,112,144,480
//#define RES_SampleTime ADC_SampleTime_28Cycles

#define  ADC_ANZ 16
#define  PID_WAVES 4

void setup(void);
void setup_res(void);

volatile uint32_t ADC_DMA_Buffer[ADC_ANZ * PID_WAVES];
