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

#include "hal.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "commands.h"
#include "defines.h"
#include "version.h"

hal_t hal;

hal_comp_t *comp_by_name(NAME name) {
  for(int i = 0; i < comp_count; i++) {
    if(!strncmp(name, comps[i]->name, sizeof(NAME))) {
      return (comps[i]);
    }
  }
  return (0);
}

hal_comp_inst_t *comp_inst_by_name(NAME name, uint32_t instance) {
  hal_comp_t *comp = comp_by_name(name);
  if(comp) {
    for(int i = 0; i < hal.comp_inst_count; i++) {
      if(hal.comp_insts[i].comp == comp && instance == hal.comp_insts[i].instance) {
        return (&hal.comp_insts[i]);
      }
    }
  }
  return (0);
}

pin_t *pin_by_name(NAME comp_name, NAME pin_name) {
  uint32_t offset = 0;
  for(int i = 0; i < comp_count; i++) {
    if(!strncmp(comp_name, comps[i]->name, sizeof(NAME))) {
      for(int j = 0; j < comps[i]->pin_count; j++) {
        if(!strncmp(pin_name, pins[j + offset], sizeof(NAME))) {
          return (&pins[j + offset]);
        }
      }
    }
  }
  return (0);
}

hal_pin_inst_t *pin_inst_by_name(NAME comp_name, uint32_t instance, NAME pin_name) {
  hal_comp_inst_t *comp = comp_inst_by_name(comp_name, instance);
  if(comp) {
    for(int i = 0; i < comp->comp->pin_count; i++) {
      if(!strncmp(pin_name, comp->pins[i], sizeof(NAME))) {
        return (&comp->pin_insts[i]);
      }
    }
  }
  return (0);
}

pin_t *pin_by_pin_inst(hal_pin_inst_t *p) {
  for(int i = 0; i < hal.comp_inst_count; i++) {
    for(int j = 0; j < hal.comp_insts[i].comp->pin_count; j++) {
      if(&(hal.comp_insts[i].pin_insts[j]) == p) {
        return (&hal.comp_insts[i].pins[j]);
      }
    }
  }
  return (0);
}

hal_comp_inst_t *comp_inst_by_pin_inst(hal_pin_inst_t *p) {
  for(int i = 0; i < hal.comp_inst_count; i++) {
    for(int j = 0; j < hal.comp_insts[i].comp->pin_count; j++) {
      if(&(hal.comp_insts[i].pin_insts[j]) == p) {
        return (&hal.comp_insts[i]);
      }
    }
  }
  return (0);
}


void hal_term_print_state() {
  switch(hal.hal_state) {
    break;
    case HAL_OK2:
      printf("HAL state:  HAL_OK2\n");
      break;
    case RT_TOO_LONG:
      printf("HAL state:  RT_TOO_LONG\n");
      break;
    case FRT_TOO_LONG:
      printf("HAL state:  FRT_TOO_LONG\n");
      break;
    case MISC_ERROR:
      printf("HAL state:  MISC_ERROR\n");
      switch(hal.error_info.error_handler) {
        case HardFault:
          printf("error handler: HardFault\n");
          break;

        case NMI:
          printf("error handler: NMI\n");
          break;

        case MemManage:
          printf("error handler: MemManage\n");
          break;

        case BusFault:
          printf("error handler: BusFault\n");
          break;

        case UsageFault:
          printf("error handler: UsageFault\n");
          break;
      }
      printf("active rt func: %lu\n", hal.error_info.active_rt_func);
      printf("active frt func: %lu\n", hal.error_info.active_frt_func);
      printf("active nrt func: %lu\n", hal.error_info.active_nrt_func);

      break;
    case MEM_ERROR:
      printf("HAL state:  MEM_ERROR\n");
      break;
    case CONFIG_LOAD_ERROR:
      printf("HAL state:  CONFIG_LOAD_ERROR\n");
      break;
    case CONFIG_ERROR:
      printf("HAL state:  CONFIG_ERROR\n");
      break;
    case NAN_ERROR:
      printf("HAL state:  NAN_ERROR\n");
      break;
    default:
      printf("HAL state:  unkonwn error\n");
  }
}

