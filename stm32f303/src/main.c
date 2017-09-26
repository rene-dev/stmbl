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
#include "opamp.h"
#include "tim.h"
#include "usb_device.h"

/* USER CODE BEGIN Includes */
#include <math.h>
#include "defines.h"
#include "hal.h"
#include "angle.h"
#include "usbd_cdc_if.h"
#include "version.h"
#include "common.h"
#include "commands.h"
#include "f3hw.h"

uint32_t systick_freq;
CRC_HandleTypeDef hcrc;
TIM_HandleTypeDef htim1;

static void MX_TIM1_Init(void) {
  __HAL_RCC_TIM1_CLK_ENABLE();
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig;

  htim1.Instance               = TIM1;
  htim1.Init.Prescaler         = 0;
  htim1.Init.CounterMode       = TIM_COUNTERMODE_CENTERALIGNED3;
  htim1.Init.Period            = 1440;
  htim1.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  // htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  // htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  HAL_TIM_OC_Init(&htim1);

  sMasterConfig.MasterOutputTrigger  = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_UPDATE;
  sMasterConfig.MasterSlaveMode      = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig);

  sConfigOC.OCMode       = TIM_OCMODE_PWM1;
  sConfigOC.Pulse        = 720;
  sConfigOC.OCPolarity   = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode   = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState  = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  HAL_TIM_OC_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1);

  HAL_TIM_OC_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2);

  sBreakDeadTimeConfig.OffStateRunMode  = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel        = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime         = 0;
  sBreakDeadTimeConfig.BreakState       = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity    = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.BreakFilter      = 0;
  sBreakDeadTimeConfig.Break2State      = TIM_BREAK2_DISABLE;
  sBreakDeadTimeConfig.Break2Polarity   = TIM_BREAK2POLARITY_HIGH;
  sBreakDeadTimeConfig.Break2Filter     = 0;
  sBreakDeadTimeConfig.AutomaticOutput  = TIM_AUTOMATICOUTPUT_DISABLE;
  HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig);

  GPIO_InitTypeDef GPIO_InitStruct;
  /**TIM1 GPIO Configuration    
  PB13     ------> TIM1_CH1N
  PB14     ------> TIM1_CH2N
  PA8     ------> TIM1_CH1
  PA9     ------> TIM1_CH2 
  */
  GPIO_InitStruct.Pin       = GPIO_PIN_13 | GPIO_PIN_14;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF6_TIM1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin       = GPIO_PIN_8 | GPIO_PIN_9;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF6_TIM1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}


// //hal interface TODO: move hal interface to file
// void hal_enable_rt(){
//    // TIM_Cmd(TIM4, ENABLE);
// }
// void hal_enable_frt(){
//    // TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
// }
// void hal_disable_rt(){
//    // TIM_Cmd(TIM4, DISABLE);
// }
// void hal_disable_frt(){
//    // TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
// }

uint32_t hal_get_systick_value() {
  return (SysTick->VAL);
}

uint32_t hal_get_systick_reload() {
  return (SysTick->LOAD);
}

uint32_t hal_get_systick_freq() {
  return (systick_freq);
}

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */


void DMA1_Channel1_IRQHandler() {
  GPIOC->BSRR |= GPIO_PIN_14;
  GPIOC->BSRR |= GPIO_PIN_15;
  //  __HAL_DMA_CLEAR_FLAG(&hdma1, DMA_FLAG_TC1);
  DMA1->IFCR = DMA_IFCR_CTCIF1;
  DMA2->IFCR = DMA_IFCR_CTCIF5;

  hal_run_rt();
  //  if(__HAL_DMA_GET_FLAG(&hdma1, DMA_FLAG_TC1) == SET){
  //     hal_stop();
  //     hal.hal_state = RT_TOO_LONG;
  //  }
  GPIOC->BSRR |= GPIO_PIN_14 << 16;
}

