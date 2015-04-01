#pragma once

#include <math.h>
#include "misc.h"

float sinf_(float a){
  a = mod(a);
  if(a > M_PI / 2.0){
    a = M_PI - a;
  }
  else if(a < -M_PI / 2.0){
    a = -M_PI - a;
  }
  float a_ = a;
  float ret = a;
  a_ = a_ * a * a;
  ret -= a_ / 6.0;
  a_ = a_ * a * a;
  ret += a_ / 120.0;
  a_ = a_ * a * a;
  ret -= a_ / 5040.0;

  return(ret);
}

float cosf_(float a){
  return(sinf_(a - M_PI / 2.0));
}
