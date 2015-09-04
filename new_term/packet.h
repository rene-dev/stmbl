#pragma once

struct packet{
   unsigned char start; // 255
   unsigned char type; // 1 = text, 2 = waves
   unsigned char size; // min(sizeof(data), 255)
   unsigned char key;
   union{
      char* txt;
      unsigned char* buf;
      float* value;
   };
};

void buff_packet(struct packet* p){
   int nonkey_count = 0;
   for(int i = p->size - 1; i >= 0; i--){
      if(p->buf[i] == p->start){
         p->buf[i] = nonkey_count + 1;
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
      j += temp;
   }
}
