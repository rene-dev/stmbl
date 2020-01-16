#include "hal.h"
#include "angle.h"
#include "defines.h"

HAL_COMP(rl);

HAL_PIN(r_test_cur);
HAL_PIN(l_test_volt);
HAL_PIN(r_test_time);
HAL_PIN(l_test_time);

HAL_PIN(ud_cmd);
HAL_PIN(ud_fb);
HAL_PIN(id_fb);

HAL_PIN(uq_cmd);
HAL_PIN(uq_fb);
HAL_PIN(iq_fb);

HAL_PIN(ki);

HAL_PIN(en);
HAL_PIN(en_out);

HAL_PIN(state);

HAL_PIN(timer);
HAL_PIN(counter);

HAL_PIN(r);
HAL_PIN(ur);
HAL_PIN(ir);

HAL_PIN(ld);
HAL_PIN(lq);
HAL_PIN(u0);
HAL_PIN(u1);
HAL_PIN(i0);
HAL_PIN(i1);

HAL_PIN(et);

static void nrt_init(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct rl_pin_ctx_t *pins = (struct rl_pin_ctx_t *)pin_ptr;

  PIN(r_test_cur) = 5;
  PIN(r_test_time) = 0.5;
  PIN(l_test_volt) = 10;
  PIN(l_test_time) = 1.5;
  PIN(ki) = 10;
}


static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  //struct rl_ctx_t *ctx      = (struct rl_ctx_t *)ctx_ptr;
  struct rl_pin_ctx_t *pins = (struct rl_pin_ctx_t *)pin_ptr;


  if(PIN(en) <= 0.0){
    PIN(state) = 0;
  }

  switch((int) PIN(state)){
    case 0: // off
      PIN(ud_cmd) = 0.0;
      PIN(uq_cmd) = 0.0;
      PIN(en_out) = 0.0;
      PIN(timer) = 0.0;
      
      if(PIN(en) > 0.0){
        PIN(state) = 1;
      }
      break;

    case 1: // ramp up ud_cmd
      PIN(en_out) = 1;
      PIN(ud_cmd) += PIN(ki) * period * (PIN(r_test_cur) - PIN(id_fb));

      PIN(timer) += period;

      if(PIN(timer) > PIN(r_test_time)){
        if(ABS(PIN(id_fb)) > ABS(PIN(r_test_cur)) * 0.1){
          PIN(ur) = PIN(ur) * 0.99 + PIN(ud_fb) * 0.01;
          PIN(ir) = PIN(ir) * 0.99 + PIN(id_fb) * 0.01;
          PIN(r) = PIN(ur) / PIN(ir);
        }
      }

      if(PIN(timer) > PIN(r_test_time) * 2.0){
        PIN(state) = 2;
        PIN(timer) = 0;
        PIN(counter) = 1;
      }
      break;

      case 2:
        PIN(timer) += period;
        PIN(counter) *= -1;

        if(PIN(counter) > 0){
          PIN(ud_cmd) = PIN(l_test_volt) * 0.1;
          PIN(u1) = PIN(u1) * 0.998 + PIN(ud_fb) * 0.002;
          PIN(i1) = PIN(i1) * 0.998 + PIN(id_fb) * 0.002;
        }
        else{
          PIN(ud_cmd) = PIN(l_test_volt);
          PIN(u0) = PIN(u0) * 0.998 + PIN(ud_fb) * 0.002;
          PIN(i0) = PIN(i0) * 0.998 + PIN(id_fb) * 0.002;
        }

        float dv = (PIN(u1) - PIN(u0)) / 2.0;
        float di = PIN(i1) - PIN(i0);

        PIN(ld) = ABS(dv / di) * period;

        if(PIN(timer) > PIN(l_test_time)){
          PIN(state) = 3;
        }
        break;

      case 3:
        PIN(ud_cmd) = 0.0;
        PIN(uq_cmd) = 0.0;
        PIN(en_out) = 0.0;
        PIN(timer) = 0.0;
        PIN(et) = PIN(ld) / PIN(r);
        PIN(en) = 0;
  }
}

const hal_comp_t rl_comp_struct = {
    .name      = "rl",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = nrt_init,
    .hw_init   = 0,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = 0,
    .pin_count = sizeof(struct rl_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
