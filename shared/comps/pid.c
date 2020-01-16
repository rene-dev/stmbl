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

HAL_PIN(torque_ext_cmd);  // cmd in (Nm)
HAL_PIN(torque_cmd);      // cmd out (Nm)
HAL_PIN(torque_cor_cmd);  // corrected cmd out (Nm)
HAL_PIN(torque_sum);

HAL_PIN(friction);  // (Nm)
HAL_PIN(damping);   // (Nm/rad/s)
HAL_PIN(load);      // (Nm)


HAL_PIN(enable);
HAL_PIN(pos_en);
HAL_PIN(vel_en);
HAL_PIN(acc_en);
HAL_PIN(torque_en);

HAL_PIN(pos_p);  // (1/s)

HAL_PIN(vel_p);  // (1/s)
HAL_PIN(vel_i);
HAL_PIN(vel_g);

HAL_PIN(scale);
HAL_PIN(pos_p_scale);
HAL_PIN(vel_p_scale);
HAL_PIN(vel_i_scale);

HAL_PIN(j_lpf);

HAL_PIN(acc_g);

HAL_PIN(g);  // model limit scaling

HAL_PIN(j_mot);  // (kgm^2)
HAL_PIN(j_load);  // (kgm^2)


// system limits
HAL_PIN(max_vel);     // (rad/s)
HAL_PIN(max_torque);  // (Nm)
HAL_PIN(min_torque);  // (Nm)

// user limits
HAL_PIN(max_usr_vel);     // (rad/s)
HAL_PIN(max_usr_acc);     // (rad/s^2)
HAL_PIN(max_usr_torque);  // (Nm)

HAL_PIN(vel_sat);
HAL_PIN(acc_sat);
HAL_PIN(torque_sat);
HAL_PIN(saturated);  // (s)

struct pid_ctx_t {
  float sat;         //saturated time
  float torque_sum;  //integrator
  float acc_cmd_lp;
};

