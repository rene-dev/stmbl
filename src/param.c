//
//  param.c
//  test
//
//  Created by crinq on 07.12.13.
//  Copyright (c) 2013 Rene Hopf. All rights reserved.
//
#include "param.h"
#include "printf.h"

void default_callback(float f){

}

int strcmp(const char* s1, const char* s2){
  while(*s1 && (*s1 == *s2)){
  	s1++;
		s2++;
	}
  return(*(const unsigned char*)s1 - *(const unsigned char*)s2);
}

void strncpy(char* dst, char* src, int n){
	for(int i = 0; i < n && *src; i++){
		*dst = *src;
		dst++;
		src++;
	}
}

void param_init(){
	PARAMS.param_count = 0;
}

int register_float(char* name, volatile float *f){
	if(PARAMS.param_count == MAX_PARAMS){
		return(0);
	}

	for(int i = 0; i < PARAMS.param_count; i++){
		if(!strcmp(PARAMS.names[i], name)){
			return(0);
		}
	}

	strncpy(PARAMS.names[PARAMS.param_count], name, MAX_PNAME);
	PARAMS.types[PARAMS.param_count] = FLOAT;
	PARAMS.floats[PARAMS.param_count] = f;
  PARAMS.read_callbacks[PARAMS.param_count] = default_callback;
  PARAMS.write_callbacks[PARAMS.param_count] = default_callback;
	PARAMS.param_count++;
	return(1);
}

int register_int(char* name, volatile int *i){
	if(PARAMS.param_count == MAX_PARAMS){
		return(0);
	}

	for(int i = 0; i < PARAMS.param_count; i++){
		if(!strcmp(PARAMS.names[i], name)){
			return(0);
		}
	}

	strncpy(PARAMS.names[PARAMS.param_count], name, MAX_PNAME);
	PARAMS.types[PARAMS.param_count] = INT;
	PARAMS.ints[PARAMS.param_count] = i;
  PARAMS.read_callbacks[PARAMS.param_count] = default_callback;
  PARAMS.write_callbacks[PARAMS.param_count] = default_callback;
	PARAMS.param_count++;
	return(1);
}

int register_read_callback(char* name, void (*cb)(float v)){
  for(int i = 0; i < PARAMS.param_count; i++){
    if(!strcmp(PARAMS.names[i], name)){
      PARAMS.read_callbacks[i] = cb;
      return(1);
    }
  }
  return(0);
}

int register_write_callback(char* name, void (*cb)(float v)){
  for(int i = 0; i < PARAMS.param_count; i++){
    if(!strcmp(PARAMS.names[i], name)){
      PARAMS.write_callbacks[i] = cb;
      return(1);
    }
  }
  return(0);
}


int is_param(char* name){
	for(int i = 0; i < PARAMS.param_count; i++){
		if(!strcmp(PARAMS.names[i], name)){
			return(1);
		}
  }
  return(0);
}

void list_param(){
	for(int i = 0; i < PARAMS.param_count; i++){
		if(PARAMS.types[i] == FLOAT){
			printf_("float %s = %f\n",PARAMS.names[i],*PARAMS.floats[i],i);
		}
		else{
			printf_("int %s = %i\n",PARAMS.names[i],*PARAMS.ints[i],i);
		}
	}
}

float get_param(char* name){
	for(int i = 0; i < PARAMS.param_count; i++){
		if(!strcmp(PARAMS.names[i], name)){
			if(PARAMS.types[i] == FLOAT){
        PARAMS.read_callbacks[i](*PARAMS.floats[i]);
				return(*PARAMS.floats[i]);
			}
			else{
        PARAMS.read_callbacks[i]((float)*PARAMS.ints[i]);
				return((float)*PARAMS.ints[i]);
			}
		}
	}
	return(0);
}

int set_param(char* name, volatile float f){
	for(int i = 0; i < PARAMS.param_count; i++){
		if(!strcmp(PARAMS.names[i], name)){
			if(PARAMS.types[i] == FLOAT){
				*PARAMS.floats[i] = f;
        PARAMS.write_callbacks[i](f);
			}
			else{
				*PARAMS.ints[i] = (int)f;
        PARAMS.write_callbacks[i](f);
			}
			return(1);
		}
	}
	return(0);
}
