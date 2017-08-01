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
#include "hal.h"
#include "setup.h"
#include "defines.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "commands.h"

uint32_t hal_get_systick_value(){
   return(SysTick->VAL);
}

uint32_t hal_get_systick_reload(){
   return(SysTick->LOAD);
}

uint32_t hal_get_systick_freq(){
   return(RCC_Clocks.HCLK_Frequency);
}

volatile uint64_t systime = 0;

void SysTick_Handler(void)
{
  systime++;
}

//20kHz
void TIM_SLAVE_HANDLER(void){
   TIM_ClearITPendingBit(TIM_SLAVE,TIM_IT_Update);
   hal_run_frt();
   if(TIM_GetITStatus(TIM_SLAVE,TIM_IT_Update) == SET){
      hal_stop();
      hal.hal_state = FRT_TOO_LONG;
   }
}

//5 kHz interrupt for hal. at this point all ADCs have been sampled,
//see setup_res() in setup.c if you are interested in the magic behind this.
void DMA2_Stream0_IRQHandler(void){
   GPIOD->BSRRL |= GPIO_Pin_1;
   DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0);
   hal_run_rt();
   if(DMA_GetITStatus(DMA2_Stream0, DMA_IT_TCIF0) == SET){
      hal_stop();
      hal.hal_state = RT_TOO_LONG;
   }
   GPIOD->BSRRH |= GPIO_Pin_1;
}

void bootloader(char * ptr){
  *((unsigned long *)0x2001C000) = 0xDEADBEEF;//set bootloader trigger
  NVIC_SystemReset();
}
COMMAND("bootloader", bootloader, "enter bootloader");

void nv_reset(char * ptr){
  NVIC_SystemReset();
}
COMMAND("reset", nv_reset, "reset STMBL");


char config[15*1024];
const char* config_ro = (char*)0x08008000;
//TODO: crc
void flashloadconf(char * ptr){
   strncpy(config,config_ro,sizeof(config));
}
COMMAND("flashloadconf", flashloadconf, "load config from flash");

void flashsaveconf(char * ptr){
   printf("erasing flash page...\n");
   FLASH_Unlock();
   if(FLASH_EraseSector(FLASH_Sector_2,VoltageRange_3) != FLASH_COMPLETE){
      printf("error!\n");
      FLASH_Lock();
      return;
   }
   printf("saving conf\n");
   int i = 0;
   int ret = 0;
   do{
      ret = FLASH_ProgramByte((uint32_t)(config_ro + i), config[i]) != FLASH_COMPLETE;
      if(ret){
         printf("error writing %i\n",ret);
         break;
      }
   }while(config[i++] != 0);//TODO: check length
   printf("OK %i bytes written\n",i);
   FLASH_Lock();
}
COMMAND("flashsaveconf", flashsaveconf, "save config to flash");

void loadconf(char * ptr){
   hal_parse(config);
}
COMMAND("loadconf", loadconf, "parse config");

void showconf(char * ptr){
   printf("%s",config_ro);
}
COMMAND("showconf", showconf, "show config");

void appendconf(char * ptr){
   printf("adding %s\n",ptr);
   strncat(config,ptr,sizeof(config) - 1);
   strncat(config,"\n",sizeof(config) - 1);
}
COMMAND("appendconf", appendconf, "append string to config");

void deleteconf(char * ptr){
   config[0] = '\0';
}
COMMAND("deleteconf", deleteconf, "delete config");



int main(void)
{
   // Relocate interrupt vectors
   //
   extern void *g_pfnVectors;
   SCB->VTOR = (uint32_t)&g_pfnVectors;

   setup();
   hal_init(0.0002, 0.00005);
   // hal load comps
   load_comp(comp_by_name("term"));
   hal_parse("flashloadconf");
   hal_parse("loadconf");
   hal_parse("start");
   
   // load_comp(comp_by_name("sim"));
   // load_comp(comp_by_name("io"));
   // load_comp(comp_by_name("encm"));
   // load_comp(comp_by_name("hv"));
   // load_comp(comp_by_name("hal_test"));
   //hal_parse("term0.rt_prio = 20");
   // hal_parse("load conf\nload adc\n        load          reslimit\n# foo bar\nload rev\nload pid");
   // hal_parse("encm0.rt_prio = 1");
   // hal_parse("sim0.rt_prio = 2");
   // hal_parse("io0.rt_prio = 10");
   // hal_parse("hv0.rt_prio = 6");
   // hal_parse("hal_test0.rt_prio = 9");
   // hal_parse("hal_test0.frt_prio = 9");
   // hal parse config
   // hal_init_nrt();
   // error foo
   //hal_start();
   
   TIM_Cmd(TIM_MASTER, ENABLE);
   TIM_ITConfig(TIM_SLAVE, TIM_IT_Update, ENABLE);

   while(1)//run non realtime stuff
   {
      hal_run_nrt();
      //cdc_poll();
      Wait(1);
   }
}

void Wait(uint32_t ms){
   uint64_t t = systime + ms;
   while(t >= systime){
   }
}
