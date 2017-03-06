#include "stm32f3xx_hal.h"
#include "commands.h"
#include "commandslist.h"
#include "hal2.h"
#include "defines.h"
#include <stdio.h>
#include <string.h>
#include "usbd_cdc_if.h"

#define TERM_NUM_WAVES 8
#define TERM_BUF_SIZE 8

HAL_COMP2(term);

HAL2_PINA(wave, 8);
HAL2_PINA(offset, 8);
HAL2_PINA(gain, 8);
HAL2_PIN(send_step);
HAL2_PIN(con);

struct term_ctx_t{
   float wave_buf[TERM_BUF_SIZE][TERM_NUM_WAVES];
   uint32_t send_counter;
   uint32_t write_pos;
   uint32_t read_pos;
};

void bootloader(char * ptr){
   RTC->BKP0R = 0xDEADBEEF;
   NVIC_SystemReset();
}

COMMAND("bootloader", bootloader);

uint32_t call_cmd(char * s, char * args){
   for(uint32_t i = 0; i < sizeof(cmd) / sizeof(cmd_t); i++){
      if(!strcmp(cmd[i].name, s)){
         cmd[i].ptr(args);
         return(1);
      }
   }
   return(0);
}

static void rt_func(float period, volatile void * ctx_ptr, volatile hal_pin_inst_t * pin_ptr){
   struct term_ctx_t * ctx = (struct term_ctx_t *)ctx_ptr;
   struct term_pin_ctx_t * pins = (struct term_pin_ctx_t *)pin_ptr;
   
   if(ctx->send_counter++ >= PIN2(send_step) - 1){
      for(int i = 0; i < TERM_NUM_WAVES; i++){
         ctx->wave_buf[ctx->write_pos][i] = PINA2(wave, i);
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
         tmp = (ctx->wave_buf[ctx->read_pos][i] + PINA2(offset,i)) * PINA2(gain,i) + 128;
         buf[i+1] = CLAMP(tmp,1,254);
      }

      ctx->read_pos++;
      ctx->read_pos %= TERM_BUF_SIZE;

      buf[TERM_NUM_WAVES + 1] = 0;

      if (cdc_is_connected() && PIN2(send_step) > 0) {
         cdc_tx(buf,TERM_NUM_WAVES + 1);
      }
   }
   
   if (cdc_is_connected()) {
      PIN2(con) = 1.0;
   }else{
      PIN2(con) = 0.0;
   }
   
   if (cdc_is_connected()) {
      char rx_buf[64];
      if (cdc_getline(rx_buf,sizeof(rx_buf))){
         if(!call_cmd(rx_buf, "")){
            hal_parse(rx_buf);
         }
      }
   }
}

hal2_comp_t term_comp_struct = {"term", nrt_func, rt_func, 0, 0, 0, 0, 0, 0, sizeof(struct term_ctx_t), sizeof(struct term_pin_ctx_t) / sizeof(struct hal_pin_inst_t)};
