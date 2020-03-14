/*
* This file is part of the stmbl project.
*
* Copyright (C) 2019 Rene Hopf <renehopf@mac.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"
#include "stm32f4xx_conf.h"
#include "hw/hw.h"

HAL_COMP(hx);

HAL_PIN(out0);
HAL_PIN(out1);
HAL_PIN(gain);
HAL_PIN(offset);
HAL_PIN(sleep);
HAL_PIN(clk_inv);
HAL_PIN(data_inv);
HAL_PIN(timer);
HAL_PIN(time);

struct hx_ctx_t {
  uint32_t error;
};

static void nopsleep(uint32_t t){
  while(t > 0){
    asm volatile("nop");
    t--;
  }
}

static void nrt_init(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  //struct hx_ctx_t *ctx = (struct hx_ctx_t *)ctx_ptr;
  struct hx_pin_ctx_t * pins = (struct hx_pin_ctx_t *)pin_ptr;

  PIN(sleep) = 20;
  PIN(time) = 0.01;
  PIN(gain) = 1;
}

static void hw_init(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  //struct hx_ctx_t *ctx = (struct hx_ctx_t *)ctx_ptr;
  //struct hx_pin_ctx_t * pins = (struct hx_pin_ctx_t *)pin_ptr;
  GPIO_InitTypeDef GPIO_InitStruct;

  //TX enable Z
  GPIO_InitStruct.GPIO_Pin   = FB1_Z_TXEN_PIN;
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(FB1_Z_TXEN_PORT, &GPIO_InitStruct);
  GPIO_SetBits(FB1_Z_TXEN_PORT, FB1_Z_TXEN_PIN);

  // output Z
  GPIO_InitStruct.GPIO_Pin   = FB1_Z_PIN;
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(FB1_Z_PORT, &GPIO_InitStruct);
}

//TODO: plausibility, saturation, channel/gain config, 2 chips

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  //struct hx_ctx_t *ctx      = (struct hx_ctx_t *)ctx_ptr;
  struct hx_pin_ctx_t *pins = (struct hx_pin_ctx_t *)pin_ptr;
  uint32_t value0 = 0;
  uint32_t value1 = 0;

  int sleep = CLAMP(PIN(sleep), 1, 50);

  if(PIN(timer) > PIN(time)){
    if((PIN(data_inv) > 0.0) ? (GPIO_ReadInputDataBit(FB1_A_PORT, FB1_A_PIN)) : (!GPIO_ReadInputDataBit(FB1_A_PORT, FB1_A_PIN))){//data line low = conversion ready
      for(int i = 0 ; i < 24;i++){
        if(PIN(clk_inv) > 0.0){
          nopsleep(sleep);
          GPIO_ResetBits(FB1_Z_PORT, FB1_Z_PIN);
          nopsleep(sleep);
          GPIO_SetBits(FB1_Z_PORT, FB1_Z_PIN);
        }
        else{
          nopsleep(sleep);
          GPIO_SetBits(FB1_Z_PORT, FB1_Z_PIN);
          nopsleep(sleep);
          GPIO_ResetBits(FB1_Z_PORT, FB1_Z_PIN);
        }
        if((PIN(data_inv) > 0.0) ? (GPIO_ReadInputDataBit(FB1_A_PORT, FB1_A_PIN)) : (!GPIO_ReadInputDataBit(FB1_A_PORT, FB1_A_PIN))){//dout = 1
          value0++;
        }
        value0 = value0 << 1;
        if((PIN(data_inv) > 0.0) ? (GPIO_ReadInputDataBit(FB1_B_PORT, FB1_B_PIN)) : (!GPIO_ReadInputDataBit(FB1_B_PORT, FB1_B_PIN))){//dout = 1
          value1++;
        }
        value1 = value1 << 1;
      }
      //clock additional config bits
      if(PIN(clk_inv) > 0.0){
        nopsleep(sleep);
        GPIO_ResetBits(FB1_Z_PORT, FB1_Z_PIN);
        nopsleep(sleep);
        GPIO_SetBits(FB1_Z_PORT, FB1_Z_PIN);
      }
      else{
        nopsleep(sleep);
        GPIO_SetBits(FB1_Z_PORT, FB1_Z_PIN);
        nopsleep(sleep);
        GPIO_ResetBits(FB1_Z_PORT, FB1_Z_PIN);
      }

      if(value0 & 0x800000){//if 24th bit is set, pad others, to get 2 complement number
        value0 |= 0xff000000;
      }
      int32_t sint = *((int32_t*)(&value0));
      PIN(out0) = ((float)sint/(float)0x7fffff)*PIN(gain)+PIN(offset);

      if(value1 & 0x800000){//if 24th bit is set, pad others, to get 2 complement number
        value1 |= 0xff000000;
      }
      sint = *((int32_t*)(&value1));
      PIN(out1) = ((float)sint/(float)0x7fffff)*PIN(gain)+PIN(offset);
      PIN(timer) = 0.0;
    }
  }
  PIN(timer) += period;
}

hal_comp_t hx_comp_struct = {
    .name      = "hx",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = nrt_init,
    .hw_init   = hw_init,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = sizeof(struct hx_ctx_t),
    .pin_count = sizeof(struct hx_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
