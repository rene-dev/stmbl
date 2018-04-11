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
#include "version.h"
#include "common.h"
#include "f3hw.h"
#include "tim.h"

uint32_t systick_freq;
CRC_HandleTypeDef hcrc;
RTC_HandleTypeDef hrtc;

volatile packet_bootloader_t rx_buf;
volatile packet_bootloader_t tx_buf;


void SystemClock_Config(void);
void Error_Handler(void);

#define APP_START 0x08004000
#define APP_END 0x08020000
#define APP_RANGE_VALID(a, s) (!(((a) | (s)) & 3) && (a) >= APP_START && ((a) + (s)) <= APP_END)
#define VERSION_INFO_OFFSET 0x188
static volatile const version_info_t *app_info = (void *)(APP_START + VERSION_INFO_OFFSET);

static int app_ok(void) {
  if(!APP_RANGE_VALID(APP_START, app_info->image_size)) {
    return 0;
  }
  uint32_t crc = HAL_CRC_Calculate(&hcrc, (uint32_t *)APP_START, app_info->image_size / 4);

  if(crc != 0) {
    return 0;
  }
  return 1;
}

void TIM8_UP_IRQHandler() {
  static uint32_t last_dma_count = 0;

  uint32_t dma_count = DMA1_Channel3->CNDTR;
  // if(USART3->ISR & USART_ISR_RTOF) {                                    // idle line
  //   USART3->ICR |= USART_ICR_RTOCF | USART_ICR_FECF | USART_ICR_ORECF;  // timeout clear flag
  if(dma_count == last_dma_count) {  // framing
    last_dma_count = 0;

    // start rx DMA
    DMA1_Channel3->CCR &= (uint16_t)(~DMA_CCR_EN);
    DMA1_Channel3->CNDTR = sizeof(packet_bootloader_t);
    DMA1_Channel3->CCR |= DMA_CCR_EN;
  } else {
    last_dma_count = dma_count;
  }

  if(dma_count == 0) {
    if(rx_buf.header.slave_addr == 255 && rx_buf.header.len == (sizeof(packet_bootloader_t) - sizeof(stmbl_talk_header_t)) / 4 && rx_buf.header.crc == HAL_CRC_Calculate(&hcrc, (uint32_t *)&(rx_buf.header.slave_addr), sizeof(packet_bootloader_t) / 4 - 1)) {
      //do stuff
      //tx_buf.state = do_stuff();

      switch(rx_buf.header.flags.cmd) {
        case NO_CMD:
          break;
        case WRITE_CONF:
          break;
        case READ_CONF:
          break;
        case DO_RESET:
          HAL_FLASH_Lock();
          HAL_NVIC_SystemReset();
          break;
        case BOOTLOADER:
          break;
      }

      HAL_StatusTypeDef status = HAL_OK;
      switch(rx_buf.cmd) {
        case BOOTLOADER_OPCODE_NOP:
          break;
        case BOOTLOADER_OPCODE_READ:
          tx_buf.value = *(uint32_t *)rx_buf.addr;
          tx_buf.addr  = rx_buf.addr;
          break;

        case BOOTLOADER_OPCODE_WRITE:
          if(*(uint32_t *)rx_buf.addr != rx_buf.value) {
            status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, rx_buf.addr, rx_buf.value);
          }
          if(*(uint32_t *)rx_buf.addr != rx_buf.value) {
            status = HAL_ERROR;
          }
          tx_buf.value = *(uint32_t *)rx_buf.addr;
          tx_buf.addr  = rx_buf.addr;
          break;

        case BOOTLOADER_OPCODE_PAGEERASE:
          HAL_FLASH_Unlock();
          uint32_t NbOfPages = 0;
          uint32_t PageError = 0;
          /* Variable contains Flash operation status */
          FLASH_EraseInitTypeDef eraseinitstruct;
          //TODO: only erase APP pages
          /* Get the number of sector to erase from 1st sector*/
          NbOfPages = (APP_END - APP_START) / FLASH_PAGE_SIZE;
          //NbOfPages                 = 1;
          eraseinitstruct.TypeErase = FLASH_TYPEERASE_PAGES;
          //eraseinitstruct.PageAddress = USBD_DFU_APP_DEFAULT_ADD;
          eraseinitstruct.PageAddress = APP_START;
          eraseinitstruct.NbPages     = NbOfPages;
          status                      = HAL_FLASHEx_Erase(&eraseinitstruct, &PageError);
          break;

        case BOOTLOADER_OPCODE_CRCCHECK:
          if(app_ok()) {
            status = HAL_OK;
          } else {
            status = HAL_ERROR;
          }
          break;
      }

      if(status != HAL_OK){
        tx_buf.state = BOOTLOADER_STATE_NAK;
      }
      else{
        tx_buf.state = BOOTLOADER_STATE_OK;
      }

      tx_buf.cmd                  = rx_buf.cmd;
      tx_buf.header.flags.counter = rx_buf.header.flags.counter;
      tx_buf.header.slave_addr    = 255;
      tx_buf.header.len           = (sizeof(packet_bootloader_t) - sizeof(stmbl_talk_header_t)) / 4;
      tx_buf.header.conf_addr     = 0;
      tx_buf.header.config.u32    = 0;
      tx_buf.header.flags.cmd     = NO_CMD;
      tx_buf.header.crc           = HAL_CRC_Calculate(&hcrc, (uint32_t *)&(tx_buf.header.slave_addr), sizeof(packet_bootloader_t) / 4 - 1);

      rx_buf.header.crc = 0;

      // start tx DMA
      DMA1_Channel2->CCR &= (uint16_t)(~DMA_CCR_EN);
      DMA1_Channel2->CNDTR = sizeof(packet_bootloader_t);
      DMA1_Channel2->CCR |= DMA_CCR_EN;
    }
  }

  __HAL_TIM_CLEAR_IT(&htim8, TIM_IT_UPDATE);
}

