#include "hal.h"

void init_hal(){
  hal.comp_type_count = 0;
  for(int i = 0; i < MAX_COMP_TYPES; i++){
    hal.comp_types_counter[i] = 0;
  }
  hal.hal_pin_count = 0;
  hal.fast_rt_lock = 0;
  hal.rt_lock = 0;
  hal.nrt_lock = 0;
}

void init_hal_pin(HPNAME name, struct hal_pin* pin, float value){
  strncpy(pin->name, hal.comp_type, MAX_HPNAME);
  strncat(pin->name, ".", MAX_HPNAME);
  strncat(pin->name, name, MAX_HPNAME);
  pin->value = value;
  pin->source = pin;
  register_hal_pin(pin);
}

int register_hal_pin(struct hal_pin* pin){
  if(hal.hal_pin_count >= MAX_HAL_PINS){
    return(0);
  }

  for(int i = 0; i < hal.hal_pin_count; i++){
    if(!strcmp(hal.hal_pins[i]->name, pin->name)){
      return(0);
    }
  }

  hal.hal_pins[hal.hal_pin_count] = pin;
  hal.hal_pin_count++;

  return(1);
}

int set_hal_pin(HPNAME name, float value){
  for(int i = 0; i < hal.hal_pin_count; i++){
    if(!strcmp(hal.hal_pins[i]->name, name)){
      hal.hal_pins[i]->value = value;
      return(1);
    }
  }
  return(0);
}

int is_hal_pin(HPNAME name){
  for(int i = 0; i < hal.hal_pin_count; i++){
    if(!strcmp(hal.hal_pins[i]->name, name)){
      return(1);
    }
  }
  return(0);
}

float get_hal_pin(HPNAME name){
  for(int i = 0; i < hal.hal_pin_count; i++){
    if(!strcmp(hal.hal_pins[i]->name, name)){
      return(hal.hal_pins[i]->source->value);
    }
  }
  return(0.0);
}

void write_hal_pin(struct hal_pin* pin, float value){
  pin->value = value;
}

float read_hal_pin(struct hal_pin* pin){
  return(pin->source->value);
}

struct hal_pin* find_hal_pin(HPNAME name){
  for(int i = 0; i < hal.hal_pin_count; i++){
    if(!strcmp(hal.hal_pins[i]->name, name)){
      return(hal.hal_pins[i]);
    }
  }
  return(0);
}

int link_hal_pins(HPNAME source, HPNAME sink){
  struct hal_pin* d;
  struct hal_pin* s;
  d = find_hal_pin(source);
  s = find_hal_pin(sink);

  if(d != 0 && s != 0){
    s->value = s->source->value;
    s->source = d->source;
    return(1);
  }
  return(0);
}

float read_float(char* buf){
  int buffer_pos = 0;
  float f = 0;
  float tf = 10;
  float invert = 1;

  if(buf[buffer_pos] && buf[buffer_pos] == '-'){
    invert = -1;
    buffer_pos++;
  }
  else if(buf[buffer_pos] && buf[buffer_pos] == '+'){
    buffer_pos++;
  }
  while(buf[buffer_pos] && isDecDigit(buf[buffer_pos])){
    f *= 10;
    f += buf[buffer_pos++] - '0';
  }
  if(buf[buffer_pos] && buf[buffer_pos] == '.'){
    buffer_pos++;
    while(buf[buffer_pos] && isDecDigit(buf[buffer_pos])){
      f += (buf[buffer_pos++] - '0') / tf;
      tf *= 10;
    }
  }
  f *= invert;
  return(f);
}

char* itoa(int i){
  hal.tmp[0] = (i % 10) + '0';
  hal.tmp[1] = '\0';
  return(hal.tmp);
}

int set_comp_type(HPNAME name){
  for(int i = 0; i < hal.comp_type_count; i++){
    if(!strcmp(hal.comp_types[i], name)){
      strncpy(hal.comp_type, name, MAX_HPNAME);
      strncat(hal.comp_type, itoa(hal.comp_types_counter[i]), MAX_HPNAME);
      return(hal.comp_types_counter[i]++);
    }
  }

  strncpy(hal.comp_types[hal.comp_type_count], name, MAX_HPNAME);
  strncpy(hal.comp_type, name, MAX_HPNAME);
  strncat(hal.comp_type, itoa(hal.comp_types_counter[hal.comp_type_count++]++), MAX_HPNAME);
  return(0);
}
