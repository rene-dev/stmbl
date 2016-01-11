#include "stm32f10x.h"
/* Cortex-M architecture allows plain C startup code
* from the linker file */
extern unsigned int __data_flash_start_addr, __data_flash_end_addr, __data_sram_start_addr, __data_sram_end_addr, __bss_start_addr, __bss_end_addr, __stack_end_addr;

/* Exception handlers prototypes */
void EmptyHandler(void);
void ResetHandler(void);
void NmiHandler(void);
void HardFaultHandler(void);
void MemManageHandler(void);
void BusFaultHandler(void);
void UsageFaultHandler(void);
void SvCallHandler(void);
void DebugMonitorHandler(void);

/* Exception and interrupt vector */
void (* const vector[])(void) __attribute__ ((section(".vector"))) __attribute__ ((used)) =
{
   (void (*)())&__stack_end_addr,  /* 0x0000_0000  stack address   */
   ResetHandler,                           /* 0x0000_0004  Reset                   */
   NmiHandler,                                     /* 0x0000_0008  NMI                             */
   HardFaultHandler,                       /* 0x0000_000C  HardFault               */
   MemManageHandler,                       /* 0x0000_0010  MemManage               */
   BusFaultHandler,                        /* 0x0000_0014  BusFault                */
   UsageFaultHandler,              /* 0x0000_0018  UsageFault              */
   0x0,                                                    /* 0x0000_001C  Reserved                */
   0x0,                                                    /* 0x0000_0020  Reserved                */
   0x0,                                                    /* 0x0000_0024  Reserved                */
   0x0,                                                    /* 0x0000_0028  Reserved                */
   SvCallHandler,                          /* 0x0000_002C  SVcall                  */
   DebugMonitorHandler,            /* 0x0000_0030  Debug Monitor   */
   0x0,                                                    /* 0x0000_0034  Reserved                */
   EmptyHandler,                           /* 0x0000_0038  PendSV                  */
   EmptyHandler,                           /* 0x0000_003C  SysTick                 */
};

/* stack */
char stack[4096] __attribute__ ((section (".stack"))) __attribute__ ((used)) = { 0 };

/* http://www.danielvik.com/2010/02/fast-memcpy-in-c.html */
inline void memcpy(void* dest, const void* src, u32 length) {
   char* dst8 = (char*)dest;
   char* src8 = (char*)src;

   while (length--) {
      *dst8++ = *src8++;
   }
}

inline void mempat(void* dest, u8 pattern, u32 length) {
   char* dst8 = (char*)dest;

   while (length--) {
      *dst8++ = pattern;
   }
}

__attribute__ ((noreturn)) void EmptyHandler(void) {
   for(;;) {}
}

__attribute__ ((noreturn)) void NmiHandler(void) {
   for(;;) {}
}

__attribute__ ((noreturn)) void HardFaultHandler(void) {
   for(;;) {}
}

__attribute__ ((noreturn)) void MemManageHandler(void) {
   for(;;) {}
}

__attribute__ ((noreturn)) void BusFaultHandler(void) {
   for(;;) {}
}

__attribute__ ((noreturn)) void UsageFaultHandler(void) {
   for(;;) {}
}

__attribute__ ((noreturn)) void SvCallHandler(void) {
   for(;;) {}
}

__attribute__ ((noreturn)) void DebugMonitorHandler(void) {
   for(;;) {}
}

int main(void);

__attribute__ ((noreturn)) void ResetHandler(void) {
   /* Copy .data to SRAM */
   memcpy(&__data_sram_start_addr, &__data_flash_start_addr, &__data_sram_end_addr - &__data_sram_start_addr);
   /* Set .bss to zero */
   mempat(&__bss_start_addr, 0x00, &__bss_end_addr - &__bss_start_addr);

   /* jump to main */
   main();

   /* should never return from main */
   for(;;) {}
}

int main(void) {
   /* #1 configuration
   * CPU now running at 8MHz (HSI) */

   /* flash settings */
   /* Enable or disable the Prefetch Buffer */
   FLASH->ACR =
      FLASH_ACR_PRFTBE
         /* FLASH_ACR_HLFCYA */
         | 0b010; /* FLASH_ACR_LATENCY: 2 wait states */

   /* Configure system clock
   * External oscillator: 8MHz
   * Max PLL multiplicator: x9
   * Max SYSCLK: 72MHz
   * Max AHB: SYSCLK = 72MHz
   * Max APB1: SYSCLK/2 = 36MHz
   * Max APB2: SYSCLK = 72MHz
   * Max ADC: SYSCLK/6 = 12MHz (max freq 14) */
   RCC->CFGR =
      RCC_CFGR_MCO_PLL                      /* Output clock is PLL/2 */
         /* USBPRE */
         | RCC_CFGR_PLLMULL9                     /* PLL multiplicator is 9 */
            | RCC_CFGR_PLLXTPRE_HSE         /* oscillator prescaler is /1 */
               | RCC_CFGR_PLLSRC_HSE           /* PLL input is external oscillator */
                  | RCC_CFGR_ADCPRE_DIV6          /* ADC prescaler is 6 */
                     | RCC_CFGR_PPRE2_DIV1           /* APB2 prescaler is 1 */
                        | RCC_CFGR_PPRE1_DIV2           /* APB1 prescaler is 2 */
                           | RCC_CFGR_HPRE_DIV1;           /* AHB prescaler is 1 */
   /* SWS */
   /* SW */

   {
      const u32 rcc_cr_hserdy_msk = 0x00020000;
      const u32 rcc_cr_pllrdy_msk = 0x02000000;
      const u32 rcc_cfgr_sw_msk   = 0x00000003;

      /* Clock control register */
      RCC->CR = RCC_CR_HSEON;         /* Enable external oscillator */

      /* Wait for locked external oscillator */
      while((RCC->CR & rcc_cr_hserdy_msk) != RCC_CR_HSERDY);

      /* Clock control register */
      RCC->CR |=
         /* PLLRDY */
         RCC_CR_PLLON;
      /* CSSON */
      /* HSEBYP */
      /* HSERDY */
      /* HSEON */
      /* HSICAL */
      /* HSITRIM */
      /* HSIRDY */
      /* HSION */

      /* Wait for locked PLL */
      while((RCC->CR & rcc_cr_pllrdy_msk) != RCC_CR_PLLRDY);

      RCC->CFGR &= ~0x00000003; /* clear */
      RCC->CFGR |= RCC_CFGR_SW_PLL;   /* SYSCLK is PLL */

      /* Wait for SYSCLK to be PPL */
      while((RCC->CFGR & rcc_cfgr_sw_msk) != RCC_CFGR_SW_PLL);
   }

   /* GPIO is in APB2 peripherals */
   /* enable APB2 clock */
   RCC->APB2ENR =
      RCC_APB2ENR_IOPAEN
         | RCC_APB2ENR_IOPBEN
            | RCC_APB2ENR_IOPCEN
               | RCC_APB2ENR_IOPDEN
                  | RCC_APB2ENR_USART1EN;

   /* Set PA to output mode */
   /* CRL: configuration register low (0..7) */
   GPIOD->CRL &= 0x00000000;       /* clear */
   GPIOD->CRL |= 0x33333333;       /* set */

   while(1);

}