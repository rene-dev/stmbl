//--------------------------------------------------------------
// File     : usb_dcd_int.h
//--------------------------------------------------------------


#ifndef USB_DCD_INT_H__
#define USB_DCD_INT_H__

//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "usb_dcd.h"





typedef struct _USBD_DCD_INT
{
  uint8_t (* DataOutStage) (USB_OTG_CORE_HANDLE *pdev , uint8_t epnum);
  uint8_t (* DataInStage)  (USB_OTG_CORE_HANDLE *pdev , uint8_t epnum);
  uint8_t (* SetupStage) (USB_OTG_CORE_HANDLE *pdev);
  uint8_t (* SOF) (USB_OTG_CORE_HANDLE *pdev);
  uint8_t (* Reset) (USB_OTG_CORE_HANDLE *pdev);
  uint8_t (* Suspend) (USB_OTG_CORE_HANDLE *pdev);
  uint8_t (* Resume) (USB_OTG_CORE_HANDLE *pdev);
  uint8_t (* IsoINIncomplete) (USB_OTG_CORE_HANDLE *pdev);
  uint8_t (* IsoOUTIncomplete) (USB_OTG_CORE_HANDLE *pdev);  
  
  uint8_t (* DevConnected) (USB_OTG_CORE_HANDLE *pdev);
  uint8_t (* DevDisconnected) (USB_OTG_CORE_HANDLE *pdev);   
  
}USBD_DCD_INT_cb_TypeDef;

extern USBD_DCD_INT_cb_TypeDef *USBD_DCD_INT_fops;


#define CLEAR_IN_EP_INTR(epnum,intr) \
  diepint.d32=0; \
  diepint.b.intr = 1; \
  USB_OTG_WRITE_REG32(&pdev->regs.INEP_REGS[epnum]->DIEPINT,diepint.d32);

#define CLEAR_OUT_EP_INTR(epnum,intr) \
  doepint.d32=0; \
  doepint.b.intr = 1; \
  USB_OTG_WRITE_REG32(&pdev->regs.OUTEP_REGS[epnum]->DOEPINT,doepint.d32);



uint32_t USBD_OTG_ISR_Handler (USB_OTG_CORE_HANDLE *pdev);




#endif



