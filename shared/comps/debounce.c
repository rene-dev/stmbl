#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"

HAL_COMP(debounce);

HAL_PIN(in);
HAL_PIN(out);
HAL_PIN(debounce_time);
HAL_PIN(timer);

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  //struct debounce_ctx_t *ctx      = (struct debounce_ctx_t *)ctx_ptr;
  struct debounce_pin_ctx_t *pins = (struct debounce_pin_ctx_t *)pin_ptr;

  if(PIN(in) > 0.0){
    PIN(timer) += period;
  }
  else{
    PIN(timer) -= period;
  }

  if(PIN(timer) > PIN(debounce_time)){
    PIN(timer) = PIN(debounce_time);
    PIN(out) = 1.0;
  }
  else if(PIN(timer) <= 0.0){
    PIN(timer) = 0.0;
    PIN(out) = 0.0;
  }
}

hal_comp_t debounce_comp_struct = {
    .name      = "debounce",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = 0,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = 0,
    .pin_count = sizeof(struct debounce_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
