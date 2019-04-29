#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"

HAL_COMP(pmsm_limits);

// motor values
HAL_PIN(psi);
HAL_PIN(r);
HAL_PIN(ld);
HAL_PIN(lq);
HAL_PIN(polecount);

// sys limit
HAL_PIN(ac_volt);

// next min max out -> pid, curpid
HAL_PIN(next_max_cur);
HAL_PIN(next_max_torque);
HAL_PIN(next_min_cur);
HAL_PIN(next_min_torque);

// min max out @ current vel
HAL_PIN(max_cur);
HAL_PIN(max_torque);
HAL_PIN(min_cur);
HAL_PIN(min_torque);

// abs max out
HAL_PIN(abs_max_cur);
HAL_PIN(abs_max_torque);
HAL_PIN(abs_max_vel);

// pmsm feedback
HAL_PIN(iq);
HAL_PIN(indq);

static void nrt_init(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  // struct pmsm_limits_ctx_t * ctx = (struct pmsm_limits_ctx_t *)ctx_ptr;
  struct pmsm_limits_pin_ctx_t *pins = (struct pmsm_limits_pin_ctx_t *)pin_ptr;

  PIN(psi)       = 0.01;
  PIN(r)         = 1.0;
  PIN(ld)        = 0.001;
  PIN(lq)        = 0.001;
  PIN(polecount) = 1.0;
}

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  // struct pmsm_limits_ctx_t * ctx = (struct pmsm_limits_ctx_t *)ctx_ptr;
  struct pmsm_limits_pin_ctx_t *pins = (struct pmsm_limits_pin_ctx_t *)pin_ptr;

  float p = (int)MAX(PIN(polecount), 1.0);
  //float ld = MAX(PIN(ld), 0.0001);
  float lq  = MAX(PIN(lq), 0.0001);
  float psi = MAX(PIN(psi), 0.01);
  float r   = MAX(PIN(r), 0.001);
  float iq  = PIN(iq);  // TODO id

  float indq = PIN(indq);  // TODO vel * (psi + id ld)

  float volt           = PIN(ac_volt);
  float abs_max_cur    = volt / r;
  float abs_max_vel    = volt / psi / p;
  float abs_max_torque = 3.0 / 2.0 * p * psi * abs_max_cur;

  float next_max_cur    = iq + (volt - r * iq - indq) / lq * period * 2.0 / 3.0;
  float next_min_cur    = iq + (-volt - r * iq - indq) / lq * period * 2.0 / 3.0;
  float next_max_torque = 3.0 / 2.0 * p * (psi * next_max_cur);
  float next_min_torque = 3.0 / 2.0 * p * (psi * next_min_cur);

  float max_cur    = (volt - indq) / r;
  float min_cur    = (-volt - indq) / r;
  float max_torque = 3.0 / 2.0 * p * (psi * max_cur);
  float min_torque = 3.0 / 2.0 * p * (psi * min_cur);


  PIN(abs_max_cur)     = abs_max_cur;
  PIN(abs_max_vel)     = abs_max_vel;
  PIN(abs_max_torque)  = abs_max_torque;
  PIN(next_max_cur)    = next_max_cur;
  PIN(next_min_cur)    = next_min_cur;
  PIN(next_max_torque) = next_max_torque;
  PIN(next_min_torque) = next_min_torque;
  PIN(max_cur)         = max_cur;
  PIN(min_cur)         = min_cur;
  PIN(max_torque)      = max_torque;
  PIN(min_torque)      = min_torque;
}

hal_comp_t pmsm_limits_comp_struct = {
    .name      = "pmsm_limits",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = nrt_init,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = 0,
    .pin_count = sizeof(struct pmsm_limits_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
