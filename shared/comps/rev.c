#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"

HAL_COMP(rev);

HAL_PIN(in);
HAL_PIN(out);
HAL_PIN(in_d);
HAL_PIN(out_d);
HAL_PIN(rev);

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  // struct rev_ctx_t * ctx = (struct rev_ctx_t *)ctx_ptr;
  struct rev_pin_ctx_t *pins = (struct rev_pin_ctx_t *)pin_ptr;

  if(PIN(rev) > 0.0) {
    PIN(out)   = minus(0, PIN(in));
    PIN(out_d) = -PIN(in_d);
  } else {
    PIN(out)   = PIN(in);
    PIN(out_d) = PIN(in_d);
  }
}

hal_comp_t rev_comp_struct = {
    .name      = "rev",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = 0,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = 0,
    .pin_count = sizeof(struct rev_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
