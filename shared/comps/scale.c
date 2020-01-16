#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"

HAL_COMP(scale);

HAL_PIN(in0);
HAL_PIN(out0);
HAL_PIN(in1);
HAL_PIN(out1);
HAL_PIN(scale);

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  // struct scale_ctx_t * ctx = (struct scale_ctx_t *)ctx_ptr;
  struct scale_pin_ctx_t *pins = (struct scale_pin_ctx_t *)pin_ptr;

  PIN(out0) = PIN(in0) * PIN(scale);
  PIN(out1) = PIN(in1) / MAX(PIN(scale), 0.001);
}

hal_comp_t scale_comp_struct = {
    .name      = "scale",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = 0,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = 0,
    .pin_count = sizeof(struct scale_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
