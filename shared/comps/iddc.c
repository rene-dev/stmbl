#include "iddc_comp.h"
#include "hal.h"
#include "defines.h"
#include "angle.h"

HAL_COMP(iddc);

HAL_PIN(q_cmd);
HAL_PIN(com_pos);
HAL_PIN(cmd_mode);
HAL_PIN(en);
HAL_PIN(en_out);

HAL_PIN(iq_fb);
HAL_PIN(uq_fb);
HAL_PIN(pos_fb);
HAL_PIN(vel_fb);

HAL_PIN(state);
HAL_PIN(timer);

HAL_PIN(r);
HAL_PIN(l);

HAL_PIN(com_offset);
HAL_PIN(out_rev);

HAL_PIN(test_cur);
HAL_PIN(test_vel);
HAL_PIN(ki);
HAL_PIN(vel_bw);

HAL_PIN(pi);

HAL_PIN(pwm_volt);

HAL_PIN(psi);

HAL_PIN(cur_bw);
HAL_PIN(cur_sum);

HAL_PIN(tmp0);
HAL_PIN(tmp1);
HAL_PIN(tmp2);
HAL_PIN(tmp3);
HAL_PIN(avg_test_volt);

static void nrt_init(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  //struct iddc_ctx_t * ctx = (struct iddc_ctx_t *)ctx_ptr;
  struct iddc_pin_ctx_t *pins = (struct iddc_pin_ctx_t *)pin_ptr;
  PIN(test_cur) = 2.0;
  PIN(test_vel) = 50.0;
  PIN(ki) = 2.0;
  PIN(pi) = 1.0;
  PIN(vel_bw) = 20.0;
  PIN(cur_bw) = 1.0;
}

