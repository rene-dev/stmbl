#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"

HAL_COMP(dc);

//dc voltage input
HAL_PIN(uq);
//U V W output
HAL_PIN(u);
HAL_PIN(v);
HAL_PIN(w);

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  // struct hv_ctx_t * ctx = (struct hv_ctx_t *)ctx_ptr;
  struct dc_pin_ctx_t *pins = (struct dc_pin_ctx_t *)pin_ptr;

  PIN(u) = PIN(uq) / 2.0;
  PIN(v) = -PIN(uq) / 2.0;
  PIN(w) = 0;
}

hal_comp_t dc_comp_struct = {
    .name      = "dc",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = 0,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = 0,
    .pin_count = sizeof(struct dc_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
