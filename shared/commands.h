#pragma once
#include <stdint.h>

#define COMMAND(c,p)

uint32_t call_cmd(char * s);

typedef struct{
  char name[32];
  void (*ptr)(char *);
}cmd_t;

