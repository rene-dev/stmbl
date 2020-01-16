#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"

HAL_COMP(mpid);

HAL_PIN(pos_ext_cmd);
HAL_PIN(pos_fb);
HAL_PIN(old_pos_ext_cmd);
HAL_PIN(old_pos_fb);
HAL_PIN(pos_error);

HAL_PIN(vel_ext_cmd);
HAL_PIN(vel_fb);
HAL_PIN(vel_error);
HAL_PIN(vel_cmd);

HAL_PIN(acc_cmd);

HAL_PIN(torque_ext_cmd);
HAL_PIN(torque_sum);
HAL_PIN(torque_cmd);

HAL_PIN(min_pos_error);
HAL_PIN(max_vel);
HAL_PIN(max_acc);
HAL_PIN(max_torque);

HAL_PIN(pos_p);
HAL_PIN(vel_i);
HAL_PIN(vel_p);
HAL_PIN(j);
HAL_PIN(scale);

HAL_PIN(en);

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  // struct mpid_ctx_t *ctx      = (struct mpid_ctx_t *)ctx_ptr;
  struct mpid_pin_ctx_t *pins = (struct mpid_pin_ctx_t *)pin_ptr;

  if(PIN(en) > 0.0){
    PIN(pos_error) += minus(PIN(pos_ext_cmd), PIN(old_pos_ext_cmd)) * PIN(scale);
    PIN(pos_error) -= minus(PIN(pos_fb), PIN(old_pos_fb));
    
    PIN(vel_cmd) = LIMIT(PIN(pos_error) * PIN(pos_p), PIN(max_vel));

    if(ABS(PIN(pos_error)) < PIN(min_pos_error)){
      PIN(vel_cmd) = 0.0;
    }

    PIN(vel_cmd) += PIN(vel_ext_cmd);

    PIN(vel_error) = PIN(vel_cmd) - PIN(vel_fb);

    PIN(acc_cmd) = LIMIT(PIN(vel_error) * PIN(vel_p), PIN(max_acc));

    PIN(torque_cmd) = LIMIT(PIN(acc_cmd) * PIN(j), PIN(max_torque));

    PIN(torque_sum) += PIN(vel_error) * PIN(vel_i) * period;
    PIN(torque_sum) = CLAMP(PIN(torque_sum), -PIN(max_torque) - PIN(torque_cmd), PIN(max_torque) - PIN(torque_cmd));

    PIN(torque_cmd) += PIN(torque_ext_cmd);
    PIN(torque_cmd) += PIN(torque_sum);
  }
  else{
    PIN(pos_error) = 0.0;
    PIN(vel_cmd) = 0.0;
    PIN(vel_error) = 0.0;
    PIN(acc_cmd) = 0.0;
    PIN(torque_sum) = 0.0;
    PIN(torque_cmd) = 0.0;
  }

  PIN(old_pos_fb) = PIN(pos_fb);
  PIN(old_pos_ext_cmd) = PIN(pos_ext_cmd);
}

hal_comp_t mpid_comp_struct = {
    .name      = "mpid",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = 0,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = 0,
    .pin_count = sizeof(struct mpid_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};