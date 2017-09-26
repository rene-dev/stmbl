/**
  ******************************************************************************
  * File Name          : ADC.c
  * Description        : This file provides code for the configuration
  *                      of the ADC instances.
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
  hadc1.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_TRGO2;
  hadc1.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion       = 4;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait      = DISABLE;
  hadc1.Init.Overrun               = ADC_OVR_DATA_OVERWRITTEN;
  if(HAL_ADC_Init(&hadc1) != HAL_OK) {
    Error_Handler();
  }

  sConfig.Channel      = ADC_CHANNEL_1;  // pa0 = dclink
  sConfig.Rank         = 1;
  sConfig.SingleDiff   = ADC_SINGLE_ENDED;
  sConfig.SamplingTime = ADC_SAMPLETIME_61CYCLES_5;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset       = 0;
  if(HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
    Error_Handler();
  }

  sConfig.Rank    = 2;
  sConfig.Channel = ADC_CHANNEL_4;  // pa3 = bemf0
  if(HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
    Error_Handler();
  }

  sConfig.Rank    = 3;
  sConfig.Channel = ADC_CHANNEL_3;  // pa2 = in0
  if(HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
    Error_Handler();
  }

  sConfig.Rank    = 4;
  sConfig.Channel = ADC_CHANNEL_2;  // pa1 = temp
  if(HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
    Error_Handler();
  }
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
  hadc2.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_TRGO2;
  hadc2.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
  hadc2.Init.NbrOfConversion       = 4;
  hadc2.Init.DMAContinuousRequests = DISABLE;
  hadc2.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
  hadc2.Init.LowPowerAutoWait      = DISABLE;
  hadc2.Init.Overrun               = ADC_OVR_DATA_OVERWRITTEN;
  if(HAL_ADC_Init(&hadc2) != HAL_OK) {
    Error_Handler();
  }

  sConfig.Channel      = ADC_CHANNEL_3;  // pa6 = opamp2_out = shunt low
  sConfig.Rank         = 1;
  sConfig.SingleDiff   = ADC_SINGLE_ENDED;
  sConfig.SamplingTime = ADC_SAMPLETIME_61CYCLES_5;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset       = 0;
  if(HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK) {
    Error_Handler();
  }

  sConfig.Rank    = 2;
  sConfig.Channel = ADC_CHANNEL_1;  // pa4 = bemf1
  if(HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK) {
    Error_Handler();
  }

  sConfig.Rank    = 3;
  sConfig.Channel = ADC_CHANNEL_2;  // pa5 = in1
  if(HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK) {
    Error_Handler();
  }

  sConfig.Rank    = 4;
  sConfig.Channel = ADC_CHANNEL_3;  // pa6 = opamp2_out = shunt low
  if(HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK) {
    Error_Handler();
  }
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
  hadc3.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_TRGO2;
  hadc3.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
  hadc3.Init.NbrOfConversion       = 4;
  hadc3.Init.DMAContinuousRequests = DISABLE;
  hadc3.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
  hadc3.Init.LowPowerAutoWait      = DISABLE;
  hadc3.Init.Overrun               = ADC_OVR_DATA_OVERWRITTEN;
  if(HAL_ADC_Init(&hadc3) != HAL_OK) {
    Error_Handler();
  }

  /**Configure Regular Channel 
    */
  sConfig.Channel      = ADC_CHANNEL_1;  // pb1 = opamp3_out = shunt a
  sConfig.Rank         = 1;
  sConfig.SingleDiff   = ADC_SINGLE_ENDED;
  sConfig.SamplingTime = ADC_SAMPLETIME_61CYCLES_5;
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
  hadc4.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_TRGO2;
  hadc4.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
  hadc4.Init.NbrOfConversion       = 4;
  hadc4.Init.DMAContinuousRequests = DISABLE;
  hadc4.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
  hadc4.Init.LowPowerAutoWait      = DISABLE;
  hadc4.Init.Overrun               = ADC_OVR_DATA_OVERWRITTEN;
  if(HAL_ADC_Init(&hadc4) != HAL_OK) {
    Error_Handler();
  }

  sConfig.Channel      = ADC_CHANNEL_3;  // pb12 = shunt b
  sConfig.Rank         = 1;
  sConfig.SingleDiff   = ADC_SINGLE_ENDED;
  sConfig.SamplingTime = ADC_SAMPLETIME_61CYCLES_5;
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

  sConfig.Rank = 4;
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
    PA0     ------> ADC1_IN1
    PA1     ------> ADC1_IN2
    PA2     ------> ADC1_IN3
    PA3     ------> ADC1_IN4 
    */
    GPIO_InitStruct.Pin  = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3;
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
    PA4     ------> ADC2_IN1
    PA5     ------> ADC2_IN2 
    PA6     ------> ADC2_IN3
    */
    GPIO_InitStruct.Pin  = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
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
    PB1      ------> ADC3_IN1
    */
    GPIO_InitStruct.Pin  = GPIO_PIN_0 | GPIO_PIN_1;
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
    */
    GPIO_InitStruct.Pin  = GPIO_PIN_12;
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
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

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
    PA4     ------> ADC2_IN1
    PA5     ------> ADC2_IN2 
    PA6     ------> ADC2_IN3
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6);

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
    PB1     ------> ADC3_IN1 
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_1);

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
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_12);

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
