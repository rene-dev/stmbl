#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"

HAL_COMP(d);

HAL_PIN(pwm_volt);//max volt in
HAL_PIN(uq);//volt in
HAL_PIN(limit);//voltage limit
HAL_PIN(id);//d current out
HAL_PIN(p);//gain

static void rt_func(float period, volatile void *ctx_ptr, volatile hal_pin_inst_t *pin_ptr) {
  // struct rev_ctx_t * ctx = (struct rev_ctx_t *)ctx_ptr;
  struct d_pin_ctx_t *pins = (struct d_pin_ctx_t *)pin_ptr;
  float reserve = PIN(pwm_volt) - ABS(PIN(uq)) - PIN(limit);
  if(reserve <= 0){
    PIN(id) = reserve * PIN(p);
  }else{
    PIN(id) = 0;
  }
}

hal_comp_t d_comp_struct = {
    .name      = "d",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = 0,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = 0,
    .pin_count = sizeof(struct d_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
