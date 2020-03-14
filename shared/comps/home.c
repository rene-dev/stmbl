#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"

HAL_COMP(home);

HAL_PIN(home_vel);
HAL_PIN(home_acc);

HAL_PIN(pos_in);
HAL_PIN(pos_out);
HAL_PIN(vel);

HAL_PIN(home_in);
HAL_PIN(home_polarity);

HAL_PIN(offset);
HAL_PIN(home_offset);
HAL_PIN(home_pos);

HAL_PIN(state);

HAL_PIN(en_in);
HAL_PIN(en_out);

HAL_PIN(re_home);

static void nrt_init(void *ctx_ptr, hal_pin_inst_t *pin_ptr){
  struct home_pin_ctx_t *pins = (struct home_pin_ctx_t *)pin_ptr;
  PIN(state) = 0;
  PIN(re_home) = 1;
  PIN(home_polarity) = 1.0;
  PIN(home_vel) = 2.0 * M_PI;
  PIN(home_acc) = 2.0 * M_PI / 0.1;
}

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  //struct home_ctx_t *ctx      = (struct home_ctx_t *)ctx_ptr;
  struct home_pin_ctx_t *pins = (struct home_pin_ctx_t *)pin_ptr;

  float vel = 0.0;

  if(PIN(en_in) <= 0.0){
    if(PIN(re_home) > 0.0){
      PIN(state) = 0;
    }
    PIN(en_out) = 0;
  }

  switch((int)PIN(state)){
    case 0: // not homed
      if(PIN(en_in) > 0.0){
        PIN(state) = 1;
        PIN(en_out) = 0;
        PIN(offset) = 0.0;
        PIN(home_offset) = 0.0;
        PIN(vel) = 0.0;
      }
    break;

    case 1: // search home rising
      if(PIN(home_polarity) > 0.0 && PIN(home_in) > 0.0){
        PIN(state) = 2;
      }
      else if(PIN(home_polarity) <= 0.0 && PIN(home_in) <= 0.0){
        PIN(state) = 2;
      }
      else{
        vel = PIN(home_vel);
      }
    break;

    case 2: // search home falling
      if(PIN(home_polarity) <= 0.0 && PIN(home_in) > 0.0){
        PIN(state) = 3;
        PIN(home_offset) = PIN(offset) + PIN(home_pos);
      }
      else if(PIN(home_polarity) > 0.0 && PIN(home_in) <= 0.0){
        PIN(state) = 3;
        PIN(home_offset) = PIN(offset) + PIN(home_pos);
      }
      else{
        vel = -PIN(home_vel);
      }
    break;

    case 3: // go to home pos
      vel = SIGN(PIN(home_offset) - PIN(offset)) * sqrtf(ABS(PIN(home_offset) - PIN(offset)) * 2.0 * PIN(home_acc));

      if(ABS(PIN(offset) - PIN(home_offset)) < 0.01 && PIN(vel) < PIN(home_vel) * 0.01){
        PIN(state) = 4;
      }
    break;

    case 4: // homed
      PIN(en_out) = 1;
      PIN(offset) = PIN(home_offset);
      PIN(vel) = 0.0;
    break;
  }

  vel = LIMIT(vel, ABS(PIN(home_vel)));
  PIN(vel) = CLAMP(vel, PIN(vel) - PIN(home_acc) * period, PIN(vel) + PIN(home_acc) * period);

  PIN(offset) += PIN(vel) * period;

  PIN(pos_out) = mod(PIN(pos_in) + mod(PIN(offset)));
}

hal_comp_t home_comp_struct = {
    .name      = "home",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = nrt_init,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = 0,
    .pin_count = sizeof(struct home_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