void hal_term_print_info(char *ptr) {
  printf("######## hal info ########\n");
  printf("HAL_MAX_COMPS %lu/%i\n", hal.comp_inst_count, HAL_MAX_COMPS);
  printf("HAL_MAX_PINS %lu/%i\n", hal.pin_inst_count, HAL_MAX_PINS);
  printf("HAL_MAX_CTX %lu/%i\n", hal.ctx_count, HAL_MAX_CTX);
  hal_term_print_state();

  // printf("link errors %lu\n", hal.link_errors);
  // printf("pin errors %lu\n", hal.pin_errors);
  // printf("comp errors %lu\n", hal.comp_errors);
  // printf("set errors %lu\n", hal.set_errors);
  // printf("get errors %lu\n", hal.get_errors);
  // printf("foo0.bar:  %f\n", hal_get_pin("foo0.bar"));
  // printf("error_name: %s\n",hal.error_name);
  float pe               = hal.rt_period;
  float ct               = (float)hal.rt_ticks / hal_get_systick_freq();
  float mct              = (float)hal.rt_max_ticks / hal_get_systick_freq();
  float cct              = 0;
  float mcct             = 0;
  struct pin_ctx_t *pins = 0;
  printf("\n### rt info ###\n");
  switch(hal.rt_state) {
    case RT_STOP:
      printf("rt state:  STOP\n");
      break;
    case RT_SLEEP:
      printf("rt state:  SLEEP\n");
      break;
    case RT_CALC:
      printf("rt state:  CALC\n");
      break;
  }
  if(mct > 0.0) {
    printf("rt_ticks: %lu(max %lu)\n", hal.rt_ticks, hal.rt_max_ticks);
    if(pe > 0.0) {
      printf("rt time: %f(max %f)us/%fus = %f(max %f)%% @ %fkHz\n", ct * 1000000.0, mct * 1000000.0, pe * 1000000.0, (ct / pe) * 100.0, (mct / pe) * 100.0, 1.0 / pe / 1000.0);
    } else {
      printf("rt time: %f(max %f)us\n", ct * 1000000.0, mct * 1000000.0);
    }
  }
  printf("active rt funcs(%lu):\n", hal.rt_comp_count);
  for(int j = 0; j < hal.rt_comp_count; j++) {
    pins = (struct pin_ctx_t *)(hal.rt_comps[j]->pin_insts);
    cct  = (float)hal.rt_comps[j]->rt_ticks / hal_get_systick_freq();
    mcct = (float)hal.rt_comps[j]->rt_max_ticks / hal_get_systick_freq();
    printf("%s%lu(%f) %f(max %f)us", hal.rt_comps[j]->comp->name, hal.rt_comps[j]->instance, PIN(rt_prio), cct * 1000000.0, mcct * 1000000.0);
    if(pe > 0.0) {
      printf(" = %f(max %f)%%\n", cct / pe * 100.0, mcct / pe * 100.0);
    } else {
      printf("\n");
    }
  }


  pe  = hal.frt_period;
  ct  = (float)hal.frt_ticks / hal_get_systick_freq();
  mct = (float)hal.frt_max_ticks / hal_get_systick_freq();
  printf("\n### frt info ###\n");
  switch(hal.frt_state) {
    case RT_STOP:
      printf("frt state:  STOP\n");
      break;
    case RT_SLEEP:
      printf("frt state:  SLEEP\n");
      break;
    case RT_CALC:
      printf("frt state:  CALC\n");
      break;
  }
  if(mct > 0.0) {
    printf("frt_ticks: %lu(max %lu)\n", hal.frt_ticks, hal.frt_max_ticks);
    if(pe > 0.0) {
      printf("frt time: %f(max %f)us/%fus = %f(max %f)%% @ %fkHz\n", ct * 1000000.0, mct * 1000000.0, pe * 1000000.0, (ct / pe) * 100.0, (mct / pe) * 100.0, 1.0 / pe / 1000.0);
    } else {
      printf("frt time: %f(max %f)us\n", ct * 1000000.0, mct * 1000000.0);
    }
  }
  printf("active frt funcs(%lu):\n", hal.frt_comp_count);
  for(int j = 0; j < hal.frt_comp_count; j++) {
    pins = (struct pin_ctx_t *)(hal.frt_comps[j]->pin_insts);
    cct  = (float)hal.frt_comps[j]->frt_ticks / hal_get_systick_freq();
    mcct = (float)hal.frt_comps[j]->frt_max_ticks / hal_get_systick_freq();
    printf("%s(%f) %f(max %f)us", hal.frt_comps[j]->comp->name, PIN(frt_prio), cct * 1000000.0, mcct * 1000000.0);
    if(pe > 0.0) {
      printf(" = %f(max %f)%%\n", cct / pe * 100.0, mcct / pe * 100.0);
    } else {
      printf("\n");
    }
  }

  ct  = (float)hal.nrt_ticks / hal_get_systick_freq();
  mct = (float)hal.nrt_max_ticks / hal_get_systick_freq();
  printf("\n### nrt info ###\n");
  if(mct > 0.0) {
    printf("nrt time: %f(max %f)us\n", ct * 1000000.0, mct * 1000000.0);
  }
  printf("active nrt funcs(%lu):\n", hal.comp_inst_count);
  for(int j = 0; j < hal.comp_inst_count; j++) {
    if(hal.comp_insts[j].comp->nrt) {
      pins = (struct pin_ctx_t *)(hal.comp_insts[j].pin_insts);

      cct  = (float)hal.comp_insts[j].nrt_ticks / hal_get_systick_freq();
      mcct = (float)hal.comp_insts[j].nrt_max_ticks / hal_get_systick_freq();
      printf("%s %f(max %f)us\n", hal.comp_insts[j].comp->name, cct * 1000000.0, mcct * 1000000.0);
    }
  }
}

