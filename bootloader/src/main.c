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

#define APP_START 0x08010000

int main(void){
   GPIO_InitTypeDef GPIO_InitDef;
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
   GPIO_InitDef.GPIO_Pin = GPIO_Pin_13;
   GPIO_InitDef.GPIO_Mode = GPIO_Mode_IN;
   GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
   GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_UP;
   GPIO_InitDef.GPIO_Speed = GPIO_Speed_2MHz;
   GPIO_Init(GPIOA, &GPIO_InitDef);
   
   uint32_t pin = !GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_13);
   
   GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
   GPIO_Init(GPIOA, &GPIO_InitDef);
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, DISABLE);
   
   void (*SysMemBootJump)(void);
   if ( (*((unsigned long *)0x2001C000) == 0xDEADBEEF) || pin) {//Memory map, datasheet
      *((unsigned long *)0x2001C000) =  0xCAFEFEED; //Reset bootloader trigger
      __set_MSP(0x20001000);
 	   //Point the PC to the System Memory reset vector (+4)
 	   //AN2606
      //Table 64. Bootloader device-dependent parameters
      SysMemBootJump = (void (*)(void)) (*((uint32_t *) 0x1FFF0004));
      SysMemBootJump();
      while (1);
   }else{
      uint32_t  stack = ((const uint32_t *)APP_START)[0];
      uint32_t  entry = ((const uint32_t *)APP_START)[1];
      asm volatile(
         "msr    msp, %0        \n\t"
         "bx     %1             \n\t"
         : : "r" (stack), "r" (entry)
      );
      while(1);
   }
}
