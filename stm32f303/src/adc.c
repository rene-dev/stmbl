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

/* Includes ------------------------------------------------------------------*/
#include "adc.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;
ADC_HandleTypeDef hadc3;
ADC_HandleTypeDef hadc4;

/* ADC1 init function */
void MX_ADC1_Init(void) {
  ADC_ChannelConfTypeDef sConfig;

  /**Common config 
    */
  hadc1.Instance                   = ADC1;
  hadc1.Init.ClockPrescaler        = ADC_CLOCK_ASYNC_DIV1;
  hadc1.Init.Resolution            = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode          = ADC_SCAN_ENABLE;
  hadc1.Init.ContinuousConvMode    = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.NbrOfDiscConversion   = 1;
  hadc1.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_RISING;
  hadc1.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T8_TRGO;
  hadc1.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion       = 6;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait      = DISABLE;
  hadc1.Init.Overrun               = ADC_OVR_DATA_OVERWRITTEN;
  if(HAL_ADC_Init(&hadc1) != HAL_OK) {
    Error_Handler();
  }

  sConfig.Channel      = ADC_CHANNEL_3;  // pa2 = opamp1_out = iw (3)
  sConfig.Rank         = 1;
  sConfig.SingleDiff   = ADC_SINGLE_ENDED;
  sConfig.SamplingTime = ADC_SAMPLETIME_19CYCLES_5;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset       = 0;
  if(HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
    Error_Handler();
  }

  sConfig.Rank = 2;
  if(HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
    Error_Handler();
  }

  sConfig.Rank = 3;
  if(HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
    Error_Handler();
  }

  sConfig.Rank = 4;
  if(HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
    Error_Handler();
  }

  sConfig.Rank = 5;
  if(HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
    Error_Handler();
  }

  sConfig.Channel      = ADC_CHANNEL_4;  // pa3 = uw
  sConfig.Rank         = 6;
  sConfig.SamplingTime = ADC_SAMPLETIME_181CYCLES_5;
  if(HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
    Error_Handler();
  }

  // sConfig.Channel = ADC_CHANNEL_2; // pa1 = opamp1_in = iw
  // sConfig.Rank = 3;
  // sConfig.SamplingTime = ADC_SAMPLETIME_61CYCLES_5;
  // if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
  // {
  //   Error_Handler();
  // }
}
/* ADC2 init function */
void MX_ADC2_Init(void) {
  ADC_ChannelConfTypeDef sConfig;
  // ADC_MultiModeTypeDef multimode;

  /**Common config 
    */
  hadc2.Instance                   = ADC2;
  hadc2.Init.ClockPrescaler        = ADC_CLOCK_ASYNC_DIV1;
  hadc2.Init.Resolution            = ADC_RESOLUTION_12B;
  hadc2.Init.ScanConvMode          = ADC_SCAN_ENABLE;
  hadc2.Init.ContinuousConvMode    = DISABLE;
  hadc2.Init.DiscontinuousConvMode = DISABLE;
  hadc2.Init.NbrOfDiscConversion   = 1;
  hadc2.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_RISING;
  hadc2.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T8_TRGO;
  hadc2.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
  hadc2.Init.NbrOfConversion       = 6;
  hadc2.Init.DMAContinuousRequests = DISABLE;
  hadc2.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
  hadc2.Init.LowPowerAutoWait      = DISABLE;
  hadc2.Init.Overrun               = ADC_OVR_DATA_OVERWRITTEN;
  if(HAL_ADC_Init(&hadc2) != HAL_OK) {
    Error_Handler();
  }

  sConfig.Channel      = ADC_CHANNEL_3;  // pa6 = opamp2_out = iu (3)
  sConfig.Rank         = 1;
  sConfig.SingleDiff   = ADC_SINGLE_ENDED;
  sConfig.SamplingTime = ADC_SAMPLETIME_19CYCLES_5;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset       = 0;
  if(HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK) {
    Error_Handler();
  }

  sConfig.Rank = 2;
  if(HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK) {
    Error_Handler();
  }

  sConfig.Rank = 3;
  if(HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK) {
    Error_Handler();
  }

  sConfig.Rank = 4;
  if(HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK) {
    Error_Handler();
  }

  sConfig.Rank = 5;
  if(HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK) {
    Error_Handler();
  }

  sConfig.Channel      = ADC_CHANNEL_2;  // pa5 = uv
  sConfig.Rank         = 6;
  sConfig.SamplingTime = ADC_SAMPLETIME_181CYCLES_5;
  if(HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK) {
    Error_Handler();
  }

  // sConfig.Channel = ADC_CHANNEL_4; // pa7 = opamp2_in = iu
  // sConfig.Rank = 3;
  // sConfig.SamplingTime = ADC_SAMPLETIME_61CYCLES_5;
  // if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  // {
  //   Error_Handler();
  // }
}
/* ADC3 init function */
void MX_ADC3_Init(void) {
  ADC_ChannelConfTypeDef sConfig;

  /**Common config 
    */
  hadc3.Instance                   = ADC3;
  hadc3.Init.ClockPrescaler        = ADC_CLOCK_ASYNC_DIV1;
  hadc3.Init.Resolution            = ADC_RESOLUTION_12B;
  hadc3.Init.ScanConvMode          = ADC_SCAN_ENABLE;
  hadc3.Init.ContinuousConvMode    = DISABLE;
  hadc3.Init.DiscontinuousConvMode = DISABLE;
  hadc3.Init.NbrOfDiscConversion   = 1;
  hadc3.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_RISING;
  hadc3.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T8_TRGO;
  hadc3.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
  hadc3.Init.NbrOfConversion       = 6;
  hadc3.Init.DMAContinuousRequests = DISABLE;
  hadc3.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
  hadc3.Init.LowPowerAutoWait      = DISABLE;
  hadc3.Init.Overrun               = ADC_OVR_DATA_OVERWRITTEN;
  if(HAL_ADC_Init(&hadc3) != HAL_OK) {
    Error_Handler();
  }

  /**Configure Regular Channel 
    */
  sConfig.Channel      = ADC_CHANNEL_1;  // pb1 = opamp3_out = iv (1)
  sConfig.Rank         = 1;
  sConfig.SingleDiff   = ADC_SINGLE_ENDED;
  sConfig.SamplingTime = ADC_SAMPLETIME_19CYCLES_5;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset       = 0;
  if(HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK) {
    Error_Handler();
  }

  sConfig.Rank = 2;
  if(HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK) {
    Error_Handler();
  }

  sConfig.Rank = 3;
  if(HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK) {
    Error_Handler();
  }

  sConfig.Rank = 4;
  if(HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK) {
    Error_Handler();
  }

  sConfig.Rank = 5;
  if(HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK) {
    Error_Handler();
  }

  sConfig.Channel      = ADC_CHANNEL_5;  // pb13 = uu
  sConfig.Rank         = 6;
  sConfig.SamplingTime = ADC_SAMPLETIME_181CYCLES_5;
  if(HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK) {
    Error_Handler();
  }
  //
  //   /**Configure Regular Channel
  //   */
  // sConfig.Channel = ADC_CHANNEL_12; // pb0 = opamp3_in = iv
  // sConfig.Rank = 3;
  // sConfig.SamplingTime = ADC_SAMPLETIME_61CYCLES_5;
  // if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
  // {
  //   Error_Handler();
  // }
}
/* ADC4 init function */
void MX_ADC4_Init(void) {
  ADC_ChannelConfTypeDef sConfig;
  // ADC_MultiModeTypeDef multimode;

  /**Common config 
    */
  hadc4.Instance                   = ADC4;
  hadc4.Init.ClockPrescaler        = ADC_CLOCK_ASYNC_DIV1;
  hadc4.Init.Resolution            = ADC_RESOLUTION_12B;
  hadc4.Init.ScanConvMode          = ADC_SCAN_ENABLE;
  hadc4.Init.ContinuousConvMode    = DISABLE;
  hadc4.Init.DiscontinuousConvMode = DISABLE;
  hadc4.Init.NbrOfDiscConversion   = 1;
  hadc4.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_RISING;
  hadc4.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T8_TRGO;
  hadc4.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
  hadc4.Init.NbrOfConversion       = 6;
  hadc4.Init.DMAContinuousRequests = DISABLE;
  hadc4.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
  hadc4.Init.LowPowerAutoWait      = DISABLE;
  hadc4.Init.Overrun               = ADC_OVR_DATA_OVERWRITTEN;
  if(HAL_ADC_Init(&hadc4) != HAL_OK) {
    Error_Handler();
  }

  sConfig.Channel      = ADC_CHANNEL_4;  // pb14 = hv_temp
  sConfig.Rank         = 1;
  sConfig.SingleDiff   = ADC_SINGLE_ENDED;
  sConfig.SamplingTime = ADC_SAMPLETIME_19CYCLES_5;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset       = 0;
  if(HAL_ADC_ConfigChannel(&hadc4, &sConfig) != HAL_OK) {
    Error_Handler();
  }

  sConfig.Rank = 2;
  if(HAL_ADC_ConfigChannel(&hadc4, &sConfig) != HAL_OK) {
    Error_Handler();
  }

  sConfig.Rank = 3;
  if(HAL_ADC_ConfigChannel(&hadc4, &sConfig) != HAL_OK) {
    Error_Handler();
  }

  sConfig.Channel = ADC_CHANNEL_5;  // pb15 = mot_temp
  sConfig.Rank    = 4;
  if(HAL_ADC_ConfigChannel(&hadc4, &sConfig) != HAL_OK) {
    Error_Handler();
  }

  sConfig.Rank = 5;
  if(HAL_ADC_ConfigChannel(&hadc4, &sConfig) != HAL_OK) {
    Error_Handler();
  }

  sConfig.Channel      = ADC_CHANNEL_3;  // pb12 = hv
  sConfig.Rank         = 6;
  sConfig.SamplingTime = ADC_SAMPLETIME_181CYCLES_5;
  if(HAL_ADC_ConfigChannel(&hadc4, &sConfig) != HAL_OK) {
    Error_Handler();
  }
}

