#include "stm32f10x_conf.h"
//#include "common.h"


// TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
// TIM_OCInitTypeDef  TIM_OCInitStructure;
// NVIC_InitTypeDef NVIC_InitStructure;
// GPIO_InitTypeDef GPIO_InitStructure;
// USART_InitTypeDef USART_InitStruct;
// ADC_InitTypeDef ADC_InitStructure;
// DMA_InitTypeDef DMA_InitStructure;

// void Wait(unsigned int ms){
//    volatile unsigned int t = systime + ms;
//    while(t >= systime){
//    }
// }
//
// void SysTick_Handler(void)
// {
//    systime++;
// }

void RCC_Configuration(void)
{
   // ErrorStatus HSEStartUpStatus;
   // RCC_DeInit();
   // RCC_HSEConfig(RCC_HSE_ON);
   // HSEStartUpStatus = RCC_WaitForHSEStartUp();
   //
   // if(HSEStartUpStatus == SUCCESS){
   //    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
   //    FLASH_SetLatency(FLASH_Latency_2);
   //    RCC_HCLKConfig(RCC_SYSCLK_Div1);
   //    RCC_PCLK2Config(RCC_HCLK_Div1);
   //    RCC_PCLK1Config(RCC_HCLK_Div2);
   //    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
   //    RCC_PLLCmd(ENABLE);
   //    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
   //    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
   //    while(RCC_GetSYSCLKSource() != 0x08){}
   // }
   //
   // RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
}



//usart_init();

int main(void)
{
   // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
   // RCC_Configuration();
   // //GPIO_Configuration();
   //
   RCC_ClocksTypeDef RCC_Clocks;
   //RCC_GetClocksFreq(&RCC_Clocks);
   //SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000 - 1);
   
   while(1);
}
