//--------------------------------------------------------------
// File     : usbd_cdc_core.h
//--------------------------------------------------------------



#ifndef __USB_CDC_CORE_H_
#define __USB_CDC_CORE_H_


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include  "usbd_ioreq.h"


#define USB_CDC_CONFIG_DESC_SIZ                (67)
#define USB_CDC_DESC_SIZ                       (67-9)

#define CDC_DESCRIPTOR_TYPE                     0x21

#define DEVICE_CLASS_CDC                        0x02
#define DEVICE_SUBCLASS_CDC                     0x00


#define USB_DEVICE_DESCRIPTOR_TYPE              0x01
#define USB_CONFIGURATION_DESCRIPTOR_TYPE       0x02
#define USB_STRING_DESCRIPTOR_TYPE              0x03
#define USB_INTERFACE_DESCRIPTOR_TYPE           0x04
#define USB_ENDPOINT_DESCRIPTOR_TYPE            0x05

#define STANDARD_ENDPOINT_DESC_SIZE             0x09

#define CDC_DATA_IN_PACKET_SIZE                 *(uint16_t *)(((USB_OTG_CORE_HANDLE *)pdev)->dev.pConfig_descriptor + 57)
        
#define CDC_DATA_OUT_PACKET_SIZE                *(uint16_t *)(((USB_OTG_CORE_HANDLE *)pdev)->dev.pConfig_descriptor + 64)

/*---------------------------------------------------------------------*/
/*  CDC definitions                                                    */
/*---------------------------------------------------------------------*/

/**************************************************/
/* CDC Requests                                   */
/**************************************************/
#define SEND_ENCAPSULATED_COMMAND               0x00
#define GET_ENCAPSULATED_RESPONSE               0x01
#define SET_COMM_FEATURE                        0x02
#define GET_COMM_FEATURE                        0x03
#define CLEAR_COMM_FEATURE                      0x04
#define SET_LINE_CODING                         0x20
#define GET_LINE_CODING                         0x21
#define SET_CONTROL_LINE_STATE                  0x22
#define SEND_BREAK                              0x23
#define NO_CMD                                  0xFF


typedef struct _CDC_IF_PROP
{
  uint16_t (*pIf_Init)     (void);   
  uint16_t (*pIf_DeInit)   (void);   
  uint16_t (*pIf_Ctrl)     (uint32_t Cmd, uint8_t* Buf, uint32_t Len);
  uint16_t (*pIf_DataTx)   (uint8_t* Buf, uint32_t Len);
  uint16_t (*pIf_DataRx)   (uint8_t* Buf, uint32_t Len);
}
CDC_IF_Prop_TypeDef;


extern USBD_Class_cb_TypeDef  USBD_CDC_cb;


#endif