void uart_init() {
  GPIO_InitTypeDef GPIO_InitStruct;

  /* Peripheral clock enable */
  __HAL_RCC_USART3_CLK_ENABLE();

  UART_HandleTypeDef huart3;
  huart3.Instance                    = USART3;
  huart3.Init.BaudRate               = DATABAUD;
  huart3.Init.WordLength             = UART_WORDLENGTH_8B;
  huart3.Init.StopBits               = UART_STOPBITS_1;
  huart3.Init.Parity                 = UART_PARITY_NONE;
  huart3.Init.Mode                   = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl              = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling           = UART_OVERSAMPLING_8;
  huart3.Init.OneBitSampling         = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  USART3->CR3 |= USART_CR3_DMAT | USART_CR3_DMAR | USART_CR3_OVRDIS;
  HAL_UART_Init(&huart3);

  /**USART3 GPIO Configuration    
   PB10     ------> USART3_TX
   PB11     ------> USART3_RX 
   */
  GPIO_InitStruct.Pin       = GPIO_PIN_10 | GPIO_PIN_11;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  __HAL_RCC_DMA1_CLK_ENABLE();

  //TX DMA
  DMA1_Channel2->CCR &= (uint16_t)(~DMA_CCR_EN);
  DMA1_Channel2->CPAR  = (uint32_t) & (USART3->TDR);
  DMA1_Channel2->CMAR  = (uint32_t)&tx_buf;
  DMA1_Channel2->CNDTR = sizeof(packet_bootloader_t);
  DMA1_Channel2->CCR   = DMA_CCR_MINC | DMA_CCR_DIR;  // | DMA_CCR_PL_0 | DMA_CCR_PL_1
  DMA1->IFCR           = DMA_IFCR_CTCIF2 | DMA_IFCR_CHTIF2 | DMA_IFCR_CGIF2;

  //RX DMA
  DMA1_Channel3->CCR &= (uint16_t)(~DMA_CCR_EN);
  DMA1_Channel3->CPAR  = (uint32_t) & (USART3->RDR);
  DMA1_Channel3->CMAR  = (uint32_t)&rx_buf;
  DMA1_Channel3->CNDTR = sizeof(packet_bootloader_t);
  DMA1_Channel3->CCR   = DMA_CCR_MINC;  // | DMA_CCR_PL_0 | DMA_CCR_PL_1
  DMA1->IFCR           = DMA_IFCR_CTCIF3 | DMA_IFCR_CHTIF3 | DMA_IFCR_CGIF3;
  DMA1_Channel3->CCR |= DMA_CCR_EN;

  USART3->RTOR = 16;               // 16 bits timeout
  USART3->CR2 |= USART_CR2_RTOEN;  // timeout en
  USART3->ICR |= USART_ICR_RTOCF;  // timeout clear flag
}

