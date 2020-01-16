#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"

HAL_COMP(psi);

HAL_PIN(vel);
HAL_PIN(dc_volt);
HAL_PIN(u);
HAL_PIN(v);
HAL_PIN(w);
HAL_PIN(polecount);
HAL_PIN(psi);
HAL_PIN(max_psi);

struct psi_ctx_t {
  float max_f;
};



static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct psi_ctx_t *ctx      = (struct psi_ctx_t *)ctx_ptr;
  struct psi_pin_ctx_t *pins = (struct psi_pin_ctx_t *)pin_ptr;

  float f  = ABS(PIN(vel)) / 2.0 / M_PI * PIN(polecount);
  float u  = PIN(dc_volt);
  float u2 = MAX3(PIN(u), PIN(v), PIN(w)) - MIN3(PIN(u), PIN(v), PIN(w));
  u        = MIN(u, u2) / M_SQRT3;  // TODO: fix
  if(f > 1.0) {
    PIN(psi) = u / f / 2.0 / M_PI;
    if(ctx->max_f < f) {
      ctx->max_f   = f;
      PIN(max_psi) = PIN(psi);
    }
  }
  ctx->max_f *= 0.999999;
}

const hal_comp_t psi_comp_struct = {
    .name      = "psi",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = 0,
    .hw_init   = 0,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = sizeof(struct psi_ctx_t),
    .pin_count = sizeof(struct psi_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
