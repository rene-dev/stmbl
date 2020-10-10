#include "common_f1.h"

void buff_packet(packet_header_t *p, uint8_t size) {
  uint8_t nonkey_count = 0;
  uint8_t *buf         = ((uint8_t *)p) + sizeof(packet_header_t);
  for(int i = size - 1; i >= 0; i--) {
    if(buf[i] == p->start) {
      buf[i]       = nonkey_count;
      nonkey_count = 0;
    } else {
      nonkey_count++;
    }
  }
  p->key = nonkey_count;
}

void unbuff_packet(packet_header_t *p, uint8_t size) {
  uint8_t temp;
  uint8_t *buf = ((uint8_t *)p) + sizeof(packet_header_t);
  for(int j = p->key; j < size;) {
    temp   = buf[j];
    buf[j] = p->start;
    j += temp + 1;
  }
}