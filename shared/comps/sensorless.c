/*
* This file is part of the stmbl project.
*
* Copyright (C) 2013-2016 Rene Hopf <renehopf@mac.com>
* Copyright (C) 2013-2016 Nico Stute <crinq@crinq.de>
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
* You shou l have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"


HAL_COMP(sensorless);

//in
HAL_PIN(r);
HAL_PIN(l);

HAL_PIN(ki);
HAL_PIN(kb);
HAL_PIN(kl);
HAL_PIN(min_vel);
HAL_PIN(vel_boost);
HAL_PIN(max_vel);

HAL_PIN(id);
HAL_PIN(iq);
HAL_PIN(ud);
HAL_PIN(uq);
HAL_PIN(ud1);
HAL_PIN(uq1);
HAL_PIN(ud2);
HAL_PIN(uq2);


//out
HAL_PIN(vel);
HAL_PIN(pos);

HAL_PIN(ed);
HAL_PIN(eq);

HAL_PIN(old_id);
HAL_PIN(old_iq);

HAL_PIN(delta_id);
HAL_PIN(delta_iq);

static void nrt_init(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  // struct sensorless_ctx_t *ctx      = (struct sensorless_ctx_t *)ctx_ptr;
  struct sensorless_pin_ctx_t *pins = (struct sensorless_pin_ctx_t *)pin_ptr;
  PIN(ki) = 1500.0;
  PIN(kb) = 1;
  PIN(kl) = 0.75;
  PIN(min_vel) = 3.0 * 2.0 * M_PI;
  PIN(vel_boost) = 0.2;
  PIN(max_vel) = 500.0 * 2.0 * M_PI * 1.1;
}

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  // struct sensorless_ctx_t *ctx      = (struct sensorless_ctx_t *)ctx_ptr;
  struct sensorless_pin_ctx_t *pins = (struct sensorless_pin_ctx_t *)pin_ptr;

  float l = MAX(PIN(l), 0.00001);
  float r = MAX(PIN(r), 0.01);

  float ud = PIN(ud2);
  float uq = PIN(uq2);
  PIN(ud2) = PIN(ud1);
  PIN(uq2) = PIN(uq1);
  PIN(ud1) = PIN(ud);
  PIN(uq1) = PIN(uq);
  
  float id = PIN(id);
  float iq = PIN(iq);

  float kb = CLAMP(PIN(kb), 0, 1);
  float ki = CLAMP(PIN(ki), 0, 1.0 / period);
  float kl = CLAMP(PIN(kl), 0, 1);

  float vel = PIN(vel);
  float pos = PIN(pos);

  // lowpass current change
  PIN(delta_id) = PIN(delta_id) * kl + (id - PIN(old_id)) * (1 - kl);
  PIN(delta_iq) = PIN(delta_iq) * kl + (iq - PIN(old_iq)) * (1 - kl);
  
  PIN(old_id) = id;
  PIN(old_iq) = iq;

  // calc bemf
  float ed = ud - r * id - PIN(delta_id) * l / period + vel * l * iq * kb;
  float eq = uq - r * iq - PIN(delta_iq) * l / period - vel * l * id * kb;

  // velocity compensation, bemf_d -> 0
  vel -= SIGN2(eq, 1.0) * ed * ki * period;

  // startup boost
  if(ABS(vel) < PIN(min_vel)){
    vel += SIGN2(id * iq, 0.1) * PIN(vel_boost);
  }

  vel = LIMIT(vel, PIN(max_vel));

  pos += vel * period;
  
  PIN(ed) = ed;
  PIN(eq) = eq;

  PIN(vel) = vel;
  PIN(pos) = mod(pos);
}

hal_comp_t sensorless_comp_struct = {
    .name      = "sensorless",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = nrt_init,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = 0,
    .pin_count = sizeof(struct sensorless_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};