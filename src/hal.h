#include "param.h"
#include "scanf.h"

#pragma once

#define MAX_HAL_PINS 64
#define MAX_HPNAME 16

typedef char HPNAME[MAX_HPNAME];

struct hal_pin{
  HPNAME name;
  float value;
  struct hal_pin* source;
};

struct hal_struct{
  struct hal_pin* hal_pins[MAX_HAL_PINS];
  int hal_pin_count;

  int fast_rt_lock;
  int rt_lock;
  int nrt_lock;
} hal;

void init_hal();

void init_hal_pin(HPNAME name, struct hal_pin* pin, float value);

int register_hal_pin(struct hal_pin* pin);

int set_hal_pin(HPNAME name, float value);

int is_hal_pin(HPNAME name);

float get_hal_pin(HPNAME name);

void write_hal_pin(struct hal_pin* pin, float value);

float read_hal_pin(struct hal_pin* pin);

struct hal_pin* find_hal_pin(HPNAME name);

int link_hal_pins(HPNAME source, HPNAME sink);

float read_float(char* buf);
