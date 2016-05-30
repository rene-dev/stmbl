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

#include "scanf.h"
#include "hal.h"
#include "setup.h"
//#include "eeprom.h"
#include "link.h"
#include "crc8.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


GLOBAL_HAL_PIN(rt_time);
GLOBAL_HAL_PIN(frt_time);
GLOBAL_HAL_PIN(rt_period_time);
GLOBAL_HAL_PIN(frt_period_time);
GLOBAL_HAL_PIN(rt_time_ms);

void Wait(uint32_t ms);

//hal interface
void enable_frt(){
   //timer_enable_counter(TIM2);
}
void enable_rt(){
   timer_enable_irq(TIM2,TIM_DIER_UIE);
}
void disable_frt(){
   //timer_disable_counter(TIM2);
}
void disable_rt(){
   timer_disable_irq(TIM2,TIM_DIER_UIE);
}

volatile uint64_t systime = 0;

void SysTick_Handler(void)
{
  //systime++;
}

//1kHz
void tim2_isr(void){
   timer_clear_flag(TIM2,TIM_SR_UIF);
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

   static unsigned int last_start = 0;
   unsigned int start = systick_get_value();

   if(last_start < start){
     last_start += systick_get_reload();
   }

   float period = ((float)(last_start - start)) / rcc_ahb_frequency;
   last_start = start;
   systime++;
   PIN(rt_time_ms) = systime;
   for(hal.active_rt_func = 0; hal.active_rt_func < hal.rt_func_count; hal.active_rt_func++){//run all realtime hal functions
      hal.rt[hal.active_rt_func](period);
   }
   hal.active_rt_func = -1;

   unsigned int end = systick_get_value();
   if(start < end){
     start += systick_get_reload();
   }
   PIN(rt_time) = ((float)(start - end)) / rcc_ahb_frequency;
   PIN(rt_period_time) = period;

   hal.rt_state = RT_SLEEP;
}



void rcc_set_pllxtpre(uint32_t pllxtpre)
{
	RCC_CFGR = (RCC_CFGR & ~RCC_CFGR_PLLXTPRE) |
			(pllxtpre << 17);
}

#define RCC_CFGR_PLLXTPRE_HSE_CLK		0x0
#define RCC_CFGR_PLLXTPRE_HSE_CLK_DIV2		0x1

void rcc_clock_setup_in_hse_8mhz_out_72mhz(void)
{
	/* Enable internal high-speed oscillator. */
	rcc_osc_on(RCC_HSI);
	rcc_wait_for_osc_ready(RCC_HSI);

	/* Select HSI as SYSCLK source. */
	//rcc_set_sysclk_source(RCC_CFGR_SW_SYSCLKSEL_HSICLK);
   rcc_set_sysclk_source(RCC_CFGR_SW_HSI);

	/* Enable external high-speed oscillator 8MHz. */
	rcc_osc_on(RCC_HSE);
	rcc_wait_for_osc_ready(RCC_HSE);
	//rcc_set_sysclk_source(RCC_CFGR_SW_SYSCLKSEL_HSECLK);
   rcc_set_sysclk_source(RCC_CFGR_SW_HSE);

	/*
	 * Set prescalers for AHB, ADC, ABP1, ABP2.
	 * Do this before touching the PLL (TODO: why?).
	 */
	// rcc_set_hpre(RCC_CFGR_HPRE_SYSCLK_NODIV);    /* Set. 72MHz Max. 72MHz */
	// rcc_set_adcpre(RCC_CFGR_ADCPRE_PCLK2_DIV8);  /* Set.  9MHz Max. 14MHz */
	// rcc_set_ppre1(RCC_CFGR_PPRE1_HCLK_DIV2);     /* Set. 36MHz Max. 36MHz */
	// rcc_set_ppre2(RCC_CFGR_PPRE2_HCLK_NODIV);    /* Set. 72MHz Max. 72MHz */
   rcc_set_hpre(RCC_CFGR_HPRE_DIV_NONE);    /* Set. 72MHz Max. 72MHz */
   rcc_adc_prescale(RCC_CFGR2_ADCxPRES_PLL_CLK_DIV_1, RCC_CFGR2_ADCxPRES_PLL_CLK_DIV_1);  /* Set.  9MHz Max. 14MHz */
   rcc_set_ppre1(RCC_CFGR_PPRE1_DIV_2);     /* Set. 36MHz Max. 36MHz */
   rcc_set_ppre2(RCC_CFGR_PPRE2_DIV_NONE);    /* Set. 72MHz Max. 72MHz */

	/*
	 * Sysclk runs with 72MHz -> 2 waitstates.
	 * 0WS from 0-24MHz
	 * 1WS from 24-48MHz
	 * 2WS from 48-72MHz
	 */
	flash_set_ws(FLASH_ACR_LATENCY_2WS);

	/*
	 * Set the PLL multiplication factor to 9.
	 * 8MHz (external) * 9 (multiplier) = 72MHz
	 */
	//rcc_set_pll_multiplication_factor(RCC_CFGR_PLLMUL_PLL_CLK_MUL9);
   rcc_set_pll_multiplier(RCC_CFGR_PLLMUL_PLL_IN_CLK_X9);

	/* Select HSE as PLL source. */
	//rcc_set_pll_source(RCC_CFGR_PLLSRC_HSE_CLK);
   rcc_set_pll_source(RCC_CFGR_PLLSRC_HSE_PREDIV);

	/*
	 * External frequency undivided before entering PLL
	 * (only valid/needed for HSE).
	 */
	//rcc_set_pllxtpre(RCC_CFGR_PLLXTPRE_HSE_CLK);
   rcc_set_pllxtpre(RCC_CFGR_PLLXTPRE_HSE_CLK);

   rcc_usb_prescale_1_5();


	/* Enable PLL oscillator and wait for it to stabilize. */
	rcc_osc_on(RCC_PLL);
	rcc_wait_for_osc_ready(RCC_PLL);

	/* Select PLL as SYSCLK source. */
	//rcc_set_sysclk_source(RCC_CFGR_SW_SYSCLKSEL_PLLCLK);
   rcc_set_sysclk_source(RCC_CFGR_SW_PLL);

	/* Set the peripheral clock frequencies used */
	rcc_ahb_frequency = 72000000;
	rcc_apb1_frequency = 36000000;
	rcc_apb2_frequency = 72000000;
}

