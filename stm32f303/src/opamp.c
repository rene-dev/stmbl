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
#include "opamp.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

OPAMP_HandleTypeDef hopamp1;
OPAMP_HandleTypeDef hopamp2;
OPAMP_HandleTypeDef hopamp3;
OPAMP_HandleTypeDef hopamp4;

/* OPAMP1 init function */
void MX_OPAMP1_Init(void) {
  hopamp1.Instance                    = OPAMP1;
  hopamp1.Init.Mode                   = OPAMP_PGA_MODE;
  hopamp1.Init.NonInvertingInput      = OPAMP_NONINVERTINGINPUT_IO0;
  hopamp1.Init.TimerControlledMuxmode = OPAMP_TIMERCONTROLLEDMUXMODE_DISABLE;
  hopamp1.Init.PgaConnect             = OPAMP_PGA_CONNECT_INVERTINGINPUT_NO;
  hopamp1.Init.PgaGain                = OPAMP_PGA_GAIN_8;
  hopamp1.Init.UserTrimming           = OPAMP_TRIMMING_FACTORY;
  if(HAL_OPAMP_Init(&hopamp1) != HAL_OK) {
    Error_Handler();
  }
}
/* OPAMP2 init function */
void MX_OPAMP2_Init(void) {
  hopamp2.Instance                    = OPAMP2;
  hopamp2.Init.Mode                   = OPAMP_PGA_MODE;
  hopamp2.Init.NonInvertingInput      = OPAMP_NONINVERTINGINPUT_IO0;
  hopamp2.Init.TimerControlledMuxmode = OPAMP_TIMERCONTROLLEDMUXMODE_DISABLE;
  hopamp2.Init.PgaConnect             = OPAMP_PGA_CONNECT_INVERTINGINPUT_NO;
  hopamp2.Init.PgaGain                = OPAMP_PGA_GAIN_8;
  hopamp2.Init.UserTrimming           = OPAMP_TRIMMING_FACTORY;
  if(HAL_OPAMP_Init(&hopamp2) != HAL_OK) {
    Error_Handler();
  }
}
/* OPAMP3 init function */
void MX_OPAMP3_Init(void) {
  hopamp3.Instance                    = OPAMP3;
  hopamp3.Init.Mode                   = OPAMP_PGA_MODE;
  hopamp3.Init.NonInvertingInput      = OPAMP_NONINVERTINGINPUT_IO0;
  hopamp3.Init.TimerControlledMuxmode = OPAMP_TIMERCONTROLLEDMUXMODE_DISABLE;
  hopamp3.Init.PgaConnect             = OPAMP_PGA_CONNECT_INVERTINGINPUT_NO;
  hopamp3.Init.PgaGain                = OPAMP_PGA_GAIN_8;
  hopamp3.Init.UserTrimming           = OPAMP_TRIMMING_FACTORY;
  if(HAL_OPAMP_Init(&hopamp3) != HAL_OK) {
    Error_Handler();
  }
}

/* OPAMP3 init function */
void MX_OPAMP4_Init(void) {
  hopamp4.Instance                    = OPAMP4;
  hopamp4.Init.Mode                   = OPAMP_PGA_MODE;
  hopamp4.Init.NonInvertingInput      = OPAMP_NONINVERTINGINPUT_IO3;
  hopamp4.Init.TimerControlledMuxmode = OPAMP_TIMERCONTROLLEDMUXMODE_DISABLE;
  hopamp4.Init.PgaConnect             = OPAMP_PGA_CONNECT_INVERTINGINPUT_NO;
  hopamp4.Init.PgaGain                = OPAMP_PGA_GAIN_8;
  hopamp4.Init.UserTrimming           = OPAMP_TRIMMING_FACTORY;
  if(HAL_OPAMP_Init(&hopamp4) != HAL_OK) {
    Error_Handler();
  }
}

