//--------------------------------------------------------------
// File     : usbd_req.h
//--------------------------------------------------------------



#ifndef __USB_REQUEST_H_
#define __USB_REQUEST_H_

//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include  "usbd_def.h"
#include  "usbd_core.h"
#include  "usbd_conf.h"






USBD_Status  USBD_StdDevReq (USB_OTG_CORE_HANDLE  *pdev, USB_SETUP_REQ  *req);
USBD_Status  USBD_StdItfReq (USB_OTG_CORE_HANDLE  *pdev, USB_SETUP_REQ  *req);
USBD_Status  USBD_StdEPReq (USB_OTG_CORE_HANDLE  *pdev, USB_SETUP_REQ  *req);
void USBD_ParseSetupRequest( USB_OTG_CORE_HANDLE  *pdev,
                                    USB_SETUP_REQ *req);

void USBD_CtlError( USB_OTG_CORE_HANDLE  *pdev,
                            USB_SETUP_REQ *req);

void USBD_GetString(uint8_t *desc, uint8_t *unicode, uint16_t *len);

#endif