COMMAND("hal", hal_term_print_info, "print HAL stats");


uint32_t load_comp(hal_comp_t *comp) {
  if(!comp) {
    if(hal.debug_level < 2) {
      printf("<font color=\"FireBrick\">load_comp: not found</font>\n");
    }
    return (0);
  }
  if(hal.comp_inst_count >= HAL_MAX_COMPS - 1) {
    if(hal.debug_level < 2) {
      printf("<font color=\"FireBrick\">load_comp: not enough space to load comp: %s</font>\n", comp->name);
    }
    return (0);
  }
  if(hal.pin_inst_count + comp->pin_count >= HAL_MAX_PINS - 1) {
    if(hal.debug_level < 2) {
      printf("<font color=\"FireBrick\">load_comp: not enough space to load comp pins: %s</font>\n", comp->name);
    }
    return (0);
  }
  uint32_t ctx_size = ((uint32_t)ceil((comp->ctx_size / 4.0))) * 4;
  if(hal.ctx_count + ctx_size >= HAL_MAX_CTX - 1) {
    if(hal.debug_level < 2) {
      printf("<font color=\"FireBrick\">load_comp: not enough space to load comp ctx: %s</font>\n", comp->name);
    }
    return (0);
  }

  // load comp
  hal.comp_insts[hal.comp_inst_count].comp      = comp;
  hal.comp_insts[hal.comp_inst_count].ctx       = &hal.ctxs[hal.ctx_count];
  hal.comp_insts[hal.comp_inst_count].pin_insts = &hal.pin_insts[hal.pin_inst_count];
  hal.comp_insts[hal.comp_inst_count].ctx_size  = ctx_size;
  hal.comp_insts[hal.comp_inst_count].state     = PRE_INIT;

  uint32_t offset = 0;
  for(int i = 0; i < comp_count; i++) {
    if(comps[i] == comp) {
      hal.comp_insts[hal.comp_inst_count].pins = &pins[offset];
    }
    offset += comps[i]->pin_count;
  }
  hal.comp_insts[hal.comp_inst_count].instance = 0;
  for(int i = 0; i < hal.comp_inst_count; i++) {
    if(hal.comp_insts[i].comp == comp) {
      hal.comp_insts[hal.comp_inst_count].instance++;
    }
  }


  // load pins
  for(int i = hal.pin_inst_count; i < hal.pin_inst_count + comp->pin_count; i++) {
    hal.pin_insts[i].value  = 0.0;
    hal.pin_insts[i].source = &hal.pin_insts[i];
  }
  hal.pin_inst_count += comp->pin_count;

  // load ctx
  for(int i = hal.ctx_count; i < hal.ctx_count + ctx_size; i++) {
    hal.ctxs[i] = 0;
  }

  hal.ctx_count += ctx_size;

  if(hal.comp_insts[hal.comp_inst_count].comp->nrt_init != 0) {
    hal.comp_insts[hal.comp_inst_count].comp->nrt_init(hal.comp_insts[hal.comp_inst_count].ctx, hal.comp_insts[hal.comp_inst_count].pin_insts);
  }
  hal.comp_insts[hal.comp_inst_count].nrt_ticks     = 0;
  hal.comp_insts[hal.comp_inst_count].nrt_max_ticks = 0;
  hal.comp_insts[hal.comp_inst_count].state         = PRE_HW_INIT;

  hal.comp_inst_count++;

  return (1);
}

void hal_run_rt() {
#ifdef HAL_CALC_TIME
  uint32_t hal_start = hal_get_systick_value();
#endif

  switch(hal.rt_state) {
    case RT_STOP:
      return;
    case RT_CALC:  // call stop
      hal.rt_state  = RT_STOP;
      hal.hal_state = RT_TOO_LONG;
      hal.frt_state = RT_STOP;
      return;
    case RT_SLEEP:
      if(hal.active_rt_func > -1) {  // call stop
        hal.rt_state  = RT_STOP;
        hal.hal_state = MISC_ERROR;
        hal.frt_state = RT_STOP;
        return;
      }
      hal.rt_state = RT_CALC;
  }

#ifdef HAL_COMP_CALC_TIME
  uint32_t start = hal_get_systick_value();
#endif

  for(hal.active_rt_func = 0; hal.active_rt_func < hal.rt_comp_count; hal.active_rt_func++) {
    hal.rt_comps[hal.active_rt_func]->comp->rt(hal.rt_period, hal.rt_comps[hal.active_rt_func]->ctx, hal.rt_comps[hal.active_rt_func]->pin_insts);
#ifdef HAL_COMP_CALC_TIME
    uint32_t end_ticks = hal_get_systick_value();
    if(start < end_ticks) {
      start += hal_get_systick_reload();
    }
    hal.rt_comps[hal.active_rt_func]->rt_ticks     = start - end_ticks;
    hal.rt_comps[hal.active_rt_func]->rt_max_ticks = MAX(hal.rt_comps[hal.active_rt_func]->rt_max_ticks, hal.rt_comps[hal.active_rt_func]->rt_ticks);
    start                                          = end_ticks;
#endif
  }
  hal.active_rt_func = -1;

  if(hal.rt_state == RT_CALC) {
    hal.rt_state = RT_SLEEP;
  }

#ifdef HAL_CALC_TIME
  uint32_t hal_end = hal_get_systick_value();
  if(hal_start < hal_end) {
    hal_start += hal_get_systick_reload();
  }
  hal.rt_ticks     = hal_start - hal_end;
  hal.rt_max_ticks = MAX(hal.rt_max_ticks, hal.rt_ticks);
#endif

#ifdef HAL_WATCHDOG
  hal_reset_watchdog();
#endif
}

