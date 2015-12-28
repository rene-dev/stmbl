//--------------------------------------------------------------
// File     : stm32_ub_usb_cdc.h
//--------------------------------------------------------------

//--------------------------------------------------------------
#ifndef __STM32F4_UB_USB_CDC_H
#define __STM32F4_UB_USB_CDC_H


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32f4xx.h"
#include "usbd_cdc_vcp.h"


//--------------------------------------------------------------
// Status der USB-Verbindung
//--------------------------------------------------------------
typedef enum {
  USB_CDC_NO_INIT =0, // USB-Schnittstelle noch nicht initialisiert
  USB_CDC_DETACHED,   // USB-Verbindung getrennt
  USB_CDC_CONNECTED   // USB-Verbindung hergestellt
}USB_CDC_STATUS_t;
USB_CDC_STATUS_t USB_CDC_STATUS;

//--------------------------------------------------------------
// Endekennung beim Senden
//--------------------------------------------------------------
typedef enum {
  NONE = 0,  // keine Endekennung
  LFCR,      // LineFeed + CarriageReturn (0x0A,0x0D)
  CRLF,      // CarriageReturn + LineFeed (0x0D,0x0A)
  LF,        // nur LineFeed (0x0A)
  CR         // nur CarriageReturn (0x0D)
}USB_CDC_LASTBYTE_t;

//--------------------------------------------------------------
// Status beim Empfangen
//--------------------------------------------------------------
typedef enum {
  RX_USB_ERR =0, // keine USB Verbindung
  RX_EMPTY,      // nichts empfangen
  RX_READY       // es steht was im Empfangspuffer
}USB_CDC_RXSTATUS_t;


//--------------------------------------------------------------
// Globale Funktionen
//--------------------------------------------------------------
void UB_USB_CDC_Init(void);
USB_CDC_STATUS_t UB_USB_CDC_GetStatus(void);
ErrorStatus UB_USB_CDC_SendString(char *ptr, USB_CDC_LASTBYTE_t end_cmd);
USB_CDC_RXSTATUS_t UB_USB_CDC_ReceiveString(char *ptr);

//--------------------------------------------------------------
#endif // __STM32F4_UB_USB_CDC_H
