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

#include <stdint.h>

#pragma once

#define MAX_HAL_PINS 600
#define MAX_HPNAME 32
#define MAX_COMP_TYPES 64
#define MAX_COMPS 64

typedef char HPNAME[MAX_HPNAME];

struct hal_pin{
  HPNAME name;
  volatile float value;
  struct hal_pin* source;
};

struct hal_comp{
   HPNAME name;
   void (*rt_init)();
   void (*rt_deinit)();
   void (*nrt_init)();
   void (*rt)(float period);
   void (*frt)(float period);
   void (*nrt)(float period);
   int hal_pin_start_index;
   int hal_pin_count;
   uint32_t instance;
};

struct hal_struct{
  HPNAME comp_types[MAX_COMP_TYPES];
  int comp_types_counter[MAX_COMP_TYPES];
  int comp_type_count;
  int comp_type;
  HPNAME tmp;

  struct hal_comp* hal_comps[MAX_COMPS];
  int comp_count;

  void (*rt_init[MAX_COMPS])();
  int rt_init_func_count;

  void (*rt_deinit[MAX_COMPS])();
  int rt_deinit_func_count;

  void (*nrt_init[MAX_COMPS])();
  int nrt_init_func_count;

  void (*rt[MAX_COMPS])(float period);
  int rt_func_count;

  void (*nrt[MAX_COMPS])(float period);
  int nrt_func_count;

  void (*frt[MAX_COMPS])(float period);
  int frt_func_count;

  struct hal_pin* hal_pins[MAX_HAL_PINS];
  int hal_pin_count;

  volatile enum{
    RT_CALC,
    RT_SLEEP,
    RT_STOP
  } rt_state;

  volatile enum{
    FRT_CALC,
    FRT_SLEEP,
    FRT_STOP
  } frt_state;

  volatile enum {
    FRT_TOO_LONG,
    RT_TOO_LONG,
    MISC_ERROR,
    MEM_ERROR,
    CONFIG_LOAD_ERROR,
    CONFIG_ERROR,
    HAL_OK
  } hal_state;

  volatile int active_rt_func;
  volatile int active_frt_func;
  volatile int active_nrt_func;

  uint32_t link_errors;
  uint32_t pin_errors;
  uint32_t set_errors;
  uint32_t get_errors;
  uint32_t comp_errors;
  char error_name[MAX_HPNAME];
} hal;

void init_hal();

void start_hal();

void stop_hal();

int start_rt();

int start_frt();

void stop_rt();

void stop_frt();

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

void add_comp(struct hal_comp* comp);

extern void enable_rt();
extern void enable_frt();
extern void disable_rt();
extern void disable_frt();

#define HAL_COMP(type)                  \
{                                   \
  static struct hal_comp self; \
  strncpy(self.name, #type, MAX_HPNAME); \
  self.nrt_init = 0; \
  self.rt_init = 0; \
  self.rt_deinit = 0; \
  self.rt = 0; \
  self.frt = 0; \
  self.nrt = 0; \
  self.hal_pin_start_index = hal.hal_pin_count; \
  self.hal_pin_count = 0; \
  set_comp_type(self.name);             \
  HAL_PIN(rt_calc_time) = 0.0; \
  HAL_PIN(frt_calc_time) = 0.0; \
  HAL_PIN(rt_prio) = -1.0; \
  HAL_PIN(frt_prio) = -1.0;

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
 self.nrt_init = ({ void function(){func} function;});

#define RT_INIT(func)                    \
 self.rt_init = ({ void function(){func} function;});

#define RT_DEINIT(func)                    \
 self.rt_deinit = ({ void function(){func} function;});

#define RT(func)                    \
 self.rt = ({ void function(float period){ \
   unsigned int __start_time__ = SysTick->VAL; \
   func \
   unsigned int __end_time__ = SysTick->VAL; \
   if(__start_time__ < __end_time__){ \
     __start_time__ += SysTick->LOAD; \
   } \
   PIN(rt_calc_time) = ((float)(__start_time__ - __end_time__)) / RCC_Clocks.HCLK_Frequency; \
   } function;});

#define FRT(func)                    \
 self.frt = ({ void function(float period){ \
   unsigned int __start_time__ = SysTick->VAL; \
   func \
   unsigned int __end_time__ = SysTick->VAL; \
   if(__start_time__ < __end_time__){ \
     __start_time__ += SysTick->LOAD; \
   } \
   PIN(frt_calc_time) = ((float)(__start_time__ - __end_time__)) / RCC_Clocks.HCLK_Frequency; \
   } function;});

#define NRT(func)                    \
 self.nrt = ({ void function(float period){func} function;});

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
  self.hal_pin_count = hal.hal_pin_count - self.hal_pin_start_index; \
  add_comp(&self); \
}

#define RISING_EDGE(sig)\
({static float __old_val__ = 0.0; uint8_t ret = (sig) > __old_val__; __old_val__ = (sig); ret;})

#define FALLING_EDGE(sig)\
({static float __old_val__ = 0.0; uint8_t ret = (sig) < __old_val__; __old_val__ = (sig); ret;})

#define EDGE(sig)\
({static float __old_val__ = 0.0; uint8_t ret = (sig) != __old_val__; __old_val__ = (sig); ret;})