void hal_run_frt() {
#ifdef HAL_CALC_TIME
  uint32_t hal_start = hal_get_systick_value();
#endif

  switch(hal.frt_state) {
    case RT_STOP:
      return;
    case RT_CALC:
      hal.rt_state  = RT_STOP;
      hal.hal_state = FRT_TOO_LONG;
      hal.frt_state = RT_STOP;
      return;
    case RT_SLEEP:
      if(hal.active_frt_func > -1) {
        hal.rt_state  = RT_STOP;
        hal.hal_state = MISC_ERROR;
        hal.frt_state = RT_STOP;
        return;
      }
      hal.frt_state = RT_CALC;
  }

#ifdef HAL_COMP_CALC_TIME
  uint32_t start = hal_get_systick_value();
#endif

  for(hal.active_frt_func = 0; hal.active_frt_func < hal.frt_comp_count; hal.active_frt_func++) {
    hal.frt_comps[hal.active_frt_func]->comp->frt(hal.frt_period, hal.frt_comps[hal.active_frt_func]->ctx, hal.frt_comps[hal.active_frt_func]->pin_insts);
#ifdef HAL_COMP_CALC_TIME
    uint32_t end_ticks = hal_get_systick_value();
    if(start < end_ticks) {
      start += hal_get_systick_reload();
    }
    hal.frt_comps[hal.active_frt_func]->frt_ticks     = start - end_ticks;
    hal.frt_comps[hal.active_frt_func]->frt_max_ticks = MAX(hal.frt_comps[hal.active_frt_func]->frt_max_ticks, hal.frt_comps[hal.active_frt_func]->frt_ticks);
    start                                             = end_ticks;
#endif
  }
  hal.active_frt_func = -1;

  if(hal.frt_state == RT_CALC) {
    hal.frt_state = RT_SLEEP;
  }

#ifdef HAL_CALC_TIME
  uint32_t hal_end = hal_get_systick_value();
  if(hal_start < hal_end) {
    hal_start += hal_get_systick_reload();
  }
  hal.frt_ticks     = hal_start - hal_end;
  hal.frt_max_ticks = MAX(hal.frt_max_ticks, hal.frt_ticks);
#endif

#ifdef HAL_WATCHDOG
  hal_reset_watchdog();
#endif
}

void hal_run_nrt() {
#ifdef HAL_CALC_TIME
  uint32_t hal_start = hal_get_systick_value();
#endif

#ifdef HAL_COMP_CALC_TIME
  uint32_t start = hal_get_systick_value();
#endif

  for(hal.active_nrt_func = 0; hal.active_nrt_func < hal.comp_inst_count; hal.active_nrt_func++) {
    if(hal.comp_insts[hal.active_nrt_func].comp->nrt != 0) {
      hal.comp_insts[hal.active_nrt_func].comp->nrt(hal.comp_insts[hal.active_nrt_func].ctx, hal.comp_insts[hal.active_nrt_func].pin_insts);
#ifdef HAL_COMP_CALC_TIME
      uint32_t end_ticks = hal_get_systick_value();
      if(start < end_ticks) {
        start += hal_get_systick_reload();
      }
      hal.comp_insts[hal.active_nrt_func].nrt_ticks     = start - end_ticks;
      hal.comp_insts[hal.active_nrt_func].nrt_max_ticks = MAX(hal.comp_insts[hal.active_nrt_func].nrt_max_ticks, hal.comp_insts[hal.active_nrt_func].nrt_ticks);
      start                                             = end_ticks;
#endif
    }
  }
  hal.active_nrt_func = -1;

#ifdef HAL_CALC_TIME
  uint32_t hal_end = hal_get_systick_value();
  if(hal_start < hal_end) {
    hal_start += hal_get_systick_reload();
  }
  hal.nrt_ticks     = hal_start - hal_end;
  hal.nrt_max_ticks = MAX(hal.nrt_max_ticks, hal.nrt_ticks);
#endif

#ifdef HAL_WATCHDOG
  hal_reset_watchdog();
#endif
}

