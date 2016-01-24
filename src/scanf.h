//
//  scanf.h
//  test
//
//  Created by crinq on 05.12.13.
//  Copyright (c) 2013 Rene Hopf. All rights reserved.
//

#pragma once

#include <stdarg.h>

int scanf_(const char *format, ...);
int sscanf_(const char* buf, const char *format, ...);
int vfsscanf_(const char *buf, const char *format, va_list arg);
int isDecDigit(char c);
