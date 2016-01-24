#pragma once

#include <stdint.h>
#include "ringbuf.h"
#include "usbd_cdc_core.h"

#define APP_TX_BUF_SIZE 128
#define APP_RX_BUF_SIZE 128

/* These are external variables imported from CDC core to be used for IN
   transfer management. */
extern uint8_t  APP_Rx_Buffer[];  /* Write CDC received data in this buffer.
                                     These data will be sent over USB IN endpoint
                                     in the CDC core functions. */
extern uint32_t APP_Rx_ptr_in;    /* Increment this pointer or roll it back to
                                     start address when writing received data
                                     in the buffer APP_Rx_Buffer. */

extern struct ringbuf usb_rx_buf;
extern struct ringbuf usb_tx_buf;

uint8_t  USB_CDC_is_connected(void);
uint16_t USB_VCP_get_string(char *ptr);
void USB_VCP_send_string(unsigned char *ptr);
void usb_init(void);