void setup_systick(){
   systick_set_reload(72000);
	systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
	systick_counter_enable();

	/* this done last */
	systick_interrupt_enable();
   //systick prio

   //NVIC_SetPriority(SysTick_IRQn, 14);
   nvic_set_priority(NVIC_SYSTICK_IRQ, 14);
   nvic_enable_irq(NVIC_SYSTICK_IRQ);
}

void setup_tim(){
   rcc_periph_clock_enable(RCC_TIM2);
   timer_reset(TIM2);
   timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
   timer_set_clock_division(TIM2, TIM_CR1_CKD_CK_INT);
   timer_direction_up(TIM2);
   timer_set_repetition_counter(TIM2, 0);
   //timer_enable_preload(TIM2);
   timer_set_period(TIM2, 900);
   timer_set_prescaler(TIM2, 19);
   timer_enable_update_event(TIM2);
   nvic_enable_irq(NVIC_TIM2_IRQ);
   nvic_set_priority(NVIC_TIM2_IRQ, 0);
   timer_enable_counter(TIM2);
   //timer_enable_irq(TIM2,TIM_DIER_UIE);
}


int main(void)
{
   //rcc_clock_setup_hse_3v3(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]);
   //rcc_clock_setup_hsi(&rcc_hsi_8mhz[RCC_CLOCK_48MHZ]);
   rcc_clock_setup_in_hse_8mhz_out_72mhz();
   setup_systick();
   cdc_init();
   setup_tim();
   rcc_periph_clock_enable(RCC_CRC);
   // Relocate interrupt vectors
   //
   extern void *vector_table;
   SCB_VTOR = (uint32_t)&vector_table;

   float period = 0.0;
   int last_start = 0;
   int start = 0;
   int end = 0;
   

   init_hal();

   set_comp_type("foo"); // default pin for mem errors
   HAL_PIN(bar) = 0.0;
   // 
   
   #include "comps/sim.comp"
   #include "comps/hw/f3troller/io.comp"
   #include "comps/term.comp"
   // 
   set_comp_type("net");
   HAL_PIN(core_temp) = 0.0;

   HAL_PIN(rt_calc_time) = 0.0;
   HAL_PIN(frt_calc_time) = 0.0;
   HAL_PIN(nrt_calc_time) = 0.0;
   HAL_PIN(rt_period) = 0.0;
   HAL_PIN(frt_period) = 0.0;
   HAL_PIN(nrt_period) = 0.0;
   HAL_PIN(time_ms) = 0.0;
   

   // 
   rt_time_hal_pin = map_hal_pin("net0.rt_calc_time");
   frt_time_hal_pin = map_hal_pin("net0.frt_calc_time");
   rt_period_time_hal_pin = map_hal_pin("net0.rt_period");
   frt_period_time_hal_pin = map_hal_pin("net0.frt_period");
   rt_time_ms_hal_pin = map_hal_pin("net0.time_ms");
   
   // 
   for(int i = 0; i < hal.nrt_init_func_count; i++){ // run nrt init
      hal.nrt_init[i]();
   }
   
   set_hal_pin("sim0.rt_prio", 1.0);
   set_hal_pin("term0.rt_prio", 2.0);
   set_hal_pin("io0.rt_prio", 1.0);
   
   
   if(hal.pin_errors + hal.comp_errors == 0){
      start_hal();
   }
   else{
      hal.hal_state = MEM_ERROR;
   }
   
   while(1)//run non realtime stuff
   {
      start = systick_get_value();
      cdc_poll();
      
      if(last_start < start){
        last_start += systick_get_reload();
      }
      // 
      period = ((float)(last_start - start)) / rcc_ahb_frequency;
      last_start = start;
      for(hal.active_nrt_func = 0; hal.active_nrt_func < hal.nrt_func_count; hal.active_nrt_func++){//run all non realtime hal functions
         hal.nrt[hal.active_nrt_func](period);
      }
      hal.active_nrt_func = -1;
      
      end = systick_get_value();
      if(start < end){
        start += systick_get_reload();
      }
      PIN(nrt_calc_time) = ((float)(start - end)) / rcc_ahb_frequency;
      PIN(nrt_period) = period;
   }
}

void Wait(uint32_t ms){
   uint64_t t = systime + ms;
   while(t >= systime){
   }
}
