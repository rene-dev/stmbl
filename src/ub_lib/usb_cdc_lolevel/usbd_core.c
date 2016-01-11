//--------------------------------------------------------------
// File     : usbd_core.c
//--------------------------------------------------------------


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "usbd_core.h"
#include "usbd_req.h"
#include "usbd_ioreq.h"
#include "usb_dcd_int.h"
#include "usb_bsp.h"







static uint8_t USBD_SetupStage(USB_OTG_CORE_HANDLE *pdev);
static uint8_t USBD_DataOutStage(USB_OTG_CORE_HANDLE *pdev , uint8_t epnum);
static uint8_t USBD_DataInStage(USB_OTG_CORE_HANDLE *pdev , uint8_t epnum);
static uint8_t USBD_SOF(USB_OTG_CORE_HANDLE  *pdev);
static uint8_t USBD_Reset(USB_OTG_CORE_HANDLE  *pdev);
static uint8_t USBD_Suspend(USB_OTG_CORE_HANDLE  *pdev);
static uint8_t USBD_Resume(USB_OTG_CORE_HANDLE  *pdev);

static uint8_t USBD_DevConnected(USB_OTG_CORE_HANDLE  *pdev);
static uint8_t USBD_DevDisconnected(USB_OTG_CORE_HANDLE  *pdev);

static uint8_t USBD_IsoINIncomplete(USB_OTG_CORE_HANDLE  *pdev);
static uint8_t USBD_IsoOUTIncomplete(USB_OTG_CORE_HANDLE  *pdev);




USBD_DCD_INT_cb_TypeDef USBD_DCD_INT_cb = 
{
  USBD_DataOutStage,
  USBD_DataInStage,
  USBD_SetupStage,
  USBD_SOF,
  USBD_Reset,
  USBD_Suspend,
  USBD_Resume,
  USBD_IsoINIncomplete,
  USBD_IsoOUTIncomplete,
  USBD_DevConnected,
  USBD_DevDisconnected,
};

USBD_DCD_INT_cb_TypeDef  *USBD_DCD_INT_fops = &USBD_DCD_INT_cb;


//--------------------------------------------------------------
void USBD_Init(USB_OTG_CORE_HANDLE *pdev,
               USB_OTG_CORE_ID_TypeDef coreID,
               USBD_DEVICE *pDevice,                  
               USBD_Class_cb_TypeDef *class_cb, 
               USBD_Usr_cb_TypeDef *usr_cb)
{
  /* Hardware Init */
  USB_OTG_BSP_Init(pdev);  
  
  USBD_DeInit(pdev);
  
  /*Register class and user callbacks */
  pdev->dev.class_cb = class_cb;
  pdev->dev.usr_cb = usr_cb;  
  pdev->dev.usr_device = pDevice;    
  
  /* set USB OTG core params */
  DCD_Init(pdev , coreID);
  
  /* Upon Init call usr callback */
  pdev->dev.usr_cb->Init();
  
  /* Enable Interrupts */
  USB_OTG_BSP_EnableInterrupt(pdev);
}

//--------------------------------------------------------------
USBD_Status USBD_DeInit(USB_OTG_CORE_HANDLE *pdev)
{
  /* Software Init */
  
  return USBD_OK;
}

//--------------------------------------------------------------
static uint8_t USBD_SetupStage(USB_OTG_CORE_HANDLE *pdev)
{
  USB_SETUP_REQ req;
  
  USBD_ParseSetupRequest(pdev , &req);
  
  switch (req.bmRequest & 0x1F) 
  {
  case USB_REQ_RECIPIENT_DEVICE:   
    USBD_StdDevReq (pdev, &req);
    break;
    
  case USB_REQ_RECIPIENT_INTERFACE:     
    USBD_StdItfReq(pdev, &req);
    break;
    
  case USB_REQ_RECIPIENT_ENDPOINT:        
    USBD_StdEPReq(pdev, &req);   
    break;
    
  default:           
    DCD_EP_Stall(pdev , req.bmRequest & 0x80);
    break;
  }  
  return USBD_OK;
}

//--------------------------------------------------------------
static uint8_t USBD_DataOutStage(USB_OTG_CORE_HANDLE *pdev , uint8_t epnum)
{
  USB_OTG_EP *ep;
  
  if(epnum == 0) 
  {
    ep = &pdev->dev.out_ep[0];
    if ( pdev->dev.device_state == USB_OTG_EP0_DATA_OUT)
    {
      if(ep->rem_data_len > ep->maxpacket)
      {
        ep->rem_data_len -=  ep->maxpacket;
        
        if(pdev->cfg.dma_enable == 1)
        {
          /* in slave mode this, is handled by the RxSTSQLvl ISR */
          ep->xfer_buff += ep->maxpacket; 
        }        
        USBD_CtlContinueRx (pdev, 
                            ep->xfer_buff,
                            MIN(ep->rem_data_len ,ep->maxpacket));
      }
      else
      {
        if((pdev->dev.class_cb->EP0_RxReady != NULL)&&
           (pdev->dev.device_status == USB_OTG_CONFIGURED))
        {
          pdev->dev.class_cb->EP0_RxReady(pdev); 
        }
        USBD_CtlSendStatus(pdev);
      }
    }
  }
  else if((pdev->dev.class_cb->DataOut != NULL)&&
          (pdev->dev.device_status == USB_OTG_CONFIGURED))
  {
    pdev->dev.class_cb->DataOut(pdev, epnum); 
  }  
  return USBD_OK;
}

