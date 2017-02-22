#include <stdio.h>
#include "hal_term.h"

void hal_term_print_pin(hal_pin_t* pin){
   if(pin == pin->source){//if pin is not linked
      printf("%s = %f\n", pin->name, pin->source->source->value);
   }else{//pin is linked
      printf("%s <= %s = %f\n", pin->name, pin->source->name, pin->source->source->value);
   }
}

void hal_term_list(){
   for(int i = 0; i < hal.hal_pin_count; i++){
      printf("%s <= %s = %f\n", hal.hal_pins[i]->name, hal.hal_pins[i]->source->name, hal.hal_pins[i]->source->source->value);
      Wait(1);//TODO: remove wait...
   }
}

void hal_term_getconf(){
   for(int i = 0; i < hal.hal_pin_count; i++){
      char name[6];
      strncpy(name,hal.hal_pins[i]->name,5);
      name[5] =  '\0';
      if(!strcmp(name, "conf0")){
         printf("%s = %f\n", hal.hal_pins[i]->name, hal.hal_pins[i]->value);
      }
   }
}

void hal_term_print_state(){
   switch(hal.hal_state){
      case HAL2_OK:
      printf("HAL state:  HAL2_OK\n");
      break;
      case RT_TOO_LONG:
      printf("HAL state:  RT_TOO_LONG\n");
      break;
      case FRT_TOO_LONG:
      printf("HAL state:  FRT_TOO_LONG\n");
      break;
      case MISC_ERROR:
      printf("HAL state:  MISC_ERROR\n");
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

void hal_term_print_info(){
   printf("######## hal info ########\n");
   printf("#pins %i\n", hal.hal_pin_count);
   printf("#comps %i\n", hal.comp_count);
   printf("link errors %lu\n", hal.link_errors);
   printf("pin errors %lu\n", hal.pin_errors);
   printf("comp errors %lu\n", hal.comp_errors);
   printf("set errors %lu\n", hal.set_errors);
   printf("get errors %lu\n", hal.get_errors);
   printf("foo0.bar:  %f\n", hal_get_pin("foo0.bar"));
   printf("error_name: %s\n",hal.error_name);
   float pe = hal_get_pin("net0.rt_period");
   float ct = hal_get_pin("net0.rt_calc_time");
   if(pe > 0.0){
      printf("rt time: %fus/%fus = %f%% @ %fkHz\n", ct * 1000000.0, pe * 1000000.0, (ct / pe) * 100.0, 1.0 / pe / 1000.0);
      //printf("=%f%%\n",(ct/pe)*100);
   }
   pe = hal_get_pin("net0.frt_period");
   ct = hal_get_pin("net0.frt_calc_time");
   if(pe > 0.0){
      printf("frt time: %fus/%fus = %f%% @ %fkHz\n", ct * 1000000.0, pe * 1000000.0, (ct / pe) * 100.0, 1.0 / pe / 1000.0);
      //printf("=%f%%\n",(ct/pe)*100);
   }
   pe = hal_get_pin("net0.nrt_period");
   ct = hal_get_pin("net0.nrt_calc_time");
   if(pe > 0.0){
      printf("nrt time: %f/%fs", ct, pe);
      printf("=%f%%\n",(ct/pe)*100);
   }
   switch(hal.rt_state){
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
   switch(hal.frt_state){
      case FRT_STOP:
      printf("frt state:  STOP\n");
      break;
      case FRT_SLEEP:
      printf("frt state:  SLEEP\n");
      break;
      case FRT_CALC:
      printf("frt state:  CALC\n");
      break;
   }
   hal_term_print_state();
   uint32_t p = 0;
   int32_t i = -1;
   char str[HAL_NAME_LENGTH];
   printf("active rt funcs(%u):\n", hal.rt_func_count);
   pe = hal_get_pin("net0.rt_period");
   for(int j = 0; j < hal.rt_func_count; j++){
      p = (uint32_t)hal.rt[j];
      i = find_comp_by_func(p);
      if(i >= 0){
         sprintf(str, "%s%lu.rt_calc_time", hal.hal_comps[i]->name, hal.hal_comps[i]->instance);
         ct = hal_get_pin(str);
         printf("-%s%lu.rt(%f) %fus", hal.hal_comps[i]->name, hal.hal_comps[i]->instance,  hal.hal_pins[hal.hal_comps[i]->hal_pin_start_index + 2]->source->source->value, ct * 1000000.0f);
         if(ct > 0.0){
            printf(" = %f%%", (ct / pe) * 100.0);
         }
         printf("\n");
      }
   }
   printf("\nactive frt funcs(%u):\n", hal.frt_func_count);
   pe = hal_get_pin("net0.frt_period");
   for(int j = 0; j < hal.frt_func_count; j++){
      p = (uint32_t)hal.frt[j];
      i = find_comp_by_func(p);
      if(i >= 0){
         ct = hal_get_pin(str);
         printf("-%s%lu.frt(%f) %fus", hal.hal_comps[i]->name, hal.hal_comps[i]->instance,  hal.hal_pins[hal.hal_comps[i]->hal_pin_start_index + 3]->source->source->value, ct * 1000000.0f);
         if(ct > 0.0){
            printf(" = %f%%", (ct / pe) * 100.0);
         }
         printf("\n");
      }
   }
   printf("\nactive rt_init funcs(%u):\n", hal.rt_init_func_count);
   for(int j = 0; j < hal.rt_init_func_count; j++){
      p = (uint32_t)hal.rt_init[j];
      i = find_comp_by_func(p);
      if(i >= 0){
         printf("-%s%lu.rt_init\n", hal.hal_comps[i]->name, hal.hal_comps[i]->instance);
      }
   }
   printf("\nactive rt_deinit funcs(%u):\n", hal.rt_deinit_func_count);
   for(int j = 0; j < hal.rt_deinit_func_count; j++){
      p = (uint32_t)hal.rt_deinit[j];
      i = find_comp_by_func(p);
      if(i >= 0){
         printf("-%s%lu.rt_deinit\n", hal.hal_comps[i]->name, hal.hal_comps[i]->instance);
      }
   }
   printf("\nactive nrt_init funcs(%u):\n", hal.nrt_init_func_count);
   for(int j = 0; j < hal.nrt_init_func_count; j++){
      p = (uint32_t)hal.nrt_init[j];
      i = find_comp_by_func(p);
      if(i >= 0){
         printf("-%s%lu.nrt_init\n", hal.hal_comps[i]->name, hal.hal_comps[i]->instance);
      }
   }
   printf("\nactive nrt funcs(%u):\n", hal.nrt_func_count);
   for(int j = 0; j < hal.nrt_func_count; j++){
      p = (uint32_t)hal.nrt[j];
      i = find_comp_by_func(p);
      if(i >= 0){
         printf("-%s%lu.nrt\n", hal.hal_comps[i]->name, hal.hal_comps[i]->instance);
      }
   }
   printf("\n");
}
