//
//  scanf.h
//  test
//
//  Created by crinq on 05.12.13.
//  Copyright (c) 2013 Rene Hopf. All rights reserved.
//

#ifndef test_scanf_h
#define test_scanf_h

#include <stdarg.h>

#ifdef USBTERM
#include "stm32_ub_usb_cdc.h"
#endif

int scanf_(const char *format, ...);
int sscanf_(const char* buf, const char *format, ...);
int vfsscanf_(const char *buf, const char *format, va_list arg);

#endif
