//--------------------------------------------------------------
// File     : usbd_desc.c
//--------------------------------------------------------------

//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_req.h"
#include "usbd_conf.h"
#include "usb_regs.h"


#define USBD_VID                        0x0483
#define USBD_PID                        0x5740


#define USBD_LANGID_STRING              0x409
#define USBD_MANUFACTURER_STRING        "STMicroelectronics"

#define USBD_PRODUCT_FS_STRING          "STM32 Virtual ComPort"
#define USBD_SERIALNUMBER_FS_STRING     "00000000050C"

#define USBD_CONFIGURATION_FS_STRING    "VCP Config"
#define USBD_INTERFACE_FS_STRING        "VCP Interface"


USBD_DEVICE USR_desc =
{
  USBD_USR_DeviceDescriptor,
  USBD_USR_LangIDStrDescriptor, 
  USBD_USR_ManufacturerStrDescriptor,
  USBD_USR_ProductStrDescriptor,
  USBD_USR_SerialStrDescriptor,
  USBD_USR_ConfigStrDescriptor,
  USBD_USR_InterfaceStrDescriptor,
  
};


/* USB Standard Device Descriptor */
__ALIGN_BEGIN uint8_t USBD_DeviceDesc[USB_SIZ_DEVICE_DESC] __ALIGN_END =
  {
    0x12,                       /*bLength */
    USB_DEVICE_DESCRIPTOR_TYPE, /*bDescriptorType*/
    0x00,                       /*bcdUSB */
    0x02,
    0x00,                       /*bDeviceClass*/
    0x00,                       /*bDeviceSubClass*/
    0x00,                       /*bDeviceProtocol*/
    USB_OTG_MAX_EP0_SIZE,      /*bMaxPacketSize*/
    LOBYTE(USBD_VID),           /*idVendor*/
    HIBYTE(USBD_VID),           /*idVendor*/
    LOBYTE(USBD_PID),           /*idVendor*/
    HIBYTE(USBD_PID),           /*idVendor*/
    0x00,                       /*bcdDevice rel. 2.00*/
    0x02,
    USBD_IDX_MFC_STR,           /*Index of manufacturer  string*/
    USBD_IDX_PRODUCT_STR,       /*Index of product string*/
    USBD_IDX_SERIAL_STR,        /*Index of serial number string*/
    USBD_CFG_MAX_NUM            /*bNumConfigurations*/
  } ; /* USB_DeviceDescriptor */


/* USB Standard Device Descriptor */
__ALIGN_BEGIN uint8_t USBD_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC] __ALIGN_END =
{
  USB_LEN_DEV_QUALIFIER_DESC,
  USB_DESC_TYPE_DEVICE_QUALIFIER,
  0x00,
  0x02,
  0x00,
  0x00,
  0x00,
  0x40,
  0x01,
  0x00,
};


/* USB Standard Device Descriptor */
__ALIGN_BEGIN uint8_t USBD_LangIDDesc[USB_SIZ_STRING_LANGID] __ALIGN_END =
{
     USB_SIZ_STRING_LANGID,         
     USB_DESC_TYPE_STRING,       
     LOBYTE(USBD_LANGID_STRING),
     HIBYTE(USBD_LANGID_STRING), 
};

//--------------------------------------------------------------
uint8_t *  USBD_USR_DeviceDescriptor( uint8_t speed , uint16_t *length)
{
  *length = sizeof(USBD_DeviceDesc);
  return USBD_DeviceDesc;
}

//--------------------------------------------------------------
uint8_t *  USBD_USR_LangIDStrDescriptor( uint8_t speed , uint16_t *length)
{
  *length =  sizeof(USBD_LangIDDesc);  
  return USBD_LangIDDesc;
}


//--------------------------------------------------------------
uint8_t *  USBD_USR_ProductStrDescriptor( uint8_t speed , uint16_t *length)
{

    USBD_GetString ((uint8_t*)USBD_PRODUCT_FS_STRING, USBD_StrDesc, length);
  return USBD_StrDesc;
}

//--------------------------------------------------------------
uint8_t *  USBD_USR_ManufacturerStrDescriptor( uint8_t speed , uint16_t *length)
{
  USBD_GetString ((uint8_t*)USBD_MANUFACTURER_STRING, USBD_StrDesc, length);
  return USBD_StrDesc;
}

//--------------------------------------------------------------
uint8_t *  USBD_USR_SerialStrDescriptor( uint8_t speed , uint16_t *length)
{

    USBD_GetString ((uint8_t*)USBD_SERIALNUMBER_FS_STRING, USBD_StrDesc, length);
  return USBD_StrDesc;
}

//--------------------------------------------------------------
uint8_t *  USBD_USR_ConfigStrDescriptor( uint8_t speed , uint16_t *length)
{

    USBD_GetString ((uint8_t*)USBD_CONFIGURATION_FS_STRING, USBD_StrDesc, length);

  return USBD_StrDesc;  
}


//--------------------------------------------------------------
uint8_t *  USBD_USR_InterfaceStrDescriptor( uint8_t speed , uint16_t *length)
{

    USBD_GetString ((uint8_t*)USBD_INTERFACE_FS_STRING, USBD_StrDesc, length);

  return USBD_StrDesc;  
}



