#include "f103.h"

#define STACK_TOP 0x20002000   // just a tiny stack for demo

static void nmi_handler(void);
static void hardfault_handler(void);
int main(void);

// Define the vector table
uint32_t *myvectors[4]
__attribute__ ((section("vectors"))) = {
    (uint32_t *) STACK_TOP,         // stack pointer
    (uint32_t *) main,              // code entry point
    (uint32_t *) nmi_handler,       // NMI handler (not really)
    (uint32_t *) hardfault_handler  // hard fault handler
};

void clock_setup(){ // 8MHz HSE -> 72MHz 
   FLASH->ACR.LATENCY = 2; // flash latency = 2 wait states
   
   RCC->CR.HSEON = 1; // enable HSE
   RCC->CFGR.PLLXTPRE = 0; // HSE prediv = 1
   while(RCC->CR.HSERDY == 0){} // wait for HSE startup
   RCC->CFGR.PLLSRC = 1; // pll input = HSE
   RCC->CFGR.PLLMUL = 7; // pll multiplicator = 9
   RCC->CR.PLLON = 1; // enable pll
   while(RCC->CR.PLLRDY == 0){} // wait for pll startup
   RCC->CFGR.HPRE = 0; // HCLK prescaler = 1
   RCC->CFGR.SW = 2; // SYSCLK = PLL
   while(RCC->CFGR.SWS != 2){} // wait for SYSCLK switch
   RCC->CFGR.PPRE1 = 4; // APB1 prescaler = 2
   RCC->CFGR.PPRE2 = 0; // APB2 prescaler = 1
   RCC->CFGR.ADCPRE = 2; // ADC prescaler = 6
}

void gpio_setup(){
   RCC->APB2ENR.IOPCEN = 1; // enable GPIOC
   GPIOC->CRL.MODE0 = 2; // PC0 = output 2MHz
   GPIOC->CRL.CNF0 = 0; // PC0 = push pull
   
   GPIOC->CRL.MODE1 = 2; // PC1 = output 2MHz
   GPIOC->CRL.CNF1 = 0; // PC1 = push pull
   
   GPIOC->CRL.MODE2 = 2; // PC2 = output 2MHz
   GPIOC->CRL.CNF2 = 0; // PC2 = push pull
   //RCC->AHBENR.IOPAEN = 1; // enable GPIOA
}

void uart_setup(){
   RCC->APB1ENR.USART2EN = 1; // usart2 en
   RCC->APB2ENR.AFIOEN = 1; // afio en
   
   RCC->APB2ENR.IOPAEN = 1; // enable GPIOA
   GPIOA->CRL.MODE2 = 1; // PA2 = output 10MHz
   GPIOA->CRL.CNF2 = 2; // PA2 = afio push pull

   GPIOA->CRL.MODE3 = 0; // PA2 = input
   GPIOA->CRL.CNF3 = 1; // PA2 = floating input
   
   uint32_t div = 72000000 / 2 / 2250000 + 0.5;
   USART2->BRR.DIV_Mantissa = div / 16;
   USART2->BRR.DIV_Fraction = div & 0x000F;
   // USART2->BRR.w = div;
   // USART2->BRR.w = 16;
   
   USART2->CR1.RE = 1; // rx en
   USART2->CR1.TE = 1; // tx en
   
   USART2->CR1.UE = 1; // usart en
}

void flash_unlock(){
   FLASH->KEYR.KEY = 0x45670123;
   FLASH->KEYR.KEY = 0xCDEF89AB;
   FLASH->CR.PG = 1;
}

void flash_clear(uint32_t page){
   while(FLASH->SR.BSY == 1){}
   FLASH->CR.PER = 1;
   FLASH->AR.FAR = page;
   FLASH->CR.STRT = 1;
   while(FLASH->SR.BSY == 1){}
   FLASH->CR.PER = 0;// only lib, not in docs
   //TODO: verify
}

void flash_write(uint32_t addr, uint16_t data){
   while(FLASH->SR.BSY == 1){}
   FLASH->CR.PG = 1;
   *(volatile uint16_t*)addr = data;
   while(FLASH->SR.BSY == 1){}
   FLASH->CR.PG = 0;// only lib, not in docs
   //TODO: verify
}

int main(void)
{
   uint32_t i=0;
   gpio_setup();
   clock_setup();
   uart_setup();
   flash_unlock();
   for(int i = 1024;i<2048*2;i+=2){
      flash_write(0x8000000+i,0x5555);
   }
   flash_clear(0x8000000+1024);
   for(;;){
      if(USART2->SR.RXNE == 1 && USART2->SR.TXE == 1){
         USART2->DR.DR = USART2->DR.DR;
      }
      i++;
      if(i > 100000){
         GPIOC->ODR.ODR0 = 0; // reset PC0
         //USART2->DR.DR = 0x55;
      }
      if(i > 200000){ // read PA0
         GPIOC->ODR.ODR0 = 1; // set PC0
         // USART2->DR.DR = 0xAA;
         i = 0;
      }
   }
}

void nmi_handler(void)
{
    for(;;);
}

void hardfault_handler(void)
{
    for(;;);
}
