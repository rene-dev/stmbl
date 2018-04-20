/**
  ******************************************************************************
  * @file           : usbd_dfu_if.c
  * @brief          :
  ******************************************************************************
  *
  * Copyright (c) 2017 STMicroelectronics International N.V. 
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
#include "usbd_dfu_if.h"
/* USER CODE BEGIN INCLUDE */
#define FLASH_ERASE_TIME (uint16_t)50
#define FLASH_PROGRAM_TIME (uint16_t)50
#define USBD_DFU_APP_END_ADD 0x08020000
/* USER CODE END INCLUDE */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @{
  */
/** @defgroup USBD_DFU 
  * @brief usbd core module
  * @{
  */

/** @defgroup USBD_DFU_Private_TypesDefinitions
  * @{
  */
/* USER CODE BEGIN PRIVATE_TYPES */
/* USER CODE END PRIVATE_TYPES */
/**
  * @}
  */

/** @defgroup USBD_DFU_Private_Defines
  * @{
  */
//#define FLASH_DESC_STR      "@Internal Flash   /0x08000000/03*016Ka,01*016Kg,01*064Kg,07*128Kg,04*016Kg,01*064Kg,07*128Kg"
#define FLASH_DESC_STR "@Internal Flash  /0x08004000/112*0002Kg"
//USBD_DFU_MEDIA Interface USBD_DFU_MEDIA Parameter Description: The description of the flash (used by PC tool DFuSe) Each Alternate setting string descriptor must follow this memory mapping so that the PC Host Software can decode the right mapping for the selected device: ● @: To detect that this is a special mapping descriptor (to avoid decoding standard descriptor) ● /: for separator between zones ● Maximum 8 digits per address starting by “0x” ● /: for separator between zones ● Maximum of 2 digits for the number of sectors ● *: For separator between number of sectors and sector size ● Maximum 3 digits for sector size between 0 and 999 ● 1 digit for the sector size multiplier. Valid entries are: B (byte), K (Kilo), M (Mega) ● 1 digit for the sector type as follows: – a (0x41): Readable – b (0x42): Erasable – c (0x43): Readable and Erasabled (0x44): Writeable – e (0x45): Readable and Writeable – f (0x46): Erasable and Writeable – g (0x47): Readable, Erasable and Writeable Note: If the target memory is not contiguous, the user can add the new sectors to be decoded just after a slash"/" as shown in the following example: "@Flash /0xF000/1*4Ka/0xE000/1*4Kg/0x8000/2*24Kg"
//int:  Found DFU: [0483:df11] ver=2200, devnum=6, cfg=1, intf=0, path="20-1", alt=0, name="@Internal Flash  /0x08000000/128*0002Kg", serial="2060374D2033"
//this: Found DFU: [0483:df11] ver=0200, devnum=5, cfg=1, intf=0, path="20-1", alt=0, name="@Internal Flash   /0x08000000/03*016Ka,01*016Kg,01*064Kg,07*128Kg,04*016Kg,01*064Kg,07*128Kg", serial="00000000001A"
//int: 2048 transfer size!
/* USER CODE BEGIN PRIVATE_DEFINES */
/* USER CODE END PRIVATE_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_DFU_Private_Macros
  * @{
  */
/* USER CODE BEGIN PRIVATE_MACRO */
/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */

/** @defgroup USBD_AUDIO_IF_Private_Variables
  * @{
  */
/* USER CODE BEGIN PRIVATE_VARIABLES */
/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_DFU_IF_Exported_Variables
  * @{
  */
extern USBD_HandleTypeDef hUsbDeviceFS;
/* USER CODE BEGIN EXPORTED_VARIABLES */
/* USER CODE END EXPORTED_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_DFU_Private_FunctionPrototypes
  * @{
  */
static uint16_t MEM_If_Init_FS(void);
static uint16_t MEM_If_Erase_FS(uint32_t Add);
static uint16_t MEM_If_Write_FS(uint8_t *src, uint8_t *dest, uint32_t Len);
static uint8_t *MEM_If_Read_FS(uint8_t *src, uint8_t *dest, uint32_t Len);
static uint16_t MEM_If_DeInit_FS(void);
static uint16_t MEM_If_GetStatus_FS(uint32_t Add, uint8_t Cmd, uint8_t *buffer);

/* USER CODE BEGIN PRIVATE_FUNCTIONS_DECLARATION */
/* USER CODE END PRIVATE_FUNCTIONS_DECLARATION */

/**
  * @}
  */

#if defined(__ICCARM__) /*!< IAR Compiler */
#pragma data_alignment = 4
#endif
__ALIGN_BEGIN USBD_DFU_MediaTypeDef USBD_DFU_fops_FS __ALIGN_END =
    {
        (uint8_t *)FLASH_DESC_STR,
        MEM_If_Init_FS,
        MEM_If_DeInit_FS,
        MEM_If_Erase_FS,
        MEM_If_Write_FS,
        MEM_If_Read_FS,
        MEM_If_GetStatus_FS,
};

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  MEM_If_Init_FS
  *         Memory initialization routine.
  * @param  None
  * @retval 0 if operation is successful, MAL_FAIL else.
  */
uint16_t MEM_If_Init_FS(void) {
  /* USER CODE BEGIN 0 */
  /* Unlock the internal flash */
  HAL_FLASH_Unlock();
  return (USBD_OK);
  /* USER CODE END 0 */
}

/**
  * @brief  MEM_If_DeInit_FS
  *         De-Initializes Memory.
  * @param  None
  * @retval 0 if operation is successful, MAL_FAIL else.
  */
uint16_t MEM_If_DeInit_FS(void) {
  /* USER CODE BEGIN 1 */
  /* Lock the internal flash */
  HAL_FLASH_Lock();
  return (USBD_OK);
  /* USER CODE END 1 */
}

/**
  * @brief  MEM_If_Erase_FS
  *         Erase sector.
  * @param  Add: Address of sector to be erased.
  * @retval 0 if operation is successful, MAL_FAIL else.
  */
uint16_t MEM_If_Erase_FS(uint32_t Add) {
  /* USER CODE BEGIN 2 */
  uint32_t NbOfPages = 0;
  uint32_t PageError = 0;
  /* Variable contains Flash operation status */
  HAL_StatusTypeDef status;
  FLASH_EraseInitTypeDef eraseinitstruct;
  //TODO: only erase APP pages
  /* Get the number of sector to erase from 1st sector*/
  //NbOfPages = (USBD_DFU_APP_END_ADD - USBD_DFU_APP_DEFAULT_ADD) / FLASH_PAGE_SIZE;
  NbOfPages                 = 1;
  eraseinitstruct.TypeErase = FLASH_TYPEERASE_PAGES;
  //eraseinitstruct.PageAddress = USBD_DFU_APP_DEFAULT_ADD;
  eraseinitstruct.PageAddress = Add;
  eraseinitstruct.NbPages     = NbOfPages;
  status                      = HAL_FLASHEx_Erase(&eraseinitstruct, &PageError);

  if(status != HAL_OK) {
    return 1;
  }
  return 0;
  /* USER CODE END 2 */
}

/**
  * @brief  MEM_If_Write_FS
  *         Memory write routine.
  * @param  src: Pointer to the source buffer. Address to be written to.
  * @param  dest: Pointer to the destination buffer.
  * @param  Len: Number of data to be written (in bytes).
  * @retval 0 if operation is successful, MAL_FAIL else.
  */
uint16_t MEM_If_Write_FS(uint8_t *src, uint8_t *dest, uint32_t Len) {
  /* USER CODE BEGIN 3 */
  uint32_t i = 0;

  for(i = 0; i < Len; i += 4) {
    /* Device voltage range supposed to be [2.7V to 3.6V], the operation will be done by byte */
    if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (uint32_t)(dest + i), *(uint32_t *)(src + i)) == HAL_OK) {
      /* Check the written value */
      if(*(uint32_t *)(src + i) != *(uint32_t *)(dest + i)) {
        /* Flash content doesn't match SRAM content */
        return 2;
      }
    } else {
      /* Error occurred while writing data in Flash memory */
      return 1;
    }
  }
  return 0;
  /* USER CODE END 3 */
}

