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
#define MAX_COMP_FUNCS 16


typedef char HPNAME[MAX_HPNAME];

struct hal_pin{
  HPNAME name;
  volatile float value;
  void (*read_callback)();
  struct hal_pin* source;
};

struct hal_struct{
  HPNAME comp_types[MAX_COMP_TYPES];
  int comp_types_counter[MAX_COMP_TYPES];
  int comp_type_count;
  HPNAME comp_type;
  HPNAME tmp;

  void (*init[MAX_COMP_FUNCS])();
  int init_func_count;

  void (*fast_rt[MAX_COMP_FUNCS])(float period);
  int fast_rt_func_count;

  void (*rt_in[MAX_COMP_FUNCS])(float period);
  int rt_in_func_count;

  void (*rt_filter[MAX_COMP_FUNCS])(float period);
  int rt_filter_func_count;

  void (*rt_pid[MAX_COMP_FUNCS])(float period);
  int rt_pid_func_count;

  void (*rt_calc[MAX_COMP_FUNCS])(float period);
  int rt_calc_func_count;

  void (*rt_out[MAX_COMP_FUNCS])(float period);
  int rt_out_func_count;

  void (*nrt[MAX_COMP_FUNCS])(float period);
  int nrt_func_count;

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

void call(void (*func)());

int addf_init(void (*init)());
int addf_fast_rt(void (*fast_rt)(float period));
int addf_rt_in(void (*rt_in)(float period));
int addf_rt_filter(void (*rt_filter)(float period));
int addf_rt_pid(void (*rt_pid)(float period));
int addf_rt_calc(void (*rt_calc)(float period));
int addf_rt_out(void (*rt_out)(float period));
int addf_nrt(void (*nrt)(float period));


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
  void (*nrt)(float period) = 0;

#define HAL_PIN(name)               \
  static struct hal_pin name;       \
  init_hal_pin(#name, &name, 0.0);  \
  (name.source->value)

#define MEM(var) static var

#define PIN(name)                       \
  *({                                   \
    if(&name != name.source){           \
      call(name.source->read_callback); \
    };                                   \
    &(name.source->value);              \
  })

#define INIT(func)                    \
 init = ({ void function(){func} function;});

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

#define RC(pin, func)                    \
 pin.read_callback = ({ void function(){func} function;});

#define LINK_RC(src_pin, dst_pin)                    \
 src_pin.read_callback = dst_pin.read_callback;

#define ENDCOMP \
  addf_init(init); \
  addf_fast_rt(fast_rt); \
  addf_rt_in(rt_in); \
  addf_rt_filter(rt_filter); \
  addf_rt_pid(rt_pid); \
  addf_rt_calc(rt_calc); \
  addf_rt_out(rt_out); \
  addf_nrt(nrt); \
}
