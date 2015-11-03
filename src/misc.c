/*
 * This file is part of the stmbl project.
 *
 * Copyright (C) 2013-2015 Rene Hopf <renehopf@mac.com>
 * Copyright (C) 2013-2015 Nico Stute <crinq@crinq.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "misc.h"

inline float minus(float a, float b){
	if(ABS(a - b) < M_PI){
		return(a - b);
	}
	else if(a > b){
		return(a - b - 2.0 * M_PI);
	}
	else{
		return(a - b + 2.0 * M_PI);
	}
}

inline float mod(float a){
	while(a < -M_PI){
		a += 2.0 * M_PI;
	}
	while(a > M_PI){
		a -= 2.0 * M_PI;
	}
	return(a);
}

int strcmp(const char* s1, const char* s2){
  while(*s1 && (*s1 == *s2)){
  	s1++;
		s2++;
	}
  return(*(const unsigned char*)s1 - *(const unsigned char*)s2);
}

void strncpy(char* dst, char* src, int n){
  while (n-- && (*dst++ = *src++));
}

char* strncat(char *dest, const char *src, int n){
    char *rdest = dest;
		dest[n] = '\0';
    while (n-- && *dest){
      dest++;
    }
		while (n-- && (*dest++ = *src++)){
		}
    return rdest;
}

char* strcpy(char *strDest, const char *strSrc) {
    if(strDest==NULL || strSrc==NULL) {
    	return NULL;
    }
    char *temp = strDest;
    while ((*strDest++ = *strSrc++) != '\0'); 
    return temp;
}