/*
* This file is part of the stmbl project.
*
* Copyright (C) 2013-2017 Rene Hopf <renehopf@mac.com>
* Copyright (C) 2013-2017 Nico Stute <crinq@crinq.de>
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

// #include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"

HAL_COMP(ypid);

HAL_PIN(pos_ext_cmd);  // cmd in (rad)
HAL_PIN(pos_fb);       // feedback in (rad)
HAL_PIN(pos_error);    // error out (rad)

HAL_PIN(vel_ext_cmd);  // cmd in (rad/s)
HAL_PIN(vel_fb);       // feedback in (rad/s)
HAL_PIN(vel_cmd);      // cmd out (rad/s)
HAL_PIN(vel_error);    // error out (rad/s)
HAL_PIN(vel_min);      // minimum velocity error 0.3

HAL_PIN(enable);
HAL_PIN(out);

HAL_PIN(pos_p);  //10

HAL_PIN(vel_p);   //0.5
HAL_PIN(vel_i);   //0.005
HAL_PIN(vel_ff);  //1.0

// system limits
HAL_PIN(max_vel);
HAL_PIN(max_acc);
HAL_PIN(max_out);

HAL_PIN(vel_sat);
HAL_PIN(out_sat);
HAL_PIN(saturated);

struct ypid_ctx_t {
  float sat;
  float vel_error_sum;
};

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct ypid_ctx_t *ctx      = (struct ypid_ctx_t *)ctx_ptr;
  struct ypid_pin_ctx_t *pins = (struct ypid_pin_ctx_t *)pin_ptr;

  float vel_cmd;
  float vel_fb = PIN(vel_fb);

  float vel_error;
  float vel_sat;
  float out_sat;

  float pos_error = minus(PIN(pos_ext_cmd), PIN(pos_fb));

  float pos_p = MAX(PIN(pos_p), 0.0);
  float vel_p = MAX(PIN(vel_p), 0.0);
  float vel_i = MAX(PIN(vel_i), 0.0);

  float velmin = MAX(-PIN(max_vel), vel_fb - PIN(max_acc) * period);
  float velmax = MIN(PIN(max_vel), vel_fb + PIN(max_acc) * period);

  float out;

  if(PIN(enable) > 0.0) {
    // pos -> vel
    vel_cmd = pos_p * pos_error + PIN(vel_ff) * PIN(vel_ext_cmd);
    vel_sat = SAT(vel_cmd, PIN(max_vel));
    vel_cmd = CLAMP(vel_cmd, velmin, velmax);  // min/max clamping

    // vel -> out
    vel_error = vel_cmd - vel_fb;
    if(ABS(vel_error) < PIN(vel_min)) {
      vel_error = 0;
    }

    ctx->vel_error_sum += vel_error;
    out = LIMIT(vel_error * vel_p, PIN(max_out));
    if(vel_i * vel_p > 0.0f) {
      ctx->vel_error_sum = CLAMP(ctx->vel_error_sum, (-PIN(max_out) - out) / (vel_i * vel_p), (PIN(max_out) - out) / (vel_i * vel_p));
    } else {
      ctx->vel_error_sum = 0;
    }
    out += ctx->vel_error_sum * vel_i * vel_p;
    out_sat = SAT(out, PIN(max_out) * 0.99);
    out     = LIMIT(out, PIN(max_out));

    // sat
    if(ABS(vel_sat) + ABS(out_sat) > 0.0) {
      ctx->sat += period;
    } else {
      ctx->sat = 0.0;
    }
  } else {
    ctx->vel_error_sum = 0.0;
    vel_cmd            = 0.0;
    vel_error          = 0.0;
    vel_sat            = 0.0;
    out_sat            = 0.0;
    ctx->sat           = 0.0;
    out                = 0.0;
  }

  PIN(pos_error) = pos_error;
  PIN(vel_cmd)   = vel_cmd;
  PIN(vel_error) = vel_error;
  PIN(out)       = out;

  PIN(vel_sat)   = vel_sat;
  PIN(out_sat)   = out_sat;
  PIN(saturated) = ctx->sat;
}


hal_comp_t ypid_comp_struct = {
    .name      = "ypid",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = 0,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = sizeof(struct ypid_ctx_t),
    .pin_count = sizeof(struct ypid_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
