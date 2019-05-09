#include "hal.h"

HAL_COMP(mad);

HAL_PIN(in);
HAL_PIN(mult);
HAL_PIN(add);

HAL_PIN(out);

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct mad_pin_ctx_t *pins = (struct mad_pin_ctx_t *)pin_ptr;

  PIN(out) = PIN(in) * PIN(mult) + PIN(add);
}

const hal_comp_t mad_comp_struct = {
    .name      = "mad",
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
    .pin_count = sizeof(struct mad_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
