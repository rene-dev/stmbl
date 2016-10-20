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

#pragma once
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NO 0
#define YES 1
#define ABS(a)	   (((a) < 0.0) ? -(a) : (a))
#define LIMIT(x, lowhigh)  (((x) > (lowhigh)) ? (lowhigh) : (((x) < (-lowhigh)) ? (-lowhigh) : (x)))
#define SAT(x, lowhigh)  (((x) > (lowhigh)) ? (1.0) : (((x) < (-lowhigh)) ? (-1.0) : (0.0)))
#define SAT2(x, low, high)  (((x) > (high)) ? (1.0) : (((x) < (low)) ? (-1.0) : (0.0)))
#define STEP(from, to, step)  (((from) < (to)) ? (MIN((from) + (step), (to))) : (MAX((from) - (step), (to))))
#define MIN(a, b)  (((a) < (b)) ? (a) : (b))
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))
#define DEG(a) ((a) * M_PI / 180.0)
#define RAD(a) ((a) * 180.0 / M_PI)
#define SIGN(a) (((a) < 0.0) ? (-1.0) : (((a) > 0.0) ? (1.0) : (0.0)))

//pid krams
#define RPM(a) ((a) / 60.0 * M_PI * 2.0)
#define KGCM2(a) ((a) / 10000.0)
//#define VKRPM(a, b) ((a)) * 60.0 / (sqrtf(3.0) * 1000.0 * M_PI * 2.0 * (b)))) // Volt/krpm, polecount
#define LP_HZ(a) (((a) <= 0.0) ? (1.0) : (1.0 / (5000.0 / ((a) * M_PI * 2.0) + 1.0)))

float minus(float a, float b);
float mod(float a);

#ifdef __cplusplus
}
#endif
