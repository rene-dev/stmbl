//
//  param.c
//  test
//
//  Created by crinq on 07.12.13.
//  Copyright (c) 2013 Rene Hopf. All rights reserved.
//
#include "param.h"

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
		return(1);
}

float get_float(char name){
		for(int i = 0; i < PARAMS.param_count; i++){
			if(PARAMS.names[i] == name){
				return(*(PARAMS.data[i]));
			}
		}
		return(0);
}

int set_float(char name, float f){
		for(int i = 0; i < PARAMS.param_count; i++){
			if(PARAMS.names[i] == name){
				*(PARAMS.data[i]) = f;
				return(1);
			}
		}
		return(0);
}