/**
  * @brief  MEM_If_Read_FS
  *         Memory read routine.
  * @param  src: Pointer to the source buffer. Address to be written to.
  * @param  dest: Pointer to the destination buffer.
  * @param  Len: Number of data to be read (in bytes).
  * @retval Pointer to the physical address where data should be read.
  */
uint8_t *MEM_If_Read_FS(uint8_t *src, uint8_t *dest, uint32_t Len) {
  /* Return a valid address to avoid HardFault */
  /* USER CODE BEGIN 4 */
  uint32_t i    = 0;
  uint8_t *psrc = src;

  for(i = 0; i < Len; i++) {
    dest[i] = *psrc++;
  }
  /* Return a valid address to avoid HardFault */
  return (uint8_t *)(dest);
  /* USER CODE END 4 */
}

/**
  * @brief  Flash_If_GetStatus_FS
  *         Get status routine.
  * @param  Add: Address to be read from.
  * @param  Cmd: Number of data to be read (in bytes).
  * @param  buffer: used for returning the time necessary for a program or an erase operation
  * @retval 0 if operation is successful
  */
uint16_t MEM_If_GetStatus_FS(uint32_t Add, uint8_t Cmd, uint8_t *buffer) {
  /* USER CODE BEGIN 5 */
  switch(Cmd) {
    case DFU_MEDIA_PROGRAM:
      buffer[1] = (uint8_t)FLASH_PROGRAM_TIME;
      buffer[2] = (uint8_t)(FLASH_PROGRAM_TIME << 8);
      buffer[3] = 0;
      break;

    case DFU_MEDIA_ERASE:
    default:
      buffer[1] = (uint8_t)FLASH_ERASE_TIME;
      buffer[2] = (uint8_t)(FLASH_ERASE_TIME << 8);
      buffer[3] = 0;
      break;
  }
  return (USBD_OK);
  /* USER CODE END 5 */
}

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */
/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */

/**
  * @}
  */

/**
  * @}
  */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
