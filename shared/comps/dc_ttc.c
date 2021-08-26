#include "dc_ttc_comp.h"
#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"

HAL_COMP(dc_ttc);

// motor values
HAL_PIN(psi);

// torque cmd in
HAL_PIN(torque);

// cur cmd out
HAL_PIN(cur);

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  // struct dc_ttc_ctx_t * ctx = (struct dc_ttc_ctx_t *)ctx_ptr;
  struct dc_ttc_pin_ctx_t *pins = (struct dc_ttc_pin_ctx_t *)pin_ptr;

  float psi_m  = MAX(PIN(psi), 0.001);
  float torque = PIN(torque);

  PIN(cur) = torque / psi_m;
}

hal_comp_t dc_ttc_comp_struct = {
    .name      = "dc_ttc",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = 0,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = 0,
    .pin_count = sizeof(struct dc_ttc_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
