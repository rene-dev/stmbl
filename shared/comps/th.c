#include "hal.h"

HAL_COMP(th);

HAL_PIN(in);
HAL_PIN(in_lpf);
HAL_PIN(in_lp);
HAL_PIN(min);
HAL_PIN(max);
HAL_PIN(out);
HAL_PIN(out_not);

static void nrt_init(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct th_pin_ctx_t *pins = (struct th_pin_ctx_t *)pin_ptr;
  PIN(in_lpf) = 1.0;
}

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct th_pin_ctx_t *pins = (struct th_pin_ctx_t *)pin_ptr;

  PIN(in_lp) += (PIN(in) - PIN(in_lp)) * PIN(in_lpf);

  if(PIN(in_lp) > PIN(min) && PIN(in_lp) < PIN(max)){
    PIN(out) = 1;
    PIN(out_not) = 0;
  }
  else{
    PIN(out) = 0;
    PIN(out_not) = 1;
  }
}

const hal_comp_t th_comp_struct = {
    .name      = "th",
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
    .pin_count = sizeof(struct th_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
