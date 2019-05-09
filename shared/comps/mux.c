#include "hal.h"
#include "defines.h"

HAL_COMP(mux);

HAL_PINA(in, 80);
HAL_PINA(out, 10);
HAL_PIN(mux);



static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct mux_pin_ctx_t *pins = (struct mux_pin_ctx_t *)pin_ptr;
  for(int i = 0; i < 10; i++){
    PINA(out, i) = PINA(in, i + (int)MIN(MAX(PIN(mux), 0.0), 7.0) * 10);
  }
}

hal_comp_t mux_comp_struct = {
    .name      = "mux",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = 0,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = 0,
    .pin_count = sizeof(struct mux_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};