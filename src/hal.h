#include "param.h"
#include "scanf.h"

#pragma once
/*
#define HAL_FLOAT(name) hal_pin name = {#name, 0.0, 0};
#define PIN(name) name.source->value;

struct hal_comp{
  void
};
*/
#define MAX_HAL_PINS 128
#define MAX_HPNAME 32
#define MAX_COMP_TYPES 16


typedef char HPNAME[MAX_HPNAME];

struct hal_pin{
  HPNAME name;
  float value;
  struct hal_pin* source;
};

struct hal_struct{
  HPNAME comp_types[MAX_COMP_TYPES];
  int comp_types_counter[MAX_COMP_TYPES];
  int comp_type_count;
  HPNAME comp_type;
  HPNAME tmp;

  struct hal_pin* hal_pins[MAX_HAL_PINS];
  int hal_pin_count;

  int fast_rt_lock;
  int rt_lock;
  int nrt_lock;
} hal;

void init_hal();

void init_hal_pin(HPNAME name, struct hal_pin* pin, float value);

int register_hal_pin(struct hal_pin* pin);

int set_comp_type(HPNAME name);

int set_hal_pin(HPNAME name, float value);

int is_hal_pin(HPNAME name);

float get_hal_pin(HPNAME name);

void write_hal_pin(struct hal_pin* pin, float value);

float read_hal_pin(struct hal_pin* pin);

struct hal_pin* find_hal_pin(HPNAME name);

int link_hal_pins(HPNAME source, HPNAME sink);

float read_float(char* buf);

#define COMP(type)                  \
{                                   \
  set_comp_type(#type);             \
  void (*init)() = 0;               \
  void (*fast_rt)(float period) = 0;\
  void (*rt_in)(float period) = 0;     \
  void (*rt_filter)(float period) = 0;     \
  void (*rt_pid)(float period) = 0;     \
  void (*rt_calc)(float period) = 0;     \
  void (*rt_out)(float period) = 0;     \
  void (*nrt)(float period) = 0;    \
  void (*read_callback)() = 0;

#define HAL_PIN(name)               \
  static struct hal_pin name;       \
  init_hal_pin(#name, &name, 0.0);  \
  (name.source->value)

#define MEM(var) static var

#define PIN(name)                   \
  (name.source->value)

#define INIT(func)                    \
 init = ({ void function(float period){func} function;});

#define FRT(func)                    \
 frt = ({ void function(float period){func} function;});

#define RT_IN(func)                    \
 rt_in = ({ void function(float period){func} function;});

#define RT_FILTER(func)                    \
 rt_filter = ({ void function(float period){func} function;});

#define RT_PID(func)                    \
 rt_pid = ({ void function(float period){func} function;});

#define RT_CALC(func)                    \
 rt_calc = ({ void function(float period){func} function;});

#define RT_OUT(func)                    \
 rt_out = ({ void function(float period){func} function;});

#define NRT(func)                    \
 nrt = ({ void function(float period){func} function;});

#define READ(func)                    \
 read_callback = ({ void function(float period){func} function;});

#define ENDCOMP \
}
/*
init_addf(init); \
frt_addf(frt); \
rt_in_addf(rt_in); \
rt_filter_addf(rt_filter); \
rt_pid_addf(rt_pid); \
rt_calc_addf(rt_calc); \
rt_out_addf(rt_out); \
nrt_addf(nrt); \
}*/
