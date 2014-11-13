#include "input.h"
#include <stm32f4xx_tim.h>

int i_init(input* i){
  i->state = DISABLED;
  i->reset(i);
  return(1);
}

int i_reset(input* i){
  if(i->state == DISABLED || i->state == ENABLED || i->state == FERROR){
    i->state = DISABLED;
    return(1);
  }
  return(0);
}

int i_enable(input* i){
  if(i->state == DISABLED || i->state == ENABLED){
    i->state = ENABLED;
    return(1);
  }
  return(0);
}

int i_disable(input* i){
  if(i->state == DISABLED || i->state == ENABLED){
    i->state = DISABLED;
    return(1);
  }
  return(0);
}

void do_rt_stuff(input* i, float period){

}

float get_pos(input* i){
  return(i->pos);
}

float get_vel(input* i){
  return(i->vel);
}

float get_acc(input* i){
  return(i->acc);
}

float get_force(input* i){
  return(i->force);
}


// quad enc tim3
void read_enc_tim3(input* i, float period){
  i->pos = TIM_GetCounter(TIM3) * i->scale;
}
