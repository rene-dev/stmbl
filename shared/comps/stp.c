#include "stp_comp.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"

HAL_COMP(stp);

HAL_PIN(target);
HAL_PIN(vel_ext_cmd);
HAL_PIN(acc_ext_cmd);
HAL_PIN(jog);

HAL_PIN(pos);
HAL_PIN(mpos);
HAL_PIN(vel_cmd);
HAL_PIN(acc_cmd);

HAL_PIN(max_pos);
HAL_PIN(min_pos);
HAL_PIN(max_vel);
HAL_PIN(max_acc);

HAL_PIN(dtg);
HAL_PIN(ttg);
HAL_PIN(at_target);

static void nrt_init(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  // struct stp_ctx_t *ctx      = (struct stp_ctx_t *)ctx_ptr;
  struct stp_pin_ctx_t *pins = (struct stp_pin_ctx_t *)pin_ptr;
  PIN(target)                = 0.0;
  PIN(max_vel)               = 1.0 * 2.0 * M_PI;
  PIN(max_acc)               = 10.0 * 2.0 * M_PI;
  PIN(max_pos)               = 10.0 * 2.0 * M_PI;
  PIN(min_pos)               = -10.0 * 2.0 * M_PI;
}

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  // struct stp_ctx_t *ctx      = (struct stp_ctx_t *)ctx_ptr;
  struct stp_pin_ctx_t *pins = (struct stp_pin_ctx_t *)pin_ptr;

  float max_acc = MAX(PIN(max_acc), 0.01);
  float max_vel = MAX(PIN(max_vel), 0.01);
  
  // jog input
  float acc_ext_cmd = LIMIT(PIN(acc_ext_cmd), PIN(max_acc) * 0.99);
  float vel_ext_cmd = LIMIT(PIN(vel_ext_cmd) + PIN(jog) * max_vel, PIN(max_vel) * 0.99);
  PIN(target) += vel_ext_cmd * period + acc_ext_cmd * period * period / 2.0;
  PIN(vel_ext_cmd) += acc_ext_cmd * period;
  PIN(vel_ext_cmd) = LIMIT(PIN(vel_ext_cmd), max_vel * 0.99);

  // pos input
  float target = CLAMP(PIN(target), PIN(min_pos), PIN(max_pos));
  PIN(target) = target;

  // update
  PIN(pos) += PIN(vel_cmd) * period + PIN(acc_cmd) * period * period / 2.0;
  PIN(vel_cmd) += PIN(acc_cmd) * period;

  // distance to go
  float to_go = target - PIN(pos);

  // time to go
  float time_to_go = sqrtf(2.0 * ABS(to_go) / max_acc);

  // real time to go
  int periods_to_go = ceilf(time_to_go / period);

  // calc new acc
  float acc = 0.0;
  if(periods_to_go){
    acc = 2.0 * to_go / (periods_to_go * periods_to_go * period * period);
  }

  float vel = acc * periods_to_go * period;
  vel = LIMIT(vel, max_vel);
  acc = (vel - PIN(vel_cmd)) / period;
  acc = LIMIT(acc, max_acc);

  if(time_to_go < period && ABS(PIN(vel_cmd)) < max_acc * period){
    acc = 0.0;
    PIN(vel_cmd) = 0.0;
    PIN(pos) = PIN(target);
  }

  PIN(acc_cmd) = acc;
  PIN(dtg) = to_go;
  PIN(ttg) = periods_to_go * period;
  PIN(mpos) = mod(PIN(pos));

  if((periods_to_go <= 1) & (ABS(PIN(vel_cmd)) < max_vel / 10000.0)){
    PIN(at_target) = 1.0;
  }
  else{
    PIN(at_target) = 0.0;
  }
}

const hal_comp_t stp_comp_struct = {
    .name      = "stp",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = nrt_init,
    .hw_init   = 0,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = 0,
    .pin_count = sizeof(struct stp_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
