#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"

HAL_COMP(pole);

HAL_PIN(pos);
HAL_PIN(cpos);
HAL_PIN(p);

static void rt_func(float period, volatile void *ctx_ptr, volatile hal_pin_inst_t *pin_ptr) {
  // struct pole_ctx_t * ctx = (struct pole_ctx_t *)ctx_ptr;
  struct pole_pin_ctx_t *pins = (struct pole_pin_ctx_t *)pin_ptr;

  PIN(cpos) = mod(PIN(pos) * PIN(p));
}

hal_comp_t pole_comp_struct = {
    .name      = "pole",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = 0,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = 0,
    .pin_count = sizeof(struct pole_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
