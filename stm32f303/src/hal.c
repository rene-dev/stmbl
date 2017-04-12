#include "hal.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

// hal_comp_inst_t comp_insts[HAL_MAX_COMPS];
// hal_pin_inst_t pin_insts[HAL_MAX_PINS];
// uint8_t ctxs[HAL_MAX_CTX];

// uint32_t comp_inst_count;
// uint32_t pin_inst_count;
// uint32_t ctx_count;
volatile hal_t hal;

hal_comp_t * comp_by_name(NAME name){
   for(int i = 0; i < comp_count; i++){
      if(!strncmp(name, comps[i]->name, sizeof(NAME))){
         return(comps[i]);
      }
   }
   return(0);
}

volatile hal_comp_inst_t * comp_inst_by_name(NAME name, uint32_t instance){
   hal_comp_t * comp = comp_by_name(name);
   if(comp){
      for(int i = 0; i < hal.comp_inst_count; i++){
         if(hal.comp_insts[i].comp == comp && instance == hal.comp_insts[i].instance){
            return(&hal.comp_insts[i]);
         }
      }
   }
   return(0);
}

pin_t * pin_by_name(NAME comp_name, NAME pin_name){
   uint32_t offset = 0;
   for(int i = 0; i < comp_count; i++){
      if(!strncmp(comp_name, comps[i]->name, sizeof(NAME))){
         for(int j = 0; j < comps[i]->pin_count; j++){
            if(!strncmp(pin_name, pins[j + offset], sizeof(NAME))){
               return(&pins[j + offset]);
            }
         }
      }
   }
   return(0);
}

volatile hal_pin_inst_t * pin_inst_by_name(NAME comp_name, uint32_t instance, NAME pin_name){
   volatile hal_comp_inst_t * comp = comp_inst_by_name(comp_name, instance);
   if(comp){
      for(int i = 0; i < comp->comp->pin_count; i++){
         if(!strncmp(pin_name, comp->pins[i], sizeof(NAME))){
            return(&comp->pin_insts[i]);
         }
      }
   }
   return(0);
}

pin_t * pin_by_pin_inst(volatile hal_pin_inst_t * p){
   for(int i = 0; i < hal.comp_inst_count; i++){
      for(int j = 0; j < hal.comp_insts[i].comp->pin_count; j++){
         if(&(hal.comp_insts[i].pin_insts[j]) == p){
            return(&hal.comp_insts[i].pins[j]);
         }
      }
   }
   return(0);
}

volatile hal_comp_inst_t * comp_inst_by_pin_inst(volatile hal_pin_inst_t * p){
   for(int i = 0; i < hal.comp_inst_count; i++){
      for(int j = 0; j < hal.comp_insts[i].comp->pin_count; j++){
         if(&(hal.comp_insts[i].pin_insts[j]) == p){
            return(&hal.comp_insts[i]);
         }
      }
   }
   return(0);
}

uint32_t load_comp(hal_comp_t * comp){
   if(!comp){
      printf("load_comp: null ptr\n");
      return(0);
   }
   if(hal.comp_inst_count >= HAL_MAX_COMPS - 1){
      printf("load_comp: not enough space to load comp: %s\n", comp->name);
      return(0);
   }
   if(hal.pin_inst_count + comp->pin_count >= HAL_MAX_PINS - 1){
      printf("load_comp: not enough space to load comp pins: %s\n", comp->name);
      return(0);
   }
   if(hal.ctx_count + comp->ctx_size >= HAL_MAX_CTX - 1){
      printf("load_comp: not enough space to load comp ctx: %s\n", comp->name);
      return(0);
   }
   
   // load comp
   hal.comp_insts[hal.comp_inst_count].comp = comp;
   hal.comp_insts[hal.comp_inst_count].ctx = &hal.ctxs[hal.ctx_count];
   hal.comp_insts[hal.comp_inst_count].pin_insts = &hal.pin_insts[hal.pin_inst_count];
   uint32_t offset = 0;
   for(int i = 0; i < comp_count; i++){
      if(comps[i] == comp){
         hal.comp_insts[hal.comp_inst_count].pins = &pins[offset];
      }
      offset += comps[i]->pin_count;
   }
   hal.comp_insts[hal.comp_inst_count].instance = 0;
   for(int i = 0; i < hal.comp_inst_count; i++){
      if(hal.comp_insts[i].comp == comp){
         hal.comp_insts[hal.comp_inst_count].instance++;
      }
   }
   hal.comp_inst_count++;
   
   // load pins 
   for(int i = hal.pin_inst_count; i <  hal.pin_inst_count + comp->pin_count; i++){
      hal.pin_insts[i].value = 0.0;
      hal.pin_insts[i].source = &hal.pin_insts[i];
   }
   hal.pin_inst_count += comp->pin_count;
   
   // load ctx
   for(int i = hal.ctx_count; i <  hal.ctx_count + comp->ctx_size; i++){
      hal.ctxs[i] = 0;
   }
   hal.ctx_count += comp->ctx_size;
   
   return(1);
}

