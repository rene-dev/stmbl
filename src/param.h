//
//  param.c
//  test
//
//  Created by crinq on 07.12.13.
//  Copyright (c) 2013 Rene Hopf. All rights reserved.
//

#pragma once

#define MAX_PARAMS 64
#define MAX_PNAME 8
typedef char PNAME[MAX_PNAME];

struct param{
    PNAME names[MAX_PARAMS];
    enum {
      FLOAT,
      INT
    } types[MAX_PARAMS];
    union{
      volatile float *floats[MAX_PARAMS];
      volatile int *ints[MAX_PARAMS];
    };

    void (*read_callbacks[MAX_PARAMS])(float v);
    void (*write_callbacks[MAX_PARAMS])(float v);

		int param_count;
} PARAMS;

int strcmp(const char* s1, const char* s2);
void strncpy(char* dst, char* src, int n);

void param_init();

int is_param(char* name);

int register_float(char* name,volatile float *f);

int register_int(char* name,volatile int *i);

int register_read_callback(char* name, void (*db)(float v));

int register_write_callback(char* name, void (*db)(float v));

float get_param(char* name);

int set_param(char* name,volatile float f);

void list_param();
