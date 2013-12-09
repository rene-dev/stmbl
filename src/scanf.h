//
//  scanf.h
//  test
//
//  Created by crinq on 05.12.13.
//  Copyright (c) 2013 Rene Hopf. All rights reserved.
//

#ifndef test_scanf_h
#define test_scanf_h
#define SCANF_BSIZE 1024
#define SCANF_ASIZE 32

char scanf_buffer[SCANF_BSIZE];
int scanf_(const char *format, ...);

#endif
