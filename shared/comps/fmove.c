#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"

HAL_COMP(fmove);

HAL_PIN(en);

HAL_PIN(gravity);
HAL_PIN(real_mass);
HAL_PIN(virtual_mass);
HAL_PIN(damping);
HAL_PIN(friction);

HAL_PIN(min_pos);
HAL_PIN(max_pos);
HAL_PIN(max_vel);
HAL_PIN(max_acc);
HAL_PIN(max_usr_vel);
HAL_PIN(max_usr_acc);
HAL_PIN(scale);

HAL_PIN(force_in);
HAL_PIN(force);
HAL_PIN(force_offset);
HAL_PIN(force_offset_lpf);
HAL_PIN(pos);
HAL_PIN(mpos);
HAL_PIN(vel);
HAL_PIN(vel_old);
HAL_PIN(acc);
HAL_PIN(target);

HAL_PIN(force_time);
HAL_PIN(force_timer);
HAL_PIN(force_th);

HAL_PIN(print_freq);
HAL_PIN(print_timer);

HAL_PIN(mode);

static void nrt_init(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct fmove_pin_ctx_t *pins = (struct fmove_pin_ctx_t *)pin_ptr;
  PIN(force_time) = 5.0;
  PIN(max_acc) = 1;
  PIN(max_vel) = 0.1;
  PIN(max_usr_acc) = 2;
  PIN(max_usr_vel) = 0.1;
  PIN(min_pos) = 0.0;
  PIN(max_pos) = 0.5;
  PIN(real_mass) = 0.0;
  PIN(virtual_mass) = 1.0;
  PIN(damping) = 1.0;
  PIN(friction) = 0.01;
  PIN(scale) = 1000.0 / 25.0 * 2.0 * M_PI * 2.0;
  PIN(force_th) = 0.1;
  PIN(gravity) = 0.0;
  PIN(force_offset_lpf) = 0.01;
}

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct fmove_pin_ctx_t *pins = (struct fmove_pin_ctx_t *)pin_ptr;

  PIN(vel_old) = PIN(vel);

  PIN(force) = PIN(force_in) - PIN(gravity) * PIN(real_mass) - PIN(acc) * PIN(real_mass);
  PIN(force) -= PIN(force_offset);
  PIN(force_offset) += PIN(force) * PIN(force_offset_lpf) * period;
  PIN(force) -= SIGN(PIN(vel)) * PIN(friction) + PIN(vel) * PIN(damping);

  PIN(force_timer) += period;
  if(ABS(PIN(force)) > PIN(force_th)){
    PIN(force_timer) = 0.0;
    PIN(mode) = 1;
  }
  else if(PIN(mode) > 0 && ABS(PIN(vel)) > PIN(max_usr_vel) * 0.05){
    PIN(force_timer) = 0.0;
  }
  else if(PIN(force_timer) > PIN(force_time)){
    PIN(force_timer) = PIN(force_time);
    PIN(mode) = 0;
  }

  PIN(target) = CLAMP(PIN(target), PIN(min_pos), PIN(max_pos));

  switch((int) PIN(mode)){
    case 0: // auto move
      PIN(vel) = SIGN(PIN(target) - PIN(pos)) * sqrtf(ABS(PIN(target) - PIN(pos)) * 2.0 * PIN(max_acc));
      PIN(vel) = CLAMP(PIN(vel), PIN(vel_old) - PIN(max_acc) * period, PIN(vel_old) + PIN(max_acc) * period);
      PIN(vel) = LIMIT(PIN(vel), PIN(max_vel));
    break;
    case 1: // force move
      PIN(acc) = PIN(force) / PIN(virtual_mass);
      PIN(acc) = LIMIT(PIN(acc), PIN(max_usr_acc));
      PIN(vel) += PIN(acc) * period;
      PIN(vel) = CLAMP(PIN(vel), PIN(vel_old) - PIN(max_usr_acc) * period, PIN(vel_old) + PIN(max_usr_acc) * period);
      PIN(vel) = LIMIT(PIN(vel), PIN(max_usr_vel));
    break;
  }

  PIN(vel) = CLAMP(PIN(vel), -sqrtf(ABS(PIN(pos) - PIN(min_pos)) * 2.0 * MAX(PIN(max_acc), PIN(max_usr_acc))), sqrtf(ABS(PIN(pos) - PIN(max_pos)) * 2.0 * MAX(PIN(max_acc), PIN(max_usr_acc))));
  PIN(acc) = (PIN(vel) - PIN(vel_old)) / period;

  PIN(pos) += PIN(vel) * period;
  PIN(pos) = CLAMP(PIN(pos), PIN(min_pos), PIN(max_pos));
  PIN(mpos) = mod(PIN(pos) * PIN(scale));
  PIN(print_timer) += period;

  if(PIN(en) <= 0.0){
    PIN(force_offset) += PIN(force) * 0.5 * period;
    PIN(pos) = 0.0;
    PIN(mpos) = 0.0;
    PIN(vel) = 0.0;
  }
}

static void nrt_func(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct fmove_pin_ctx_t *pins = (struct fmove_pin_ctx_t *)pin_ptr;
  if(PIN(print_freq) > 0.0){
    if(PIN(print_timer) > 1.0 / PIN(print_freq)){
      PIN(print_timer) = 0.0;
      if(PIN(mode) > 0.0){
        printf("force_move %f\n", PIN(pos));
      }
      else if(ABS(PIN(target) - PIN(pos)) < 0.01 && ABS(PIN(vel)) < PIN(max_vel) * 0.1){
        printf("on_target %f\n", PIN(pos));
      }
      else{
        printf("moving %f\n", PIN(pos));
      }
    }
  }
  else{
    PIN(print_timer) = 0.0;
  }
}

hal_comp_t fmove_comp_struct = {
    .name      = "fmove",
    .nrt       = nrt_func,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = nrt_init,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = 0,
    .pin_count = sizeof(struct fmove_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};