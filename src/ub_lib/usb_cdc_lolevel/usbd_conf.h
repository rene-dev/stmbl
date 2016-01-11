//--------------------------------------------------------------
// File     : usbd_conf.h
//--------------------------------------------------------------



#ifndef __USBD_CONF__H__
#define __USBD_CONF__H__

//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------



#define USBD_CFG_MAX_NUM                1
#define USBD_ITF_MAX_NUM                1
#define USB_MAX_STR_DESC_SIZ            50 


#define CDC_IN_EP                       0x81  /* EP1 for data IN */
#define CDC_OUT_EP                      0x01  /* EP1 for data OUT */
#define CDC_CMD_EP                      0x82  /* EP2 for CDC commands */

/* CDC Endpoints parameters: you can fine tune these values depending on the needed baudrates and performance. */

 #define CDC_DATA_MAX_PACKET_SIZE       64   /* Endpoint IN & OUT Packet size */
 #define CDC_CMD_PACKET_SZE             8    /* Control Endpoint Packet size */

 #define CDC_IN_FRAME_INTERVAL          5    /* Number of frames between IN transfers */
 #define APP_RX_DATA_SIZE               2048 /* Total size of IN buffer:
                                                APP_RX_DATA_SIZE*8/MAX_BAUDARATE*1000 should be > CDC_IN_FRAME_INTERVAL */


#define APP_FOPS                        VCP_fops



#endif