//--------------------------------------------------------------
static uint8_t USBD_DataInStage(USB_OTG_CORE_HANDLE *pdev , uint8_t epnum)
{
  USB_OTG_EP *ep;
  
  if(epnum == 0) 
  {
    ep = &pdev->dev.in_ep[0];
    if ( pdev->dev.device_state == USB_OTG_EP0_DATA_IN)
    {
      if(ep->rem_data_len > ep->maxpacket)
      {
        ep->rem_data_len -=  ep->maxpacket;
        if(pdev->cfg.dma_enable == 1)
        {
          /* in slave mode this, is handled by the TxFifoEmpty ISR */
          ep->xfer_buff += ep->maxpacket;
        }
        USBD_CtlContinueSendData (pdev, 
                                  ep->xfer_buff, 
                                  ep->rem_data_len);
      }
      else
      { /* last packet is MPS multiple, so send ZLP packet */
        if((ep->total_data_len % ep->maxpacket == 0) &&
           (ep->total_data_len >= ep->maxpacket) &&
             (ep->total_data_len < ep->ctl_data_len ))
        {
          
          USBD_CtlContinueSendData(pdev , NULL, 0);
          ep->ctl_data_len = 0;
        }
        else
        {
          if((pdev->dev.class_cb->EP0_TxSent != NULL)&&
             (pdev->dev.device_status == USB_OTG_CONFIGURED))
          {
            pdev->dev.class_cb->EP0_TxSent(pdev); 
          }          
          USBD_CtlReceiveStatus(pdev);
        }
      }
    }
  }
  else if((pdev->dev.class_cb->DataIn != NULL)&& 
          (pdev->dev.device_status == USB_OTG_CONFIGURED))
  {
    pdev->dev.class_cb->DataIn(pdev, epnum); 
  }  
  return USBD_OK;
}


//--------------------------------------------------------------
static uint8_t USBD_Reset(USB_OTG_CORE_HANDLE  *pdev)
{
  /* Open EP0 OUT */
  DCD_EP_Open(pdev,
              0x00,
              USB_OTG_MAX_EP0_SIZE,
              EP_TYPE_CTRL);
  
  /* Open EP0 IN */
  DCD_EP_Open(pdev,
              0x80,
              USB_OTG_MAX_EP0_SIZE,
              EP_TYPE_CTRL);
  
  /* Upon Reset call usr call back */
  pdev->dev.device_status = USB_OTG_DEFAULT;
  pdev->dev.usr_cb->DeviceReset(pdev->cfg.speed);
  
  return USBD_OK;
}


//--------------------------------------------------------------
static uint8_t USBD_Resume(USB_OTG_CORE_HANDLE  *pdev)
{
  /* Upon Resume call usr call back */
  pdev->dev.usr_cb->DeviceResumed(); 
  pdev->dev.device_status = USB_OTG_CONFIGURED;  
  return USBD_OK;
}



//--------------------------------------------------------------
static uint8_t USBD_Suspend(USB_OTG_CORE_HANDLE  *pdev)
{
  
  pdev->dev.device_status  = USB_OTG_SUSPENDED;
  /* Upon Resume call usr call back */
  pdev->dev.usr_cb->DeviceSuspended(); 
  return USBD_OK;
}



//--------------------------------------------------------------
static uint8_t USBD_SOF(USB_OTG_CORE_HANDLE  *pdev)
{
  if(pdev->dev.class_cb->SOF)
  {
    pdev->dev.class_cb->SOF(pdev); 
  }
  return USBD_OK;
}

//--------------------------------------------------------------
USBD_Status USBD_SetCfg(USB_OTG_CORE_HANDLE  *pdev, uint8_t cfgidx)
{
  pdev->dev.class_cb->Init(pdev, cfgidx); 
  
  /* Upon set config call usr call back */
  pdev->dev.usr_cb->DeviceConfigured();
  return USBD_OK; 
}

//--------------------------------------------------------------
USBD_Status USBD_ClrCfg(USB_OTG_CORE_HANDLE  *pdev, uint8_t cfgidx)
{
  pdev->dev.class_cb->DeInit(pdev, cfgidx);   
  return USBD_OK;
}

//--------------------------------------------------------------
static uint8_t USBD_IsoINIncomplete(USB_OTG_CORE_HANDLE  *pdev)
{
  pdev->dev.class_cb->IsoINIncomplete(pdev);   
  return USBD_OK;
}

//--------------------------------------------------------------
static uint8_t USBD_IsoOUTIncomplete(USB_OTG_CORE_HANDLE  *pdev)
{
  pdev->dev.class_cb->IsoOUTIncomplete(pdev);   
  return USBD_OK;
}


//--------------------------------------------------------------
static uint8_t USBD_DevConnected(USB_OTG_CORE_HANDLE  *pdev)
{
  pdev->dev.usr_cb->DeviceConnected();
  return USBD_OK;
}

//--------------------------------------------------------------
static uint8_t USBD_DevDisconnected(USB_OTG_CORE_HANDLE  *pdev)
{
  pdev->dev.usr_cb->DeviceDisconnected();
  pdev->dev.class_cb->DeInit(pdev, 0);
  return USBD_OK;
}




