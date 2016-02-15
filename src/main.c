/*
* This file is part of the stmbl project.
*
* Copyright (C) 2013-2015 Rene Hopf <renehopf@mac.com>
* Copyright (C) 2013-2015 Nico Stute <crinq@crinq.de>
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

#include "stm32f4xx_conf.h"
#include "scanf.h"
#include "hal.h"
#include "setup.h"
#include "eeprom.h"
#include "link.h"
#include "crc8.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "usb_cdc.h"


GLOBAL_HAL_PIN(rt_time);
GLOBAL_HAL_PIN(frt_time);
GLOBAL_HAL_PIN(rt_period_time);
GLOBAL_HAL_PIN(frt_period_time);

void Wait(uint32_t ms);

//hal interface
void enable_rt(){
   TIM_Cmd(TIM2, ENABLE);
}
void enable_frt(){
   TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
}
void disable_rt(){
   TIM_Cmd(TIM2, DISABLE);
}
void disable_frt(){
   TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
}

extern char _binary_obj_hv_hv_bin_start;
extern char _binary_obj_hv_hv_bin_size;
extern char _binary_obj_hv_hv_bin_end;

volatile uint64_t systime = 0;

void SysTick_Handler(void)
{
  systime++;
}

//20kHz
void TIM2_IRQHandler(void){
   TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
   switch(hal.frt_state){
      case FRT_STOP:
         return;
      case FRT_CALC:
         hal.frt_state = FRT_STOP;
         hal.hal_state = FRT_TOO_LONG;
         hal.rt_state = RT_STOP;
         return;
      case FRT_SLEEP:
         if(hal.active_frt_func > -1){
            hal.frt_state = FRT_STOP;
            hal.hal_state = MISC_ERROR;
            hal.rt_state = RT_STOP;
            return;
         }
         hal.frt_state = FRT_CALC;
   }

   GPIO_SetBits(GPIOB,GPIO_Pin_9);

   static unsigned int last_start = 0;
   unsigned int start = SysTick->VAL;

   if(last_start < start){
     last_start += SysTick->LOAD;
   }

   float period = ((float)(last_start - start)) / RCC_Clocks.HCLK_Frequency;
   last_start = start;

   for(hal.active_frt_func = 0; hal.active_frt_func < hal.frt_func_count; hal.active_frt_func++){//run all fast realtime hal functions
      hal.frt[hal.active_frt_func](period);
   }
   hal.active_frt_func = -1;

   unsigned int end = SysTick->VAL;
   if(start < end){
     start += SysTick->LOAD;
   }
   PIN(frt_time) = ((float)(start - end)) / RCC_Clocks.HCLK_Frequency;
   PIN(frt_period_time) = period;

   hal.frt_state = FRT_SLEEP;
   GPIO_ResetBits(GPIOB,GPIO_Pin_9);
}

//5 kHz interrupt for hal. at this point all ADCs have been sampled,
//see setup_res() in setup.c if you are interested in the magic behind this.
void DMA2_Stream0_IRQHandler(void){
   DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0);
   switch(hal.rt_state){
      case RT_STOP:
         return;
      case RT_CALC:
         hal.rt_state = RT_STOP;
         hal.hal_state = RT_TOO_LONG;
         hal.frt_state = FRT_STOP;
         return;
      case RT_SLEEP:
         if(hal.active_rt_func > -1){
            hal.rt_state = RT_STOP;
            hal.hal_state = MISC_ERROR;
            hal.frt_state = FRT_STOP;
            return;
         }
         hal.rt_state = RT_CALC;
   }

   GPIO_SetBits(GPIOB,GPIO_Pin_8);

   static unsigned int last_start = 0;
   unsigned int start = SysTick->VAL;

   if(last_start < start){
     last_start += SysTick->LOAD;
   }

   float period = ((float)(last_start - start)) / RCC_Clocks.HCLK_Frequency;
   last_start = start;

   for(hal.active_rt_func = 0; hal.active_rt_func < hal.rt_func_count; hal.active_rt_func++){//run all realtime hal functions
      hal.rt[hal.active_rt_func](period);
   }
   hal.active_rt_func = -1;

   unsigned int end = SysTick->VAL;
   if(start < end){
     start += SysTick->LOAD;
   }
   PIN(rt_time) = ((float)(start - end)) / RCC_Clocks.HCLK_Frequency;
   PIN(rt_period_time) = period;

   hal.rt_state = RT_SLEEP;
   GPIO_ResetBits(GPIOB,GPIO_Pin_8);
}

int main(void)
{
   // Relocate interrupt vectors
   //
   extern void *g_pfnVectors;
   SCB->VTOR = (uint32_t)&g_pfnVectors;

   float period = 0.0;
   int last_start = 0;
   int start = 0;
   int end = 0;

   setup();
   init_hal();

   set_comp_type("foo"); // default pin for mem errors
   HAL_PIN(bar) = 0.0;

   //feedback comps
   #include "comps/adc.comp"
   #include "comps/res.comp"
   #include "comps/enc_fb.comp"
   #include "comps/encm.comp"
   #include "comps/encs.comp"
   #include "comps/yaskawa.comp"
   //TODO: hyperface

   //command comps
   #include "comps/sserial.comp"
   #include "comps/sim.comp"
   #include "comps/enc_cmd.comp"
   #include "comps/en.comp"

   //PID
   #include "comps/stp.comp"
   #include "comps/rev.comp"
   #include "comps/rev.comp"
   #include "comps/vel.comp"
   #include "comps/vel.comp"
   #include "comps/cauto.comp"
   #include "comps/pid.comp"
   #include "comps/pmsm_t2c.comp"
   #include "comps/curpid.comp"
   #include "comps/pmsm.comp"
   #include "comps/pmsm_limits.comp"
   #include "comps/idq.comp"
   #include "comps/dq.comp"
   #include "comps/hv.comp"

   //other comps
   #include "comps/fault.comp"
   #include "comps/term.comp"
   #include "comps/io.comp"


   set_comp_type("net");
   HAL_PIN(enable) = 0.0;
   HAL_PIN(cmd) = 0.0;
   HAL_PIN(fb) = 0.0;
   HAL_PIN(fb_error) = 0.0;
   HAL_PIN(cmd_d) = 0.0;
   HAL_PIN(fb_d) = 0.0;
   HAL_PIN(core_temp0) = 0.0;
   HAL_PIN(core_temp1) = 0.0;
   HAL_PIN(motor_temp) = 0.0;
   HAL_PIN(rt_calc_time) = 0.0;
   HAL_PIN(frt_calc_time) = 0.0;
   HAL_PIN(nrt_calc_time) = 0.0;
   HAL_PIN(rt_period) = 0.0;
   HAL_PIN(frt_period) = 0.0;
   HAL_PIN(nrt_period) = 0.0;

   set_comp_type("conf");
   HAL_PIN(r) = 1.0;
   HAL_PIN(l) = 0.01;
   HAL_PIN(j) = KGCM2(0.26);
   HAL_PIN(psi) = 0.05;
   HAL_PIN(polecount) = 4.0;
   HAL_PIN(mot_type) = 0.0;//ac sync,async/dc,2phase
   HAL_PIN(out_rev) = 0.0;
   HAL_PIN(high_motor_temp) = 80.0;
   HAL_PIN(max_motor_temp) = 100.0;
   HAL_PIN(phase_time) = 0.5;
   HAL_PIN(phase_cur) = 1.0;

   HAL_PIN(max_vel) = RPM(1000.0);
   HAL_PIN(max_acc) = RPM(1000.0)/0.01;
   HAL_PIN(max_force) = 1.0;
   HAL_PIN(max_dc_cur) = 1.0;
   HAL_PIN(max_ac_cur) = 2.0;

   HAL_PIN(fb_type) = RES;
   HAL_PIN(fb_polecount) = 1.0;
   HAL_PIN(fb_offset) = 0.0;
   HAL_PIN(fb_rev) = 0.0;
   HAL_PIN(fb_res) = 1000.0;
   HAL_PIN(autophase) = 1.0;//constant,cauto,hfi

   HAL_PIN(cmd_type) = ENC;
   HAL_PIN(cmd_unit) = 0.0;//pos,vel,torque
   HAL_PIN(cmd_rev) = 0.0;
   HAL_PIN(cmd_res) = 2000.0;
   HAL_PIN(en_condition) = 0.0;
   HAL_PIN(error_out) = 0.0;
   HAL_PIN(pos_static) = 0.0;//track pos in disabled and error condition

   HAL_PIN(sin_offset) = 0.0;
   HAL_PIN(cos_offset) = 0.0;
   HAL_PIN(sin_gain) = 1.0;
   HAL_PIN(cos_gain) = 1.0;
   HAL_PIN(max_dc_volt) = 370.0;
   HAL_PIN(max_hv_temp) = 90.0;
   HAL_PIN(max_core_temp) = 55.0;
   HAL_PIN(max_pos_error) = M_PI / 2.0;
   HAL_PIN(high_dc_volt) = 350.0;
   HAL_PIN(low_dc_volt) = 12.0;
   HAL_PIN(high_hv_temp) = 70.0;
   HAL_PIN(fan_hv_temp) = 60.0;
   HAL_PIN(fan_core_temp) = 450.0;
   HAL_PIN(fan_motor_temp) = 60.0;

   HAL_PIN(p) = 0.99;
   HAL_PIN(pos_p) = 100.0;
   HAL_PIN(vel_p) = 1.0;
   HAL_PIN(acc_p) = 0.3;
   HAL_PIN(acc_pi) = 50.0;
   HAL_PIN(cur_p) = 0.0;
   HAL_PIN(cur_i) = 0.0;
   HAL_PIN(cur_ff) = 1.0;
   HAL_PIN(cur_ind) = 0.0;
   HAL_PIN(max_sat) = 0.2;

   rt_time_hal_pin = map_hal_pin("net0.rt_calc_time");
   frt_time_hal_pin = map_hal_pin("net0.frt_calc_time");
   rt_period_time_hal_pin = map_hal_pin("net0.rt_period");
   frt_period_time_hal_pin = map_hal_pin("net0.frt_period");

   for(int i = 0; i < hal.nrt_init_func_count; i++){ // run nrt init
      hal.nrt_init[i]();
   }

   link_pid();


   if(hal.pin_errors + hal.comp_errors == 0){
      start_hal();
   }
   else{
      hal.hal_state = MEM_ERROR;
   }

   while(1)//run non realtime stuff
   {
      start = SysTick->VAL;

      if(last_start < start){
        last_start += SysTick->LOAD;
      }

      period = ((float)(last_start - start)) / RCC_Clocks.HCLK_Frequency;
      last_start = start;

      for(hal.active_nrt_func = 0; hal.active_nrt_func < hal.nrt_func_count; hal.active_nrt_func++){//run all non realtime hal functions
         hal.nrt[hal.active_nrt_func](period);
      }
      hal.active_nrt_func = -1;

      end = SysTick->VAL;
      if(start < end){
        start += SysTick->LOAD;
      }
      PIN(nrt_calc_time) = ((float)(start - end)) / RCC_Clocks.HCLK_Frequency;
      PIN(nrt_period) = period;
      Wait(2);
   }
}

void Wait(uint32_t ms){
   uint64_t t = systime + ms;
   while(t >= systime){
   }
}
