/* Michael Pratt <michael@pratt.im> */

/* Generic system call library for STM32F4-DISCOVERY
 * Eliminates need for the huge STM libraries */
#include <stm32f4_discovery.h>
#ifndef SYSTEM_H_INCLUDED
#define SYSTEM_H_INCLUDED

/* We will be wanting some typedefs */
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;

/* System Initialization */
extern void SystemInit(void);

/* Exception Handlers */
void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);

#endif /* SYSTEM_H_INCLUDED */
