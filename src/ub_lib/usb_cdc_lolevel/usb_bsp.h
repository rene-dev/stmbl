//--------------------------------------------------------------
// File     : usb_bsp.h
//--------------------------------------------------------------


#ifndef __USB_BSP__H__
#define __USB_BSP__H__

//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "usb_core.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "misc.h"




void BSP_Init(void);

void USB_OTG_BSP_Init (USB_OTG_CORE_HANDLE *pdev);
void USB_OTG_BSP_uDelay (const uint32_t usec);
void USB_OTG_BSP_mDelay (const uint32_t msec);
void USB_OTG_BSP_EnableInterrupt (USB_OTG_CORE_HANDLE *pdev);

#endif



