//
//  param.c
//  test
//
//  Created by crinq on 07.12.13.
//  Copyright (c) 2013 Rene Hopf. All rights reserved.
//
#include "param.h"

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
		printf_("\nregister OK:%i",PARAMS.param_count);
		return(1);
}

float get_float(char name){
		for(int i = 0; i < PARAMS.param_count; i++){
			if(PARAMS.names[i] == name){
				printf_("\ngot %f for param %i",*PARAMS.data[0],i);
				return(*(PARAMS.data[i]));
			}
		}
		return(0);
}

int set_float(char name, float f){
		for(int i = 0; i < PARAMS.param_count; i++){
			if(PARAMS.names[i] == name){
				*(PARAMS.data[i]) = f;
				printf_("\n%c set to %f",name,f);
				return(1);
			}
		}
		return(0);
}