void hal_init_hw() {
  for(int i = 0; i < hal.comp_inst_count; i++) {
    if(hal.comp_insts[i].state == PRE_HW_INIT) {
      if(hal.comp_insts[i].comp->hw_init != 0) {
        hal.comp_insts[i].comp->hw_init(hal.comp_insts[i].ctx, hal.comp_insts[i].pin_insts);
      }
      hal.comp_insts[i].state = STARTED;
    }
  }
}


void load(char *ptr) {
  load_comp(comp_by_name(ptr));
}

COMMAND("load", load, "load comp from flash");

void show(char *ptr) {
  int j          = 0;
  int pin_offset = 0;
  for(int i = 0; i < comp_count; i++) {
    printf("%s\n", comps[i]->name);
    printf("#pins: %lu\n", comps[i]->pin_count);
    printf("#ctx: %lu byte\n", comps[i]->ctx_size);
    for(; j < pin_offset + comps[i]->pin_count; j++) {
      printf("- %s\n", pins[j]);
    }
    pin_offset += comps[i]->pin_count;
  }
}
COMMAND("show", show, "show comps in flash");

void list(char *ptr) {
  for(int i = 0; i < hal.comp_inst_count; i++) {
    printf("%s%lu:\n", hal.comp_insts[i].comp->name, hal.comp_insts[i].instance);
    printf("#pins: %lu\n", hal.comp_insts[i].comp->pin_count);
    printf("#ctx: %lu byte\n", hal.comp_insts[i].comp->ctx_size);
    printf("real #ctx: %lu byte\n", hal.comp_insts[i].ctx_size);
    printf("*pins: 0x%x\n", (unsigned int)hal.comp_insts[i].pin_insts);
    printf("*ctx: 0x%x\n", (unsigned int)hal.comp_insts[i].ctx);
    printf("state: ");
    switch(hal.comp_insts[i].state) {
      case PRE_INIT:
        printf("PRE_INIT\n");
        break;
      case PRE_HW_INIT:
        printf("PRE_HW_INIT\n");
        break;
      case STARTED:
        printf("STARTED\n");
        break;
      default:
        printf("unknown\n");
    }
    for(int j = 0; j < hal.comp_insts[i].comp->pin_count; j++) {
      hal_comp_inst_t *comp = comp_inst_by_pin_inst(hal.comp_insts[i].pin_insts[j].source->source);
      printf("-  %s <= %s%lu.%s = %f\n", hal.comp_insts[i].pins[j], comp->comp->name, comp->instance, (char *)pin_by_pin_inst(hal.comp_insts[i].pin_insts[j].source->source), hal.comp_insts[i].pin_insts[j].source->source->value);
    }
  }
}
COMMAND("list", list, "show comp instances");

void show_hal(char *ptr) {
  for(int i = 0; i < hal.comp_inst_count; i++) {
    for(int j = 0; j < hal.comp_insts[i].comp->pin_count; j++) {
      hal_comp_inst_t *comp = comp_inst_by_pin_inst(hal.comp_insts[i].pin_insts[j].source->source);
      printf("%s%lu.%s <= %s%lu.%s = %f\n", hal.comp_insts[i].comp->name, hal.comp_insts[i].instance, hal.comp_insts[i].pins[j], comp->comp->name, comp->instance, (char *)pin_by_pin_inst(hal.comp_insts[i].pin_insts[j].source->source), hal.comp_insts[i].pin_insts[j].source->source->value);
    }
  }
}
COMMAND("show_hal", show_hal, "show hal structure");


void sort_rt() {
  float min     = INFINITY;
  int min_index = -1;
  float rt_prio = 0.0;
  char added[HAL_MAX_COMPS];
  struct pin_ctx_t *pins;

  for(int i = 0; i < hal.comp_inst_count; i++) {
    added[i] = 0;
  }

  hal.rt_comp_count = 0;
  for(int i = 0; i < hal.comp_inst_count; i++) {
    min       = INFINITY;
    min_index = -1;
    for(int j = hal.comp_inst_count - 1; j >= 0; j--) {
      pins    = (struct pin_ctx_t *)(hal.comp_insts[j].pin_insts);
      rt_prio = PIN(rt_prio);
      if(rt_prio <= min && added[j] == 0 && rt_prio > 0.0 && hal.comp_insts[j].comp->rt != 0) {
        min       = rt_prio;
        min_index = j;
      }
    }
    if(min_index >= 0) {
      added[min_index]                  = 1;
      hal.rt_comps[hal.rt_comp_count++] = &hal.comp_insts[min_index];
    }
  }
}