void HAL_OPAMP_MspInit(OPAMP_HandleTypeDef *opampHandle) {
  GPIO_InitTypeDef GPIO_InitStruct;
  if(opampHandle->Instance == OPAMP1) {
    /* USER CODE BEGIN OPAMP1_MspInit 0 */

    /* USER CODE END OPAMP1_MspInit 0 */

    /**OPAMP1 GPIO Configuration    
    PA1     ------> OPAMP1_VINP
    PA2     ------> OPAMP1_VOUT 
    */
    GPIO_InitStruct.Pin  = GPIO_PIN_1 | GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USER CODE BEGIN OPAMP1_MspInit 1 */

    /* USER CODE END OPAMP1_MspInit 1 */
  } else if(opampHandle->Instance == OPAMP2) {
    /* USER CODE BEGIN OPAMP2_MspInit 0 */

    /* USER CODE END OPAMP2_MspInit 0 */

    /**OPAMP2 GPIO Configuration    
    PA6     ------> OPAMP2_VOUT
    PA7     ------> OPAMP2_VINP 
    */
    GPIO_InitStruct.Pin  = GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USER CODE BEGIN OPAMP2_MspInit 1 */

    /* USER CODE END OPAMP2_MspInit 1 */
  } else if(opampHandle->Instance == OPAMP3) {
    /* USER CODE BEGIN OPAMP3_MspInit 0 */

    /* USER CODE END OPAMP3_MspInit 0 */

    /**OPAMP3 GPIO Configuration    
    PB0     ------> OPAMP3_VINP
    PB1     ------> OPAMP3_VOUT 
    */
    GPIO_InitStruct.Pin  = GPIO_PIN_0 | GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* USER CODE BEGIN OPAMP3_MspInit 1 */

    /* USER CODE END OPAMP3_MspInit 1 */
  } else if(opampHandle->Instance == OPAMP4) {
    /* USER CODE BEGIN OPAMP4_MspInit 0 */

    /* USER CODE END OPAMP4_MspInit 0 */

    /**OPAMP4 GPIO Configuration    
    PB11     ------> OPAMP4_VINP
    PB12     ------> OPAMP4_VOUT 
    */
    GPIO_InitStruct.Pin  = GPIO_PIN_11 | GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* USER CODE BEGIN OPAMP4_MspInit 1 */

    /* USER CODE END OPAMP4_MspInit 1 */
  }
}

void HAL_OPAMP_MspDeInit(OPAMP_HandleTypeDef *opampHandle) {
  if(opampHandle->Instance == OPAMP1) {
    /* USER CODE BEGIN OPAMP1_MspDeInit 0 */

    /* USER CODE END OPAMP1_MspDeInit 0 */

    /**OPAMP1 GPIO Configuration    
    PA1     ------> OPAMP1_VINP
    PA2     ------> OPAMP1_VOUT 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_1 | GPIO_PIN_2);

    /* USER CODE BEGIN OPAMP1_MspDeInit 1 */

    /* USER CODE END OPAMP1_MspDeInit 1 */
  } else if(opampHandle->Instance == OPAMP2) {
    /* USER CODE BEGIN OPAMP2_MspDeInit 0 */

    /* USER CODE END OPAMP2_MspDeInit 0 */

    /**OPAMP2 GPIO Configuration    
    PA6     ------> OPAMP2_VOUT
    PA7     ------> OPAMP2_VINP 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_6 | GPIO_PIN_7);

    /* USER CODE BEGIN OPAMP2_MspDeInit 1 */

    /* USER CODE END OPAMP2_MspDeInit 1 */
  } else if(opampHandle->Instance == OPAMP3) {
    /* USER CODE BEGIN OPAMP3_MspDeInit 0 */

    /* USER CODE END OPAMP3_MspDeInit 0 */

    /**OPAMP3 GPIO Configuration    
    PB0     ------> OPAMP3_VINP
    PB1     ------> OPAMP3_VOUT 
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_0 | GPIO_PIN_1);

    /* USER CODE BEGIN OPAMP3_MspDeInit 1 */

    /* USER CODE END OPAMP3_MspDeInit 1 */
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
