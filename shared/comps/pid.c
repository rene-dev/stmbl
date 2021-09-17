#include "pid_comp.h"
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

HAL_COMP(pid);

HAL_PIN(pos_ext_cmd);  // cmd in (rad)
HAL_PIN(pos_fb);       // feedback in (rad)
HAL_PIN(pos_error);    // error out (rad)

HAL_PIN(vel_ext_cmd);  // cmd in (rad/s)
HAL_PIN(vel_fb);       // feedback in (rad/s)
HAL_PIN(vel_cmd);      // cmd out (rad/s)
HAL_PIN(vel_error);    // error out (rad/s)

HAL_PIN(acc_ext_cmd);  // cmd in (rad/s^2)
HAL_PIN(acc_cmd);      // cmd out (rad/s^2)
HAL_PIN(acc_cmd_lp);      // cmd out (rad/s^2)
HAL_PIN(acc_cmd_hp);      // cmd out (rad/s^2)

HAL_PIN(torque_ext_cmd); // cmd in (Nm)
HAL_PIN(fb_torque_cmd); // feedback cmd out (Nm)
HAL_PIN(ff_torque_cmd); // feedforward cmd out (Nm)
HAL_PIN(torque_cmd); // cmd out (Nm)
HAL_PIN(torque_sum);

HAL_PIN(en);
HAL_PIN(pos_en);
HAL_PIN(vel_en);

HAL_PIN(pos_bw);  // (1/s)

HAL_PIN(vel_bw);  // (1/s)
HAL_PIN(vel_d);
HAL_PIN(vel_g);
HAL_PIN(torque_g);

HAL_PIN(scale);

HAL_PIN(j_lpf);

HAL_PIN(acc_g);

HAL_PIN(j_mot); // motor inertia (kgm^2)
HAL_PIN(f); // (Nm)
HAL_PIN(d); // (Nm/rad/s)
HAL_PIN(j_sys); // system inertia(kgm^2)
HAL_PIN(o); // (Nm)

// user limits
HAL_PIN(max_vel);     // (rad/s)
HAL_PIN(neg_min_vel);     // (rad/s)
HAL_PIN(max_acc);     // (rad/s^2)
HAL_PIN(max_torque);  // (Nm)
HAL_PIN(neg_min_torque);  // (Nm)

HAL_PIN(vel_sat);
HAL_PIN(torque_sat);
HAL_PIN(sat);  // (s)

