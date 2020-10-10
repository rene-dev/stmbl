#include "usbd_cdc_if.h"
#include "ringbuf.h"
#define RX_QUEUE_SIZE 1024
#define TX_QUEUE_SIZE 4096

struct ringbuf rx_buf = RINGBUF(RX_QUEUE_SIZE);
struct ringbuf tx_buf = RINGBUF(TX_QUEUE_SIZE);

void cdc_usbtx() {}

int cdc_tx(void *data, uint32_t len) {
  return rb_write(&tx_buf, data, len);
}

void cdc_poll() {}

int cdc_is_connected() {
  return 1;
}

int cdc_getline(char *ptr, int len) {
  return rb_getline(&rx_buf, ptr, len);
}

int _write(int file, char *ptr, int len) {
  return cdc_tx(ptr, len);
}