static void nrt_init(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct pid_ctx_t *ctx      = (struct pid_ctx_t *)ctx_ptr;
  struct pid_pin_ctx_t *pins = (struct pid_pin_ctx_t *)pin_ptr;

  ctx->sat        = 0.0;
  ctx->torque_sum = 0.0;
  ctx->acc_cmd_lp = 0.0;
  PIN(pos_en)     = 1.0;
  PIN(vel_en)     = 1.0;
  PIN(acc_en)     = 1.0;
  PIN(torque_en)  = 1.0;
  PIN(pos_p)      = 100.0;   // (1/s)
  PIN(vel_p)      = 2000.0;  // (1/s)
  PIN(vel_i)      = 10.0;
  PIN(vel_g)      = 1.0;
  PIN(acc_g)      = 1.0;
  PIN(g)          = 0.99;  // model limit scaling

  PIN(scale) = 1.0;
  PIN(pos_p_scale) = 1.0;
  PIN(vel_p_scale) = 1.0;
  PIN(vel_i_scale) = 1.0;
}

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct pid_ctx_t *ctx      = (struct pid_ctx_t *)ctx_ptr;
  struct pid_pin_ctx_t *pins = (struct pid_pin_ctx_t *)pin_ptr;

  float mot_j = MAX(PIN(j_mot), 0.0);
  float load_j = MAX(PIN(j_load), 0.0);
  float lpf = LP_HZ(MAX(PIN(j_lpf), 0.0));
  float g = CLAMP(PIN(g), 0.0, 1.0);

  float torque_ext_cmd = PIN(torque_ext_cmd);
  float torque_min     = MAX(PIN(min_torque) * g, -PIN(max_usr_torque));
  float torque_max     = MIN(PIN(max_torque) * g, PIN(max_usr_torque));
  float torque_cmd;
  float torque_sat;

  float acc_ext_cmd = PIN(acc_ext_cmd);
  float acc_min     = -PIN(max_usr_acc);  //MAX(torque_min / j, -PIN(max_usr_acc));
  float acc_max     = PIN(max_usr_acc);   //MIN(torque_max / j, PIN(max_usr_acc));
  float acc_cmd;
  float acc_cmd_hp;
  float acc_sat;

  float vel_ext_cmd = PIN(vel_ext_cmd);
  float vel_fb      = PIN(vel_fb);
  float vel_min     = MAX(-PIN(max_vel) * g, -PIN(max_usr_vel));  //MAX(MAX(velfb + accmin * period, -PIN(max_vel) * p) , -PIN(max_usr_vel));
  float vel_max     = MIN(PIN(max_vel) * g, PIN(max_usr_vel));    //MIN(MIN(velfb + accmax * period, PIN(max_vel) * p), PIN(max_usr_vel));
  float vel_cmd;
  float vel_error;
  float vel_sat;

  float pos_ext_cmd = PIN(pos_ext_cmd);
  float pos_fb      = PIN(pos_fb);
  float pos_error   = minus(pos_ext_cmd, pos_fb);

  float pos_p = MAX(PIN(pos_p), 0.0);
  float vel_p = MAX(PIN(vel_p), 0.0);
  float vel_i = MAX(PIN(vel_i), 0.0);
  float vel_g = CLAMP(PIN(vel_g), 0.0, 1.0);
  float acc_g = CLAMP(PIN(acc_g), 0.0, 1.0);

  float scale = CLAMP(PIN(scale), 0, 1);
  pos_p *= scale + (1 - scale) * PIN(pos_p_scale);
  vel_p *= scale + (1 - scale) * PIN(vel_p_scale);
  vel_i *= scale + (1 - scale) * PIN(vel_i_scale);  

  float pos_en    = PIN(pos_en);
  float vel_en    = PIN(vel_en);
  float acc_en    = PIN(acc_en);
  float torque_en = PIN(torque_en);

  if(PIN(enable) > 0.0) {
    if(torque_en <= 0.0) {
      pos_en         = 0.0;
      vel_en         = 0.0;
      acc_en         = 0.0;
      torque_ext_cmd = 0.0;
    }
    if(acc_en <= 0.0) {
      pos_en      = 0.0;
      vel_en      = 0.0;
      acc_g       = 0.0;
      acc_ext_cmd = 0.0;
    }
    if(vel_en <= 0.0) {
      pos_en          = 0.0;
      pos_error       = 0.0;
      vel_ext_cmd     = 0.0;
      vel_fb          = 0.0;
      ctx->torque_sum = 0.0;
    }
    if(pos_en <= 0.0) {
      pos_p     = 0.0;
      pos_error = 0.0;
    }

    // pos -> vel
    vel_cmd = pos_error * pos_p;  // p
    //vel_cmd = CLAMP(vel_cmd, vel_fb + acc_min * period, vel_fb + acc_max * period); // adaptive loop clamping
    vel_cmd = CLAMP(vel_cmd, vel_min * vel_g, vel_max * vel_g);  // p clamping
    vel_cmd += vel_ext_cmd;                                      // ff
    vel_sat = SAT2(vel_cmd, vel_min * 0.99, vel_max * 0.99);  // TODO include vel_g
    vel_cmd = CLAMP(vel_cmd, vel_min, vel_max);  // clamping

    // vel -> acc
    vel_error = vel_cmd - vel_fb;
    acc_cmd   = vel_error * vel_p;                                 // p
    acc_cmd   = CLAMP(acc_cmd, acc_min * acc_g, acc_max * acc_g);  // p clamping
    acc_cmd += acc_ext_cmd;                                        // ff
    acc_sat = SAT2(acc_cmd, acc_min * 0.99, acc_max * 0.99);  // TODO include acc_g
    acc_cmd = CLAMP(acc_cmd, acc_min, acc_max);  // clamping

    // acc -> torque
    ctx->acc_cmd_lp = acc_cmd * lpf + ctx->acc_cmd_lp * (1.0 - lpf);
    acc_cmd_hp = acc_cmd - ctx->acc_cmd_lp;
    torque_cmd = acc_cmd_hp * mot_j + ctx->acc_cmd_lp * (mot_j + load_j) + torque_ext_cmd;            // ff
    torque_cmd = CLAMP(torque_cmd, torque_min, torque_max);                                      // clamping
    ctx->torque_sum += vel_error * period * vel_i;                                               // i
    ctx->torque_sum = CLAMP(ctx->torque_sum, torque_min - torque_cmd, torque_max - torque_cmd);  // dynamic integral clamping
    torque_cmd += ctx->torque_sum;
    torque_sat = SAT2(torque_cmd, torque_min * 0.99, torque_max * 0.99);


    if(vel_i == 0.0) {
      ctx->torque_sum = 0.0;
    }

    PIN(torque_cmd) = torque_cmd;
    if(vel_fb != 0.0) {  // friction ff
      torque_cmd += SIGN(vel_fb) * PIN(friction);
    } else {
      torque_cmd += SIGN(torque_cmd) * PIN(friction);
    }
    torque_cmd += vel_fb * PIN(damping);  // damping ff
    torque_cmd -= PIN(load);              // load ff
    torque_sat += 2 * SAT2(torque_cmd, torque_min, torque_max);
    torque_cmd = CLAMP(torque_cmd, torque_min, torque_max);  // min/max clamping

    // sat
    if(ABS(vel_sat) + ABS(acc_sat) + ABS(torque_sat) > 0.0) {
      ctx->sat += period;
    } else {
      ctx->sat -= period;
    }
    ctx->sat = MAX(ctx->sat, 0.0);
  } else {
    ctx->torque_sum = 0.0;
    // pos_error       = 0.0;
    vel_cmd         = 0.0;
    vel_error       = 0.0;
    acc_cmd         = 0.0;
    torque_cmd      = 0.0;
    vel_sat         = 0.0;
    acc_sat         = 0.0;
    torque_sat      = 0.0;
    ctx->sat        = 0.0;
    PIN(torque_cmd) = 0.0;
  }

  PIN(pos_error)      = pos_error;
  PIN(vel_cmd)        = vel_cmd;
  PIN(vel_error)      = vel_error;
  PIN(acc_cmd)        = acc_cmd;
  PIN(torque_cor_cmd) = torque_cmd;
  PIN(torque_sum)     = ctx->torque_sum;

  PIN(vel_sat)    = vel_sat;
  PIN(acc_sat)    = acc_sat;
  PIN(torque_sat) = torque_sat;
  PIN(saturated)  = ctx->sat;
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
    .ctx_size  = sizeof(struct pid_ctx_t),
    .pin_count = sizeof(struct pid_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
