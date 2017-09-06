#pragma once
#include <stdint.h>

#define COMMAND(c, p, d)

uint32_t call_cmd(char *s);

typedef struct {
  char name[32];
  char *doc;
  void (*ptr)(char *);
} cmd_t;
