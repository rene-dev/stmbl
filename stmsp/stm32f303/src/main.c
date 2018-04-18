/*
* This file is part of the stmbl project.
*
* Copyright (C) 2013-2017 Rene Hopf <renehopf@mac.com>
* Copyright (C) 2013-2017 Nico Stute <crinq@crinq.de>
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

#include "main.h"
#include "stm32f3xx_hal.h"
#include "adc.h"
#include "opamp.h"

#include <math.h>
#include "defines.h"
#include "hal.h"
#include "angle.h"

#include "version.h"
#include "common.h"
#include "commands.h"
#include "stmsp_f3hw.h"

#include "usbd_cdc_if.h"
#ifdef USB_TERM
#include "usb_device.h"
#endif

volatile uint64_t systime = 0;

void SysTick_Handler(void) {
  /* USER CODE BEGIN SysTick_IRQn 0 */
  systime++;
  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

uint32_t systick_freq;
CRC_HandleTypeDef hcrc;

uint32_t hal_get_systick_value() {
  return (SysTick->VAL);
}

uint32_t hal_get_systick_reload() {
  return (SysTick->LOAD);
}

uint32_t hal_get_systick_freq() {
  return (systick_freq);
}

void SystemClock_Config(void);
void Error_Handler(void);



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
  htim1.Init.Period            = PWM_RES;
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
  sConfigOC.Pulse        = PWM_RES / 2;
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

void DMA1_Channel1_IRQHandler() {
  DMA1->IFCR = DMA_IFCR_CTCIF1;
  DMA2->IFCR = DMA_IFCR_CTCIF5;

  hal_run_rt();
  //  if(__HAL_DMA_GET_FLAG(&hdma1, DMA_FLAG_TC1) == SET){
  //     hal_stop();
  //     hal.hal_state = RT_TOO_LONG;
  //  }
  // if(__HAL_TIM_GET_FLAG(&hdma1, DMA_FLAG_TC1) == SET) {
  //   hal_stop();
  //   hal.hal_state = RT_TOO_LONG;
  // }
  // reset
  IWDG->KR = 0xAAAA;
}


void about(char *ptr) {
  printf("######## software info ########\n");
  printf(
      "%s v%i.%i.%i %s\n",
      version_info.product_name,
      version_info.major,
      version_info.minor,
      version_info.patch,
      version_info.git_version);
  printf("Branch %s\n", version_info.git_branch);
  printf("Compiled %s %s ", version_info.build_date, version_info.build_time);
  printf("by %s on %s\n", version_info.build_user, version_info.build_host);
  printf("GCC        %s\n", __VERSION__);
  printf("newlib     %s\n", _NEWLIB_VERSION);
#ifdef __CM4_CMSIS_VERSION
  printf("CMSIS      %i.%i\n", __CM4_CMSIS_VERSION_MAIN, __CM4_CMSIS_VERSION_SUB);
#endif
#ifdef __STM32F4XX_STDPERIPH_VERSION
  printf("StdPeriph  %i.%i.%i\n", __STM32F4XX_STDPERIPH_VERSION_MAIN, __STM32F4XX_STDPERIPH_VERSION_SUB1, __STM32F4XX_STDPERIPH_VERSION_SUB2);
#endif
#ifdef __STM32F3xx_HAL_VERSION
  printf("HAL lib... TODO: print version\n");
#endif
}

COMMAND("about", about, "show system infos");

void bootloader(char *ptr) {
#ifdef USB_DISCONNECT_PIN
  HAL_GPIO_WritePin(USB_DISCONNECT_PORT, USB_DISCONNECT_PIN, GPIO_PIN_SET);
  HAL_Delay(100);
#endif
#ifdef USB_CONNECT_PIN
  HAL_GPIO_WritePin(USB_CONNECT_PORT, USB_CONNECT_PIN, GPIO_PIN_RESET);
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
  // Relocate interrupt vectors
  extern void *g_pfnVectors;
  SCB->VTOR = (uint32_t)&g_pfnVectors;

  // reset
  // IWDG->KR = 0xAAAA;

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

  GPIO_InitStruct.Pin   = LED_Pin | GPIO_PIN_14 | GPIO_PIN_15;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);
  HAL_GPIO_WritePin(LED_GPIO_Port, GPIO_PIN_15, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LED_GPIO_Port, GPIO_PIN_14, GPIO_PIN_SET);

#ifdef USB_DISCONNECT_PIN
  GPIO_InitStruct.Pin   = USB_DISCONNECT_PIN;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USB_DISCONNECT_PORT, &GPIO_InitStruct);
  HAL_GPIO_WritePin(USB_DISCONNECT_PORT, USB_DISCONNECT_PIN, GPIO_PIN_RESET);
#endif

  MX_TIM1_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_ADC3_Init();
  MX_ADC4_Init();

  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

  MX_OPAMP2_Init();
  MX_OPAMP3_Init();
  MX_OPAMP4_Init();

#ifdef USB_TERM
  MX_USB_DEVICE_Init();