static void nrt(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  //struct iddc_ctx_t * ctx = (struct iddc_ctx_t *)ctx_ptr;
  struct iddc_pin_ctx_t *pins = (struct iddc_pin_ctx_t *)pin_ptr;

  switch((int)(PIN(state) * 10.0 + 0.5)){
    case 0:
      PIN(r) = 0.1;
      PIN(l) = 0.001;
      PIN(psi) = 0.055;
      PIN(com_offset) = 0.0;
      PIN(out_rev) = 0.0;
      PIN(cur_bw) = 1.0;
    break;

    case 10: // r
      PIN(state) = 1.1;
      PIN(timer) = 0.0;
      PIN(q_cmd) = 0.0;
      PIN(com_pos) = 0.0;
      PIN(cmd_mode) = 0.0;

      printf("Measure r, l\n");
      printf("<font color='green'>block the rotor</font>\n");
      printf("iddc0.state = 1.2 <font color='green'>to start</font>\n");
    break;

    case 14:
      printf("conf0.r = %f <font color='green'># append to config</font>\n", PIN(r));
      printf("conf0.l = %f <font color='green'># append to config</font>\n", PIN(l));
      PIN(state) = 2.0;
    break;

    case 20:
      PIN(state) = 3.1;
      PIN(timer) = 0.0;
      PIN(q_cmd) = 0.0;
      PIN(cur_sum) = 0.0;
      PIN(cmd_mode) = 0.0;
      PIN(cur_bw) = 250.0;

      printf("Measure torque constant\n");
      printf("<font color='green'>unblock the rotor</font>\n");
      printf("the motor will move\n");
      printf("iddc0.state = 2.2 <font color='green'>to start</font>\n");
    break;

    case 23:
      printf("conf0.psi = %f <font color='green'># append to config</font>\n", PIN(psi));
      if(PIN(out_rev) > 0.0){
        printf("conf0.out_rev = 1 <font color='green'># append to config</font>\n");
      }
      printf("done\n");
      printf("continue with id_mot\n");

      PIN(state) = 2.4;
    break;
  }
 
}

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  //struct iddc_ctx_t * ctx = (struct iddc_ctx_t *)ctx_ptr;
  struct iddc_pin_ctx_t *pins = (struct iddc_pin_ctx_t *)pin_ptr;

  if(PIN(en) <= 0.0){
    PIN(state) = 0.0;
  }

  switch((int)(PIN(state) * 10.0 + 0.5)){
    case 0:
      PIN(en_out) = 0.0;
      PIN(cmd_mode) = 0.0;
      PIN(timer) = 0.0;
      PIN(q_cmd) = 0.0;
      PIN(cur_bw) = 10.0;

      if(PIN(en) > 0.0){
        PIN(state) = 1.0;
      }
    break;

    case 12: // r
      PIN(en_out) = 1.0;
      PIN(cmd_mode) = 1.0;
      PIN(cur_bw) = 10.0;
      PIN(com_pos) = 0.0;

      PIN(q_cmd) = PIN(test_cur);

      PIN(r) = PIN(r) * 0.99 + PIN(uq_fb) / MAX(PIN(iq_fb), 0.01) * 0.01;

      PIN(timer) += period;
      if(PIN(timer) >= 2.0){
        PIN(timer) = 0.0;
        PIN(state) = 1.3;
        PIN(en_out) = 0.0;
        PIN(tmp0) = 0.0;
        PIN(tmp1) = 0.0;

        PIN(avg_test_volt) = PIN(test_cur) * MAX(PIN(r), 0.01);
        PIN(avg_test_volt) = LIMIT(PIN(avg_test_volt), PIN(pwm_volt) / 2.0);
      }
    break;

    case 13: // l
      PIN(en_out) = 1.0;
      PIN(cmd_mode) = 0.0;
      PIN(cur_bw) = 10.0;
      
      //PIN(l) = PIN(l) * 0.995 + ABS(PIN(uq_fb) - avg_test_volt / 2.0) / MAX(ABS(PIN(iq_fb) - PIN(test_cur)), 0.001) * period * 0.005;
      if(PIN(q_cmd) < PIN(avg_test_volt)){
        PIN(tmp0) = PIN(tmp0) * 0.99 + PIN(iq_fb) * 0.01;
        PIN(tmp1) = PIN(tmp1) * 0.99 + PIN(uq_fb) * 0.01;
        PIN(q_cmd) = PIN(avg_test_volt) * 1.5;
      }
      else{
        PIN(tmp2) = PIN(tmp2) * 0.99 + PIN(iq_fb) * 0.01;
        PIN(tmp3) = PIN(tmp3) * 0.99 + PIN(uq_fb) * 0.01;
        PIN(q_cmd) = PIN(avg_test_volt) * 0.5;
      }

      PIN(timer) += period;
      if(PIN(timer) >= 1.0){
        PIN(l) = ABS(PIN(tmp1) - PIN(tmp3)) / ABS(PIN(tmp0) - PIN(tmp2)) * period;
        PIN(timer) = 0.0;
        PIN(state) = 1.4;
        PIN(q_cmd) = PIN(avg_test_volt);
        PIN(en_out) = 0.0;
        // PIN(tmp0) = 0.0;
        // PIN(tmp1) = 0.0;
      }
    break;

    case 22: // psi
      PIN(en_out) = 1.0;
      PIN(cmd_mode) = 1.0;
      PIN(cur_bw) = 250.0;

      float vel_error = PIN(test_vel) - ABS(PIN(vel_fb));
      vel_error = LIMIT(vel_error, PIN(test_vel) / 100.0);

      PIN(cur_sum) += PIN(ki) * vel_error * period;

      PIN(q_cmd) = PIN(vel_bw) * period * vel_error + PIN(cur_sum);
      
      if(ABS(PIN(vel_fb)) > 0.1){
        float psi  = (PIN(uq_fb) - PIN(iq_fb) * PIN(r)) / (PIN(vel_fb));
        PIN(psi) = PIN(psi) * (1.0 - period / PIN(pi)) + psi * period / PIN(pi);
      }

      //PIN(psi) = CLAMP(PIN(psi), 0.001, 1.0);

      PIN(timer) += period;
      if(PIN(timer) >= 5.0){
        if(PIN(psi) < 0.0){
          PIN(psi) *= -1.0;
          PIN(out_rev) = 1.0;
        }

        PIN(timer) = 0.0;
        PIN(en_out) = 0.0;
        PIN(q_cmd) = 0.0;
        PIN(cur_sum) = 0.0;
        PIN(cmd_mode) = 0.0;

        PIN(state) = 2.3;
      }
    break;
  }
}


hal_comp_t iddc_comp_struct = {
    .name      = "iddc",
    .nrt       = nrt,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = nrt_init,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = 0,//sizeof(struct iddc_ctx_t),
    .pin_count = sizeof(struct iddc_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};