void bootloader(char *ptr) {
#ifdef USB_DISCONNECT_PIN
  HAL_GPIO_WritePin(USB_DISCONNECT_PORT, USB_DISCONNECT_PIN, GPIO_PIN_SET);
  HAL_Delay(100);
#endif
  RTC->BKP0R = 0xDEADBEEF;
  NVIC_SystemReset();
}

COMMAND("bootloader", bootloader, "enter bootloader");

void reset(char *ptr) {
  HAL_NVIC_SystemReset();
}
COMMAND("reset", reset, "reset STMBL");

int main(void) {
  /* USER CODE BEGIN 1 */
  SCB->VTOR = 0x8004000;
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();
  systick_freq = HAL_RCC_GetHCLKFreq();
  /* Initialize all configured peripherals */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();

  GPIO_InitTypeDef GPIO_InitStruct;

#ifdef USB_DISCONNECT_PIN
  GPIO_InitStruct.Pin   = USB_DISCONNECT_PIN;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USB_DISCONNECT_PORT, &GPIO_InitStruct);
  HAL_GPIO_WritePin(USB_DISCONNECT_PORT, USB_DISCONNECT_PIN, GPIO_PIN_RESET);
#endif

  MX_TIM8_Init();
  MX_TIM1_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_ADC3_Init();
  MX_ADC4_Init();
  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
  /* DMA2_Channel5_IRQn interrupt configuration */
  // HAL_NVIC_SetPriority(DMA2_Channel5_IRQn, 0, 0);
  // HAL_NVIC_EnableIRQ(DMA2_Channel5_IRQn);


  // MX_DAC_Init();
  // MX_OPAMP1_Init();
  MX_OPAMP2_Init();
  MX_OPAMP3_Init();
  MX_OPAMP4_Init();
  // MX_USART1_UART_Init();
  MX_USB_DEVICE_Init();

#ifdef USB_CONNECT_PIN
  GPIO_InitStruct.Pin   = USB_CONNECT_PIN;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USB_CONNECT_PORT, &GPIO_InitStruct);
  HAL_GPIO_WritePin(USB_CONNECT_PORT, USB_CONNECT_PIN, GPIO_PIN_SET);
#endif

  __HAL_RCC_DMA1_CLK_ENABLE();
  __HAL_RCC_DMA2_CLK_ENABLE();
  __HAL_RCC_RTC_ENABLE();

  /* USER CODE BEGIN 2 */
  HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
  HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);
  HAL_ADCEx_Calibration_Start(&hadc3, ADC_SINGLE_ENDED);
  HAL_ADCEx_Calibration_Start(&hadc4, ADC_SINGLE_ENDED);

  // HAL_OPAMP_SelfCalibrate(&hopamp1);
  HAL_OPAMP_SelfCalibrate(&hopamp2);
  HAL_OPAMP_SelfCalibrate(&hopamp3);
  HAL_OPAMP_SelfCalibrate(&hopamp4);

  hcrc.Instance                     = CRC;
  hcrc.Init.DefaultPolynomialUse    = DEFAULT_POLYNOMIAL_ENABLE;
  hcrc.Init.DefaultInitValueUse     = DEFAULT_INIT_VALUE_ENABLE;
  hcrc.Init.InputDataInversionMode  = CRC_INPUTDATA_INVERSION_NONE;
  hcrc.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
  hcrc.InputDataFormat              = CRC_INPUTDATA_FORMAT_WORDS;

  __HAL_RCC_CRC_CLK_ENABLE();

  if(HAL_CRC_Init(&hcrc) != HAL_OK) {
    Error_Handler();
  }

  //IO pins
  // GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10;
  // GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  // GPIO_InitStruct.Pull = GPIO_NOPULL;
  // GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  // HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  if(HAL_OPAMP_Start(&hopamp2) != HAL_OK) {
    Error_Handler();
  }
  if(HAL_OPAMP_Start(&hopamp3) != HAL_OK) {
    Error_Handler();
  }
  if(HAL_OPAMP_Start(&hopamp4) != HAL_OK) {
    Error_Handler();
  }

  // htim8.Instance->CCR1 = 0;
  // htim8.Instance->CCR2 = 0;
  // htim8.Instance->CCR3 = 0;

  // htim1.Instance->CCR1 = 1200;
  // htim1.Instance->CCR2 = 1200;


  HAL_ADC_Start(&hadc1);
  HAL_ADC_Start(&hadc2);
  HAL_ADC_Start(&hadc3);
  HAL_ADC_Start(&hadc4);
