//--------------------------------------------------------------
// File     : usb_conf.h
//--------------------------------------------------------------


#ifndef __USB_CONF__H__
#define __USB_CONF__H__

//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32f4xx.h"







/****************** USB OTG FS CONFIGURATION **********************************/
 #define RX_FIFO_FS_SIZE                          128
 #define TX0_FIFO_FS_SIZE                          32
 #define TX1_FIFO_FS_SIZE                         128
 #define TX2_FIFO_FS_SIZE                          32 
 #define TX3_FIFO_FS_SIZE                           0

/****************** USB OTG MODE CONFIGURATION ********************************/







  #define __ALIGN_BEGIN
  #define __ALIGN_END

  #define __packed    __attribute__ ((__packed__))








#endif




