//
//  param.c
//  test
//
//  Created by crinq on 07.12.13.
//  Copyright (c) 2013 Rene Hopf. All rights reserved.
//
#include "param.h"
#include "printf.h"

void param_init(){
	PARAMS.param_count = 0;
}

int register_float(char name, float *f){
		if(PARAMS.param_count == MAX_PARAMS){
			return(0);
		}

		for(int i = 0; i < PARAMS.param_count; i++){
			if(PARAMS.names[i] == name){
				return(0);
			}
		}

		PARAMS.names[PARAMS.param_count] = name;
		PARAMS.data[PARAMS.param_count] = f;
		PARAMS.param_count++;
		//printf_("register OK:%i\n",PARAMS.param_count);
		return(1);
}

int is_param(char name){
	for(int i = 0; i < PARAMS.param_count; i++){
		if(PARAMS.names[i] == name){
			return(1);
		}
  }
  return(0);
}

void list_param(){
	for(int i = 0; i < PARAMS.param_count; i++){
		printf_("%c=%f\n",PARAMS.names[i],*PARAMS.data[i],i);
	}
}

float get_float(char name){
		for(int i = 0; i < PARAMS.param_count; i++){
			if(PARAMS.names[i] == name){
				//printf_("got %f for param %i\n",*PARAMS.data[i],i);
				return(*(PARAMS.data[i]));
			}
		}
		return(0);
}

int set_float(char name, float f){
		for(int i = 0; i < PARAMS.param_count; i++){
			if(PARAMS.names[i] == name){
				*(PARAMS.data[i]) = f;
				//printf_("%c set to %f\n",name,f);
				return(1);
			}
		}
		return(0);
}