#include "crc.h"

void calc_crc8_table(uint8_t* crc_table, uint8_t poly){
  for(int i = 0; i < 256; i++){
    uint8_t byte = i;
    for(int bit = 0; bit < 8; bit++){
      if((byte & 0x80) != 0){
        byte <<= 1;
        byte ^= poly;
      }
      else{
        byte <<= 1;
      }
    }
    crc_table[i] = byte;
  }
}

uint8_t calc_crc8(uint8_t* data, uint32_t len, uint8_t* crc_table){
  uint8_t crc = 0;
  for(int i = 0; i < len; i++){
    crc = crc_table[data[i] ^ crc];
  }
  return crc;
}