#pragma once

#define COMMAND(c,p)

typedef struct{
  char name[32];
  void (*ptr)(char *);
}cmd_t;

