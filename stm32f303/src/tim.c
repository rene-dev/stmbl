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
#include "tim.h"
#include "f3hw.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

TIM_HandleTypeDef htim8;

/* TIM8 init function */
void MX_TIM8_Init(void) {
  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig;

  htim8.Instance           = TIM8;
  htim8.Init.Prescaler     = 0;
  htim8.Init.CounterMode   = TIM_COUNTERMODE_CENTERALIGNED3;
  htim8.Init.Period        = PWM_RES;
  htim8.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
#ifdef PWM_INVERT
  htim8.Init.RepetitionCounter = 1;
#else
  htim8.Init.RepetitionCounter = 0;
#endif
  if(HAL_TIM_Base_Init(&htim8) != HAL_OK) {
    Error_Handler();
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if(HAL_TIM_ConfigClockSource(&htim8, &sClockSourceConfig) != HAL_OK) {
    Error_Handler();
  }

  if(HAL_TIM_PWM_Init(&htim8) != HAL_OK) {
    Error_Handler();
  }

  sMasterConfig.MasterOutputTrigger  = TIM_TRGO_UPDATE;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode      = TIM_MASTERSLAVEMODE_DISABLE;
  if(HAL_TIMEx_MasterConfigSynchronization(&htim8, &sMasterConfig) != HAL_OK) {
    Error_Handler();
  }

  sConfigOC.OCMode       = TIM_OCMODE_PWM1;
  sConfigOC.Pulse        = 0;
  sConfigOC.OCPolarity   = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode   = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState  = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if(HAL_TIM_PWM_ConfigChannel(&htim8, &sConfigOC, TIM_CHANNEL_1) != HAL_OK) {
    Error_Handler();
  }

  if(HAL_TIM_PWM_ConfigChannel(&htim8, &sConfigOC, TIM_CHANNEL_2) != HAL_OK) {
    Error_Handler();
  }

  if(HAL_TIM_PWM_ConfigChannel(&htim8, &sConfigOC, TIM_CHANNEL_3) != HAL_OK) {
    Error_Handler();
  }

  sBreakDeadTimeConfig.OffStateRunMode  = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel        = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime         = PWM_DEADTIME;
  sBreakDeadTimeConfig.BreakState       = TIM_BREAK_ENABLE;
  sBreakDeadTimeConfig.BreakPolarity    = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.BreakFilter      = 0xa;  //0.55uS
  sBreakDeadTimeConfig.Break2State      = TIM_BREAK2_DISABLE;
  sBreakDeadTimeConfig.Break2Polarity   = TIM_BREAK2POLARITY_HIGH;
  sBreakDeadTimeConfig.Break2Filter     = 0;
  sBreakDeadTimeConfig.AutomaticOutput  = TIM_AUTOMATICOUTPUT_DISABLE;
  if(HAL_TIMEx_ConfigBreakDeadTime(&htim8, &sBreakDeadTimeConfig) != HAL_OK) {
    Error_Handler();
  }

  HAL_TIM_MspPostInit(&htim8);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *tim_baseHandle) {
  if(tim_baseHandle->Instance == TIM8) {
    /* USER CODE BEGIN TIM8_MspInit 0 */

    /* USER CODE END TIM8_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_TIM8_CLK_ENABLE();
    HAL_NVIC_SetPriority(TIM8_UP_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM8_UP_IRQn);
    /* USER CODE BEGIN TIM8_MspInit 1 */

    /* USER CODE END TIM8_MspInit 1 */
  }
}
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *timHandle) {
  GPIO_InitTypeDef GPIO_InitStruct;
  if(timHandle->Instance == TIM8) {
    /* USER CODE BEGIN TIM8_MspPostInit 0 */

    /* USER CODE END TIM8_MspPostInit 0 */

    /**TIM8 GPIO Configuration    
    PB3     ------> TIM8_CH1N
    PB4     ------> TIM8_CH2N
    PB5     ------> TIM8_CH3N
    PB6     ------> TIM8_CH1
    PB8     ------> TIM8_CH2
    PB9     ------> TIM8_CH3 
    */
    GPIO_InitStruct.Pin       = GPIO_PIN_3 | GPIO_PIN_4;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF4_TIM8;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin       = GPIO_PIN_5;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF3_TIM8;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin       = GPIO_PIN_6;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF5_TIM8;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin       = GPIO_PIN_8 | GPIO_PIN_9;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF10_TIM8;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* USER CODE BEGIN TIM8_MspPostInit 1 */

    /* USER CODE END TIM8_MspPostInit 1 */
  }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef *tim_baseHandle) {
  if(tim_baseHandle->Instance == TIM8) {
    /* USER CODE BEGIN TIM8_MspDeInit 0 */

    /* USER CODE END TIM8_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM8_CLK_DISABLE();
  }
  /* USER CODE BEGIN TIM8_MspDeInit 1 */

  /* USER CODE END TIM8_MspDeInit 1 */
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
