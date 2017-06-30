#include <stdio.h>
#include "hal_conf.h"
#include "eeprom.h"
#include "crc16.h"
#include "link.h"
#include "main.h" // for Wait
#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"

HAL_COMP(conf);

HAL_PIN(r);
HAL_PIN(l);
HAL_PIN(j);
HAL_PIN(psi);
HAL_PIN(polecount);
HAL_PIN(mot_type);//ac sync,async/dc,2phase
HAL_PIN(out_rev);
HAL_PIN(high_motor_temp);
HAL_PIN(max_motor_temp);
HAL_PIN(phase_time);
HAL_PIN(phase_cur);

HAL_PIN(max_vel);
HAL_PIN(max_acc);
HAL_PIN(max_force);
HAL_PIN(max_dc_cur);
HAL_PIN(max_ac_cur);

HAL_PIN(fb_type);
HAL_PIN(fb_polecount);
HAL_PIN(fb_offset);
HAL_PIN(fb_rev);
HAL_PIN(fb_res);
HAL_PIN(autophase);//constant,cauto,hfi

HAL_PIN(cmd_type);
HAL_PIN(cmd_unit);//pos,vel,torque
HAL_PIN(cmd_rev);
HAL_PIN(cmd_res);
HAL_PIN(en_condition);//enable condition
HAL_PIN(error_out);
HAL_PIN(pos_static);//track pos in disabled and error condition TODO: not implemented

HAL_PIN(sin_offset);
HAL_PIN(cos_offset);
HAL_PIN(sin_gain);
HAL_PIN(cos_gain);
HAL_PIN(max_dc_volt);
HAL_PIN(max_hv_temp);
HAL_PIN(max_core_temp);
HAL_PIN(max_pos_error);
HAL_PIN(high_dc_volt);
HAL_PIN(low_dc_volt);
HAL_PIN(high_hv_temp);
HAL_PIN(fan_hv_temp);
HAL_PIN(fan_core_temp);
HAL_PIN(fan_motor_temp);

HAL_PIN(g);
HAL_PIN(pos_p);
HAL_PIN(vel_p);
HAL_PIN(vel_i);
HAL_PIN(vel_g);
HAL_PIN(cur_p);
HAL_PIN(cur_i);
HAL_PIN(cur_ff);
HAL_PIN(cur_ind);
HAL_PIN(max_sat);

// void conf_bla(char * ptr){
//    hal_comp_inst_t *inst;
//    inst = comp_inst_by_name("conf",0);
//    if(inst){
//       for(int i = 2; i < inst->comp->pin_count; i++){
//          printf("%s: %f\n",inst->pins[i],inst->pin_insts[i].source->value);
//       }
//       printf("found! %u\n",inst->comp->pin_count);
//    }else{
//       printf("conf0 not loaded\n");
//    }
// }
//
// CDOMMAND("confbla", conf_bla);

uint16_t conf_init(){
   FLASH_Unlock();
   uint16_t ret = EE_Init();
   FLASH_Lock();
   printf("OK\n");
   return ret;
}
COMMAND("confinit", conf_init);

void conf_save(){
   //TODO: check hal running?
   // if(hal.rt_state != RT_STOP || hal.frt_state != FRT_STOP){
   //    printf("cannot save while hal is running(run 'stop' command)\n");
   //    return;
   // }
   hal_comp_inst_t *inst;
   inst = comp_inst_by_name("conf",0);
   if(!inst){
      printf("conf0 not loaded\n");
      return 0;
   }
   typedef union{
      float f;
      uint16_t byte[2];
   }param_t;
   param_t param;
   uint16_t elo;
   uint16_t ehi;
   uint16_t address = 0;
   crc16_t crc = crc16_init();
   FLASH_Unlock();

   for(int i = 2; i < inst->comp->pin_count; i++){
      if(address >= NB_OF_VAR){
         printf("NB_OF_VAR too small\n");
         FLASH_Lock();
         return;
      }
      param.f = inst->pin_insts[i].source->value;
      crc = crc16_update(crc, (void*)&param.byte[0], 2);
      crc = crc16_update(crc, (void*)&param.byte[1], 2);
      //printf("param: %s=%f address:%i\n",hal.hal_pins[i]->name,param.f,address);
      elo = EE_WriteVariable(address,param.byte[0]);
      ehi = EE_WriteVariable(address+1,param.byte[1]);
      if(elo != FLASH_COMPLETE || ehi != FLASH_COMPLETE){
         FLASH_Lock();
         printf("error writing to address %i: error: %i,%i\n",address,elo,ehi);
         return;
      }
      address+=2;
   }
   //write crc
   crc = crc16_finalize(crc);
   elo = EE_WriteVariable(address, crc);
   if(elo != FLASH_COMPLETE){
      printf("error writing crc to address %i: error: %i\n",address,elo);
      FLASH_Lock();
      return;
   }
   FLASH_Lock();
   printf("saved %u parameters to flash\n",inst->comp->pin_count-2);
}

