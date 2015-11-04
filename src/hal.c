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

char* itoa(int i){
  hal.tmp[0] = (i % 10) + '0';
  hal.tmp[1] = '\0';
  return(hal.tmp);
}

void init_hal(){
  hal.comp_type_count = 0;
  for(int i = 0; i < MAX_COMP_TYPES; i++){
    hal.comp_types_counter[i] = 0;
  }
  hal.hal_pin_count = 0;
  hal.rt_lock = 0;
  hal.nrt_lock = 0;

  hal.init_func_count = 0;
  hal.rt_func_count = 0;
  hal.nrt_func_count = 0;
  hal.frt_func_count = 0;
}

void init_hal_pin(HPNAME name, struct hal_pin* pin, float value){
  strncpy(pin->name, hal.comp_types[hal.comp_type], MAX_HPNAME);
  strncat(pin->name, itoa(hal.comp_types_counter[hal.comp_type]), MAX_HPNAME);
  strncat(pin->name, ".", MAX_HPNAME);
  strncat(pin->name, name, MAX_HPNAME);
  pin->value = value;
  pin->source = pin;
  register_hal_pin(pin);
}

int register_hal_pin(struct hal_pin* pin){
  if(hal.hal_pin_count >= MAX_HAL_PINS){
    printf_("reg hal pin: too many pins: %i\n", hal.hal_pin_count);
    return(0);
  }

  for(int i = 0; i < hal.hal_pin_count; i++){
    if(!strcmp(hal.hal_pins[i]->name, pin->name)){
      printf_("reg hal pin: name fault: %s\n", pin->name);
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
  printf_("set not possible %s = %f\n", name, value);
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
  printf_("get not possible %s\n", name);
  return(0.0);
}

struct hal_pin map_hal_pin(HPNAME name){
    for(int i = 0; i < hal.hal_pin_count; i++){
      if(!strcmp(hal.hal_pins[i]->name, name)){
		  return(*hal.hal_pins[i]);
      }
    }
	struct hal_pin tmp;
//	tmp.name = "";
	tmp.value = 0.0;
	tmp.source = &tmp;
    return(tmp);
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
   //   if(s->source != s){
   //      printf_("relink %s:%i -> %s:%i to %s:%i -> %s:%i\n", s->source->name, s->source, sink, s, source, d, sink, s);
   //   }
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

int set_comp_type(HPNAME name){
  for(int i = 0; i < hal.comp_type_count; i++){
    if(!strcmp(hal.comp_types[i], name)){
      hal.comp_type = i;
      return(hal.comp_types_counter[hal.comp_type]++);
    }
  }

  if(hal.comp_type_count < MAX_COMP_TYPES){
    strncpy(hal.comp_types[hal.comp_type_count], name, MAX_HPNAME);
    hal.comp_type = hal.comp_type_count++;
    return(0);
  }
  printf_("set comp type: too many comps types: %i\n", hal.comp_type_count);
  return(-1);
}

int addf_init(void (*init)()){
  if(init != 0 && hal.init_func_count < MAX_COMP_FUNCS){
    hal.init[hal.init_func_count++] = init;
    return(hal.init_func_count);
  }
  return(-1);
}

int addf_rt(void (*rt)(float period)){
  if(rt != 0 && hal.rt_func_count < MAX_COMP_FUNCS){
    hal.rt[hal.rt_func_count++] = rt;
    return(hal.rt_func_count);
  }
  return(-1);
}

int addf_frt(void (*frt)(float period)){
  if(frt != 0 && hal.frt_func_count < MAX_COMP_FUNCS){
    hal.frt[hal.frt_func_count++] = frt;
    return(hal.frt_func_count);
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