// if (HAL_TIM_Base_Start_IT(&htim8) != HAL_OK){
// Error_Handler();
// }
#ifndef PWM_INVERT
  TIM8->RCR = 1;  //uptate event foo
#endif
  // if (HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1) != HAL_OK){
  // Error_Handler();
  // }
  // if (HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2) != HAL_OK){
  // 	Error_Handler();
  // }
  // if (HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_3) != HAL_OK){
  // 	Error_Handler();
  // }
  // if (HAL_TIMEx_PWMN_Start(&htim8, TIM_CHANNEL_1) != HAL_OK){
  // Error_Handler();
  // }
  // if (HAL_TIMEx_PWMN_Start(&htim8, TIM_CHANNEL_2) != HAL_OK){
  // 	Error_Handler();
  // }
  // if (HAL_TIMEx_PWMN_Start(&htim8, TIM_CHANNEL_3) != HAL_OK){
  // 	Error_Handler();
  // }

  hal_init(1.0 / RT_FREQ, 0.0);
  // hal load comps
  load_comp(comp_by_name("term"));
  load_comp(comp_by_name("sim"));
  load_comp(comp_by_name("io"));
  load_comp(comp_by_name("dq"));
  load_comp(comp_by_name("idq"));
  load_comp(comp_by_name("tle"));
  load_comp(comp_by_name("pole"));
  load_comp(comp_by_name("map"));
  load_comp(comp_by_name("vel"));
  load_comp(comp_by_name("vel"));
  load_comp(comp_by_name("ypid"));
  load_comp(comp_by_name("hv"));
  load_comp(comp_by_name("curpid"));

  hal_parse("io0.rt_prio = 1.0");  
  hal_parse("tle0.rt_prio = 2.0");
  hal_parse("map0.rt_prio = 3.0");
  hal_parse("vel0.rt_prio = 4.0");
  hal_parse("vel1.rt_prio = 4.0");
  hal_parse("pole0.rt_prio = 4.0");
  hal_parse("dq0.rt_prio = 5.0");
  hal_parse("ypid0.rt_prio = 6.0");
  hal_parse("curpid0.rt_prio = 7.0");
  hal_parse("idq0.rt_prio = 8.0");  
  hal_parse("hv0.rt_prio = 9.0");
  
  hal_parse("term0.rt_prio = 10");
  hal_parse("sim0.rt_prio = 10.0");

  hal_parse("term0.send_step = 10.0");
  hal_parse("term0.gain0 = 20.0");
  hal_parse("term0.gain1 = 20.0");
  hal_parse("term0.gain2 = 1.0");
  hal_parse("term0.gain3 = 1.0");
  hal_parse("term0.gain4 = 10.0");
  hal_parse("term0.gain5 = 10.0");
  hal_parse("term0.gain6 = 10.0");
  hal_parse("term0.gain7 = 10.0");

  hal_parse("hv0.a = idq0.a");
  hal_parse("hv0.b = idq0.b");
  hal_parse("hv0.udc = io0.dc_link");
  hal_parse("hv0.hv_temp = io0.hv_temp");

  hal_parse("curpid0.id_fb = dq0.d");
  hal_parse("curpid0.iq_fb = dq0.q");
  hal_parse("idq0.d = curpid0.ud");
  hal_parse("idq0.q = curpid0.uq");
  hal_parse("dq0.a = io0.ia");
  hal_parse("dq0.b = io0.ib");

  hal_parse("curpid0.rd = 1.6");
  hal_parse("curpid0.rq = 1.6");
  hal_parse("curpid0.ld = 0.0038");
  hal_parse("curpid0.lq = 0.0038");
  hal_parse("curpid0.psi = 0.005");
  hal_parse("curpid0.kp = 0.1");
  hal_parse("curpid0.ki = 0.002");
  hal_parse("curpid0.ff = 1");
  hal_parse("curpid0.kind = 0");
  hal_parse("curpid0.max_cur = 4");
  hal_parse("curpid0.pwm_volt = io0.dc_link");

  hal_parse("pole0.p = 50.0");
  hal_parse("io0.led = term0.con");

  hal_parse("dq0.pos = pole0.cpos");
  hal_parse("idq0.pos = pole0.cpos");
  hal_parse("pole0.pos = map0.pos_out2");
  hal_parse("map0.pos_in = tle0.pos");
  hal_parse("map0.freq = 0.1");
  hal_parse("curpid0.cmd_mode = 1");
  hal_parse("vel1.pos_in = map0.pos_out2");
  hal_parse("ypid0.pos_ext_cmd = vel0.pos_out");
  hal_parse("ypid0.vel_ext_cmd = vel0.vel");
  hal_parse("ypid0.pos_fb = map0.pos_out2");
  hal_parse("ypid0.vel_fb = vel1.vel");
  hal_parse("curpid0.iq_cmd = ypid0.out");

  hal_parse("ypid0.max_vel = 30");
  hal_parse("ypid0.max_acc = 200000");
  hal_parse("ypid0.max_out = 4");
  hal_parse("ypid0.pos_p = 100");
  hal_parse("ypid0.vel_p = 0.1");
  hal_parse("ypid0.vel_i = 0.05");
  hal_parse("ypid0.vel_ff = 1.0");
  
  hal_parse("term0.wave0 = vel0.pos_out");
  hal_parse("term0.wave1 = map0.pos_out2");
  hal_parse("term0.wave2 = vel0.vel");
  hal_parse("term0.wave3 = vel1.vel");
  
  hal_parse("vel0.w = 3000");
  hal_parse("vel1.w = 3000");

  hal_parse("vel0.en = 1");
  hal_parse("vel1.en = 1");
  hal_parse("ypid0.enable = 1");
  hal_parse("curpid0.en = 1");
  hal_parse("hv0.en = 1");

  hal_parse("flashloadconf");
  hal_parse("loadconf");
  hal_parse("start");

  // hal parse config
  // hal_init_nrt();
  // error foo
  // hal_start();

  HAL_TIM_Base_Start(&htim1);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);

  while(1) {
    hal_run_nrt();
    cdc_poll();
    HAL_Delay(1);
  }
}

