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

#include "hal.h"
#include "misc.h"

void init_hal(){
  hal.comp_type_count = 0;
  for(int i = 0; i < MAX_COMP_TYPES; i++){
    hal.comp_types_counter[i] = 0;
  }
  hal.hal_pin_count = 0;
  hal.fast_rt_lock = 0;
  hal.rt_lock = 0;
  hal.nrt_lock = 0;

  hal.init_func_count = 0;
  hal.fast_rt_func_count = 0;
  hal.rt_in_func_count = 0;
  hal.rt_filter_func_count = 0;
  hal.rt_pid_func_count = 0;
  hal.rt_calc_func_count = 0;
  hal.rt_out_func_count = 0;
  hal.nrt_func_count = 0;
}

void init_hal_pin(HPNAME name, struct hal_pin* pin, float value){
  strncpy(pin->name, hal.comp_type, MAX_HPNAME);
  strncat(pin->name, ".", MAX_HPNAME);
  strncat(pin->name, name, MAX_HPNAME);
  pin->value = value;
  pin->source = pin;
  pin->read_callback = 0;
  register_hal_pin(pin);
}

int register_hal_pin(struct hal_pin* pin){
  if(hal.hal_pin_count >= MAX_HAL_PINS){
    return(0);
  }

  for(int i = 0; i < hal.hal_pin_count; i++){
    if(!strcmp(hal.hal_pins[i]->name, pin->name)){
      return(0);
    }
  }

  hal.hal_pins[hal.hal_pin_count] = pin;
  hal.hal_pin_count++;

  return(1);
}

int set_hal_pin(HPNAME name, float value){
  for(int i = 0; i < hal.hal_pin_count; i++){
    if(!strcmp(hal.hal_pins[i]->name, name)){
      hal.hal_pins[i]->value = value;
      hal.hal_pins[i]->source = hal.hal_pins[i];
      return(1);
    }
  }
  return(0);
}

int is_hal_pin(HPNAME name){
  for(int i = 0; i < hal.hal_pin_count; i++){
    if(!strcmp(hal.hal_pins[i]->name, name)){
      return(1);
    }
  }
  return(0);
}

float get_hal_pin(HPNAME name){
  for(int i = 0; i < hal.hal_pin_count; i++){
    if(!strcmp(hal.hal_pins[i]->name, name)){
      return(hal.hal_pins[i]->source->source->value);
    }
  }
  return(0.0);
}

void write_hal_pin(struct hal_pin* pin, float value){
  pin->value = value;
  pin->source = pin;
}

float read_hal_pin(struct hal_pin* pin){
  return(pin->source->source->value);
}

struct hal_pin* find_hal_pin(HPNAME name){
  for(int i = 0; i < hal.hal_pin_count; i++){
    if(!strcmp(hal.hal_pins[i]->name, name)){
      return(hal.hal_pins[i]);
    }
  }
  return(0);
}

int link_hal_pins(HPNAME source, HPNAME sink){
  struct hal_pin* d;
  struct hal_pin* s;
  d = find_hal_pin(source);
  s = find_hal_pin(sink);

  if(d != 0 && s != 0){
    s->value = s->source->source->value;
    s->source = d;
	return(1);
  }
  printf_("link not possible %s:%i -> %s:%i\n", source, d, sink, s);
  return(0);
}

float read_float(char* buf){
  int buffer_pos = 0;
  float f = 0;
  float tf = 10;
  float invert = 1;

  if(buf[buffer_pos] && buf[buffer_pos] == '-'){
    invert = -1;
    buffer_pos++;
  }
  else if(buf[buffer_pos] && buf[buffer_pos] == '+'){
    buffer_pos++;
  }
  while(buf[buffer_pos] && isDecDigit(buf[buffer_pos])){
    f *= 10;
    f += buf[buffer_pos++] - '0';
  }
  if(buf[buffer_pos] && buf[buffer_pos] == '.'){
    buffer_pos++;
    while(buf[buffer_pos] && isDecDigit(buf[buffer_pos])){
      f += (buf[buffer_pos++] - '0') / tf;
      tf *= 10;
    }
  }
  f *= invert;
  return(f);
}

char* itoa(int i){
  hal.tmp[0] = (i % 10) + '0';
  hal.tmp[1] = '\0';
  return(hal.tmp);
}

int set_comp_type(HPNAME name){
  for(int i = 0; i < hal.comp_type_count; i++){
    if(!strcmp(hal.comp_types[i], name)){
      strncpy(hal.comp_type, name, MAX_HPNAME);
      strncat(hal.comp_type, itoa(hal.comp_types_counter[i]), MAX_HPNAME);
      return(hal.comp_types_counter[i]++);
    }
  }

  strncpy(hal.comp_types[hal.comp_type_count], name, MAX_HPNAME);
  strncpy(hal.comp_type, name, MAX_HPNAME);
  strncat(hal.comp_type, itoa(hal.comp_types_counter[hal.comp_type_count++]++), MAX_HPNAME);
  return(0);
}

void call(void (*func)()){
  if(func){
    func();
  }
}


int addf_init(void (*init)()){
  if(init != 0 && hal.init_func_count < MAX_COMP_FUNCS){
    hal.init[hal.init_func_count++] = init;
    return(hal.init_func_count);
  }
  return(-1);
}

int addf_fast_rt(void (*fast_rt)(float period)){
  if(fast_rt != 0 && hal.fast_rt_func_count < MAX_COMP_FUNCS){
    hal.fast_rt[hal.fast_rt_func_count++] = fast_rt;
    return(hal.fast_rt_func_count);
  }
  return(-1);
}

int addf_rt_in(void (*rt_in)(float period)){
  if(rt_in != 0 && hal.rt_in_func_count < MAX_COMP_FUNCS){
    hal.rt_in[hal.rt_in_func_count++] = rt_in;
    return(hal.rt_in_func_count);
  }
  return(-1);
}

int addf_rt_filter(void (*rt_filter)(float period)){
  if(rt_filter != 0 && hal.rt_filter_func_count < MAX_COMP_FUNCS){
    hal.rt_filter[hal.rt_filter_func_count++] = rt_filter;
    return(hal.rt_filter_func_count);
  }
  return(-1);
}

int addf_rt_pid(void (*rt_pid)(float period)){
  if(rt_pid != 0 && hal.rt_pid_func_count < MAX_COMP_FUNCS){
    hal.rt_pid[hal.rt_pid_func_count++] = rt_pid;
    return(hal.rt_pid_func_count);
  }
  return(-1);
}

int addf_rt_calc(void (*rt_calc)(float period)){
  if(rt_calc != 0 && hal.rt_calc_func_count < MAX_COMP_FUNCS){
    hal.rt_calc[hal.rt_calc_func_count++] = rt_calc;
    return(hal.rt_calc_func_count);
  }
  return(-1);
}

int addf_rt_out(void (*rt_out)(float period)){
  if(rt_out != 0 && hal.rt_out_func_count < MAX_COMP_FUNCS){
    hal.rt_out[hal.rt_out_func_count++] = rt_out;
    return(hal.rt_out_func_count);
  }
  return(-1);
}

int addf_nrt(void (*nrt)(float period)){
  if(nrt != 0 && hal.nrt_func_count < MAX_COMP_FUNCS){
    hal.nrt[hal.nrt_func_count++] = nrt;
    return(hal.nrt_func_count);
  }
  return(-1);
}
