#include "idpmsm_comp.h"
#include "hal.h"
#include "defines.h"
#include "angle.h"

HAL_COMP(idpmsm);

HAL_PIN(d_cmd);
HAL_PIN(q_cmd);
HAL_PIN(com_pos);
HAL_PIN(cmd_mode);
HAL_PIN(en);
HAL_PIN(en_out);

HAL_PIN(id_fb);
HAL_PIN(iq_fb);
HAL_PIN(ud_fb);
HAL_PIN(uq_fb);
HAL_PIN(pos_fb);
HAL_PIN(vel_fb);

HAL_PIN(state);
HAL_PIN(timer);

HAL_PIN(r);
HAL_PIN(l);

HAL_PIN(pp);
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
HAL_PIN(auto_step);

HAL_PIN(tmp0);
HAL_PIN(tmp1);
HAL_PIN(tmp2);
HAL_PIN(tmp3);
HAL_PIN(avg_test_volt);

static void nrt_init(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  //struct idpmsm_ctx_t * ctx = (struct idpmsm_ctx_t *)ctx_ptr;
  struct idpmsm_pin_ctx_t *pins = (struct idpmsm_pin_ctx_t *)pin_ptr;
  PIN(test_cur) = 3.0;
  PIN(test_vel) = 50.0;
  PIN(ki) = 1.0;
  PIN(pi) = 1.0;
  PIN(vel_bw) = 20.0;
  PIN(cur_bw) = 1.0;
  PIN(auto_step) = 4.2;
}