/* RTC init function */
static void MX_RTC_Init(void) {
  /**Initialize RTC Only 
    */
  hrtc.Instance            = RTC;
  hrtc.Init.HourFormat     = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv   = 127;
  hrtc.Init.SynchPrediv    = 255;
  hrtc.Init.OutPut         = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType     = RTC_OUTPUT_TYPE_OPENDRAIN;
  if(HAL_RTC_Init(&hrtc) != HAL_OK) {
    Error_Handler();
  }
}

int main(void) {
  // Relocate interrupt vectors
  extern void *g_pfnVectors;
  SCB->VTOR = (uint32_t)&g_pfnVectors;

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

  __HAL_RCC_DMA1_CLK_ENABLE();
  __HAL_RCC_DMA2_CLK_ENABLE();
  __HAL_RCC_RTC_ENABLE();

  hcrc.Instance                     = CRC;
  hcrc.Init.DefaultPolynomialUse    = DEFAULT_POLYNOMIAL_ENABLE;
  hcrc.Init.DefaultInitValueUse     = DEFAULT_INIT_VALUE_ENABLE;
  hcrc.Init.InputDataInversionMode  = CRC_INPUTDATA_INVERSION_NONE;
  hcrc.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
  hcrc.InputDataFormat              = CRC_INPUTDATA_FORMAT_WORDS;

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED_PIN */
  GPIO_InitStruct.Pin   = LED_PIN;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);

  __HAL_RCC_CRC_CLK_ENABLE();

  if(HAL_CRC_Init(&hcrc) != HAL_OK) {
    Error_Handler();
  }

  MX_RTC_Init();


  if(app_ok() && RTC->BKP0R == 0x00000000) {
    // SCB->VTOR = APP_START;
    /* Jump to user application */
    void (*JumpToApplication)(void);
    uint32_t JumpAddress = *(__IO uint32_t *)(APP_START + 4);
    JumpToApplication    = (void *)JumpAddress;

    /* Initialize user application's Stack Pointer */
    __set_MSP(*(__IO uint32_t *)APP_START);
    JumpToApplication();
    while(1) {
    }
  } else {
    uart_init();

    tx_buf.header.slave_addr = 255;
    tx_buf.header.len        = (sizeof(packet_bootloader_t) - sizeof(stmbl_talk_header_t)) / 4;
    tx_buf.header.conf_addr  = 0;
    tx_buf.header.config.u32 = 0;
    tx_buf.header.flags.cmd  = NO_CMD;

    // start rx DMA
    DMA1_Channel3->CCR &= (uint16_t)(~DMA_CCR_EN);
    DMA1_Channel3->CNDTR = sizeof(packet_bootloader_t);
    DMA1_Channel3->CCR |= DMA_CCR_EN;

    MX_TIM8_Init();
    if(HAL_TIM_Base_Start_IT(&htim8) != HAL_OK) {
      Error_Handler();
    }
  }
  RTC->BKP0R = 0x00000000;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while(1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);
    HAL_Delay(50);
    HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET);
    HAL_Delay(50);
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_HSE;
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

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART3 | RCC_PERIPHCLK_TIM8 | RCC_PERIPHCLK_RTC;
  PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_SYSCLK;
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

//Delay implementation for hal_term.c
void Wait(uint32_t ms) {
  HAL_Delay(ms);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void) {
  /* User can add his own implementation to report the HAL error return state */
  while(1) {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
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
