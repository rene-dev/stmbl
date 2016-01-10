#include "stm32f10x.h"
#include "version.h"
#include "common.h"

#define APP_START 0x08001000
#define APP_END   0x08008000
#define APP_RANGE_VALID(a, s) (!(((a) | (s)) & 3) && (a) >= APP_START && ((a) + (s)) <= APP_END)
#define VERSION_INFO_OFFSET 0x188
static volatile const struct version_info *app_info = (void*)(APP_START + VERSION_INFO_OFFSET);

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
inline void memcpy(void *dest, const void *src, u32 length) {
   char *dst8 = (char *) dest;
   char *src8 = (char *) src;

   while (length--) {
      *dst8++ = *src8++;
   }
}

inline void mempat(void *dest, u8 pattern, u32 length) {
   char *dst8 = (char *) dest;

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

static int app_ok(void)
{
    if (!APP_RANGE_VALID(APP_START, app_info->image_size)) {
        return 0;
    }

    CRC_ResetDR();
    uint32_t crc = CRC_CalcBlockCRC((uint32_t *) APP_START, app_info->image_size / 4);

    if (crc != 0) {
        return 0;
    }

    return 1;
}
static void rcc_config(void)
{
    /* Enable external oscillator */
    RCC->CR |= ((uint32_t) RCC_CR_HSEON);

    /* Wait for locked external oscillator */
    while ((RCC->CR & RCC_CR_HSERDY) != RCC_CR_HSERDY);

    /* Enable Prefetch Buffer */
    FLASH->ACR |= FLASH_ACR_PRFTBE;

    /* Flash 2 wait state */
    FLASH->ACR &= (uint32_t) ((uint32_t)~FLASH_ACR_LATENCY);
    FLASH->ACR |= (uint32_t) FLASH_ACR_LATENCY_2;

    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE |
                                        RCC_CFGR_PLLMULL));

    /* PLLCLK = HSE * 9 = 72 MHz, HCLK = SYSCLK, PCLK2 = HCLK, PCLK1 = HCLK */
    RCC->CFGR |= (uint32_t) (RCC_CFGR_PLLSRC_HSE | RCC_CFGR_PLLMULL9 | RCC_CFGR_HPRE_DIV1 | RCC_CFGR_PPRE2_DIV1 | RCC_CFGR_PPRE1_DIV2 | RCC_CFGR_ADCPRE_DIV6);

    /* Enable PLL */
    RCC->CR |= RCC_CR_PLLON;

    /* Wait till PLL is ready */
    while((RCC->CR & RCC_CR_PLLRDY) == 0);

    /* Select PLL as system clock source */
    RCC->CFGR &= (uint32_t) ((uint32_t) ~(RCC_CFGR_SW));
    RCC->CFGR |= (uint32_t) RCC_CFGR_SW_PLL;

    /* Wait till PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t) RCC_CFGR_SWS) != (uint32_t) 0x08);

    /* enable GPIOA-C and AFIO clock */
    RCC->APB2ENR = RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPCEN | RCC_APB2ENR_AFIOEN;
    /* enabe CRC unit clock */
    RCC->AHBENR |= RCC_AHBENR_CRCEN;
    /* enable usart2 clock */
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
}

static void io_init(void)
{
    // init usart
    GPIOA->CRL = 0x4444EB44; // set gpioa[2,3] to 50 MHz, AFIO; 2 -> pp, 3 -> open drain

    #define APBCLOCK 36000000

    /* Integer part computing in case Oversampling mode is 16 Samples */
    #define INTEGERDIVIDER ((25 * APBCLOCK) / (4 * (DATABAUD)))

    uint32_t tmpreg = (INTEGERDIVIDER / 100) << 4;

    /* Determine the fractional part */
    uint32_t fractionaldivider = INTEGERDIVIDER - (100 * (tmpreg >> 4));

    tmpreg |= ((((fractionaldivider * 16) + 50) / 100)) & ((uint8_t) 0x0F);

    /* Write to USART BRR */
    USART2->BRR = (uint16_t) tmpreg;
    USART2->CR1 = USART_Clock_Enable | USART_Mode_Rx | USART_Mode_Tx; // configure usart: rx, tx
    // init led
    GPIOC->CRL = 0x44444222; // set gpioc[2-0] to pp, 2 MHz, output
}

int main(void)
{
    rcc_config();
    io_init();
    /* Configure system clock
    * External oscillator: 8MHz
    * Max PLL multiplicator: x9
    * Max SYSCLK: 72MHz
    * Max AHB: SYSCLK = 72MHz
    * Max APB1: SYSCLK/2 = 36MHz
    * Max APB2: SYSCLK = 72MHz
    * Max ADC: SYSCLK/6 = 12MHz (max freq 14) */
//    RCC->CFGR =
//      RCC_CFGR_MCO_PLL                      /* Output clock is PLL/2 */
//          | RCC_CFGR_PLLXTPRE_HSE         /* oscillator prescaler is /1 */

    GPIOC->BSRR = 0x01; // red led on
    if (  !app_ok()) {//Memory map, datasheet (*((unsigned long *) 0x2001C000) == 0xDEADBEEF) ||
//        *((unsigned long *) 0x2001C000) = 0xCAFEFEED; //Reset bootloader trigger
        GPIOC->BSRR = 0x10002; //gelb
        while (1);
    } else {
        GPIOC->BSRR = 0x00004; // rot, grün
        uint32_t  stack = ((const uint32_t *) APP_START)[0];
        uint32_t  entry = ((const uint32_t *) APP_START)[1];
        asm volatile(
            "msr    msp, %0        \n\t"
            "bx     %1             \n\t"
            : : "r" (stack), "r" (entry)
        );
        while (1);
    }
}

