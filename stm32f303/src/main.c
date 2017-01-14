/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * Copyright (c) 2016 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f3xx_hal.h"
#include "adc.h"
#include "dac.h"
#include "opamp.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include <math.h>
#include "defines.h"
#include "hal.h"
#include "hal_term.h"
#include "angle.h"
#include "scanf.h"
#include "usbd_cdc_if.h"

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

GLOBAL_HAL_PIN(rt_time);
GLOBAL_HAL_PIN(frt_time);
GLOBAL_HAL_PIN(rt_period_time);
GLOBAL_HAL_PIN(frt_period_time);
uint32_t systick_freq;

//hal interface TODO: move hal interface to file
void hal_enable_rt(){
   // TIM_Cmd(TIM4, ENABLE);
}
void hal_enable_frt(){
   // TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
}
void hal_disable_rt(){
   // TIM_Cmd(TIM4, DISABLE);
}
void hal_disable_frt(){
   // TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
}

uint32_t hal_get_systick_value(){
   return(SysTick->VAL);
}

uint32_t hal_get_systick_reload(){
   return(SysTick->LOAD);
}

uint32_t hal_get_systick_freq(){
   return(systick_freq);
}

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

#define ARES 4096.0// analog resolution, 12 bit
#define AREF 3.338// analog reference voltage

#define VDIVUP 249000.0 * 2.0//HV div pullup R1,R12
#define VDIVDOWN 3900.0//HV div pulldown R2,R9

#define VOLT(a) ((a) / (ARES) * (AREF) / (VDIVDOWN) * ((VDIVUP) + (VDIVDOWN)))
//#define TEMP(a) (log10f((a) * (AREF) / (ARES) * (TPULLUP) / ((AREF) - (a) * (AREF) / (ARES))) * (-53.0) + 290.0)

#define SHUNT 0.003//shunt
#define SHUNT_PULLUP 15000
#define SHUNT_SERIE 470
#define SHUNT_GAIN 16

#define AMP(a, gain) (((a) * AREF / ARES / (gain) - AREF / (SHUNT_PULLUP + SHUNT_SERIE) * SHUNT_SERIE) / (SHUNT * SHUNT_PULLUP) * (SHUNT_PULLUP + SHUNT_SERIE))


/* USER CODE END 0 */

