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

//#include "stm32f4xx_conf.h"
#include "hal.h"
#include "setup.h"
#include "defines.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "commands.h"

uint32_t hal_get_systick_value() {
  return (SysTick->VAL);
}

uint32_t hal_get_systick_reload() {
  return (SysTick->LOAD);
}

uint32_t hal_get_systick_freq() {
  return (RCC_Clocks.HCLK_Frequency);
}

volatile uint64_t systime = 0;

void SysTick_Handler(void) {
  systime++;
}

//20kHz
void TIM_SLAVE_HANDLER(void) {
  TIM_ClearITPendingBit(TIM_SLAVE, TIM_IT_Update);
  hal_run_frt();
  if(TIM_GetITStatus(TIM_SLAVE, TIM_IT_Update) == SET) {
    hal_stop();
    hal.hal_state = FRT_TOO_LONG;
  }
}

//5 kHz interrupt for hal. at this point all ADCs have been sampled,
//see setup_res() in setup.c if you are interested in the magic behind this.
void DMA2_Stream0_IRQHandler(void) {
  DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0);
  hal_run_rt();
  if(DMA_GetITStatus(DMA2_Stream0, DMA_IT_TCIF0) == SET) {
    hal_stop();
    hal.hal_state = RT_TOO_LONG;
  }
}

void bootloader(char *ptr) {
  hal_stop();

  NVIC_DisableIRQ(TIM_SLAVE_IRQ);
  NVIC_DisableIRQ(DMA2_Stream0_IRQn);
  NVIC_DisableIRQ(SysTick_IRQn);

  void (*SysMemBootJump)(void);
  volatile uint32_t addr = 0x1FFF0000;

  RCC_DeInit();
  SysTick->CTRL = 0;
  SysTick->LOAD = 0;
  SysTick->VAL = 0;
  
  RCC->AHB1RSTR = 0x22E017FF;
  RCC->AHB1RSTR = 0;
  RCC->AHB2RSTR = 0xF1;
  RCC->AHB2RSTR = 0;
  RCC->AHB3RSTR = 0x1;
  RCC->AHB3RSTR = 0;
  RCC->APB1RSTR = 0xF6FEC9FF;
  RCC->APB1RSTR = 0;
  RCC->APB2RSTR = 0x4777933;
  RCC->APB2RSTR = 0;

  SYSCFG->MEMRMP = 0x01;
  SysMemBootJump = (void (*)(void)) (*((uint32_t *)(addr + 4)));
  __set_MSP(*(uint32_t *)addr);
  SysMemBootJump();
}
COMMAND("bootloader", bootloader, "enter bootloader");

void nv_reset(char *ptr) {
  NVIC_SystemReset();
}
COMMAND("reset", nv_reset, "reset STMBL");

int main(void) {
  // Relocate interrupt vectors
  extern void *g_pfnVectors;
  SCB->VTOR = (uint32_t)&g_pfnVectors;

  setup();
  hal_init(0.0002, 0.00005);
  // hal load comps
  load_comp(comp_by_name("term"));
  hal_parse("flashloadconf");
  hal_parse("loadconf");
  hal_parse("start");

  TIM_Cmd(TIM_MASTER, ENABLE);
  TIM_ITConfig(TIM_SLAVE, TIM_IT_Update, ENABLE);

  while(1)  //run non realtime stuff
  {
    hal_run_nrt();
    //cdc_poll();
    Wait(1);
  }
}

void Wait(uint32_t ms) {
  uint64_t t = systime + ms;
  while(t >= systime) {
  }
}
