#pragma once

#include <stdint.h>
#include "ringbuf.h"
#include "usbd_cdc_core.h"

#define APP_TX_BUF_SIZE 128
#define APP_RX_BUF_SIZE 128

/* These are external variables imported from CDC core to be used for IN
   transfer management. */
extern uint8_t APP_Rx_Buffer[]; /* Write CDC received data in this buffer.
                                     These data will be sent over USB IN endpoint
                                     in the CDC core functions. */
extern uint32_t APP_Rx_ptr_in;  /* Increment this pointer or roll it back to
                                     start address when writing received data
                                     in the buffer APP_Rx_Buffer. */

extern struct ringbuf usb_rx_buf;
extern struct ringbuf usb_tx_buf;

uint8_t USB_CDC_is_connected(void);
void usb_init(void);

void cdc_init(void);
int cdc_tx(void *data, uint32_t len);
int cdc_getline(char *ptr, int len);
int cdc_is_connected();
void cdc_poll();
