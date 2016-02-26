#pragma once

#include <libopencm3/usb/usbd.h>
#include <libopencm3/usb/cdc.h>
#include "ringbuf.h"

#define RX_QUEUE_SIZE  512
#define TX_QUEUE_SIZE  512

void cdc_init(void);
int cdcacm_tx(void* data, uint32_t len);
int cdcacm_getstring(char *ptr);
int cdcacm_is_connected();
void cdcacm_flush();

usbd_device *usbd_dev;
int usb_connected;

extern struct ringbuf rx_buf;
extern struct ringbuf tx_buf;