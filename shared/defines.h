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
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NO 0
#define YES 1
#define ABS(a) (((a) < 0.0) ? -(a) : (a))
#define LIMIT(x, lowhigh) (((x) > (lowhigh)) ? (lowhigh) : (((x) < (-lowhigh)) ? (-lowhigh) : (x)))
#define SAT(x, lowhigh) (((x) > (lowhigh)) ? (1.0) : (((x) < (-lowhigh)) ? (-1.0) : (0.0)))
#define SAT2(x, low, high) (((x) > (high)) ? (1.0) : (((x) < (low)) ? (-1.0) : (0.0)))
#define STEP(from, to, step) (((from) < (to)) ? (MIN((from) + (step), (to))) : (MAX((from) - (step), (to))))
#define DEG(a) ((a)*M_PI / 180.0)
#define RAD(a) ((a)*180.0 / M_PI)
#define SIGN(a) (((a) < 0.0) ? (-1.0) : (((a) > 0.0) ? (1.0) : (0.0)))
#define CLAMP(x, low, high) (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))
#define SCALE(value, high, max) MIN(MAX(((max) - (value)) / ((max) - (high)), 0.0), 1.0)
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN3(a, b, c) MIN(a, MIN(b, c))
#define MAX3(a, b, c) MAX(a, MAX(b, c))

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#define FIELD_SIZEOF(t, f) (sizeof(((t *)0)->f))

#define STATIC_ASSERT(x) _Static_assert(x, #x)

#define STRINGIFY_(x) #x
#define STRINGIFY(x) STRINGIFY_(x)

extern volatile uint64_t systime;
#define BLINK(N)                                           \
  ({                                                       \
    int t = (systime / 300) % (2 * (unsigned int)(N) + 2); \
    if(t < 2) {                                            \
      t = 0;                                               \
    } else {                                               \
      t = t % 2;                                           \
    }                                                      \
    t;                                                     \
  })

//TODO: change type to typeof()
//TODO: change __old_val__ to something more useful
//BUG: fails with multiple comps TODO: move to ctx

#define RISING_EDGE(sig) \
  ({static float __old_val__ = 0.0; uint8_t ret = (sig) > __old_val__; __old_val__ = (sig); ret; })

#define FALLING_EDGE(sig) \
  ({static float __old_val__ = 0.0; uint8_t ret = (sig) < __old_val__; __old_val__ = (sig); ret; })

#define EDGE(sig) \
  ({static float __old_val__ = 0.0; uint8_t ret = (sig) != __old_val__; __old_val__ = (sig); ret; })

#define LP_HZ(a) (((a) <= 0.0) ? (1.0) : (1.0 / (1.0 / period / ((a)*M_PI * 2.0) + 1.0)))

#ifdef __cplusplus
}
#endif
