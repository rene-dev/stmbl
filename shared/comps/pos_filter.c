#include "hal.h"
#include "angle.h"
#include "defines.h"

HAL_COMP(pos_filter);

HAL_PIN(pos_in);
HAL_PIN(vel_in);

HAL_PIN(pos_out);
HAL_PIN(vel_out);
HAL_PIN(acc_out);

HAL_PIN(bandwidth);

HAL_PIN(en);

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  //struct pos_filter_ctx_t *ctx      = (struct pos_filter_ctx_t *)ctx_ptr;
  struct pos_filter_pin_ctx_t *pins = (struct pos_filter_pin_ctx_t *)pin_ptr;

  float ki = 2.0 * MIN(PIN(bandwidth), 1.0 / period / 2.0);
  float kp = 0.25 * ki * ki;

  float pos_error = minus(PIN(pos_in), PIN(pos_out));
  float vel_error = PIN(vel_in) - PIN(vel_out);
  

  if(PIN(en) > 0.0){
    PIN(acc_out) = kp * pos_error + ki * vel_error;
    PIN(vel_out) += period * PIN(acc_out);
    PIN(pos_out) = mod(PIN(pos_out) + period * PIN(vel_out));
  }
  else{
    PIN(acc_out) = 0;
    PIN(vel_out) = PIN(vel_in);
    PIN(pos_out) = PIN(pos_in);
  }
}

const hal_comp_t pos_filter_comp_struct = {
    .name      = "pos_filter",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = 0,
    .hw_init   = 0,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = 0,
    .pin_count = sizeof(struct pos_filter_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};