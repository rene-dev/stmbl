#include "hal.h"

HAL_COMP(or);

HAL_PIN(in0);
HAL_PIN(in1);
HAL_PIN(in2);
HAL_PIN(in3);

HAL_PIN(out);

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct or_pin_ctx_t *pins = (struct or_pin_ctx_t *)pin_ptr;

  PIN(out) = (PIN(in0) > 0.0) | (PIN(in1) > 0.0) | (PIN(in2) > 0.0) | (PIN(in3) > 0.0);
}

const hal_comp_t or_comp_struct = {
    .name      = "or",
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
    .pin_count = sizeof(struct or_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