/** System Clock Configuration
*/
void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

  /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState       = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState       = RCC_HSI_ON;
  RCC_OscInitStruct.LSIState       = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL     = RCC_PLL_MUL9;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    Error_Handler();
  }

  /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
    Error_Handler();
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB | RCC_PERIPHCLK_USART1 | RCC_PERIPHCLK_USART3 | RCC_PERIPHCLK_TIM8 | RCC_PERIPHCLK_TIM1 | RCC_PERIPHCLK_ADC12 | RCC_PERIPHCLK_ADC34 | RCC_PERIPHCLK_RTC;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_SYSCLK;
  PeriphClkInit.Adc12ClockSelection  = RCC_ADC12PLLCLK_DIV1;
  PeriphClkInit.Adc34ClockSelection  = RCC_ADC34PLLCLK_DIV1;
  PeriphClkInit.USBClockSelection    = RCC_USBCLKSOURCE_PLL_DIV1_5;
  // PeriphClkInit.Tim8ClockSelection = RCC_TIM8CLK_PLLCLK;
  PeriphClkInit.Tim1ClockSelection = RCC_TIM1CLK_PLLCLK;
  PeriphClkInit.RTCClockSelection  = RCC_RTCCLKSOURCE_LSI;
  if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
    Error_Handler();
  }

  /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

  /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

//Delay implementation for hal_term.c
void Wait(uint32_t ms) {
  HAL_Delay(ms);
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void) {
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  while(1) {
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
void assert_failed(uint8_t *file, uint32_t line) {
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
