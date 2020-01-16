#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"

HAL_COMP(veltime);

HAL_PIN(pos);
HAL_PIN(old_pos);
HAL_PIN(timer);
HAL_PIN(vel);
HAL_PIN(vel_lp);

HAL_PIN(max_time);
HAL_PIN(lpf)


static void nrt_init(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  // struct veltime_ctx_t *ctx      = (struct veltime_ctx_t *)ctx_ptr;
  struct veltime_pin_ctx_t *pins = (struct veltime_pin_ctx_t *)pin_ptr;
  
  PIN(max_time) = 0.1;

  PIN(lpf) = 100.0;
}


static void frt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  // struct veltime_ctx_t *ctx      = (struct veltime_ctx_t *)ctx_ptr;
  struct veltime_pin_ctx_t *pins = (struct veltime_pin_ctx_t *)pin_ptr;

  PIN(timer) += period;

  if(PIN(pos) != PIN(old_pos)){
    PIN(vel) = minus(PIN(pos), PIN(old_pos)) / PIN(timer);
    PIN(old_pos) = PIN(pos);
    PIN(timer) = 0.0;
  }

  if(PIN(timer) > PIN(max_time)){
    PIN(timer) = PIN(max_time);
    PIN(vel) = 0.0;
  }

    PIN(vel_lp) = PIN(vel) * LP_HZ(PIN(lpf)) + PIN(vel_lp) * (1.0 - LP_HZ(PIN(lpf)));

}

hal_comp_t veltime_comp_struct = {
    .name      = "veltime",
    .nrt       = 0,
    .rt        = 0,
    .frt       = frt_func,
    .nrt_init  = nrt_init,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = 0,
    .pin_count = sizeof(struct veltime_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};