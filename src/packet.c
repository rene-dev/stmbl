#include "packet.h"

void buff_packet(struct packet* p){
   int nonkey_count = 0;
   for(int i = p->size - 1; i >= 0; i--){
      if(p->buf[i] == p->start){
         p->buf[i] = nonkey_count;
         nonkey_count = 0;
      }
      else{
         nonkey_count++;
      }
   }
   p->key = nonkey_count;
}

void unbuff_packet(struct packet* p){
   int temp;
   for(int j = p->key; j < p->size;){
      temp = p->buf[j];
      p->buf[j] = p->start;
      j += temp + 1;
   }
}
