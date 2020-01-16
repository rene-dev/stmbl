#include "hal.h"
#include "defines.h"

HAL_COMP(auto_ac);

HAL_PIN(in);
HAL_PIN(out);
HAL_PIN(lpf);
HAL_PIN(th);
HAL_PIN(offset);

static void nrt_init(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  // struct auto_ac_ctx_t * ctx = (struct auto_ac_ctx_t *)ctx_ptr;
  struct auto_ac_pin_ctx_t *pins = (struct auto_ac_pin_ctx_t *)pin_ptr;
  PIN(lpf) = 1.0;
  PIN(th) = -1.0;
  PIN(offset) = 0.0;
}


static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct auto_ac_pin_ctx_t *pins = (struct auto_ac_pin_ctx_t *)pin_ptr;

  if(ABS(PIN(in)) <= PIN(th)){
    PIN(offset) = PIN(in) * LP_HZ(PIN(lpf)) + (1.0 - LP_HZ(PIN(lpf))) * PIN(offset);
  }

  PIN(out) = PIN(in) - PIN(offset);
}

hal_comp_t auto_ac_comp_struct = {
    .name      = "auto_ac",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = nrt_init,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = 0,
    .pin_count = sizeof(struct auto_ac_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};