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
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"


HAL_COMP(pmsm);

//in
HAL_PIN(psi);
HAL_PIN(r);
HAL_PIN(ld);
HAL_PIN(lq);
HAL_PIN(polecount);

HAL_PIN(vel);
HAL_PIN(ud);
HAL_PIN(uq);

HAL_PIN(indd);
HAL_PIN(indq);

//out
HAL_PIN(id);
HAL_PIN(iq);
HAL_PIN(psi_d);
HAL_PIN(psi_q);
HAL_PIN(torque);

static void nrt_init(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  // struct pmsm_ctx_t *ctx      = (struct pmsm_ctx_t *)ctx_ptr;
  struct pmsm_pin_ctx_t *pins = (struct pmsm_pin_ctx_t *)pin_ptr;
  PIN(psi)       = 0.01;
  PIN(r)         = 1.0;
  PIN(ld)        = 0.001;
  PIN(lq)        = 0.001;
  PIN(polecount) = 1.0;
}

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  // struct pmsm_ctx_t *ctx      = (struct pmsm_ctx_t *)ctx_ptr;
  struct pmsm_pin_ctx_t *pins = (struct pmsm_pin_ctx_t *)pin_ptr;


  float p     = (int)MAX(PIN(polecount), 1.0);
  float vel_e = PIN(vel) * p;
  float ld    = MAX(PIN(ld), 0.0001);
  float lq    = MAX(PIN(lq), 0.0001);
  float ud    = PIN(ud);
  float uq    = PIN(uq);
  float psi_m = MAX(PIN(psi), 0.01);
  float r     = MAX(PIN(r), 0.01);
  float id    = PIN(id);
  float iq    = PIN(iq);

  float psi_d = ld * id + psi_m;
  float psi_q = lq * iq;

  float indd = vel_e * psi_q;  // todo redundant calculation
  float indq = vel_e * psi_d;

  id += (ud - r * id + indd) / ld * period / 4.0;
  iq += (uq - r * iq - indq) / lq * period / 4.0;

  id += (ud - r * id + indd) / ld * period / 4.0;
  iq += (uq - r * iq - indq) / lq * period / 4.0;

  id += (ud - r * id + indd) / ld * period / 4.0;
  iq += (uq - r * iq - indq) / lq * period / 4.0;

  id += (ud - r * id + indd) / ld * period / 4.0;
  iq += (uq - r * iq - indq) / lq * period / 4.0;

  float t = 3.0 / 2.0 * p * (psi_m * iq + (ld - lq) * id * iq);

  PIN(id)     = id;
  PIN(iq)     = iq;
  PIN(indd)   = indd;
  PIN(indq)   = indq;
  PIN(psi_d)  = psi_d;
  PIN(psi_q)  = psi_q;
  PIN(torque) = t;
}

hal_comp_t pmsm_comp_struct = {
    .name      = "pmsm",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = nrt_init,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = 0,
    .pin_count = sizeof(struct pmsm_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