void sort_frt() {
  float min      = INFINITY;
  int min_index  = -1;
  float frt_prio = 0.0;
  char added[HAL_MAX_COMPS];
  struct pin_ctx_t *pins;

  for(int i = 0; i < hal.comp_inst_count; i++) {
    added[i] = 0;
  }

  hal.frt_comp_count = 0;
  for(int i = 0; i < hal.comp_inst_count; i++) {
    min       = INFINITY;
    min_index = -1;
    for(int j = hal.comp_inst_count - 1; j >= 0; j--) {
      pins     = (struct pin_ctx_t *)(hal.comp_insts[j].pin_insts);
      frt_prio = PIN(frt_prio);
      if(frt_prio <= min && added[j] == 0 && frt_prio > 0.0 && hal.comp_insts[j].comp->frt != 0) {
        min       = frt_prio;
        min_index = j;
      }
    }
    if(min_index >= 0) {
      added[min_index]                    = 1;
      hal.frt_comps[hal.frt_comp_count++] = &hal.comp_insts[min_index];
    }
  }
}

void start_rt() {
  for(int i = 0; i < hal.rt_comp_count; i++) {
    if(hal.rt_comps[i]->comp->rt_start != 0) {
      hal.rt_comps[i]->comp->rt_start(hal.rt_comps[i]->ctx, hal.rt_comps[i]->pin_insts);
    }
    hal.rt_comps[i]->rt_ticks     = 0;
    hal.rt_comps[i]->rt_max_ticks = 0;
  }

  hal.rt_ticks     = 0.0;
  hal.rt_max_ticks = 0.0;

  hal.rt_state = RT_SLEEP;
}

void start_frt() {
  for(int i = 0; i < hal.frt_comp_count; i++) {
    if(hal.frt_comps[i]->comp->frt_start != 0) {
      hal.frt_comps[i]->comp->frt_start(hal.frt_comps[i]->ctx, hal.frt_comps[i]->pin_insts);
    }
    hal.frt_comps[i]->frt_ticks     = 0;
    hal.frt_comps[i]->frt_max_ticks = 0;
  }

  hal.frt_ticks     = 0.0;
  hal.frt_max_ticks = 0.0;

  hal.frt_state = RT_SLEEP;
}

void hal_start() {
  hal.hal_state = HAL_OK2;

  sort_rt();
  sort_frt();
  hal_init_hw();
  start_rt();
  start_frt();
}

COMMAND("start", hal_start, "start rt system");

void stop_rt() {
  hal.rt_state = RT_STOP;

  for(int i = 0; i < hal.rt_comp_count; i++) {
    if(hal.rt_comps[i]->comp->rt_stop != 0) {
      hal.rt_comps[i]->comp->rt_stop(hal.rt_comps[i]->ctx, hal.rt_comps[i]->pin_insts);
    }
  }
}

void stop_frt() {
  hal.frt_state = RT_STOP;

  for(int i = 0; i < hal.frt_comp_count; i++) {
    if(hal.frt_comps[i]->comp->frt_stop != 0) {
      hal.frt_comps[i]->comp->frt_stop(hal.frt_comps[i]->ctx, hal.frt_comps[i]->pin_insts);
    }
  }
}

void hal_stop() {
  stop_rt();
  stop_frt();
}

COMMAND("stop", hal_stop, "stop rt system");

void hal_init(float rt_period, float frt_period) {
  hal.rt_state  = RT_STOP;
  hal.frt_state = RT_STOP;

  for(int i = 0; i < HAL_MAX_COMPS; i++) {
    hal.rt_comps[i]  = 0;
    hal.frt_comps[i] = 0;
  }

  hal.comp_inst_count = 0;
  hal.rt_comp_count   = 0;
  hal.frt_comp_count  = 0;
  hal.pin_inst_count  = 0;

  for(int i = 0; i < HAL_MAX_CTX; i++) {
    hal.ctxs[i] = 0;
  }
  hal.ctx_count = 0;

  hal.active_rt_func  = -1;
  hal.active_frt_func = -1;
  hal.active_nrt_func = -1;

  hal.rt_ticks      = 0.0;
  hal.rt_max_ticks  = 0.0;
  hal.frt_ticks     = 0.0;
  hal.frt_max_ticks = 0.0;
  hal.nrt_ticks     = 0.0;
  hal.nrt_max_ticks = 0.0;
  hal.rt_period     = rt_period;
  hal.frt_period    = frt_period;
  hal.nrt_ticks     = 0.0;
  hal.nrt_max_ticks = 0.0;
}

void hal_print_pin(hal_pin_inst_t *p) {
  pin_t *pin                     = pin_by_pin_inst(p);
  hal_comp_inst_t *comp = comp_inst_by_pin_inst(p);

  pin_t *pin2;
  hal_comp_inst_t *comp2;
  pin_t *pin3;
  hal_comp_inst_t *comp3;

  if(p && pin && comp) {
    if(p == p->source) {  //if pin is not linked
      printf("%s%lu.%s = %f\n", (char *)comp->comp->name, comp->instance, (char *)pin, p->value);
    } else if(p->source == p->source->source) {  //pin is single linked
      pin2  = pin_by_pin_inst(p->source);
      comp2 = comp_inst_by_pin_inst(p->source);
      printf("%s%lu.%s <= %s%lu.%s = %f\n", (char *)comp->comp->name, comp->instance, (char *)pin, (char *)comp2->comp->name, comp2->instance, (char *)pin2, p->source->value);
    } else {  //pin is double linked
      pin2  = pin_by_pin_inst(p->source);
      comp2 = comp_inst_by_pin_inst(p->source);
      pin3  = pin_by_pin_inst(p->source->source);
      comp3 = comp_inst_by_pin_inst(p->source->source);
      printf("%s%lu.%s <= %s%lu.%s <= %s%lu.%s = %f\n", (char *)comp->comp->name, comp->instance, (char *)pin, (char *)comp2->comp->name, comp2->instance, (char *)pin2, (char *)comp3->comp->name, comp3->instance, (char *)pin3, p->source->source->value);
    }
  }
}