static void nrt_init(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  //struct pid_ctx_t *ctx      = (struct pid_ctx_t *)ctx_ptr;
  struct pid_pin_ctx_t *pins = (struct pid_pin_ctx_t *)pin_ptr;

  PIN(pos_en)     = 1.0;
  PIN(vel_en)     = 1.0;
  PIN(pos_bw)     = 100.0;   // (1/s)
  PIN(vel_bw)     = 2000.0;  // (1/s)
  PIN(vel_d)      = 2.0;
  PIN(vel_g)      = 0.1;
  PIN(torque_g)   = 1.0;

  PIN(scale) = 1.0;
}

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  //struct pid_ctx_t *ctx      = (struct pid_ctx_t *)ctx_ptr;
  struct pid_pin_ctx_t *pins = (struct pid_pin_ctx_t *)pin_ptr;

  float lpf = LP_HZ(MAX(PIN(j_lpf), 0.0));

  if(PIN(en) > 0.0) {
    if(PIN(pos_en) > 0.0){
      // pos -> vel
      PIN(pos_error) = minus(PIN(pos_ext_cmd), PIN(pos_fb));
      PIN(vel_cmd) = PIN(pos_error) * PIN(pos_bw) * PIN(scale);  // p
      if(ABS(SAT2(PIN(vel_cmd), -PIN(neg_min_vel) * PIN(vel_g), PIN(max_vel) * PIN(vel_g))) > 0.0){ // saturation
        PIN(vel_sat) += period;
      }
      else{
        PIN(vel_sat) = MAX(PIN(vel_sat) - period, 0.0);
      }
      PIN(vel_cmd) = CLAMP(PIN(vel_cmd), -PIN(neg_min_vel) * PIN(vel_g), PIN(max_vel) * PIN(vel_g)); // p clamping
      PIN(vel_cmd) += PIN(vel_ext_cmd); // ff
      PIN(vel_cmd) = CLAMP(PIN(vel_cmd), -PIN(neg_min_vel), PIN(max_vel)); // clamping
    }
    else{
      PIN(pos_error) = 0.0;
      PIN(vel_cmd) = 0.0;
      PIN(vel_sat) = 0.0;
    }
    
    if(PIN(vel_en) > 0.0){
      // vel -> acc
      PIN(vel_error) = PIN(vel_cmd) - PIN(vel_fb);
      PIN(acc_cmd)   = PIN(vel_error) * PIN(vel_bw) * PIN(scale); // p
      PIN(acc_cmd) = LIMIT(PIN(acc_cmd), PIN(max_acc));  // clamping

      PIN(torque_sum) += PIN(vel_error) * PIN(vel_bw) * PIN(vel_bw) * PIN(scale) * PIN(scale) / MAX(PIN(vel_d), 0.1) * (PIN(j_mot) + PIN(j_sys)) * period; // i
    }
    else{
      PIN(vel_error) = 0.0;
      PIN(acc_cmd) = 0.0;
      PIN(torque_sum) = 0.0;
    }


    // acc -> torque
    PIN(acc_cmd_lp) = PIN(acc_cmd) * lpf + PIN(acc_cmd_lp) * (1.0 - lpf);
    PIN(acc_cmd_hp) = PIN(acc_cmd) - PIN(acc_cmd_lp);

    PIN(fb_torque_cmd) = PIN(acc_cmd_hp) * PIN(j_mot) + PIN(acc_cmd_lp) * (PIN(j_mot) + PIN(j_sys));
    PIN(fb_torque_cmd) = CLAMP(PIN(fb_torque_cmd), -PIN(neg_min_torque) * PIN(torque_g), PIN(max_torque) * PIN(torque_g)); // clamping
    PIN(torque_sum) = CLAMP(PIN(torque_sum), -PIN(neg_min_torque) * PIN(torque_g) - PIN(fb_torque_cmd), PIN(max_torque) * PIN(torque_g) - PIN(fb_torque_cmd)); // dynamic integral clamping
    PIN(fb_torque_cmd) += PIN(torque_sum);

    if(ABS(SAT2(PIN(fb_torque_cmd), -PIN(neg_min_torque) * PIN(torque_g) * 0.99, PIN(max_torque) * PIN(torque_g) * 0.99)) > 0.0){ // saturation
      PIN(torque_sat) += period;
    }
    else{
      PIN(torque_sat) = MAX(PIN(torque_sat) - period, 0.0);
    }

    PIN(ff_torque_cmd) = PIN(acc_ext_cmd) * (PIN(j_mot) + PIN(j_sys));
    PIN(ff_torque_cmd) += PIN(d) * PIN(vel_cmd);
    PIN(ff_torque_cmd) += PIN(f) * SIGN2(PIN(vel_cmd), PIN(max_vel) * 0.001);
    PIN(ff_torque_cmd) += PIN(o);
    
    PIN(torque_cmd) = CLAMP(PIN(torque_ext_cmd) + PIN(ff_torque_cmd) + PIN(fb_torque_cmd), -PIN(neg_min_torque), PIN(max_torque));

    // sat
    PIN(sat) = MAX(PIN(vel_sat), PIN(torque_sat));
  }
  else {
    PIN(pos_error)  = 0.0;
    PIN(vel_cmd)    = 0.0;
    PIN(vel_error)  = 0.0;
    PIN(acc_cmd)    = 0.0;
    PIN(torque_cmd) = 0.0;
    PIN(acc_cmd_lp) = 0.0;
    PIN(acc_cmd_hp) = 0.0;
    PIN(vel_sat)    = 0.0;
    PIN(torque_sat) = 0.0;
    PIN(sat)        = 0.0;
    PIN(torque_sum) = 0.0;
    PIN(fb_torque_cmd) = 0.0;
    PIN(ff_torque_cmd) = 0.0;
    PIN(torque_cmd) = 0.0;
  }
}

hal_comp_t pid_comp_struct = {
    .name      = "pid",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = nrt_init,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = 0,
    .pin_count = sizeof(struct pid_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
