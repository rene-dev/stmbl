/*
 * This file is part of the stmbl project.
 *
 * Copyright (C) 2013-2015 Rene Hopf <renehopf@mac.com>
 * Copyright (C) 2013-2015 Nico Stute <crinq@crinq.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "scanf.h"
#include "printf.h"


#pragma once

#define MAX_HAL_PINS 256
#define MAX_HPNAME 32
#define MAX_COMP_TYPES 32
#define MAX_COMP_FUNCS 32


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
  int comp_type;
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

struct hal_pin map_hal_pin(HPNAME name);

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
  (name.value)

#define GLOBAL_HAL_PIN(name)               \
  volatile struct hal_pin name;       

#define MEM(var) static var

#define PIN(name)                       \
  *({                                   \
    if(&name != name.source->source){           \
      call(name.source->source->read_callback); \
    };                                   \
    &(name.source->source->value);              \
  })

#define INIT(func)                    \
 init = ({ void function(){func} function;});

#define FRT(func)                    \
 fast_rt = ({ void function(float period){func} function;});

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

#define TRG(func)                    \
  static struct hal_pin trg;       \
  init_hal_pin("trg", &trg, 0.0);  \
  trg.read_callback = ({ void function(){func} function;});

#define LINK_RC(src_pin, dst_pin)                    \
 src_pin.read_callback = dst_pin.read_callback;

#define HT(ht_code) \
{ \
inline void ht_function(){ \
static float ht_time_count; \
(void) ht_time_count; \
static int jump_label_pointer = -__COUNTER__ - 2; \
static int jump_label_pointer_old = 0; \
(void) jump_label_pointer_old; \
switch(jump_label_pointer){ \
case -__COUNTER__ - 1:; \
  ht_code; \
default: \
goto jump_label_ht_end; \
jump_label_ht_end: \
  break; \
}\
}\
ht_function(); \
}

#define GOTO(ht_label) \
jump_label_pointer = (ht_label); \
break;

#define NEXT(ht_label) \
jump_label_pointer = (ht_label);

#define STATE(ht_state) \
break; \
case (ht_state):

#define SLEEP(time) \
ht_time_count = 0.0; \
jump_label_pointer_old = jump_label_pointer; \
case -__COUNTER__ - 2:; jump_label_pointer =  -__COUNTER__ - 1; \
if(ht_time_count < (time)){ \
  ht_time_count += period; \
  goto jump_label_ht_end; \
} \
jump_label_pointer = jump_label_pointer_old;


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
