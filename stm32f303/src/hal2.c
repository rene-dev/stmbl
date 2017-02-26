#include "hal2.h"
#include <string.h>
#include <stdio.h>

hal_comp_inst_t comp_insts[HAL2_MAX_COMPS];
hal_pin_inst_t pin_insts[HAL2_MAX_PINS];
uint8_t ctxs[HAL2_MAX_CTX];

uint32_t comp_inst_count;
uint32_t pin_inst_count;
uint32_t ctx_count;

hal2_comp_t * comp_by_name(NAME2 name){
   for(int i = 0; i < comp_count; i++){
      if(!strncmp(name, comps[i]->name, sizeof(NAME2))){
         return(comps[i]);
      }
   }
   return(0);
}

hal_comp_inst_t * comp_inst_by_name(NAME2 name, uint32_t instance){
   hal2_comp_t * comp = comp_by_name(name);
   if(comp){
      for(int i = 0; i < comp_inst_count; i++){
         if(comp_insts[i].comp == comp && instance == comp_insts[i].instance){
            return(&comp_insts[i]);
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

hal_pin_inst_t * pin_inst_by_name(NAME2 comp_name, uint32_t instance, NAME2 pin_name){
   hal_comp_inst_t * comp = comp_inst_by_name(comp_name, instance);
   if(comp){
      for(int i = 0; i < comp->comp->pin_count; i++){
         if(!strncmp(pin_name, comp->pins[i], sizeof(NAME2))){
            return(&comp->pin_insts[i]);
         }
      }
   }
   return(0);
}

pin2_t * pin_by_pin_inst(hal_pin_inst_t * p){
   for(int i = 0; i < comp_inst_count; i++){
      for(int j = 0; j < comp_insts[i].comp->pin_count; j++){
         if(&(comp_insts[i].pin_insts[j]) == p){
            return(&comp_insts[i].pins[j]);
         }
      }
   }
   return(0);
}

hal_comp_inst_t * comp_inst_by_pin_inst(hal_pin_inst_t * p){
   for(int i = 0; i < comp_inst_count; i++){
      for(int j = 0; j < comp_insts[i].comp->pin_count; j++){
         if(&(comp_insts[i].pin_insts[j]) == p){
            return(&comp_insts[i]);
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
   if(comp_inst_count >= HAL2_MAX_COMPS - 1){
      printf("load_comp: not enough space to load comp: %s\n", comp->name);
      return(0);
   }
   if(pin_inst_count + comp->pin_count >= HAL2_MAX_PINS - 1){
      printf("load_comp: not enough space to load comp pins: %s\n", comp->name);
      return(0);
   }
   if(ctx_count + comp->ctx_size >= HAL2_MAX_CTX - 1){
      printf("load_comp: not enough space to load comp ctx: %s\n", comp->name);
      return(0);
   }
   
   // load comp
   comp_insts[comp_inst_count].comp = comp;
   comp_insts[comp_inst_count].ctx = &ctxs[ctx_count];
   comp_insts[comp_inst_count].pin_insts = &pin_insts[pin_inst_count];
   uint32_t offset = 0;
   for(int i = 0; i < comp_count; i++){
      if(comps[i] == comp){
         comp_insts[comp_inst_count].pins = &pins[offset];
      }
      offset += comps[i]->pin_count;
   }
   comp_insts[comp_inst_count].instance = 0;
   for(int i = 0; i < comp_inst_count; i++){
      if(comp_insts[i].comp == comp){
         comp_insts[comp_inst_count].instance++;
      }
   }
   comp_inst_count++;
   
   // load pins 
   // TODO calc_time, prio, ...
   for(int i = pin_inst_count; i <  pin_inst_count + comp->pin_count; i++){
      pin_insts[i].value = 0.0;
      pin_insts[i].source = &pin_insts[i];
   }
   pin_inst_count += comp->pin_count;
   
   // load ctx
   for(int i = ctx_count; i <  ctx_count + comp->ctx_size; i++){
      ctxs[i] = 0;
   }
   ctx_count += comp->ctx_size;
   
   return(1);
}