void TIM8_UP_IRQHandler(){
   __HAL_TIM_CLEAR_IT(&htim8, TIM_IT_UPDATE);
   //TODO: hw trigger and dma for ADC
   // HAL_ADC_Start(&hadc1);
   // HAL_ADC_Start(&hadc2);
   // HAL_ADC_Start(&hadc3);
   // HAL_ADC_Start(&hadc4);
   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
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
   unsigned int start = hal_get_systick_value();
   
   if(last_start < start){
     last_start += hal_get_systick_reload();
   }
   
   float period = ((float)(last_start - start)) / hal_get_systick_freq();
   last_start = start;

   hal_run_rt(0.000066);

   unsigned int end = hal_get_systick_value();
   if(start < end){
     start += hal_get_systick_reload();
   }
   PIN(rt_time) = ((float)(start - end)) / hal_get_systick_freq();
   PIN(rt_period_time) = period;

   hal.rt_state = RT_SLEEP;
   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
}

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();
  systick_freq = HAL_RCC_GetHCLKFreq();
  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM8_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_ADC3_Init();
  MX_ADC4_Init();
  // MX_DAC_Init();
  MX_OPAMP1_Init();
  MX_OPAMP2_Init();
  MX_OPAMP3_Init();
  // MX_USART1_UART_Init();
  // MX_USART3_UART_Init();
  MX_USB_DEVICE_Init();

  /* USER CODE BEGIN 2 */
  HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
  HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);
  HAL_ADCEx_Calibration_Start(&hadc3, ADC_SINGLE_ENDED);
  HAL_ADCEx_Calibration_Start(&hadc4, ADC_SINGLE_ENDED);

  HAL_OPAMP_SelfCalibrate(&hopamp1);
  HAL_OPAMP_SelfCalibrate(&hopamp2);
  HAL_OPAMP_SelfCalibrate(&hopamp3);
  
  //USB EN IO board: PB15
  // GPIO_InitStruct.Pin = GPIO_PIN_15;
  // GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  // GPIO_InitStruct.Pull = GPIO_NOPULL;
  // GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  // HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  // HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
  
  //IO pins
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

     if (HAL_OPAMP_Start(&hopamp1) != HAL_OK){
       Error_Handler();
     }
     if (HAL_OPAMP_Start(&hopamp2) != HAL_OK){
       Error_Handler();
     }
     if (HAL_OPAMP_Start(&hopamp3) != HAL_OK){
       Error_Handler();
     }

  htim8.Instance->CCR1 = 0;
  htim8.Instance->CCR2 = 0;
  htim8.Instance->CCR3 = 0;
  
  HAL_ADC_Start(&hadc1);
  HAL_ADC_Start(&hadc2);
  HAL_ADC_Start(&hadc3);
  HAL_ADC_Start(&hadc4);
  if (HAL_TIM_Base_Start_IT(&htim8) != HAL_OK){
 	Error_Handler();
  }
  TIM8->RCR = 1;//uptate event foo
  if (HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1) != HAL_OK){
	Error_Handler();
  }
  if (HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2) != HAL_OK){
  	Error_Handler();
  }
  if (HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_3) != HAL_OK){
  	Error_Handler();
  }
  if (HAL_TIMEx_PWMN_Start(&htim8, TIM_CHANNEL_1) != HAL_OK){
	Error_Handler();
  }
  if (HAL_TIMEx_PWMN_Start(&htim8, TIM_CHANNEL_2) != HAL_OK){
  	Error_Handler();
  }
  if (HAL_TIMEx_PWMN_Start(&htim8, TIM_CHANNEL_3) != HAL_OK){
  	Error_Handler();
  }

  hal_init();

  hal_set_comp_type("foo"); // default pin for mem errors
  HAL_PIN(bar) = 0.0;
  
  #include "../src/comps/sim.comp"
  #include "comps/term.comp"
  #include "../src/comps/idq.comp"
  #include "../src/comps/iclarke.comp"
  #include "comps/clarke.comp"
  #include "comps/io.comp"
  #include "comps/svm.comp"
  #include "comps/hv.comp"
  
  hal_set_comp_type("net");
  HAL_PIN(enable) = 0.0;
  HAL_PIN(cmd) = 0.0;
  HAL_PIN(fb) = 0.0;
  HAL_PIN(fb_error) = 0.0;
  HAL_PIN(cmd_d) = 0.0;
  HAL_PIN(fb_d) = 0.0;
  HAL_PIN(core_temp0) = 0.0;
  HAL_PIN(core_temp1) = 0.0;
  HAL_PIN(motor_temp) = 0.0;
  HAL_PIN(rt_calc_time) = 0.0;
  HAL_PIN(frt_calc_time) = 0.0;
  HAL_PIN(nrt_calc_time) = 0.0;
  HAL_PIN(rt_period) = 0.0;
  HAL_PIN(frt_period) = 0.0;
  HAL_PIN(nrt_period) = 0.0;

  rt_time_hal_pin = hal_map_pin("net0.rt_calc_time");
  frt_time_hal_pin = hal_map_pin("net0.frt_calc_time");
  rt_period_time_hal_pin = hal_map_pin("net0.rt_period");
  frt_period_time_hal_pin = hal_map_pin("net0.frt_period");
  
  // hal_set_pin("sim0.rt_prio", 1.0);
  hal_set_pin("idq0.rt_prio", 2.0);
  hal_set_pin("iclarke0.rt_prio", 3.0);
  hal_set_pin("clarke0.rt_prio", 3.1);
  hal_set_pin("io0.rt_prio", 4.0);
  hal_set_pin("svm0.rt_prio", 4.1);
  hal_set_pin("hv0.rt_prio", 4.2);
  
  hal_set_pin("term0.rt_prio", 15.0);
  hal_set_pin("term0.send_step", 50.0);
  hal_set_pin("term0.gain0", 10.0);
  hal_set_pin("term0.gain1", 10.0);
  hal_set_pin("term0.gain2", 10.0);
  hal_set_pin("term0.gain3", 1.0);
  hal_set_pin("term0.gain4", 20.0);
  hal_set_pin("term0.gain5", 20.0);
  hal_set_pin("term0.gain6", 20.0);
  hal_set_pin("term0.gain7", 20.0);
  
  //ADC TEST
  hal_link_pins("io0.udc", "term0.wave3");
  hal_link_pins("io0.udc", "hv0.udc");
  hal_link_pins("io0.iu", "clarke0.u");
  hal_link_pins("io0.iv", "clarke0.v");
  hal_link_pins("io0.iw", "clarke0.w");
  hal_link_pins("clarke0.a", "term0.wave0");
  hal_link_pins("clarke0.b", "term0.wave1");
  hal_link_pins("clarke0.y", "term0.wave2");
  
  hal_link_pins("sim0.vel", "idq0.pos");
  hal_link_pins("idq0.a", "iclarke0.a");
  hal_link_pins("idq0.b", "iclarke0.b");
  hal_link_pins("iclarke0.u", "svm0.u");
  hal_link_pins("iclarke0.v", "svm0.v");
  hal_link_pins("iclarke0.w", "svm0.w");
  hal_link_pins("svm0.su", "hv0.u");
  hal_link_pins("svm0.sv", "hv0.v");
  hal_link_pins("svm0.sw", "hv0.w");
  hal_link_pins("io0.udc", "svm0.udc");

  hal_link_pins("term0.con", "io0.led");
  
  hal_comp_init();//call init function of all comps

  if(hal.pin_errors + hal.comp_errors == 0){
     hal_start();
  }
  else{
     hal.hal_state = MEM_ERROR;
  }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
     hal_run_nrt(0.1);
     cdc_poll();
     HAL_Delay(1);
  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB|RCC_PERIPHCLK_USART1
                              |RCC_PERIPHCLK_USART3|RCC_PERIPHCLK_TIM8
                              |RCC_PERIPHCLK_ADC12|RCC_PERIPHCLK_ADC34;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_SYSCLK;
  PeriphClkInit.Adc12ClockSelection = RCC_ADC12PLLCLK_DIV1;
  PeriphClkInit.Adc34ClockSelection = RCC_ADC34PLLCLK_DIV1;
  PeriphClkInit.USBClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  PeriphClkInit.Tim8ClockSelection = RCC_TIM8CLK_PLLCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

//Delay implementation for hal_term.c
void Wait(uint32_t ms){
   HAL_Delay(ms);
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
  }
  /* USER CODE END Error_Handler */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
