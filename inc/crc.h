#include <stdint.h>

void calc_crc8_table(uint8_t* crc_table, uint8_t poly);
uint8_t calc_crc8(uint8_t* data, uint32_t len, uint8_t* crc_table);