#include "hal.h"

HAL_COMP(ac);

HAL_PIN(in);
HAL_PIN(ac);
HAL_PIN(out);

struct ac_ctx_t {
  float avg;
};

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct ac_ctx_t *ctx      = (struct ac_ctx_t *)ctx_ptr;
  struct ac_pin_ctx_t *pins = (struct ac_pin_ctx_t *)pin_ptr;

  float i = PIN(in);
  float a = PIN(ac);

  ctx->avg = i * a + ctx->avg * (1 - a);

  PIN(out) = i - ctx->avg;
}

const hal_comp_t ac_comp_struct = {
    .name      = "ac",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = 0,
    .hw_init   = 0,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = sizeof(struct ac_ctx_t),
    .pin_count = sizeof(struct ac_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
