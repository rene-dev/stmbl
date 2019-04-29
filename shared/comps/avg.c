#include "hal.h"
#include "defines.h"

HAL_COMP(avg);

HAL_PIN(in);
HAL_PIN(out);
HAL_PIN(lpf);
HAL_PIN(mult);
HAL_PIN(offset);

static void nrt_init(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  // struct avg_ctx_t * ctx = (struct avg_ctx_t *)ctx_ptr;
  struct avg_pin_ctx_t *pins = (struct avg_pin_ctx_t *)pin_ptr;
  PIN(lpf) = 100;
  PIN(out) = 0.0;
  PIN(offset) = 0.0;
}


static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct avg_pin_ctx_t *pins = (struct avg_pin_ctx_t *)pin_ptr;
  PIN(out) = (PIN(in) * PIN(mult) + PIN(offset)) * LP_HZ(PIN(lpf)) + (1.0 - LP_HZ(PIN(lpf))) * PIN(out);
}

hal_comp_t avg_comp_struct = {
    .name      = "avg",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = nrt_init,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = 0,
    .pin_count = sizeof(struct avg_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};