COMMAND("confsave", conf_save);

int conf_load(){
   //TODO: check hal running?
   // if(hal.rt_state != RT_STOP || hal.frt_state != FRT_STOP){
   //    return(-4);
   // }
   hal_comp_inst_t *inst;
   inst = comp_inst_by_name("conf",0);
   if(!inst){
      printf("conf0 not loaded\n");
      return 0;
   }

   typedef union{
      float f;
      uint16_t byte[2];
   }param_t;
   param_t param;
   uint16_t address = 0;
   uint16_t lo;
   uint16_t hi;
   uint16_t elo;
   uint16_t ehi;
   crc16_t crc = crc16_init();
   
   for(int i = 2; i < inst->comp->pin_count; i++){
      if(address >= NB_OF_VAR){
         return -2;
      }
      elo = EE_ReadVariable(address,&lo);
      ehi = EE_ReadVariable(address+1,&hi);
      crc = crc16_update(crc, (void*)&lo, 2);
      crc = crc16_update(crc, (void*)&hi, 2);
      if(elo == 0 && ehi == 0){
         param.byte[0] = lo;
         param.byte[1] = hi;
         inst->pin_insts[i].source = &(inst->pin_insts[i]);
         inst->pin_insts[i].value = param.f;
      }else{
         printf("error reading address %i: %i,%i\n",address,elo,ehi);
         return -3;
      }
      address+=2;
      Wait(1);//TODO: remove wait?
   }
   elo = EE_ReadVariable(address,&lo);
   if(elo != 0){
      printf("error reading crc from address %i: %i\n",address,elo);
      return -3;
   }
   crc = crc16_finalize(crc);
   if(crc != lo){//CRC error
      return -5;
   }
   printf("loaded %u parameters from flash\n",inst->comp->pin_count-2);
   //TODO: update stuff
   //update cmd/fb links
   //update_cmd();
   //update_fb();
   //update_mot();
   return 0;
}
COMMAND("confload", conf_load);

//
// void hal_conf_diff(){
//    typedef union{
//       float f;
//       uint16_t byte[2];
//    }param_t;
//    param_t param;
//    uint16_t address = 0;
//    uint16_t lo;
//    uint16_t hi;
//    uint16_t elo;
//    uint16_t ehi;
//    crc16_t crc = crc16_init();
//    for(int i = 0; i < hal.hal_pin_count; i++){
//       if(address >= NB_OF_VAR){
//          printf("NB_OF_VAR too small\n");
//          return;
//       }
//       char name[6];
//       strncpy(name,hal.hal_pins[i]->name,5);
//       name[5] =  '\0';
//       if(!strcmp(name, "conf0")){
//          elo = EE_ReadVariable(address,&lo);
//          ehi = EE_ReadVariable(address+1,&hi);
//          crc = crc16_update(crc, (void*)&lo, 2);
//          crc = crc16_update(crc, (void*)&hi, 2);
//          if(elo == 0 && ehi == 0){
//             param.byte[0] = lo;
//             param.byte[1] = hi;
//             if(hal.hal_pins[i]->value != param.f){
//                hal_term_print_pin(hal.hal_pins[i]);
//             }
//          }else{
//             printf("error reading address %i: %i,%i\n",address,elo,ehi);
//             return;
//          }
//          address+=2;
//          Wait(1);//TODO: remove wait?
//       }
//    }
// }

hal_comp_t conf_comp_struct = {
  .name = "conf",
  .nrt = 0,
  .rt = 0,
  .frt = 0,
  .nrt_init = 0,
  .rt_start = 0,
  .frt_start = 0,
  .rt_stop = 0,
  .frt_stop = 0,
  .ctx_size = 0,
  .pin_count = sizeof(struct conf_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
