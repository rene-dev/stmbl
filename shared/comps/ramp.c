#include "commands.h"
#include "hal.h"
#include "defines.h"

HAL_COMP(ramp);

// input
HAL_PIN(vel_ext_cmd);
HAL_PIN(vel_fb);

HAL_PIN(max_torque);
HAL_PIN(torque_fb);
HAL_PIN(scale);
HAL_PIN(max_acc);

// output
HAL_PIN(vel_cmd);

static void rt_func(float period, volatile void *ctx_ptr, volatile hal_pin_inst_t *pin_ptr) {
  // struct ramp_ctx_t *ctx      = (struct ramp_ctx_t *)ctx_ptr;
  struct ramp_pin_ctx_t *pins = (struct ramp_pin_ctx_t *)pin_ptr;

  float vel_error = PIN(vel_ext_cmd) - PIN(vel_fb);

  PIN(vel_cmd) = PIN(vel_fb) + LIMIT(vel_error, PIN(max_acc) * period) * LIMIT(5.0 * (1.0 - ABS(PIN(torque_fb)) / MAX(PIN(scale) * PIN(max_torque), 0.01)), 1.0);
  ;
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