void hal_run_rt(){
   static unsigned int last_start = 0;
   unsigned int start = hal_get_systick_value();
   
   if(last_start < start){
     last_start += hal_get_systick_reload();
   }
   
   float period = ((float)(last_start - start)) / hal_get_systick_freq();
   last_start = start;
   
   switch(hal.rt_state){
      case RT_STOP:
         return;
      case RT_CALC: // call stop
         hal.rt_state = RT_STOP;
         hal.hal_state = RT_TOO_LONG;
         hal.frt_state = FRT_STOP;
         return;
      case RT_SLEEP:
         if(hal.active_rt_func > -1){ // call stop
            hal.rt_state = RT_STOP;
            hal.hal_state = MISC_ERROR;
            hal.frt_state = FRT_STOP;
            return;
         }
         hal.rt_state = RT_CALC;
   }
   
   for(hal.active_rt_func = 0; hal.active_rt_func < HAL_MAX_COMPS; hal.active_rt_func++){
      if(hal.rt_comps[hal.active_rt_func] != 0){
         hal.rt_comps[hal.active_rt_func]->comp->rt(period, hal.rt_comps[hal.active_rt_func]->ctx, hal.rt_comps[hal.active_rt_func]->pin_insts);
      }
      else{
         break;
      }
   }
   hal.active_rt_func = -1;
   
   if(hal.rt_state == RT_CALC){
      hal.rt_state = RT_SLEEP;
   }
   
   unsigned int end = hal_get_systick_value();
   if(start < end){
     start += hal_get_systick_reload();
   }
   
   hal.rt_calc_time = ((float)(start - end)) / hal_get_systick_freq();
   hal.rt_period = period;
}

void hal_run_frt(){
   static unsigned int last_start = 0;
   unsigned int start = hal_get_systick_value();
   
   if(last_start < start){
     last_start += hal_get_systick_reload();
   }
   
   float period = ((float)(last_start - start)) / hal_get_systick_freq();
   last_start = start;
   
   switch(hal.frt_state){
      case FRT_STOP:
         return;
      case FRT_CALC:
         hal.rt_state = RT_STOP;
         hal.hal_state = FRT_TOO_LONG;
         hal.frt_state = FRT_STOP;
         return;
      case FRT_SLEEP:
         if(hal.active_frt_func > -1){
            hal.rt_state = RT_STOP;
            hal.hal_state = MISC_ERROR;
            hal.frt_state = FRT_STOP;
            return;
         }
         hal.frt_state = FRT_CALC;
   }
   
   for(hal.active_frt_func = 0; hal.active_frt_func < HAL_MAX_COMPS; hal.active_frt_func++){
      if(hal.frt_comps[hal.active_frt_func] != 0){
         hal.frt_comps[hal.active_frt_func]->comp->frt(period, hal.frt_comps[hal.active_frt_func]->ctx, hal.frt_comps[hal.active_frt_func]->pin_insts);
      }
      else{
         break;
      }
   }
   hal.active_frt_func = -1;
   
   if(hal.frt_state == FRT_CALC){
      hal.frt_state = FRT_SLEEP;
   }
   
   unsigned int end = hal_get_systick_value();
   hal.frt_calc_time = ((float)(start - end)) / hal_get_systick_freq();
   hal.frt_period = period;
}

void hal_run_nrt(){
   static unsigned int last_start = 0;
   unsigned int start = hal_get_systick_value();
   
   if(last_start < start){
     last_start += hal_get_systick_reload();
   }
   
   float period = ((float)(last_start - start)) / hal_get_systick_freq();
   last_start = start;
   
   for(hal.active_nrt_func = 0; hal.active_nrt_func < hal.comp_inst_count; hal.active_nrt_func++){
      if(hal.comp_insts[hal.active_nrt_func].comp->nrt != 0){
         hal.comp_insts[hal.active_nrt_func].comp->nrt(period, hal.comp_insts[hal.active_nrt_func].ctx, hal.comp_insts[hal.active_nrt_func].pin_insts);
      }
   }
   hal.active_nrt_func = -1;
   
   unsigned int end = hal_get_systick_value();
   hal.nrt_calc_time = ((float)(start - end)) / hal_get_systick_freq();
   hal.nrt_period = period;
}