char *findline(char *ptr) {
  for(int i = 0; i < 64; i++) {
    if(ptr[i] == 0) {
      return (0);
    }
    if(ptr[i] == '\n') {
      return (ptr + i + 1);
    }
  }
  return (0);
}

uint32_t hal_parse_(char *cmd);

uint32_t hal_parse(char *cmd) {
  do {
    hal_parse_(cmd);
    cmd = findline(cmd);
  } while(cmd);
  return (0);
}

uint32_t hal_parse_(char *cmd) {
  if(cmd == 0){
    return(2);
  }
  if(cmd[0] == '\n'){
    return(2);
  }

  if(call_cmd(cmd)) {
    return (1);
  }

  int32_t foo = 0;

  char sinkc[64];
  int32_t sinki = 0;
  char sinkp[64];

  float value = 0.0;

  char sourcec[64];
  int32_t sourcei = 0;
  char sourcep[64];

  hal_pin_inst_t *sink;
  hal_pin_inst_t *source;

  uint32_t found = 0;

  foo = sscanf(cmd, " %[a-zA-Z_]%li.%[a-zA-Z0-9_] = %f", sinkc, &sinki, sinkp, &value);
  if(sinki < 0){
    for(int i = 0; i < hal.comp_inst_count; i++) {
       if(!strcmp(hal.comp_insts[i].comp->name, sinkc)) {
         sinki++;
       }
    }
  }

  switch(foo) {
    case 0:
      break;
    case 1:  // search comps
      for(int i = 0; i < hal.comp_inst_count; i++) {
        if(!strncmp(hal.comp_insts[i].comp->name, sinkc, strlen(sinkc))) {
          if(hal.debug_level < 1) {
            printf("%s%lu\n", hal.comp_insts[i].comp->name, hal.comp_insts[i].instance);
          }
          found = 1;
        }
      }
      if(!found) {
        if(hal.debug_level < 2) {
          printf("<font color=\"FireBrick\">not found: %s</font>\n", cmd);
        }
      }
      break;
    case 2:  // search comps + instance
      for(int i = 0; i < hal.comp_inst_count; i++) {
        if(hal.comp_insts[i].instance == sinki && !strcmp(hal.comp_insts[i].comp->name, sinkc)) {
          for(int j = 0; j < hal.comp_insts[i].comp->pin_count; j++) {
            //hal_comp_inst_t * comp = comp_inst_by_pin_inst(hal.comp_insts[i].pin_insts[j].source->source);
            //printf("%s%lu.%s <= %s%lu.%s = %f\n", hal.comp_insts[i].comp->name, hal.comp_insts[i].instance, hal.comp_insts[i].pins[j], comp->comp->name, comp->instance, (char *)pin_by_pin_inst(hal.comp_insts[i].pin_insts[j].source->source), hal.comp_insts[i].pin_insts[j].source->source->value);
            hal_print_pin(&(hal.comp_insts[i].pin_insts[j]));
            found = 1;
          }
        }
      }
      if(!found) {
        if(hal.debug_level < 2) {
          printf("<font color=\"FireBrick\">not found: %s</font>\n", cmd);
        }
      }
      break;
    case 3:
      foo = sscanf(cmd, " %[a-zA-Z_]%li.%[a-zA-Z0-9_] = %[a-zA-Z_]%li.%[a-zA-Z0-9_]", sinkc, &sinki, sinkp, sourcec, &sourcei, sourcep);
      if(sinki < 0){
        for(int i = 0; i < hal.comp_inst_count; i++) {
          if(!strcmp(hal.comp_insts[i].comp->name, sinkc)) {
            sinki++;
          }
        }
      } 
      if(sourcei < 0){
        for(int i = 0; i < hal.comp_inst_count; i++) {
          if(!strcmp(hal.comp_insts[i].comp->name, sourcec)) {
            sourcei++;
          }
        }
      }

      if(foo == 6) {  // link pins
        sink   = pin_inst_by_name(sinkc, sinki, sinkp);
        source = pin_inst_by_name(sourcec, sourcei, sourcep);
        if(sink && source) {
          sink->source = source;
          if(hal.debug_level < 1) {
            printf("OK %s%lu.%s <= %s%lu.%s = %f\n", sinkc, sinki, sinkp, sourcec, sourcei, sourcep, source->source->value);
          }
        } else if(sink) {
          if(hal.debug_level < 2) {
            printf("<font color=\"FireBrick\">not found: %s%lu.%s</font>\n", sourcec, sourcei, sourcep);
          }
        } else {
          if(hal.debug_level < 2) {
            printf("<font color=\"FireBrick\">not found: %s%lu.%s</font>\n", sinkc, sinki, sinkp);
          }
        }
      } else {  // search comps + instance + pin
        for(int i = 0; i < hal.comp_inst_count; i++) {
          if(hal.comp_insts[i].instance == sinki && !strcmp(hal.comp_insts[i].comp->name, sinkc)) {
            for(int j = 0; j < hal.comp_insts[i].comp->pin_count; j++) {
              //hal_comp_inst_t * comp = comp_inst_by_pin_inst(hal.comp_insts[i].pin_insts[j].source->source);
              if(!strncmp(hal.comp_insts[i].pins[j], sinkp, strlen(sinkp))) {
                hal_print_pin(&(hal.comp_insts[i].pin_insts[j]));
                //printf("%s%lu.%s <= %s%lu.%s = %f\n", hal.comp_insts[i].comp->name, hal.comp_insts[i].instance, hal.comp_insts[i].pins[j], comp->comp->name, comp->instance, (char *)pin_by_pin_inst(hal.comp_insts[i].pin_insts[j].source->source), hal.comp_insts[i].pin_insts[j].source->source->value);
                found = 1;
              }
            }
          }
        }
        if(!found) {
          if(hal.debug_level < 2) {
            printf("<font color=\"FireBrick\">not found: %s</font>\n", cmd);
          }
        }
      }
      break;
    case 4:  // set pin
      sink = pin_inst_by_name(sinkc, sinki, sinkp);
      if(sink) {
        sink->value  = value;
        sink->source = sink;
        if(hal.debug_level < 1) {
          printf("OK %s%lu.%s = %f\n", sinkc, sinki, sinkp, value);
        }
      } else {
        if(hal.debug_level < 2) {
          printf("<font color=\"FireBrick\">not found: %s%lu.%s</font>\n", sinkc, sinki, sinkp);
        }
      }
      break;
    default:
      if(hal.debug_level < 2) {
        printf("<font color=\"FireBrick\">not found: %s</font>\n", cmd);
      }
  }
  return (0);
}

