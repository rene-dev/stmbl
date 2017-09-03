#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"

HAL_COMP(acim_ttc);

// motor values
HAL_PIN(torque_n);
HAL_PIN(cur_n);
HAL_PIN(vel_n);
HAL_PIN(freq_n);
HAL_PIN(polecount);

// torque cmd in
HAL_PIN(torque);
HAL_PIN(vel);

// cur cmd out
HAL_PIN(cur);
HAL_PIN(freq);
HAL_PIN(slip)
HAL_PIN(pos);

static void rt_func(float period, volatile void * ctx_ptr, volatile hal_pin_inst_t * pin_ptr){
   // struct acim_ttc_ctx_t * ctx = (struct acim_ttc_ctx_t *)ctx_ptr;
   struct acim_ttc_pin_ctx_t * pins = (struct acim_ttc_pin_ctx_t *)pin_ptr;

   float poles = MAX(PIN(polecount), 1.0);
   float torque = PIN(torque);
   float slip_n = PIN(freq_n) - PIN(vel_n) / 2.0 / M_PI * poles;
   float torque_n = MAX(PIN(torque_n), 0.01);

   float slip = slip_n / torque_n * torque;
   slip = slip_n * SIGN(torque);
   float freq = PIN(vel) / 2.0 / M_PI * poles + slip;
   PIN(cur) = PIN(cur_n) / torque_n * torque;
   PIN(freq) = freq;
   PIN(slip) = slip;
   PIN(pos) = mod(PIN(pos) + freq * period * 2.0 * M_PI);
}

hal_comp_t acim_ttc_comp_struct = {
  .name = "acim_ttc",
  .nrt = 0,
  .rt = rt_func,
  .frt = 0,
  .nrt_init = 0,
  .rt_start = 0,
  .frt_start = 0,
  .rt_stop = 0,
  .frt_stop = 0,
  .ctx_size = 0,
  .pin_count = sizeof(struct acim_ttc_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
