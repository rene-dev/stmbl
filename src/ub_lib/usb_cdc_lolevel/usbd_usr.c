//--------------------------------------------------------------
// File     : usbd_usr.c
//--------------------------------------------------------------

//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "usbd_usr.h"
#include "usbd_ioreq.h"



USBD_Usr_cb_TypeDef USR_cb =
{
  USBD_USR_Init,
  USBD_USR_DeviceReset,
  USBD_USR_DeviceConfigured,
  USBD_USR_DeviceSuspended,
  USBD_USR_DeviceResumed,
  USBD_USR_DeviceConnected,
  USBD_USR_DeviceDisconnected,
};





//--------------------------------------------------------------
// wird einmal aufgerufen beim Init der USB-Schnittstelle
//--------------------------------------------------------------
void USBD_USR_Init(void)
{  
  USB_CDC_STATUS=USB_CDC_DETACHED;
}


//--------------------------------------------------------------
// wird vom USB-Handler aufgerufen
// nach dem stecken oder ziehen vom Kabel
// (VBUS_SENSING)
//--------------------------------------------------------------
void USBD_USR_DeviceReset(uint8_t speed )
{
  USB_CDC_STATUS=USB_CDC_DETACHED;
}


//--------------------------------------------------------------
// wird aufgerufen, wenn nach dem herstellen der Verbindung
// die USB-Schnittstelle Configuriert wurde
// USB-Port ist damit bereit
//--------------------------------------------------------------
void USBD_USR_DeviceConfigured (void)
{
  USB_CDC_STATUS=USB_CDC_CONNECTED;
}


//--------------------------------------------------------------
// wird beim einstecken vom Kabel aufgerufen
// (VBUS_SENSING)
//--------------------------------------------------------------
void USBD_USR_DeviceConnected (void)
{
  USB_CDC_STATUS=USB_CDC_DETACHED;
}


//--------------------------------------------------------------
// wird beim ziehen vom Kabel aufgerufen
// (VBUS_SENSING)
//--------------------------------------------------------------
void USBD_USR_DeviceDisconnected (void)
{
  USB_CDC_STATUS=USB_CDC_DETACHED;
}

//--------------------------------------------------------------
// wird aufgerufen, wenn USB-Verbindung getrennt oder
// hergestellt wurde
//--------------------------------------------------------------
void USBD_USR_DeviceSuspended(void)
{
  USB_CDC_STATUS=USB_CDC_DETACHED;
}


//--------------------------------------------------------------
// ??
//--------------------------------------------------------------
void USBD_USR_DeviceResumed(void)
{
  USB_CDC_STATUS=USB_CDC_DETACHED;
}




