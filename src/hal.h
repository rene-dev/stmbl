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

#define MAX_HAL_PINS 512
#define MAX_HPNAME 32
#define MAX_COMP_TYPES 64
#define MAX_COMP_FUNCS 128


typedef char HPNAME[MAX_HPNAME];

struct hal_pin{
  HPNAME name;
  volatile float value;
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

  void (*rt[MAX_COMP_FUNCS])(float period);
  int rt_func_count;

  void (*nrt[MAX_COMP_FUNCS])(float period);
  int nrt_func_count;
  
  void (*frt[MAX_COMP_FUNCS])(float period);
  int frt_func_count;

  struct hal_pin* hal_pins[MAX_HAL_PINS];
  int hal_pin_count;

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

int addf_init(void (*init)());
int addf_rt(void (*rt)(float period));
int addf_nrt(void (*nrt)(float period));
int addf_frt(void (*frt)(float period));


#define COMP(type)                  \
{                                   \
  set_comp_type(#type);             \
  void (*init)() = 0;               \
  void (*rt)(float period) = 0;\
  void (*nrt)(float period) = 0; \
  void (*frt)(float period) = 0; \
  HAL_PIN(calc_time) = 0.0;

#define HAL_PIN(name)               \
  static struct hal_pin name##_hal_pin;       \
  init_hal_pin(#name, &name##_hal_pin, 0.0);  \
  (name##_hal_pin.value)

#define GLOBAL_HAL_PIN(name)               \
  volatile struct hal_pin name##_hal_pin;

#define MEM(var) static var

#define PIN(name)                       \
  (name##_hal_pin.source->source->value)

#define INIT(func)                    \
 init = ({ void function(){func} function;});

#define RT(func)                    \
 rt = ({ void function(float period){ \
   unsigned int __start_time__ = SysTick->VAL; \
   func \
   unsigned int __end_time__ = SysTick->VAL; \
   if(__start_time__ < __end_time__){ \
     __start_time__ += SysTick->LOAD; \
   } \
   PIN(calc_time) = ((float)(__start_time__ - __end_time__)) / RCC_Clocks.HCLK_Frequency; \
   } function;});

#define FRT(func)                    \
 frt = ({ void function(float period){ \
   unsigned int __start_time__ = SysTick->VAL; \
   func \
   unsigned int __end_time__ = SysTick->VAL; \
   if(__start_time__ < __end_time__){ \
     __start_time__ += SysTick->LOAD; \
   } \
   PIN(calc_time) = ((float)(__start_time__ - __end_time__)) / RCC_Clocks.HCLK_Frequency; \
   } function;});

#define NRT(func)                    \
 nrt = ({ void function(float period){func} function;});

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

#define BLINK(N) \
({ \
  int t = (systime / 300) % (2 * N + 2); \
  if(t < 2){ \
    t = 0; \
  } \
  else{ \
    t = t % 2; \
  } \
  t;\
})

#define ENDCOMP \
  addf_init(init); \
  addf_rt(rt); \
  addf_nrt(nrt); \
  addf_frt(frt); \
}