void hal_init_nrt(){
   for(int i = 0; i < hal.comp_inst_count; i++){
      if(hal.comp_insts[i].comp->nrt_init != 0){
         hal.comp_insts[i].comp->nrt_init(hal.comp_insts[i].ctx, hal.comp_insts[i].pin_insts);
      }
      else{
         break;
      }
   }
}

void sort_rt(){
   float min = INFINITY;
   int min_index = -1;
   float rt_prio = 0.0;
   char added[HAL_MAX_COMPS];

   for(int i = 0; i < hal.comp_inst_count; i++){
      added[i] = 0;
   }

   hal.rt_comp_count = 0;
   for(int i = 0; i < hal.comp_inst_count; i++){
      min = INFINITY;
      min_index = -1;
      for(int j = hal.comp_inst_count - 1; j >= 0; j--){
         rt_prio = hal.comp_insts[j].pin_insts[hal.comp_insts[j].comp->pin_count - 5].source->source->value;
         if(rt_prio <= min && added[j] == 0 && rt_prio >= 0.0 && hal.comp_insts[j].comp->rt != 0){
            min = rt_prio;
            min_index = j;
         }
      }
      if(min_index >= 0){
         added[min_index] = 1;
         hal.rt_comps[hal.rt_comp_count++] = &hal.comp_insts[min_index];
      }
   }
}

void sort_frt(){
   float min = INFINITY;
   int min_index = -1;
   float frt_prio = 0.0;
   char added[HAL_MAX_COMPS];

   for(int i = 0; i < hal.comp_inst_count; i++){
      added[i] = 0;
   }

   hal.frt_comp_count = 0;
   for(int i = 0; i < hal.comp_inst_count; i++){
      min = INFINITY;
      min_index = -1;
      for(int j = hal.comp_inst_count - 1; j >= 0; j--){
         frt_prio = hal.comp_insts[j].pin_insts[hal.comp_insts[j].comp->pin_count - 4].source->source->value;
         if(frt_prio <= min && added[j] == 0 && frt_prio >= 0.0 && hal.comp_insts[j].comp->frt != 0){
            min = frt_prio;
            min_index = j;
         }
      }
      if(min_index >= 0){
         added[min_index] = 1;
         hal.frt_comps[hal.frt_comp_count++] = &hal.comp_insts[min_index];
      }
   }
}

void start_rt(){   
   for(int i = 0; i < hal.rt_comp_count; i++){
      if(hal.rt_comps[i]->comp->rt_start != 0){
         hal.rt_comps[i]->comp->rt_start(hal.rt_comps[i]->ctx, hal.rt_comps[i]->pin_insts);
      }
   }
   
   hal.rt_state = RT_SLEEP;
}

void start_frt(){
   for(int i = 0; i < HAL_MAX_COMPS; i++){
      if(hal.frt_comps[i]){
         if(hal.frt_comps[i]->comp->frt_start != 0){
            hal.frt_comps[i]->comp->frt_start(hal.frt_comps[i]->ctx, hal.frt_comps[i]->pin_insts);
         }
      }
      else{
         break;
      }
   }
   
   hal.frt_state = FRT_SLEEP;
}

void hal_start(){
   sort_rt();
   sort_frt();
   start_rt();
   start_frt();
}

void stop_rt(){
   hal.rt_state = RT_STOP;
   
   for(int i = 0; i < hal.rt_comp_count; i++){
      if(hal.rt_comps[i]->comp->rt_stop != 0){
         hal.rt_comps[i]->comp->rt_stop(hal.rt_comps[i]->ctx, hal.rt_comps[i]->pin_insts);
      }
   }
}

void stop_frt(){
   hal.frt_state = FRT_STOP;
   
   for(int i = 0; i < hal.frt_comp_count; i++){
      if(hal.frt_comps[i]->comp->frt_stop != 0){
         hal.frt_comps[i]->comp->frt_stop(hal.frt_comps[i]->ctx, hal.frt_comps[i]->pin_insts);
      }
   }
}

void hal_stop(){
   stop_rt();
   stop_frt();
}

void hal_init(){
   hal.rt_state = RT_STOP;
   hal.frt_state = FRT_STOP;
   
   hal.hal_state = HAL_OK2;
   
   for(int i = 0; i < HAL_MAX_COMPS; i++){
      hal.rt_comps[i] = 0;
      hal.frt_comps[i] = 0;
   }
   
   hal.comp_inst_count = 0;
   hal.rt_comp_count = 0;
   hal.frt_comp_count = 0;
   hal.pin_inst_count = 0;
   
   for(int i = 0; i < HAL_MAX_CTX; i++){
      hal.ctxs[i] = 0;
   }
   hal.ctx_count = 0;
   
   hal.active_rt_func = -1;
   hal.active_frt_func = -1;
   hal.active_nrt_func = -1;
}

