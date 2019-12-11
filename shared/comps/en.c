#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"

HAL_COMP(en);

HAL_PIN(en_in);
HAL_PIN(en_out0);
HAL_PIN(en_out1);
HAL_PIN(fault);

HAL_PIN(time);
HAL_PIN(timer);

static void nrt_init(void *ctx_ptr, hal_pin_inst_t *pin_ptr){
  struct en_pin_ctx_t *pins = (struct en_pin_ctx_t *)pin_ptr;
  PIN(time) = 5;
}

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  //struct en_ctx_t *ctx      = (struct en_ctx_t *)ctx_ptr;
  struct en_pin_ctx_t *pins = (struct en_pin_ctx_t *)pin_ptr;

  if(PIN(en_in) <= 0.0){
    PIN(timer) = 0.0;
  }
  else{
    PIN(timer) += period;
  }

  if(PIN(fault) > 0.0){
    PIN(timer) = 0.0;
  }

  if(PIN(timer) > PIN(time) / 2.0){
    PIN(en_out0) = 1.0;
  }
  else{
    PIN(en_out0) = 0.0;
  }

  if(PIN(timer) > PIN(time)){
    PIN(en_out1) = 1.0;
    PIN(timer) = PIN(time);
  }
  else{
    PIN(en_out1) = 0.0;
  }
}

hal_comp_t en_comp_struct = {
    .name      = "en",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = nrt_init,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = 0,
    .pin_count = sizeof(struct en_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