#endif

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

  if(HAL_OPAMP_Start(&hopamp2) != HAL_OK) {
    Error_Handler();
  }
  if(HAL_OPAMP_Start(&hopamp3) != HAL_OK) {
    Error_Handler();
  }
  if(HAL_OPAMP_Start(&hopamp4) != HAL_OK) {
    Error_Handler();
  }

  HAL_ADC_Start(&hadc1);
  HAL_ADC_Start(&hadc2);
  HAL_ADC_Start(&hadc3);
  HAL_ADC_Start(&hadc4);

  

  hal_init(1.0 / RT_FREQ, 0.0);
  // hal load comps
  hal_parse("debug_level 1");

  hal_parse("load term");
  // hal_parse("load sim");
  // hal_parse("load io");
  // hal_parse("load dq_sp");
  // hal_parse("load idq");
  // hal_parse("load tle");
  // hal_parse("load pole");
  // hal_parse("load map");
  // hal_parse("load vel");
  // hal_parse("load vel");
  // hal_parse("load ypid");
  // // hal_parse("load rl"));
  // hal_parse("load hv");
  // hal_parse("load curpid");

  hal_parse("io0.rt_prio = 1.0");  
  hal_parse("tle0.rt_prio = 2.0");
  hal_parse("map0.rt_prio = 3.0");
  hal_parse("vel0.rt_prio = 4.0");
  hal_parse("vel1.rt_prio = 4.0");
  hal_parse("pole0.rt_prio = 4.0");
  hal_parse("dq_sp0.rt_prio = 5.0");
  hal_parse("ypid0.rt_prio = 6.0");
  hal_parse("trc0.rt_prio = 6.5");
  hal_parse("curpid0.rt_prio = 7.0");
  hal_parse("idq0.rt_prio = 8.0");  
  hal_parse("rl0.rt_prio = 8.0");
  hal_parse("hv0.rt_prio = 9.0");
  
  hal_parse("term0.rt_prio = 10");
  // hal_parse("sim0.rt_prio = 10.0");

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

  hal_parse("curpid0.id_fb = dq_sp0.d");
  hal_parse("curpid0.iq_fb = dq_sp0.q");
  hal_parse("idq0.d = curpid0.ud");
  hal_parse("idq0.q = curpid0.uq");
  hal_parse("dq_sp0.a = io0.ia");
  hal_parse("dq_sp0.b = io0.ib");

  hal_parse("curpid0.rd = 1.6");
  hal_parse("curpid0.rq = 1.6");
  hal_parse("curpid0.ld = 0.0038");
  hal_parse("curpid0.lq = 0.0038");
  hal_parse("curpid0.psi = 0.005");
  hal_parse("curpid0.kp = 1.0");
  hal_parse("curpid0.ki = 0.0005");
  hal_parse("curpid0.ff = 1");
  hal_parse("curpid0.kind = 0");
  hal_parse("curpid0.max_cur = 3.1");
  hal_parse("curpid0.pwm_volt = io0.dc_link");

  hal_parse("pole0.p = 50.0");
  // hal_parse("io0.led = term0.con");

  hal_parse("dq_sp0.pos = pole0.cpos");
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
  // hal_parse("trc0.in = ypid0.out");
  // hal_parse("curpid0.iq_cmd = trc0.out");
  // hal_parse("trc0.pos = map0.pos_out2");
  // hal_parse("trc0.mult = 200");
  
  hal_parse("ypid0.max_vel = 50");
  hal_parse("ypid0.max_acc = 10000");
  hal_parse("ypid0.max_out = 3");
  hal_parse("ypid0.pos_p = 100");
  hal_parse("ypid0.vel_p = 0.05");
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

  hal_parse("rl0.ua_fb = hv0.a_fb");
  hal_parse("rl0.ub_fb = hv0.b_fb");
  hal_parse("rl0.ia_fb = io0.ia");
  hal_parse("rl0.ib_fb = io0.ib");
  hal_parse("term0.wave4 = rl0.ra");
  hal_parse("term0.wave5 = rl0.rb");
  hal_parse("term0.wave0 = rl0.t");
  hal_parse("term0.wave1 = rl0.state");
  
  // hal_parse("hv0.a = rl0.ua");
  // hal_parse("hv0.b = rl0.ub");
  
  hal_parse("flashloadconf");
  hal_parse("loadconf");
  
  hal_parse("debug_level 0");

  // reset
  IWDG->KR = 0xAAAA;

  hal_parse("start");

  // reset
  IWDG->KR = 0xAAAA;

  // enable access
  IWDG->KR = 0x5555;

  // set reaload 0.002s
  IWDG->RLR = 0.002 * 40000 / 4;

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

  //TODO: usb optional
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB | RCC_PERIPHCLK_USART1 | RCC_PERIPHCLK_USART3 | RCC_PERIPHCLK_TIM8 | RCC_PERIPHCLK_ADC12 | RCC_PERIPHCLK_ADC34 | RCC_PERIPHCLK_RTC;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_SYSCLK;
  PeriphClkInit.Adc12ClockSelection  = RCC_ADC12PLLCLK_DIV1;
  PeriphClkInit.Adc34ClockSelection  = RCC_ADC34PLLCLK_DIV1;
  PeriphClkInit.USBClockSelection    = RCC_USBCLKSOURCE_PLL_DIV1_5;
  PeriphClkInit.Tim8ClockSelection   = RCC_TIM8CLK_PLLCLK;
  PeriphClkInit.RTCClockSelection    = RCC_RTCCLKSOURCE_LSI;
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

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void) {
  /* User can add his own implementation to report the HAL error return state */
  while(1) {
  }
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
