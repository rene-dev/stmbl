#pragma once
#include <stdint.h>
#include "usbd_cdc.h"

extern USBD_CDC_ItfTypeDef USBD_Interface_fops_FS;
uint8_t CDC_Transmit_FS(uint8_t *Buf, uint16_t Len);


//void cdc_init(void);
int cdc_tx(void *data, uint32_t len);
int cdc_getline(char *ptr, int len);
int cdc_is_connected();
void cdc_poll();