static void nrt(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  //struct idpmsm_ctx_t * ctx = (struct idpmsm_ctx_t *)ctx_ptr;
  struct idpmsm_pin_ctx_t *pins = (struct idpmsm_pin_ctx_t *)pin_ptr;

  switch((int)(PIN(state) * 10.0 + 0.5)){
    case 0:
      PIN(r) = 0.1;
      PIN(l) = 0.001;
      PIN(psi) = 0.055;
      PIN(pp) = 3.0;
      PIN(com_offset) = 0.0;
      PIN(out_rev) = 0.0;
      PIN(cur_bw) = 1.0;
    break;

    case 10: // r
      PIN(state) = 1.1;
      PIN(timer) = 0.0;
      PIN(d_cmd) = 0.0;
      PIN(q_cmd) = 0.0;
      PIN(com_pos) = 0.0;
      PIN(cmd_mode) = 0.0;

      if(PIN(auto_step) >= 1){
        PIN(state) = 1.2;
      }
      else{
        printf("Measure r, l\n");
        printf("the motor can move a bit\n");
        printf("idpmsm0.state = 1.2 <font color='green'>to start</font>\n");
      }
    break;

    case 14:
      printf("conf0.r = %f <font color='green'># append to config</font>\n", PIN(r));
      printf("conf0.l = %f <font color='green'># append to config</font>\n", PIN(l));
      PIN(state) = 2.0;
    break;

    case 20: // pp, out_rev, com_offset
      PIN(state) = 2.1;
      PIN(timer) = 0.0;
      //PIN(d_cmd) = 0.0;
      PIN(q_cmd) = 0.0;
      PIN(com_pos) = 0.0;
      PIN(cmd_mode) = 0.0;

      if(PIN(auto_step) >= 2){
        PIN(state) = 2.2;
      }
      else{
        printf("Measure com_offset, polepairs, out_rev\n");
        printf("the motor will move\n");
        printf("idpmsm0.state = 2.2 <font color='green'>to start</font>\n");
      }
    break;

    case 25: // pp, out_rev, com_offset
      printf("conf0.polecount = %f <font color='green'># append to config</font>\n", PIN(pp));
      printf("conf0.mot_fb_offset = %f <font color='green'># append to config</font>\n", PIN(com_offset));
      if(PIN(out_rev) > 0.0){
        printf("conf0.out_rev = 1 <font color='green'># append to config</font>\n");
      }
      PIN(state) = 3.0;
    break;

    case 30:
      PIN(state) = 3.1;
      PIN(timer) = 0.0;
      PIN(d_cmd) = 0.0;
      PIN(q_cmd) = 0.0;
      PIN(cur_sum) = 0.0;
      PIN(cmd_mode) = 0.0;
      PIN(cur_bw) = 250.0;

      if(PIN(auto_step) >= 3){
        PIN(state) = 3.2;
      }
      else{
        printf("Measure torque constant\n");
        printf("the motor will move\n");
        printf("id0.state = 3.2 to start\n");
      }
    break;

    case 33:
      printf("conf0.psi = %f <font color='green'># append to config</font>\n", PIN(psi));
      printf("done\n");
      printf("continue with id_mot\n");

      PIN(state) = 3.4;
    break;
  }
 
}

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  //struct idpmsm_ctx_t * ctx = (struct idpmsm_ctx_t *)ctx_ptr;
  struct idpmsm_pin_ctx_t *pins = (struct idpmsm_pin_ctx_t *)pin_ptr;

  if(PIN(en) <= 0.0){
    PIN(state) = 0.0;
  }

  switch((int)(PIN(state) * 10.0 + 0.5)){
    case 0:
      PIN(en_out) = 0.0;
      PIN(cmd_mode) = 0.0;
      PIN(timer) = 0.0;
      PIN(d_cmd) = 0.0;
      PIN(q_cmd) = 0.0;
      PIN(cur_bw) = 1.0;

      if(PIN(en) > 0.0){
        PIN(state) = 1.0;
      }
    break;

    case 12: // r
      PIN(en_out) = 1.0;
      PIN(cmd_mode) = 1.0;
      PIN(cur_bw) = 1.0;
      PIN(q_cmd) = 0.0;
      PIN(com_pos) = 0.0;

      PIN(d_cmd) = PIN(test_cur);

      PIN(r) = PIN(r) * 0.99 + PIN(ud_fb) / MAX(PIN(id_fb), 0.01) * 0.01;

      PIN(timer) += period;
      if(PIN(timer) >= 2.0){
        PIN(timer) = 0.0;
        PIN(state) = 1.3;
        PIN(d_cmd) = 0.0;
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
      PIN(q_cmd) = 0.0;
      PIN(cur_bw) = 1.0;
      
      //PIN(l) = PIN(l) * 0.995 + ABS(PIN(ud_fb) - avg_test_volt / 2.0) / MAX(ABS(PIN(id_fb) - PIN(test_cur)), 0.001) * period * 0.005;
      if(PIN(d_cmd) < PIN(avg_test_volt)){
        PIN(tmp0) = PIN(tmp0) * 0.99 + PIN(id_fb) * 0.01;
        PIN(tmp1) = PIN(tmp1) * 0.99 + PIN(ud_fb) * 0.01;
        PIN(d_cmd) = PIN(avg_test_volt) * 1.5;
      }
      else{
        PIN(tmp2) = PIN(tmp2) * 0.99 + PIN(id_fb) * 0.01;
        PIN(tmp3) = PIN(tmp3) * 0.99 + PIN(ud_fb) * 0.01;
        PIN(d_cmd) = PIN(avg_test_volt) * 0.5;
      }

      PIN(timer) += period;
      if(PIN(timer) >= 1.0){
        PIN(l) = ABS(PIN(tmp1) - PIN(tmp3)) / ABS(PIN(tmp0) - PIN(tmp2)) * period;
        PIN(timer) = 0.0;
        PIN(state) = 1.4;
        PIN(d_cmd) = PIN(avg_test_volt);
        PIN(en_out) = 0.0;
        // PIN(tmp0) = 0.0;
        // PIN(tmp1) = 0.0;
      }
    break;

    case 22: // pp, out_rev
      PIN(en_out) = 1.0;
      PIN(cmd_mode) = 1.0;
      PIN(cur_bw) = 100.0;
      PIN(q_cmd) = 0.0;

      PIN(d_cmd) = PIN(test_cur);

      PIN(com_pos) += PIN(test_vel) * period;
      PIN(com_pos) = mod(PIN(com_pos));

      if(ABS(PIN(vel_fb)) > 0.1){
        PIN(pp) = PIN(pp) * 0.995 + PIN(test_vel) / PIN(vel_fb) * 0.005;
      }

      PIN(timer) += period;
      if(PIN(timer) >= 3.0){
        PIN(timer) = 0.0;
        
        if(PIN(pp) < 0.0){
          PIN(out_rev) = 1.0;
          PIN(pp) *= -1.0;
        }
        PIN(pp) = (int)(PIN(pp) + 0.5);

        PIN(state) = 2.3;
      }
    break;

    case 23: // mot_fb_offset
      PIN(en_out) = 1.0;
      PIN(cmd_mode) = 1.0;
      PIN(cur_bw) = 100.0;
      PIN(q_cmd) = 0.0;

      PIN(d_cmd) = PIN(test_cur);

      PIN(com_pos) = 0.0;

      PIN(com_offset) = PIN(com_offset) * 0.99 - PIN(pos_fb) * 0.01;

      PIN(timer) += period;
      if(PIN(timer) >= 2.0){
        PIN(en) = 0.0;
        PIN(d_cmd) = 0.0;
        PIN(state) = 2.5;
        PIN(timer) = 0.0;
      }
    break;

    case 32: // psi
      PIN(en_out) = 1.0;
      PIN(cmd_mode) = 1.0;
      PIN(cur_bw) = 250.0;
      PIN(d_cmd) = 0.0;
      PIN(com_pos) = mod((PIN(pos_fb) + PIN(com_offset)) * PIN(pp));

      float vel_error = PIN(test_vel) - PIN(vel_fb);
      vel_error = LIMIT(vel_error, PIN(test_vel) / 100.0);
      PIN(cur_sum) += PIN(ki) * vel_error * period;

      PIN(q_cmd) = PIN(vel_bw) * period * vel_error + PIN(cur_sum);
      
      if(ABS(PIN(vel_fb)) > 0.1){
        float psi  = (PIN(uq_fb) - PIN(iq_fb) * PIN(r)) / (PIN(vel_fb) * PIN(pp));
        PIN(psi) = PIN(psi) * (1.0 - period / PIN(pi)) + psi * period / PIN(pi);
      }

      PIN(psi) = CLAMP(PIN(psi), 0.001, 1.0);

      PIN(timer) += period;
      if(PIN(timer) >= 5.0){
        PIN(timer) = 0.0;
        PIN(en_out) = 0.0;
        PIN(d_cmd) = 0.0;
        PIN(q_cmd) = 0.0;
        PIN(cur_sum) = 0.0;
        PIN(cmd_mode) = 0.0;

        PIN(state) = 3.3;
      }
    break;

    case 100:
      PIN(com_pos) = mod((PIN(pos_fb) + PIN(com_offset)) * PIN(pp));
  }
}


hal_comp_t idpmsm_comp_struct = {
    .name      = "idpmsm",
    .nrt       = nrt,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = nrt_init,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = 0,//sizeof(struct idpmsm_ctx_t),
    .pin_count = sizeof(struct idpmsm_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};