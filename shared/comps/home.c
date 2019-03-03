#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"

HAL_COMP(home);

HAL_PIN(en);

HAL_PIN(home_vel);
HAL_PIN(home_acc);
HAL_PIN(home_pos);

HAL_PIN(pos_in);
HAL_PIN(pos_out);
HAL_PIN(vel_cmd);

HAL_PIN(home_in);
HAL_PIN(home_polarity);

HAL_PIN(limit_in0);
HAL_PIN(limit_in1);
HAL_PIN(limit_polarity);

HAL_PIN(state);
HAL_PIN(en_out);
HAL_PIN(target);
HAL_PIN(home_offset);

HAL_PIN(re_home);
HAL_PIN(limit_fault);

static void nrt_init(volatile void *ctx_ptr, volatile hal_pin_inst_t *pin_ptr){
  struct home_pin_ctx_t *pins = (struct home_pin_ctx_t *)pin_ptr;
  PIN(state) = 0;
  PIN(re_home) = 1;
  PIN(home_polarity) = 1.0;
}

static void rt_func(float period, volatile void *ctx_ptr, volatile hal_pin_inst_t *pin_ptr) {
  //struct home_ctx_t *ctx      = (struct home_ctx_t *)ctx_ptr;
  struct home_pin_ctx_t *pins = (struct home_pin_ctx_t *)pin_ptr;

  float vel = 0.0;

  if(PIN(en) <= 0.0){
    if(PIN(re_home) > 0.0){
      PIN(state) = 0;
    }
    PIN(en_out) = 0;
    PIN(limit_fault) = 0;
  }

  switch((int)PIN(state)){
    case 0: // not homed
      if(PIN(en) > 0.0){
        PIN(state) = 1;
        PIN(en_out) = 0;
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
      }
      else if(PIN(home_polarity) > 0.0 && PIN(home_in) <= 0.0){
        PIN(state) = 3;
      }
      else{
        vel = -PIN(home_vel);
      }
    break;

    case 3: // set home pos
      PIN(target) = PIN(home_pos);
      PIN(state) = 4;
    break;


    case 4: // go to home pos
      vel = PIN(target) * 10;
      if(ABS(PIN(target)) < 0.0001){
        PIN(state) = 5;
      }
    break;

    case 5: // homed
      PIN(en_out) = 1;

      if(PIN(limit_polarity) > 0.0 && (PIN(limit_in0) > 0.0 || PIN(limit_in1) > 0.0)){
        PIN(state) = 6;
        PIN(en_out) = 0;
        PIN(limit_fault) = 1;
      }
      else if(PIN(limit_polarity) <= 0.0 && (PIN(limit_in0) <= 0.0 || PIN(limit_in1) <= 0.0)){
        PIN(state) = 6;
        PIN(en_out) = 0;
        PIN(limit_fault) = 1;
      }
    break;

    case 6: // fault
      PIN(en_out) = 0;
      PIN(limit_fault) = 1;
    break;
  }

  vel = LIMIT(vel, ABS(PIN(home_vel)));
  PIN(vel_cmd) = CLAMP(vel, PIN(vel_cmd) - PIN(home_acc) * period, PIN(vel_cmd) + PIN(home_acc) * period);

  PIN(target) -= PIN(vel_cmd) * period;
  PIN(home_offset) += PIN(vel_cmd) * period;
  PIN(pos_out) = PIN(pos_in) + PIN(home_offset);
  PIN(pos_out) = mod(PIN(pos_out));
  
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
