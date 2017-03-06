#include "hal2.h"
#include <string.h>
#include <stdio.h>

// hal_comp_inst_t comp_insts[HAL2_MAX_COMPS];
// hal_pin_inst_t pin_insts[HAL2_MAX_PINS];
// uint8_t ctxs[HAL2_MAX_CTX];

// uint32_t comp_inst_count;
// uint32_t pin_inst_count;
// uint32_t ctx_count;
volatile hal2_t hal2;

hal2_comp_t * comp_by_name(NAME2 name){
   for(int i = 0; i < comp_count; i++){
      if(!strncmp(name, comps[i]->name, sizeof(NAME2))){
         return(comps[i]);
      }
   }
   return(0);
}

volatile hal_comp_inst_t * comp_inst_by_name(NAME2 name, uint32_t instance){
   hal2_comp_t * comp = comp_by_name(name);
   if(comp){
      for(int i = 0; i < hal2.comp_inst_count; i++){
         if(hal2.comp_insts[i].comp == comp && instance == hal2.comp_insts[i].instance){
            return(&hal2.comp_insts[i]);
         }
      }
   }
   return(0);
}

pin2_t * pin_by_name(NAME2 comp_name, NAME2 pin_name){
   uint32_t offset = 0;
   for(int i = 0; i < comp_count; i++){
      if(!strncmp(comp_name, comps[i]->name, sizeof(NAME2))){
         for(int j = 0; j < comps[i]->pin_count; j++){
            if(!strncmp(pin_name, pins[j + offset], sizeof(NAME2))){
               return(&pins[j + offset]);
            }
         }
      }
   }
   return(0);
}

volatile hal_pin_inst_t * pin_inst_by_name(NAME2 comp_name, uint32_t instance, NAME2 pin_name){
   volatile hal_comp_inst_t * comp = comp_inst_by_name(comp_name, instance);
   if(comp){
      for(int i = 0; i < comp->comp->pin_count; i++){
         if(!strncmp(pin_name, comp->pins[i], sizeof(NAME2))){
            return(&comp->pin_insts[i]);
         }
      }
   }
   return(0);
}

pin2_t * pin_by_pin_inst(volatile hal_pin_inst_t * p){
   for(int i = 0; i < hal2.comp_inst_count; i++){
      for(int j = 0; j < hal2.comp_insts[i].comp->pin_count; j++){
         if(&(hal2.comp_insts[i].pin_insts[j]) == p){
            return(&hal2.comp_insts[i].pins[j]);
         }
      }
   }
   return(0);
}

volatile hal_comp_inst_t * comp_inst_by_pin_inst(volatile hal_pin_inst_t * p){
   for(int i = 0; i < hal2.comp_inst_count; i++){
      for(int j = 0; j < hal2.comp_insts[i].comp->pin_count; j++){
         if(&(hal2.comp_insts[i].pin_insts[j]) == p){
            return(&hal2.comp_insts[i]);
         }
      }
   }
   return(0);
}

uint32_t load_comp(hal2_comp_t * comp){
   if(!comp){
      printf("load_comp: null ptr\n");
      return(0);
   }
   if(hal2.comp_inst_count >= HAL2_MAX_COMPS - 1){
      printf("load_comp: not enough space to load comp: %s\n", comp->name);
      return(0);
   }
   if(hal2.pin_inst_count + comp->pin_count >= HAL2_MAX_PINS - 1){
      printf("load_comp: not enough space to load comp pins: %s\n", comp->name);
      return(0);
   }
   if(hal2.ctx_count + comp->ctx_size >= HAL2_MAX_CTX - 1){
      printf("load_comp: not enough space to load comp ctx: %s\n", comp->name);
      return(0);
   }
   
   // load comp
   hal2.comp_insts[hal2.comp_inst_count].comp = comp;
   hal2.comp_insts[hal2.comp_inst_count].ctx = &hal2.ctxs[hal2.ctx_count];
   hal2.comp_insts[hal2.comp_inst_count].pin_insts = &hal2.pin_insts[hal2.pin_inst_count];
   uint32_t offset = 0;
   for(int i = 0; i < comp_count; i++){
      if(comps[i] == comp){
         hal2.comp_insts[hal2.comp_inst_count].pins = &pins[offset];
      }
      offset += comps[i]->pin_count;
   }
   hal2.comp_insts[hal2.comp_inst_count].instance = 0;
   for(int i = 0; i < hal2.comp_inst_count; i++){
      if(hal2.comp_insts[i].comp == comp){
         hal2.comp_insts[hal2.comp_inst_count].instance++;
      }
   }
   hal2.comp_inst_count++;
   
   // load pins 
   for(int i = hal2.pin_inst_count; i <  hal2.pin_inst_count + comp->pin_count; i++){
      hal2.pin_insts[i].value = 0.0;
      hal2.pin_insts[i].source = &hal2.pin_insts[i];
   }
   hal2.pin_inst_count += comp->pin_count;
   
   // load ctx
   for(int i = hal2.ctx_count; i <  hal2.ctx_count + comp->ctx_size; i++){
      hal2.ctxs[i] = 0;
   }
   hal2.ctx_count += comp->ctx_size;
   
   return(1);
}

