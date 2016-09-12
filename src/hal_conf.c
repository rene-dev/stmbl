#include "hal_conf.h"
#include "eeprom.h"
#include "crc16.h"
#include "link.h"
#include <stdio.h>

void hal_conf_init(){
   FLASH_Unlock();
   EE_Init();
   FLASH_Lock();
}

void hal_conf_save(){
   // if(ee_error != FLASH_COMPLETE){
   //    printf("flash error:%i\n",ee_error);
   //    return;
   // }
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
   for(int i = 0; i < hal.hal_pin_count; i++){
      if(address >= NB_OF_VAR){
         printf("NB_OF_VAR too small\n");
         FLASH_Lock();
         return;
      }
      char name[6];
      strncpy(name,hal.hal_pins[i]->name,5);
      name[5] =  '\0';
      if(!strcmp(name, "conf0")){
         param.f = hal.hal_pins[i]->source->source->value;
         crc = crc16_update(crc, (void*)&param.byte[0], 2);
         crc = crc16_update(crc, (void*)&param.byte[1], 2);
         //printf("param: %s=%f address:%i\n",hal.hal_pins[i]->name,param.f,address);
         elo = EE_WriteVariable(address,param.byte[0]);
         ehi = EE_WriteVariable(address+1,param.byte[1]);
         if(elo != FLASH_COMPLETE || ehi != FLASH_COMPLETE){
            printf("error writing to %i: error%i,%i\n",address,elo,ehi);
            break;
         }
         address+=2;
      }
   }
   //write crc
   crc = crc16_finalize(crc);
   elo = EE_WriteVariable(address, crc);
   if(elo != FLASH_COMPLETE){
      printf("error writing crc to %i: error%i\n",address,elo);
   }
   FLASH_Lock();
   printf("done\n");
}

int hal_conf_load(){
   if(hal.rt_state != RT_STOP || hal.frt_state != FRT_STOP){
      return(-4);
   }
   // if(ee_error != FLASH_COMPLETE){
   //    return -1;
   // }
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
   for(int i = 0; i < hal.hal_pin_count; i++){
      if(address >= NB_OF_VAR){
         return -2;
      }
      char name[6];
      strncpy(name,hal.hal_pins[i]->name,5);
      name[5] =  '\0';
      if(!strcmp(name, "conf0")){
         elo = EE_ReadVariable(address,&lo);
         ehi = EE_ReadVariable(address+1,&hi);
         crc = crc16_update(crc, (void*)&lo, 2);
         crc = crc16_update(crc, (void*)&hi, 2);
         if(elo == 0 && ehi == 0){
            param.byte[0] = lo;
            param.byte[1] = hi;
            hal.hal_pins[i]->value = param.f;
         }else{
            return -3;
         }
         address+=2;
         Wait(1);//TODO: remove wait?
      }
   }
   elo = EE_ReadVariable(address,&lo);
   if(elo != 0){
      return -3;
   }
   crc = crc16_finalize(crc);
   if(crc != lo){//CRC error
      return -5;
   }
   //update cmd/fb links
   update_cmd();
   update_fb();
   update_mot();
   return 0;
}
