#include <stdint.h>

void cdc_init(void);
int cdc_tx(void* data, uint32_t len);
int cdc_getline(char *ptr, int len);
int cdc_is_connected();
void cdc_poll();