void run_rt(float period){
   for(hal2.active_rt_func = 0; hal2.active_rt_func < HAL2_MAX_COMPS; hal2.active_rt_func++){
      if(hal2.rt_comps[hal2.active_rt_func] != 0){
         hal2.rt_comps[hal2.active_rt_func]->comp->rt(period, hal2.rt_comps[hal2.active_rt_func]->ctx, hal2.rt_comps[hal2.active_rt_func]->pin_insts);
      }
      else{
         break;
      }
   }
   hal2.active_rt_func = -1;
}

void run_frt(float period){
   for(hal2.active_frt_func = 0; hal2.active_frt_func < HAL2_MAX_COMPS; hal2.active_frt_func++){
      if(hal2.frt_comps[hal2.active_frt_func] != 0){
         hal2.frt_comps[hal2.active_frt_func]->comp->frt(period, hal2.frt_comps[hal2.active_frt_func]->ctx, hal2.frt_comps[hal2.active_frt_func]->pin_insts);
      }
      else{
         break;
      }
   }
   hal2.active_frt_func = -1;
}

void run_nrt(float period){
   for(hal2.active_nrt_func = 0; hal2.active_nrt_func < hal2.comp_inst_count; hal2.active_nrt_func++){
      hal2.comp_insts[hal2.active_nrt_func].comp->nrt(period, hal2.comp_insts[hal2.active_nrt_func].ctx, hal2.comp_insts[hal2.active_nrt_func].pin_insts);
   }
   hal2.active_nrt_func = -1;
}

void start(){   
   for(int i = 0; i < HAL2_MAX_COMPS; i++){
      if(hal2.nrt_comps[i]){
         if(hal2.nrt_comps[i]->comp->nrt_init != 0){
            hal2.nrt_comps[i]->comp->nrt_init(hal2.nrt_comps[i]->ctx, hal2.nrt_comps[i]->pin_insts);
         }
      }
      else{
         break;
      }
   }
}

void start_rt(){
   // TODO prio sort
   for(int i = 0; i < HAL2_MAX_COMPS; i++){
      if(hal2.rt_comps[i]){
         if(hal2.rt_comps[i]->comp->rt_start != 0){
            hal2.rt_comps[i]->comp->rt_start(hal2.rt_comps[i]->ctx, hal2.rt_comps[i]->pin_insts);
         }
      }
      else{
         break;
      }
   }
   
   hal2.rt_state = RT_SLEEP;
}

void start_frt(){
   // TODO prio sort
   for(int i = 0; i < HAL2_MAX_COMPS; i++){
      if(hal2.frt_comps[i]){
         if(hal2.frt_comps[i]->comp->frt_start != 0){
            hal2.frt_comps[i]->comp->frt_start(hal2.frt_comps[i]->ctx, hal2.frt_comps[i]->pin_insts);
         }
      }
      else{
         break;
      }
   }
   
   hal2.frt_state = FRT_SLEEP;
}

void stop(){
   stop_rt();
   stop_frt();
}

void stop_rt(){
   hal2.rt_state = RT_STOP;
   
   for(int i = 0; i < HAL2_MAX_COMPS; i++){
      if(hal2.rt_comps[i]){
         if(hal2.rt_comps[i]->comp->rt_stop != 0){
            hal2.rt_comps[i]->comp->rt_stop(hal2.rt_comps[i]->ctx, hal2.rt_comps[i]->pin_insts);
         }
      }
      else{
         break;
      }
   }
}