uint32_t hal_parse(char * cmd){
   int32_t foo = 0;
   
   char sinkc[64];
   uint32_t sinki = 0;
   char sinkp[64];
   
   float value = 0.0;
   
   char sourcec[64];
   uint32_t sourcei = 0;
   char sourcep[64];
   
   volatile hal_pin_inst_t * sink;
   volatile hal_pin_inst_t * source;
   
   uint32_t found = 0;
   
   foo = sscanf(cmd," %[a-zA-Z_]%i.%[a-zA-Z0-9_] = %f", sinkc, &sinki, sinkp, &value);
   switch(foo){
      case 0:
      break;
      case 1: // search comps
         for(int i = 0; i < hal.comp_inst_count; i++){
            if(!strncmp(hal.comp_insts[i].comp->name, sinkc, strlen(sinkc))){
               printf("%s%i\n", hal.comp_insts[i].comp->name, hal.comp_insts[i].instance);
               found = 1;
            }
         }
         if(!found){
            printf("not found: %s\n", cmd);
         }
         break;
      case 2: // search comps + instance
         for(int i = 0; i < hal.comp_inst_count; i++){
            if(hal.comp_insts[i].instance == sinki && !strcmp(hal.comp_insts[i].comp->name, sinkc)){
               for(int j = 0; j < hal.comp_insts[i].comp->pin_count; j++){
                  volatile hal_comp_inst_t * comp = comp_inst_by_pin_inst(hal.comp_insts[i].pin_insts[j].source->source);
                  printf("%s%i.%s <= %s%i.%s = %f\n", hal.comp_insts[i].comp->name, hal.comp_insts[i].instance, hal.comp_insts[i].pins[j], comp->comp->name, comp->instance, pin_by_pin_inst(hal.comp_insts[i].pin_insts[j].source->source), hal.comp_insts[i].pin_insts[j].source->source->value);
                  found = 1;
               }
            }
         }
         if(!found){
            printf("not found: %s\n", cmd);
         }
         break;
      case 3: 
         foo = sscanf(cmd," %[a-zA-Z_]%i.%[a-zA-Z0-9_] = %[a-zA-Z_]%i.%[a-zA-Z0-9_]", sinkc, &sinki, sinkp, sourcec, &sourcei, sourcep);
         if(foo == 6){ // link pins
            sink = pin_inst_by_name(sinkc, sinki, sinkp);
            source = pin_inst_by_name(sourcec, sourcei, sourcep);
            if(sink && source){
               sink->source = source->source;
               printf("OK %s%i.%s <= %s%i.%s = %f\n", sinkc, sinki, sinkp, sourcec, sourcei, sourcep, source->source->value);

            }
            else if(sink){
               printf("not found: %s%i.%s\n", sourcec, sourcei, sourcep);
            }
            else{
               printf("not found: %s%i.%s\n", sinkc, sinki, sinkp);
            }
         }
         else{ // search comps + instance + pin
            for(int i = 0; i < hal.comp_inst_count; i++){
               if(hal.comp_insts[i].instance == sinki && !strcmp(hal.comp_insts[i].comp->name, sinkc)){
                  for(int j = 0; j < hal.comp_insts[i].comp->pin_count; j++){
                     volatile hal_comp_inst_t * comp = comp_inst_by_pin_inst(hal.comp_insts[i].pin_insts[j].source->source);
                     if(!strncmp(hal.comp_insts[i].pins[j], sinkp, strlen(sinkp))){
                        printf("%s%i.%s <= %s%i.%s = %f\n", hal.comp_insts[i].comp->name, hal.comp_insts[i].instance, hal.comp_insts[i].pins[j], comp->comp->name, comp->instance, pin_by_pin_inst(hal.comp_insts[i].pin_insts[j].source->source), hal.comp_insts[i].pin_insts[j].source->source->value);
                        found = 1;
                     }
                  }
               }
            }
            if(!found){
               printf("not found: %s\n", cmd);
            }
         }
         break;
      case 4: // set pin
         sink = pin_inst_by_name(sinkc, sinki, sinkp);
         if(sink){
            sink->value = value;
            sink->source = sink;
            printf("OK %s%i.%s = %f\n", sinkc, sinki, sinkp, value);
         }
         else{
            printf("not found: %s%i.%s\n", sinkc, sinki, sinkp);
         }
         break;
      default:
         printf("not found: %s\n", cmd);
   }
   return(0);
}