void hal_error(uint32_t error_handler) {
  hal.error_info.active_rt_func  = hal.active_rt_func;
  hal.error_info.active_frt_func = hal.active_frt_func;
  hal.error_info.active_nrt_func = hal.active_nrt_func;
  hal.error_info.error_handler   = error_handler;
  hal_stop();
  hal.hal_state = MISC_ERROR;
}

void fault(char *ptr) {
  printf("trigger fault handler\n");
  uint32_t *p = (uint32_t *)0x08010000;
  p[0]                 = 1;
}

COMMAND("fault", fault, "trigger fault");

void hal_set_debug_level(uint32_t debug_level) {
  hal.debug_level = debug_level;
}

void debug_level(char *ptr) {
  int debug_level = 0;
  if(ptr) {
    sscanf(ptr, "%i", &debug_level);
  }

  hal_set_debug_level(CLAMP(debug_level, 0, 2));
}

COMMAND("debug_level", debug_level, "set hal debug level, 0 = print all, 1 = print errors, 2 = no output");

void hal_linked_pins(char *ptr){
  char sinkc[64];
  uint32_t sinki = 0;
  char sinkp[64];
  int foo;
  foo = sscanf(ptr, " %[a-zA-Z_]%lu.%[a-zA-Z0-9_]", sinkc, &sinki, sinkp);
  if(foo == 3){
    hal_pin_inst_t *pin = pin_inst_by_name(sinkc, sinki, sinkp);
    if(pin){
      hal_print_pin(pin);
      for(int i = 0; i < hal.comp_inst_count; i++) {
        for(int j = 0; j < hal.comp_insts[i].comp->pin_count; j++) {
          if(hal.comp_insts[i].pin_insts[j].source->source == pin && &(hal.comp_insts[i].pin_insts[j]) != pin){
            hal_print_pin(&(hal.comp_insts[i].pin_insts[j]));
          }
        }
      }
      return;
    }
  }
  printf("not found: %s\n",ptr);
}

COMMAND("linked", hal_linked_pins, "show linked pins");

void hal_relink_pins(char *ptr){
  for(int i = 0; i < hal.comp_inst_count; i++) {
    for(int j = 0; j < hal.comp_insts[i].comp->pin_count; j++) {
      hal.comp_insts[i].pin_insts[j].source = hal.comp_insts[i].pin_insts[j].source->source;
    }
  }
}

COMMAND("relink", hal_relink_pins, "relink all hal pins (a = b, b = c => a = c, b = c)");