#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"

HAL_COMP(move);

HAL_PIN(fwd);
HAL_PIN(rev);
HAL_PIN(scale);
HAL_PIN(out);

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct move_pin_ctx_t *pins = (struct move_pin_ctx_t *)pin_ptr;
  PIN(out)                    = PIN(scale) * PIN(fwd) + PIN(scale) * PIN(rev) * -1;
}

hal_comp_t move_comp_struct = {
    .name      = "move",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = 0,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = 0,
    .pin_count = sizeof(struct move_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