void stop_frt(){
   hal2.frt_state = FRT_STOP;
   
   for(int i = 0; i < HAL2_MAX_COMPS; i++){
      if(hal2.frt_comps[i]){
         if(hal2.frt_comps[i]->comp->frt_stop != 0){
            hal2.frt_comps[i]->comp->frt_stop(hal2.frt_comps[i]->ctx, hal2.frt_comps[i]->pin_insts);
         }
      }
      else{
         break;
      }
   }
}

void hal2_init(){
   hal2.rt_state = RT_STOP;
   hal2.frt_state = FRT_STOP;
   
   hal2.hal_state = HAL2_OK;
   
   for(int i = 0; i < HAL2_MAX_COMPS; i++){
      hal2.rt_comps[i] = 0;
      hal2.frt_comps[i] = 0;
      hal2.nrt_comps[i] = 0;
   }
   
   hal2.comp_inst_count = 0;
   hal2.pin_inst_count = 0;
   
   for(int i = 0; i < HAL2_MAX_CTX; i++){
      hal2.ctxs[i] = 0;
   }
   hal2.ctx_count = 0;
   
   hal2.active_rt_func = 0;
   hal2.active_frt_func = 0;
   hal2.active_nrt_func = 0;
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
   
   foo = sscanf(cmd,"%[a-zA-Z]%i.%[a-zA-Z0-9]=%f", sinkc, &sinki, sinkp, &value);
   switch(foo){
      case 0:
      break;
      case 1: // search comps
         for(int i = 0; i < hal2.comp_inst_count; i++){
            if(!strncmp(hal2.comp_insts[i].comp->name, sinkc, strlen(sinkc))){
               printf("%s%i\n", hal2.comp_insts[i].comp->name, hal2.comp_insts[i].instance);
               found = 1;
            }
         }
         if(!found){
            printf("not found: %s\n", cmd);
         }
         break;
      case 2: // search comps + instance
         for(int i = 0; i < hal2.comp_inst_count; i++){
            if(hal2.comp_insts[i].instance == sinki && !strcmp(hal2.comp_insts[i].comp->name, sinkc)){
               for(int j = 0; j < hal2.comp_insts[i].comp->pin_count; j++){
                  volatile hal_comp_inst_t * comp = comp_inst_by_pin_inst(hal2.comp_insts[i].pin_insts[j].source->source);
                  printf("%s%i.%s <= %s%i.%s = %f\n", hal2.comp_insts[i].comp->name, hal2.comp_insts[i].instance, hal2.comp_insts[i].pins[j], comp->comp->name, comp->instance, pin_by_pin_inst(hal2.comp_insts[i].pin_insts[j].source->source), hal2.comp_insts[i].pin_insts[j].source->source->value);
                  found = 1;
               }
            }
         }
         if(!found){
            printf("not found: %s\n", cmd);
         }
         break;
      case 3: 
         foo = sscanf(cmd,"%[a-zA-Z]%i.%[a-zA-Z0-9]=%[a-zA-Z]%i.%[a-zA-Z0-9]", sinkc, &sinki, sinkp, sourcec, &sourcei, sourcep);
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
            for(int i = 0; i < hal2.comp_inst_count; i++){
               if(hal2.comp_insts[i].instance == sinki && !strcmp(hal2.comp_insts[i].comp->name, sinkc)){
                  for(int j = 0; j < hal2.comp_insts[i].comp->pin_count; j++){
                     volatile hal_comp_inst_t * comp = comp_inst_by_pin_inst(hal2.comp_insts[i].pin_insts[j].source->source);
                     if(!strncmp(hal2.comp_insts[i].pins[j], sinkp, strlen(sinkp))){
                        printf("%s%i.%s <= %s%i.%s = %f\n", hal2.comp_insts[i].comp->name, hal2.comp_insts[i].instance, hal2.comp_insts[i].pins[j], comp->comp->name, comp->instance, pin_by_pin_inst(hal2.comp_insts[i].pin_insts[j].source->source), hal2.comp_insts[i].pin_insts[j].source->source->value);
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
