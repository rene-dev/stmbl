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

#include <string.h>
#include <math.h>
#include "hal.h"

//global hal struct
hal_struct_t hal;

//TODO: only works for single positive digit
char* hal_itoa(int i){
  hal.tmp[0] = (i % 10) + '0';
  hal.tmp[1] = '\0';
  return(hal.tmp);
}

void hal_init(){
  hal.comp_type_count = 0;
  for(int i = 0; i < MAX_COMP_TYPES; i++){
    hal.comp_types_counter[i] = 0;
  }
  hal.hal_pin_count = 0;
  hal.comp_count = 0;

  hal.nrt_init_func_count = 0;
  hal.rt_func_count = 0;
  hal.nrt_func_count = 0;
  hal.frt_func_count = 0;

  hal.link_errors = 0;
  hal.pin_errors = 0;
  hal.set_errors = 0;
  hal.get_errors = 0;
  hal.comp_errors = 0;

  hal.rt_state = RT_STOP;
  hal.frt_state = FRT_STOP;
  hal.hal_state = HAL_OK;
  hal.active_rt_func = -1;
  hal.active_frt_func = -1;
  hal.active_nrt_func = -1;
  strcpy(hal.error_name,"no error");
}

void hal_comp_init(){
   for(int i = 0; i < hal.nrt_init_func_count; i++){ // run nrt init
      hal.nrt_init[i]();
   }
}

void hal_run_nrt(float period){
   //run all non realtime hal functions
   for(hal.active_nrt_func = 0; hal.active_nrt_func < hal.nrt_func_count; hal.active_nrt_func++){
      hal.nrt[hal.active_nrt_func](period);
   }
   hal.active_nrt_func = -1;
}

void hal_run_rt(float period){
   //run all realtime hal functions
   for(hal.active_rt_func = 0; hal.active_rt_func < hal.rt_func_count; hal.active_rt_func++){
      hal.rt[hal.active_rt_func](period);
   }
   hal.active_rt_func = -1;
}

void hal_run_frt(float period){
   //run all fast realtime hal functions
   for(hal.active_frt_func = 0; hal.active_frt_func < hal.frt_func_count; hal.active_frt_func++){
      hal.frt[hal.active_frt_func](period);
   }
   hal.active_frt_func = -1;
}

int hal_start_rt(){
   if(hal.rt_state == RT_STOP && hal.hal_state == HAL_OK){
      hal.active_rt_func = -1;
      hal.rt_state = RT_SLEEP;
      return(1);
   }
   return(0);
}

//TODO: crashes when hal is already running?
void hal_start(){
   float min = INFINITY;
   int min_index = -1;
   float rt_prio = 0.0;
   float frt_prio = 0.0;

   char added[MAX_COMPS];

   for(int i = 0; i < MAX_COMPS; i++){
      added[i] = 0;
   }
   // add rt func
   hal.rt_func_count = 0;
   for(int i = 0; i < hal.comp_count; i++){
      min = INFINITY;
      min_index = -1;
      for(int j = hal.comp_count - 1; j >= 0; j--){
         rt_prio = hal.hal_pins[hal.hal_comps[j]->hal_pin_start_index + 2]->source->source->value;
         if(rt_prio <= min && added[j] == 0 && rt_prio >= 0.0 && hal.hal_comps[j]->rt != 0){
            min = rt_prio;
            min_index = j;
         }
      }
      if(min_index >= 0){
         added[min_index] = 1;
         hal.rt[hal.rt_func_count++] = hal.hal_comps[min_index]->rt;
      }
   }

   for(int i = 0; i < hal.comp_count; i++){
      added[i] = 0;
   }
   // add frt func
   hal.frt_func_count = 0;
   for(int i = 0; i < hal.comp_count; i++){
      min = INFINITY;
      min_index = -1;
      for(int j = hal.comp_count - 1; j >= 0; j--){
         frt_prio = hal.hal_pins[hal.hal_comps[j]->hal_pin_start_index + 3]->source->source->value;
         if(frt_prio <= min && added[j] == 0 && frt_prio >= 0.0 && hal.hal_comps[j]->frt != 0){
            min = frt_prio;
            min_index = j;
         }
      }
      if(min_index >= 0){
         added[min_index] = 1;
         hal.frt[hal.frt_func_count++] = hal.hal_comps[min_index]->frt;
      }
   }
   // add (de)init func
   hal.rt_init_func_count = 0;
   hal.rt_deinit_func_count = 0;
   for(int i = 0; i < hal.comp_count; i++){
      rt_prio = hal.hal_pins[hal.hal_comps[i]->hal_pin_start_index + 2]->source->source->value;
      frt_prio = hal.hal_pins[hal.hal_comps[i]->hal_pin_start_index + 3]->source->source->value;

      if(rt_prio >= 0.0 || frt_prio >= 0.0){
         if(hal.hal_comps[i]->rt_init != 0){
            hal.rt_init[hal.rt_init_func_count++] = hal.hal_comps[i]->rt_init;
         }
         if(hal.hal_comps[i]->rt_deinit != 0){
            hal.rt_deinit[hal.rt_deinit_func_count++] = hal.hal_comps[i]->rt_deinit;
         }
      }
   }
   for(int i = 0; i < hal.rt_init_func_count; i++){
      hal.rt_init[i]();
   }

   hal_start_rt();

   if(hal.frt_func_count > 0){
      hal_start_frt();
      hal_enable_frt();
   }
   hal_enable_rt();
}

