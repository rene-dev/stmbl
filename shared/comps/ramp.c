#include "commands.h"
#include "hal.h"
#include "defines.h"

HAL_COMP(ramp);

// input
HAL_PIN(vel_ext_cmd);

HAL_PIN(max_speed);
HAL_PIN(max_slip_speed);
HAL_PIN(max_acc);

// output
HAL_PIN(vel_cmd);
HAL_PIN(mode);

static void rt_func(float period, volatile void *ctx_ptr, volatile hal_pin_inst_t *pin_ptr) {
  // struct ramp_ctx_t *ctx      = (struct ramp_ctx_t *)ctx_ptr;
  struct ramp_pin_ctx_t *pins = (struct ramp_pin_ctx_t *)pin_ptr;
  
  float vel_ext_cmd = LIMIT(PIN(vel_ext_cmd), PIN(max_speed));

  float vel_error = vel_ext_cmd - PIN(vel_cmd);

  PIN(vel_cmd) += LIMIT(vel_error, PIN(max_acc) * period);

  if(ABS(PIN(vel_cmd)) > PIN(max_slip_speed)){
    PIN(mode) = 1;
  }
  else{
    PIN(mode) = 0;
  }
}

hal_comp_t ramp_comp_struct = {
    .name      = "ramp",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = 0,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = 0,
    .pin_count = sizeof(struct ramp_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};