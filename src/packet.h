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

void buff_packet(struct packet* p);

void unbuff_packet(struct packet* p);
