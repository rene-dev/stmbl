//--------------------------------------------------------------
// File     : usbd_cdc_vcp.h
//--------------------------------------------------------------


#ifndef __USBD_CDC_VCP_H
#define __USBD_CDC_VCP_H

//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32f4xx.h"
#include "usbd_cdc_core.h"
#include "usbd_conf.h"
#include "usbd_usr.h"
#include "usbd_desc.h"

/* Exported typef ------------------------------------------------------------*/
/* The following structures groups all needed parameters to be configured for the 
   ComPort. These parameters can modified on the fly by the host through CDC class
   command class requests. */
typedef struct
{
  uint32_t bitrate;
  uint8_t  format;
  uint8_t  paritytype;
  uint8_t  datatype;
}LINE_CODING;



#define APP_TX_BUF_SIZE         128  // Grösse vom RX-Puffer in Bytes (32,64,128,256 usw)
#define APP_TX_BUF_MASK       (APP_TX_BUF_SIZE-1)
#define  USB_CDC_RX_END_CHR    0x0D  // Endekennung (Ascii-Wert)
#define  USB_CDC_FIRST_ASCII   32    // erstes Ascii-Zeichen
#define  USB_CDC_LAST_ASCII    255   // letztes Ascii-Zeichen



/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void UB_VCP_DataTx (uint8_t wert);
uint16_t UB_VCP_StringRx(char *ptr);

#endif