static uint32_t HAL_RCC_ADC12_CLK_ENABLED = 0;
static uint32_t HAL_RCC_ADC34_CLK_ENABLED = 0;

void HAL_ADC_MspInit(ADC_HandleTypeDef *adcHandle) {
  GPIO_InitTypeDef GPIO_InitStruct;
  if(adcHandle->Instance == ADC1) {
    /* USER CODE BEGIN ADC1_MspInit 0 */

    /* USER CODE END ADC1_MspInit 0 */
    /* Peripheral clock enable */
    HAL_RCC_ADC12_CLK_ENABLED++;
    if(HAL_RCC_ADC12_CLK_ENABLED == 1) {
      __HAL_RCC_ADC12_CLK_ENABLE();
    }

    /**ADC1 GPIO Configuration    
    PA1     ------> ADC1_IN2
    PA2     ------> ADC1_IN3
    PA3     ------> ADC1_IN4 
    */
    GPIO_InitStruct.Pin  = GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USER CODE BEGIN ADC1_MspInit 1 */

    /* USER CODE END ADC1_MspInit 1 */
  } else if(adcHandle->Instance == ADC2) {
    /* USER CODE BEGIN ADC2_MspInit 0 */

    /* USER CODE END ADC2_MspInit 0 */
    /* Peripheral clock enable */
    HAL_RCC_ADC12_CLK_ENABLED++;
    if(HAL_RCC_ADC12_CLK_ENABLED == 1) {
      __HAL_RCC_ADC12_CLK_ENABLE();
    }

    /**ADC2 GPIO Configuration    
    PA5     ------> ADC2_IN2 
    PA6     ------> ADC2_IN3
    PA7     ------> ADC2_IN4
    */
    GPIO_InitStruct.Pin  = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USER CODE BEGIN ADC2_MspInit 1 */

    /* USER CODE END ADC2_MspInit 1 */
  } else if(adcHandle->Instance == ADC3) {
    /* USER CODE BEGIN ADC3_MspInit 0 */

    /* USER CODE END ADC3_MspInit 0 */
    /* Peripheral clock enable */
    HAL_RCC_ADC34_CLK_ENABLED++;
    if(HAL_RCC_ADC34_CLK_ENABLED == 1) {
      __HAL_RCC_ADC34_CLK_ENABLE();
    }

    /**ADC3 GPIO Configuration    
    PB0      ------> ADC3_IN12
    PB1      ------> ADC3_IN1
    PB13     ------> ADC3_IN5 
    */
    GPIO_InitStruct.Pin  = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* USER CODE BEGIN ADC3_MspInit 1 */

    /* USER CODE END ADC3_MspInit 1 */
  } else if(adcHandle->Instance == ADC4) {
    /* USER CODE BEGIN ADC4_MspInit 0 */

    /* USER CODE END ADC4_MspInit 0 */
    /* Peripheral clock enable */
    HAL_RCC_ADC34_CLK_ENABLED++;
    if(HAL_RCC_ADC34_CLK_ENABLED == 1) {
      __HAL_RCC_ADC34_CLK_ENABLE();
    }

    /**ADC4 GPIO Configuration    
    PB12     ------> ADC4_IN3
    PB14     ------> ADC4_IN4
    PB15     ------> ADC4_IN5 
    */
    GPIO_InitStruct.Pin  = GPIO_PIN_12 | GPIO_PIN_14 | GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* USER CODE BEGIN ADC4_MspInit 1 */

    /* USER CODE END ADC4_MspInit 1 */
  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef *adcHandle) {
  if(adcHandle->Instance == ADC1) {
    /* USER CODE BEGIN ADC1_MspDeInit 0 */

    /* USER CODE END ADC1_MspDeInit 0 */
    /* Peripheral clock disable */
    HAL_RCC_ADC12_CLK_ENABLED--;
    if(HAL_RCC_ADC12_CLK_ENABLED == 0) {
      __HAL_RCC_ADC12_CLK_DISABLE();
    }

    /**ADC1 GPIO Configuration    
    PA3     ------> ADC1_IN4 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_3);

    /* USER CODE BEGIN ADC1_MspDeInit 1 */

    /* USER CODE END ADC1_MspDeInit 1 */
  } else if(adcHandle->Instance == ADC2) {
    /* USER CODE BEGIN ADC2_MspDeInit 0 */

    /* USER CODE END ADC2_MspDeInit 0 */
    /* Peripheral clock disable */
    HAL_RCC_ADC12_CLK_ENABLED--;
    if(HAL_RCC_ADC12_CLK_ENABLED == 0) {
      __HAL_RCC_ADC12_CLK_DISABLE();
    }

    /**ADC2 GPIO Configuration    
    PA5     ------> ADC2_IN2 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5);

    /* USER CODE BEGIN ADC2_MspDeInit 1 */

    /* USER CODE END ADC2_MspDeInit 1 */
  } else if(adcHandle->Instance == ADC3) {
    /* USER CODE BEGIN ADC3_MspDeInit 0 */

    /* USER CODE END ADC3_MspDeInit 0 */
    /* Peripheral clock disable */
    HAL_RCC_ADC34_CLK_ENABLED--;
    if(HAL_RCC_ADC34_CLK_ENABLED == 0) {
      __HAL_RCC_ADC34_CLK_DISABLE();
    }

    /**ADC3 GPIO Configuration    
    PB13     ------> ADC3_IN5 
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_13);

    /* USER CODE BEGIN ADC3_MspDeInit 1 */

    /* USER CODE END ADC3_MspDeInit 1 */
  } else if(adcHandle->Instance == ADC4) {
    /* USER CODE BEGIN ADC4_MspDeInit 0 */

    /* USER CODE END ADC4_MspDeInit 0 */
    /* Peripheral clock disable */
    HAL_RCC_ADC34_CLK_ENABLED--;
    if(HAL_RCC_ADC34_CLK_ENABLED == 0) {
      __HAL_RCC_ADC34_CLK_DISABLE();
    }

    /**ADC4 GPIO Configuration    
    PB12     ------> ADC4_IN3
    PB14     ------> ADC4_IN4
    PB15     ------> ADC4_IN5 
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_12 | GPIO_PIN_14 | GPIO_PIN_15);

    /* USER CODE BEGIN ADC4_MspDeInit 1 */

    /* USER CODE END ADC4_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