void hal_stop(){
   hal_stop_frt();
   hal_stop_rt();
   hal_disable_frt();
   hal_disable_rt();
   for(; hal.rt_deinit_func_count > 0; hal.rt_deinit_func_count--){
      hal.rt_deinit[hal.rt_deinit_func_count - 1]();
   }
}

int hal_start_frt(){
   if(hal.frt_state == FRT_STOP && hal.hal_state == HAL_OK){
      hal.active_frt_func = -1;
      hal.frt_state = FRT_SLEEP;
      return(1);
   }
   return(0);
}

void hal_stop_rt(){
   hal.active_rt_func = -1;
   hal.rt_state = RT_STOP;
   hal.active_rt_func = -1;
   hal.rt_state = RT_STOP;
}

void hal_stop_frt(){
   hal.active_frt_func = -1;
   hal.frt_state = FRT_STOP;
   hal.active_frt_func = -1;
   hal.frt_state = FRT_STOP;
}

//TODO: calculate and reduce name length...
void hal_init_pin(hal_name_t name, hal_pin_t* pin, float value){
  strncpy(pin->name, hal.comp_types[hal.comp_type], HAL_NAME_LENGTH);
  strncat(pin->name, hal_itoa(hal.comp_types_counter[hal.comp_type]), HAL_NAME_LENGTH);
  strncat(pin->name, ".", HAL_NAME_LENGTH);
  strncat(pin->name, name, HAL_NAME_LENGTH);
  pin->value = value;
  pin->source = pin;
  hal_register_pin(pin);
}

int hal_register_pin(hal_pin_t* pin){
  if(hal.hal_pin_count >= MAX_HAL_PINS){
    hal.pin_errors++;
    return(0);
  }

  for(int i = 0; i < hal.hal_pin_count; i++){
    if(!strcmp(hal.hal_pins[i]->name, pin->name)){
      hal.pin_errors++;
      return(0);
    }
  }

  hal.hal_pins[hal.hal_pin_count] = pin;
  hal.hal_pin_count++;

  return(1);
}

int hal_set_pin(hal_name_t name, float value){
  for(int i = 0; i < hal.hal_pin_count; i++){
    if(!strcmp(hal.hal_pins[i]->name, name)){
      hal.hal_pins[i]->value = value;
      hal.hal_pins[i]->source = hal.hal_pins[i];
      return(1);
    }
  }
  strcpy(hal.error_name,name);
  hal.set_errors++;
  return(0);
}

int hal_is_pin(hal_name_t name){
  for(int i = 0; i < hal.hal_pin_count; i++){
    if(!strcmp(hal.hal_pins[i]->name, name)){
      return(1);
    }
  }
  return(0);
}

int hal_is_compname(hal_name_t name){
  for(int i = 0; i < hal.hal_pin_count; i++){
    if(!strncmp(hal.hal_pins[i]->name, name, strlen(name))){
      return(1);
    }
  }
  return(0);
}

float hal_get_pin(hal_name_t name){
  for(int i = 0; i < hal.hal_pin_count; i++){
    if(!strcmp(hal.hal_pins[i]->name, name)){
      return(hal.hal_pins[i]->source->source->value);
    }
  }
  hal.get_errors++;
  return(0.0);
}

hal_pin_t hal_map_pin(hal_name_t name){
    for(int i = 0; i < hal.hal_pin_count; i++){
      if(!strcmp(hal.hal_pins[i]->name, name)){
		  return(*hal.hal_pins[i]);
      }
    }
    return(*hal.hal_pins[0]);
}

void hal_write_pin(hal_pin_t* pin, float value){
  pin->value = value;
  pin->source = pin;
}

float hal_read_pin(hal_pin_t* pin){
  return(pin->source->source->value);
}

hal_pin_t* hal_find_pin(hal_name_t name){
  for(int i = 0; i < hal.hal_pin_count; i++){
    if(!strcmp(hal.hal_pins[i]->name, name)){
      return(hal.hal_pins[i]);
    }
  }
  return(0);
}

int hal_link_pins(hal_name_t source, hal_name_t sink){
  hal_pin_t* d;
  hal_pin_t* s;
  d = hal_find_pin(source);
  s = hal_find_pin(sink);

  if(d != 0 && s != 0){
    s->value = s->source->source->value;
    s->source = d;
	return(1);
  }
  strcpy(hal.error_name,source);
  hal.link_errors++;
  return(0);
}

int hal_set_comp_type(hal_name_t name){
  for(int i = 0; i < hal.comp_type_count; i++){
    if(!strcmp(hal.comp_types[i], name)){
      hal.comp_type = i;
      return(hal.comp_types_counter[hal.comp_type]++);
    }
  }

  if(hal.comp_type_count < MAX_COMP_TYPES){
    strncpy(hal.comp_types[hal.comp_type_count], name, HAL_NAME_LENGTH);
    hal.comp_type = hal.comp_type_count++;
    return(0);
  }
  hal.comp_errors++;
  return(-1);
}

void hal_add_comp(hal_comp_t* comp){
   if(comp != 0 && hal.comp_count < MAX_COMPS){
      hal.hal_comps[hal.comp_count++] = comp;
      comp->instance = hal.comp_types_counter[hal.comp_type];
      if(comp->nrt_init != 0){
         hal.nrt_init[hal.nrt_init_func_count++] = comp->nrt_init;
      }
      if(comp->nrt != 0 && hal.nrt_func_count < MAX_COMPS){
         hal.nrt[hal.nrt_func_count++] = comp->nrt;
      }
   }
   else{
      hal.comp_errors++;
   }
}
