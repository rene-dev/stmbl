/*
* This file is part of the stmbl project.
*
* Copyright (C) 2013-2017 Rene Hopf <renehopf@mac.com>
* Copyright (C) 2013-2017 Nico Stute <crinq@crinq.de>
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

#pragma once
#include <stdint.h>
#include <stdio.h>

#define HAL_CALC_TIME
#define HAL_COMP_CALC_TIME
//#define HAL_WATCHDOG

#ifndef HAL_MAX_PINS
#define HAL_MAX_PINS 1024
#endif

#ifndef HAL_MAX_COMPS
#define HAL_MAX_COMPS 32
#endif

#ifndef HAL_MAX_CTX
#define HAL_MAX_CTX 16384
#endif

#define HAL_COMP(name)
#define HAL_PIN(name)
#define HAL_PINA(name, index)

extern uint32_t hal_get_systick_value();
extern uint32_t hal_get_systick_reload();
extern uint32_t hal_get_systick_freq();
extern void hal_init_watchdog(float time);
extern void hal_reset_watchdog();

typedef char NAME[32];

typedef NAME const pin_t;

typedef struct hal_pin_inst_t {
  float value;
  struct hal_pin_inst_t *source;
} hal_pin_inst_t;

typedef const struct {
  NAME name;
  void (*nrt)(void *ctx_ptr, hal_pin_inst_t *pin_ptr);
  void (*rt)(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr);
  void (*frt)(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr);

  void (*nrt_init)(void *ctx_ptr, hal_pin_inst_t *pin_ptr);
  void (*hw_init)(void *ctx_ptr, hal_pin_inst_t *pin_ptr);
  void (*rt_start)(void *ctx_ptr, hal_pin_inst_t *pin_ptr);
  void (*frt_start)(void *ctx_ptr, hal_pin_inst_t *pin_ptr);
  void (*rt_stop)(void *ctx_ptr, hal_pin_inst_t *pin_ptr);
  void (*frt_stop)(void *ctx_ptr, hal_pin_inst_t *pin_ptr);

  uint32_t ctx_size;
  uint32_t pin_count;
} hal_comp_t;

typedef struct hal_comp_inst_t {
  hal_comp_t *comp;
  uint32_t instance;
  void *ctx;
  hal_pin_inst_t *pin_insts;
  pin_t *pins;
  uint32_t ctx_size;
  int32_t rt_ticks;
  int32_t frt_ticks;
  int32_t nrt_ticks;

  int32_t rt_max_ticks;
  int32_t frt_max_ticks;
  int32_t nrt_max_ticks;

  enum {
    PRE_INIT,
    PRE_HW_INIT,
    STARTED,
  } state;
} hal_comp_inst_t;

#define PIN(p) (pins->p.source->value)
#define PINA(p, i) (pins->p[i].source->value)

typedef struct {
  uint32_t active_rt_func;
  uint32_t active_frt_func;
  uint32_t active_nrt_func;
  enum {
    HardFault,
    NMI,
    MemManage,
    BusFault,
    UsageFault,
  } error_handler;
} hal_error_t;

typedef struct {
  enum thread_state_t {
    RT_CALC,
    RT_SLEEP,
    RT_STOP
  } rt_state;

  enum thread_state_t frt_state;

  enum hal_state_t {
    FRT_TOO_LONG,
    RT_TOO_LONG,
    MISC_ERROR,
    MEM_ERROR,
    CONFIG_LOAD_ERROR,
    CONFIG_ERROR,
    NAN_ERROR,
    HAL_OK2
  } hal_state;

  struct hal_comp_inst_t *rt_comps[HAL_MAX_COMPS];
  struct hal_comp_inst_t *frt_comps[HAL_MAX_COMPS];

  int32_t active_rt_func;
  int32_t active_frt_func;
  int32_t active_nrt_func;

  struct hal_comp_inst_t comp_insts[HAL_MAX_COMPS];
  struct hal_pin_inst_t pin_insts[HAL_MAX_PINS];
  uint8_t ctxs[HAL_MAX_CTX];  // create runtime ctx print in python based on COMP_ctx_t
  uint32_t comp_inst_count;
  uint32_t rt_comp_count;
  uint32_t frt_comp_count;
  uint32_t pin_inst_count;
  uint32_t ctx_count;

  uint32_t rt_ticks;
  uint32_t frt_ticks;
  uint32_t nrt_ticks;

  uint32_t rt_max_ticks;
  uint32_t frt_max_ticks;
  uint32_t nrt_max_ticks;

  float rt_period;
  float frt_period;

  hal_error_t error_info;

  enum {
    PRINT_ALL,
    PRINT_ERRORS,
    NO_PRINT,
  } debug_level;
} hal_t;

extern hal_t hal;

hal_comp_t *comp_by_name(NAME name);
hal_comp_inst_t *comp_inst_by_name(NAME name, uint32_t instance);
uint32_t pin_offset_by_comp_name(NAME name);
pin_t *pin_by_name(NAME comp_name, NAME pin_name);
hal_pin_inst_t *pin_inst_by_name(NAME comp_name, uint32_t instance, NAME pin_name);
uint32_t load_comp(hal_comp_t *comp);
pin_t *pin_by_pin_inst(hal_pin_inst_t *p);
hal_comp_inst_t *comp_inst_by_pin_inst(hal_pin_inst_t *p);
void hal_print_pin(hal_pin_inst_t *p);

void hal_init(float rt_period, float frt_period);
// void hal_init_nrt();
void hal_start();
void hal_stop();
void hal_run_rt();
void hal_run_frt();
void hal_run_nrt();
uint32_t hal_parse(char *cmd);
void hal_error(uint32_t error_handler);
void hal_set_debug_level(uint32_t debug_level);

#include "hal_tbl.h"
