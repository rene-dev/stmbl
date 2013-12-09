//
//  param.c
//  test
//
//  Created by crinq on 07.12.13.
//  Copyright (c) 2013 Rene Hopf. All rights reserved.
//
/*#define R 1
#define W 2
#define F 1;
#define I 2;
#define C 3;*/
#define MAX_PARAMS 32
//#define MAX_PNAME 8


struct param{
    char names[MAX_PARAMS];
    //unsigned char type[MAX_PARAMS];
    //unsigned char flags[MAX_PARAMS];
    float *data[MAX_PARAMS];
		int param_count;
} PARAMS;

int register_float(char name, float *f);

float get_float(char name);

int set_float(char name, float f);