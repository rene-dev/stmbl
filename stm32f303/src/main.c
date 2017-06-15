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
//#include "usb_device.h"

/* USER CODE BEGIN Includes */
#include <math.h>
#include "defines.h"
#include "hal.h"
#include "angle.h"
//#include "usbd_cdc_if.h"
#include "version.h"
#include "common.h"
#include "commands.h"
#include "f3hw.h"
#include "uart_cdc.h"

uint32_t systick_freq;
CRC_HandleTypeDef hcrc;

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


void TIM8_UP_IRQHandler(){
   GPIOA->BSRR |= GPIO_PIN_9;
   __HAL_TIM_CLEAR_IT(&htim8, TIM_IT_UPDATE);
   hal_run_rt();
   if(__HAL_TIM_GET_FLAG(&htim8, TIM_IT_UPDATE) == SET){
      hal_stop();
      hal.hal_state = RT_TOO_LONG;
   }
   GPIOA->BSRR |= GPIO_PIN_9 << 16;
}

void bootloader(char * ptr){
  #ifdef USB_DISCONNECT_PIN
  HAL_GPIO_WritePin(USB_DISCONNECT_PORT, USB_DISCONNECT_PIN, GPIO_PIN_SET);
  HAL_Delay(100);
  #endif
  RTC->BKP0R = 0xDEADBEEF;
  NVIC_SystemReset();
}

COMMAND("bootloader", bootloader);

void about(char * ptr){
   printf("flash acr: %lu\n", FLASH->ACR);
}
COMMAND("about", about);

void reset(char * ptr){
   HAL_NVIC_SystemReset();
}
COMMAND("reset", reset);

int main(void)
{
  
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
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

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
    //MX_USB_DEVICE_Init();
    cdc_init();
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
    __HAL_RCC_DMA1_CLK_ENABLE();
    __HAL_RCC_DMA2_CLK_ENABLE();
    __HAL_RCC_RTC_ENABLE();

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

    hal_init(1.0 / 15000.0, 0.0);
    // hal load comps
    load_comp(comp_by_name("term"));
    load_comp(comp_by_name("sim"));
    load_comp(comp_by_name("io"));
    load_comp(comp_by_name("vel"));
    load_comp(comp_by_name("vel"));
    load_comp(comp_by_name("ypid"));
    // load_comp(comp_by_name("ls"));
    // load_comp(comp_by_name("dq"));
    // load_comp(comp_by_name("idq"));
    // load_comp(comp_by_name("svm"));
    // load_comp(comp_by_name("hv"));
    load_comp(comp_by_name("hvdc"));
    // load_comp(comp_by_name("dc"));
    load_comp(comp_by_name("enc"));
    // load_comp(comp_by_name("curpid"));
    // hal parse config
    hal_init_nrt();

    hal_parse("enc0.rt_prio = 0.01");
    hal_parse("vel0.rt_prio = 0.02");
    hal_parse("vel1.rt_prio = 0.021");
    hal_parse("ypid0.rt_prio = 0.03");
    hal_parse("term0.rt_prio = 0.1");
    hal_parse("io0.rt_prio = 1.0");
    // hal_parse("curpid0.rt_prio = 3.0");
    // hal_parse("dc0.rt_prio = 4.0");
    // hal_parse("svm0.rt_prio = 5.0");
    // hal_parse("hv0.rt_prio = 6.0");
    hal_parse("hvdc0.rt_prio = 6.0");
    hal_parse("sim0.rt_prio = 7.0");

    hal_parse("term0.send_step = 100.0");
    hal_parse("term0.gain0 = 20.0");
    hal_parse("term0.gain1 = 20.0");
    hal_parse("term0.gain2 = 20.0");
    hal_parse("term0.gain3 = 20.0");
    hal_parse("term0.gain4 = 20.0");
    hal_parse("term0.gain5 = 20.0");
    hal_parse("term0.gain6 = 20.0");
    hal_parse("term0.gain7 = 20.0");

    // hal_parse("svm0.u = dc0.u");
    // hal_parse("svm0.v = dc0.v");
    // hal_parse("svm0.w = dc0.w");
    // hal_parse("svm0.mode = 2.0");//TODO: BUG, only 2 works
    // hal_parse("hv0.u = svm0.su");
    // hal_parse("hv0.v = svm0.sv");
    // hal_parse("hv0.w = svm0.sw");
    // hal_parse("svm0.udc = io0.udc");
    hal_parse("hv0.hv_temp = io0.hv_temp");

    hal_parse("io0.led = term0.con");

    hal_parse("term0.wave0 = vel1.pos_out");
    hal_parse("term0.wave1 = enc0.pos");
    hal_parse("term0.wave2 = vel1.vel");
    hal_parse("term0.wave3 = vel0.vel");

    hal_parse("term0.gain0 = 20.0");
    hal_parse("term0.gain1 = 20.0");
    hal_parse("term0.gain2 = 1.0");
    hal_parse("term0.gain3 = 1.0");

    hal_parse("ypid0.pos_ext_cmd = vel1.pos_out");
    hal_parse("ypid0.vel_ext_cmd = vel1.vel");

    hal_parse("ypid0.pos_fb = enc0.pos");
    hal_parse("ypid0.vel_fb = vel0.vel");
    hal_parse("ypid0.max_vel = 800");
    hal_parse("ypid0.max_acc = 80000");
    // hal_parse("net0.fb_d", "ypid0.vel_fb");
    // hal_parse("fault0.en_pid", "ypid0.enable");
    hal_parse("ypid0.pos_p = 10");
    hal_parse("ypid0.vel_p = 1");
    hal_parse("ypid0.vel_i = 0.005");
    hal_parse("ypid0.vel_ff = 1");
    // hal_parse("ypid0.max_vel");
    // hal_parse("ypid0.max_acc");
    hal_parse("ypid0.max_out = io0.udc");
    hal_parse("hvdc0.uq = ypid0.out");
    hal_parse("hvdc0.udc = io0.udc");
    // hal_parse("ypid0.saturated", "fault0.sat");
    // hal_parse("ypid0.pos_error", "fault0.pos_error");
    // hal_parse("conf0.acc_p", "ypid0.vel_p");//TODO: rename config pins?
    // hal_parse("conf0.acc_pi", "ypid0.vel_i");//TODO: rename config pins?
    hal_parse("vel0.w = 2000.0");//TODO: change to velbuf
    hal_parse("vel0.pos_in = enc0.pos");
    //TODO: pins here are overwritten by Init
    //TODO: unlink by setting to value does not work


    // error foo
    hal_start();

  while (1)
  {
     hal_run_nrt();
     cdc_poll();
     HAL_Delay(1);
  }

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
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
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
                              |RCC_PERIPHCLK_ADC12|RCC_PERIPHCLK_ADC34|RCC_PERIPHCLK_RTC;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_SYSCLK;
  PeriphClkInit.Adc12ClockSelection = RCC_ADC12PLLCLK_DIV1;
  PeriphClkInit.Adc34ClockSelection = RCC_ADC34PLLCLK_DIV1;
  PeriphClkInit.USBClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  PeriphClkInit.Tim8ClockSelection = RCC_TIM8CLK_PLLCLK;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
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
