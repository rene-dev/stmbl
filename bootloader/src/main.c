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
#include "version.h"

#if __GNUC__ < 5
#error gcc to old (< 5.0)
#endif

#define APP_START 0x08010000
#define APP_END 0x08100000
#define APP_RANGE_VALID(a, s) (!(((a) | (s)) & 3) && (a) >= APP_START && ((a) + (s)) <= APP_END)
#define VERSION_INFO_OFFSET 0x188
const version_info_t *app_info = (void *)(APP_START + VERSION_INFO_OFFSET);

static int app_ok(void) {
  if(!APP_RANGE_VALID(APP_START, app_info->image_size)) {
    return 0;
  }
  CRC_ResetDR();
  uint32_t crc = CRC_CalcBlockCRC((uint32_t *)APP_START, app_info->image_size / 4);

  if(crc != 0) {
    return 0;
  }

  return 1;
}

int main(void) {
  RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_CRC, ENABLE);

  void (*SysMemBootJump)(void);
  if(!app_ok() || (RCC->CSR & (RCC_CSR_WDGRSTF | RCC_CSR_WWDGRSTF))) {  //Memory map, datasheet
    // reset rcc reset flag
    RCC->CSR |= RCC_CSR_RMVF;

    __set_MSP(0x20001000);

    //Point the PC to the System Memory reset vector (+4)
    //AN2606
    //Table 64. Bootloader device-dependent parameters
    SysMemBootJump = (void (*)(void))(*((uint32_t *)0x1FFF0004));
    SysMemBootJump();
    while(1)
      ;
  }
  else {
    // enable access
    IWDG->KR = 0x5555;

    // set prescalser 32
    IWDG->PR = 3;
    
    // while(IWDG->SR & IWDG_SR_PVU) {
    // }

    // set reaload 0.5s
    IWDG->RLR = 0.5 * 32000 / 32;

    // start
    IWDG->KR = 0xCCCC;

    // reset
    IWDG->KR = 0xAAAA;

    uint32_t stack = ((const uint32_t *)APP_START)[0];
    uint32_t entry = ((const uint32_t *)APP_START)[1];
    asm volatile(
        "msr    msp, %0        \n\t"
        "bx     %1             \n\t"
        :
        : "r"(stack), "r"(entry));
    while(1)
      ;
  }
}
