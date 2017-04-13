#include "stm32f3xx_hal.h"
#include "commands.h"
#include "commandslist.h"
#include "hal.h"
#include "defines.h"
#include <stdio.h>
#include <string.h>
#include "usbd_cdc_if.h"

#define TERM_NUM_WAVES 8
#define TERM_BUF_SIZE 8

HAL_COMP(term);

HAL_PINA(wave, 8);
HAL_PINA(offset, 8);
HAL_PINA(gain, 8);
HAL_PIN(send_step);
HAL_PIN(con);

struct term_ctx_t{
   float wave_buf[TERM_BUF_SIZE][TERM_NUM_WAVES];
   uint32_t send_counter;
   uint32_t write_pos;
   uint32_t read_pos;
};

void load(char * ptr){
   printf("load :%s:\n", ptr);
   load_comp(comp_by_name(ptr));
}

COMMAND("load", load);

void show(char * ptr){
   int j = 0;
   int pin_offset = 0;
   for(int i = 0; i < comp_count; i++){
      printf("%s\n", comps[i]->name);
      for(; j < pin_offset + comps[i]->pin_count; j++){
         printf("- %s\n", pins[j]);
      }
      pin_offset += comps[i]->pin_count;
   }
}
COMMAND("show", show);

void list(char * ptr){
   for(int i = 0; i < hal.comp_inst_count; i++){
      printf("%s%lu:\n", hal.comp_insts[i].comp->name, hal.comp_insts[i].instance);
      for(int j = 0; j < hal.comp_insts[i].comp->pin_count; j++){
         volatile hal_comp_inst_t * comp = comp_inst_by_pin_inst(hal.comp_insts[i].pin_insts[j].source->source);
         printf("-  %s <= %s%lu.%s = %f\n", hal.comp_insts[i].pins[j], comp->comp->name, comp->instance, (char *)pin_by_pin_inst(hal.comp_insts[i].pin_insts[j].source->source), hal.comp_insts[i].pin_insts[j].source->source->value);
      }
   }
}
COMMAND("list", list);

void bootloader(char * ptr){
   RTC->BKP0R = 0xDEADBEEF;
   NVIC_SystemReset();
}

COMMAND("bootloader", bootloader);

void hal_term_print_state(){
   switch(hal.hal_state){
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

void hal_term_print_info(char * ptr){
   printf("######## hal info ########\n");
   printf("#pins %lu\n", hal.pin_inst_count);
   printf("#comps %lu\n", hal.comp_inst_count);
   // printf("link errors %lu\n", hal.link_errors);
   // printf("pin errors %lu\n", hal.pin_errors);
   // printf("comp errors %lu\n", hal.comp_errors);
   // printf("set errors %lu\n", hal.set_errors);
   // printf("get errors %lu\n", hal.get_errors);
   // printf("foo0.bar:  %f\n", hal_get_pin("foo0.bar"));
   // printf("error_name: %s\n",hal.error_name);
   float pe = hal.rt_period;
   float ct = hal.rt_calc_time;
   if(pe > 0.0){
      printf("rt time: %fus/%fus = %f%% @ %fkHz\n", ct * 1000000.0, pe * 1000000.0, (ct / pe) * 100.0, 1.0 / pe / 1000.0);
      //printf("=%f%%\n",(ct/pe)*100);
   }
   pe = hal.frt_period;
   ct = hal.rt_calc_time;
   if(pe > 0.0){
      printf("frt time: %fus/%fus = %f%% @ %fkHz\n", ct * 1000000.0, pe * 1000000.0, (ct / pe) * 100.0, 1.0 / pe / 1000.0);
      //printf("=%f%%\n",(ct/pe)*100);
   }
   pe = hal.nrt_period;
   ct = hal.nrt_calc_time;
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
   
   printf("active rt funcs(%lu):\n", hal.rt_comp_count);
   for(int j = 0; j < hal.rt_comp_count; j++){
      printf("%s\n", hal.rt_comps[j]->comp->name);
   }
   
   printf("active frt funcs(%lu):\n", hal.frt_comp_count);
   for(int j = 0; j < hal.frt_comp_count; j++){
      printf("%s\n", hal.frt_comps[j]->comp->name);
   }
   
   printf("active nrt funcs:\n");
   for(int j = 0; j < hal.comp_inst_count; j++){
      if(hal.comp_insts[j].comp->nrt){
         printf("%s\n", hal.comp_insts[j].comp->name);
      }
   }
}

COMMAND("hal", hal_term_print_info);






uint32_t call_cmd(char * s){
   char c[64];
   char a[64];
   uint32_t foo = sscanf(s, " %[a-zA-Z_0-9] %[ -~]", c, a);
   switch(foo){
      case 0:
         return(0);
      case 1:
         a[0] = 0;
      case 2:
         for(uint32_t i = 0; i < sizeof(cmd) / sizeof(cmd_t); i++){
            if(!strcmp(cmd[i].name, c)){
               cmd[i].ptr(a);
               return(1);
            }
         }
      default:
         return(0);
   }
   return(0);
}

static void rt_func(float period, volatile void * ctx_ptr, volatile hal_pin_inst_t * pin_ptr){
   struct term_ctx_t * ctx = (struct term_ctx_t *)ctx_ptr;
   struct term_pin_ctx_t * pins = (struct term_pin_ctx_t *)pin_ptr;
   
   if(ctx->send_counter++ >= PIN(send_step) - 1){
      for(int i = 0; i < TERM_NUM_WAVES; i++){
         ctx->wave_buf[ctx->write_pos][i] = PINA(wave, i);
      }
      
      ctx->write_pos++;
      ctx->write_pos %= TERM_BUF_SIZE;
      ctx->send_counter = 0;
   }
}

static void nrt_func(float period, volatile void * ctx_ptr, volatile hal_pin_inst_t * pin_ptr){
   struct term_ctx_t * ctx = (struct term_ctx_t *)ctx_ptr;
   struct term_pin_ctx_t * pins = (struct term_pin_ctx_t *)pin_ptr;

   int tmp = 0;
   unsigned char buf[TERM_NUM_WAVES + 2];

   buf[0] = 255;
   buf[TERM_NUM_WAVES + 1] = 0;

   unsigned int wp = ctx->write_pos;
   unsigned int bc = 0;

   while(ctx->read_pos != wp){
      bc++;
      
      for(int i = 0; i < TERM_NUM_WAVES; i++){
         tmp = (ctx->wave_buf[ctx->read_pos][i] + PINA(offset,i)) * PINA(gain,i) + 128;
         buf[i+1] = CLAMP(tmp,1,254);
      }

      ctx->read_pos++;
      ctx->read_pos %= TERM_BUF_SIZE;

      buf[TERM_NUM_WAVES + 1] = 0;

      if (cdc_is_connected() && PIN(send_step) > 0) {
         cdc_tx(buf,TERM_NUM_WAVES + 1);
      }
   }
   
   if (cdc_is_connected()) {
      PIN(con) = 1.0;
   }else{
      PIN(con) = 0.0;
   }
   
   if (cdc_is_connected()) {
      char rx_buf[64];
      if (cdc_getline(rx_buf,sizeof(rx_buf))){
         if(!call_cmd(rx_buf)){
            hal_parse(rx_buf);
         }
      }
   }
}

hal_comp_t term_comp_struct = {"term", nrt_func, rt_func, 0, 0, 0, 0, 0, 0, sizeof(struct term_ctx_t), sizeof(struct term_pin_ctx_t) / sizeof(struct hal_pin_inst_t)};
