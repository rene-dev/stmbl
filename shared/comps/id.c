#include "id_comp.h"
#include "hal.h"
#include "defines.h"
#include "angle.h"

HAL_COMP(id);

HAL_PIN(d_cmd);
HAL_PIN(q_cmd);
HAL_PIN(com_pos);
HAL_PIN(com_mode);
HAL_PIN(cmd_mode);
HAL_PIN(en);

HAL_PIN(id_fb);
HAL_PIN(iq_fb);
HAL_PIN(ud_fb);
HAL_PIN(uq_fb);
HAL_PIN(pos_fb);
HAL_PIN(vel_fb);
HAL_PIN(acc_fb);

HAL_PIN(state);
HAL_PIN(timer);

HAL_PIN(r);
HAL_PIN(l);
HAL_PIN(lm);
HAL_PIN(t);

HAL_PIN(pp);
HAL_PIN(com_offset);
HAL_PIN(out_rev);

HAL_PIN(max_torque);
HAL_PIN(test_cur);
HAL_PIN(test_vel);
HAL_PIN(ki);

HAL_PIN(freq);
HAL_PIN(amp);
HAL_PIN(min_pos);
HAL_PIN(max_pos);
HAL_PIN(max_vel);
HAL_PIN(max_acc);
HAL_PIN(pos);
HAL_PIN(vel);
HAL_PIN(acc);

HAL_PIN(pi);
HAL_PIN(fi);
HAL_PIN(li);
HAL_PIN(ji);
HAL_PIN(di);

HAL_PIN(fb_torque);
HAL_PIN(ff_torque);
HAL_PIN(acc_torque);
HAL_PIN(torque);

HAL_PIN(pwm_volt);

HAL_PIN(psi);
HAL_PIN(friction);
HAL_PIN(load);
HAL_PIN(inertia);
HAL_PIN(damping);

HAL_PIN(pos_bw);
HAL_PIN(vel_bw);
HAL_PIN(cur_bw);
HAL_PIN(torque_sum);
HAL_PIN(id_error_sum);
HAL_PIN(iq_error_sum);
HAL_PIN(target);
HAL_PIN(auto_step);

HAL_PIN(tmp0);
HAL_PIN(tmp1);
HAL_PIN(tmp2);
HAL_PIN(tmp3);
HAL_PIN(avg_test_volt);

static void nrt_init(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  //struct id_ctx_t * ctx = (struct id_ctx_t *)ctx_ptr;
  struct id_pin_ctx_t *pins = (struct id_pin_ctx_t *)pin_ptr;
  PIN(test_cur) = 4.0;
  PIN(max_torque) = 3.0;
  PIN(test_vel) = 50.0;
  PIN(ki) = 1.0;
  PIN(pi) = 1.0;
  PIN(fi) = 0.001;
  PIN(li) = 5.0;
  PIN(ji) = 200.0;
  PIN(di) = 10.0;
  PIN(pos_bw) = 5.0;
  PIN(vel_bw) = 20.0;
  PIN(cur_bw) = 250.0;
  PIN(max_vel) = 50.0;
  PIN(max_acc) = 1000.0;
  PIN(min_pos) = -10.0;
  PIN(max_pos) = 10.0;
  PIN(auto_step) = 4.2;
}

static void nrt(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  //struct id_ctx_t * ctx = (struct id_ctx_t *)ctx_ptr;
  struct id_pin_ctx_t *pins = (struct id_pin_ctx_t *)pin_ptr;

  switch((int)(PIN(state) * 10.0 + 0.5)){
    case 0:
      PIN(r) = 0.1;
      PIN(l) = 0.001;
      PIN(lm) = 0.0;
      PIN(t) = 0.0;
      PIN(psi) = 0.055;
      PIN(pp) = 3.0;
      PIN(com_offset) = 0.0;
      PIN(out_rev) = 0.0;
      PIN(inertia) = 0.0001;
    break;

    case 10: // r
      printf("Measure r, l\n");
      printf("the motor can move a bit\n");
      printf("id0.state = 1.2 to start\n");
      
      PIN(state) = 1.1;
      PIN(timer) = 0.0;
      PIN(d_cmd) = 0.0;
      PIN(q_cmd) = 0.0;
      PIN(com_pos) = 0.0;
      PIN(cmd_mode) = 0.0;

      if(PIN(auto_step) >= 1){
        PIN(state) = 1.2;
      }
    break;

    case 14:
      printf("conf0.r = %f\n", PIN(r));
      printf("conf0.l = %f\n", PIN(l));
      PIN(state) = 2.0;
    break;

    case 20: // pp, out_rev, com_offset
      printf("Measure com_offset, polepairs, out_rev\n");
      printf("the motor will move\n");
      printf("id0.state = 2.2 to start\n");

      PIN(state) = 2.1;
      PIN(timer) = 0.0;
      //PIN(d_cmd) = 0.0;
      PIN(q_cmd) = 0.0;
      PIN(com_pos) = 0.0;
      PIN(cmd_mode) = 0.0;

      if(PIN(auto_step) >= 2){
        PIN(state) = 2.2;
      }
    break;

    case 25: // pp, out_rev, com_offset
      printf("conf0.polepairs = %f\n", PIN(pp));
      printf("conf0.com_offset = %f\n", PIN(com_offset));
      if(PIN(out_rev) > 0.0){
        printf("conf0.out_rev = 1\n");
      }
      PIN(state) = 3.0;
    break;

    case 30:
      printf("Measure torque constant\n");
      printf("the motor will move\n");
      printf("id0.state = 3.2 to start\n");

      PIN(state) = 3.1;
      PIN(timer) = 0.0;
      PIN(d_cmd) = 0.0;
      PIN(q_cmd) = 0.0;
      PIN(torque_sum) = 0.0;
      PIN(id_error_sum) = 0.0;
      PIN(iq_error_sum) = 0.0;
      PIN(cmd_mode) = 0.0;

      if(PIN(auto_step) >= 3){
        PIN(state) = 3.2;
      }
    break;

    case 33:
      printf("conf0.psi = %f\n", PIN(psi));

      PIN(state) = 4.0;
    break;
    
    case 40:
      printf("Measure friction, damping and inertia\n");
      printf("the motor will move\n");
      printf("id0.state = 4.2 to start\n");

      PIN(state) = 4.1;
      PIN(timer) = 0.0;
      PIN(d_cmd) = 0.0;
      PIN(q_cmd) = 0.0;
      PIN(torque_sum) = 0.0;
      PIN(id_error_sum) = 0.0;
      PIN(iq_error_sum) = 0.0;
      PIN(cmd_mode) = 0.0;
      PIN(target) = PIN(min_pos);

      if(PIN(auto_step) >= 4){
        PIN(state) = 4.2;
      }
    break;

    case 43:
      printf("conf0.j = %f\n", PIN(inertia));
      printf("conf0.friction = %f\n", PIN(friction));
      printf("conf0.damping = %f\n", PIN(damping));
      PIN(state) = 4.4;
    break;
  }
 
}

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  //struct id_ctx_t * ctx = (struct id_ctx_t *)ctx_ptr;
  struct id_pin_ctx_t *pins = (struct id_pin_ctx_t *)pin_ptr;

  // if(ABS(PIN(id_fb)) + ABS(PIN(iq_fb)) > 5.0 * PIN(test_cur)){
  //   PIN(state) = 0;
  //   PIN(d_cmd) = 0.0;
  //   PIN(q_cmd) = 0.0;
  //   PIN(com_pos) = 0.0;
  //   PIN(cmd_mode) = 0.0;
  //   PIN(en) = 0.0;
  // }

  switch((int)(PIN(state) * 10.0 + 0.5)){
    case 0:
      PIN(en) = 0;
      PIN(timer) = 0.0;
      PIN(d_cmd) = 0.0;
      PIN(q_cmd) = 0.0;
    break;

    case 12: // r
      PIN(en) = 1.0;
      PIN(d_cmd) += PIN(ki) * period * PIN(r) * (PIN(test_cur) - PIN(id_fb));
      PIN(d_cmd) = LIMIT(PIN(d_cmd), PIN(pwm_volt));
      PIN(r) = PIN(r) * 0.99 + PIN(ud_fb) / MAX(PIN(id_fb), 0.01) * 0.01;

      PIN(timer) += period;
      if(PIN(timer) >= 2.0){
        PIN(timer) = 0.0;
        PIN(state) = 1.3;
        //PIN(d_cmd) = 0.0;
        //PIN(en) = 0.0;
        PIN(tmp0) = 0.0;
        PIN(tmp1) = 0.0;

        PIN(avg_test_volt) = PIN(test_cur) * MAX(PIN(r), 0.01);
        PIN(avg_test_volt) = LIMIT(PIN(avg_test_volt), PIN(pwm_volt) / 2.0);
      }
    break;

    case 13: // l
      PIN(en) = 1.0;

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
        PIN(en) = 0.0;
        // PIN(tmp0) = 0.0;
        // PIN(tmp1) = 0.0;
      }
    break;

    case 22: // pp, out_rev
      PIN(en) = 1.0;
      PIN(d_cmd) += PIN(ki) * period * PIN(r) * (PIN(test_cur) - PIN(id_fb));
      PIN(d_cmd) = LIMIT(PIN(d_cmd), PIN(pwm_volt));
      PIN(com_pos) += PIN(test_vel) * period;
      PIN(com_pos) = mod(PIN(com_pos));

      if(ABS(PIN(vel_fb)) > 0.1){
        PIN(pp) = PIN(pp) * 0.99 + PIN(test_vel) / PIN(vel_fb) * 0.01;
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

    case 23:
      PIN(en) = 1.0;
      PIN(d_cmd) += PIN(ki) * period * PIN(r) * (PIN(test_cur) - PIN(id_fb));
      PIN(d_cmd) = LIMIT(PIN(d_cmd), PIN(pwm_volt));
      PIN(com_pos) = 0.0;

      PIN(com_offset) = PIN(com_offset) * 0.99 + -PIN(pos_fb) * 0.01;

      PIN(timer) += period;
      if(PIN(timer) >= 2.0){
        PIN(en) = 0.0;
        PIN(d_cmd) = 0.0;
        PIN(state) = 2.5;
        PIN(timer) = 0.0;
      }

    break;

    case 32: // psi
      PIN(en) = 1.0;
      PIN(com_pos) = mod((PIN(pos_fb) + PIN(com_offset)) * PIN(pp));

      float vel_error = PIN(test_vel) - PIN(vel_fb);
      PIN(torque_sum) += PIN(ki) * vel_error * period;
      
      float id_error = 0.0 - PIN(id_fb);
      PIN(d_cmd) = PIN(r) * PIN(id_fb) + PIN(cur_bw) * PIN(l) * id_error;
      PIN(id_error_sum) = PIN(id_error_sum) + PIN(cur_bw) * PIN(r) * id_error * period;
      PIN(d_cmd) += PIN(id_error_sum);
      PIN(d_cmd) = LIMIT(PIN(d_cmd), PIN(pwm_volt));

      float iq_error = PIN(vel_bw) * period * vel_error + PIN(torque_sum) - PIN(iq_fb);
      PIN(q_cmd) = PIN(r) * PIN(iq_fb) + PIN(cur_bw) * PIN(l) * iq_error;
      PIN(iq_error_sum) = PIN(iq_error_sum) + PIN(cur_bw) * PIN(r) * iq_error * period;
      PIN(q_cmd) += PIN(iq_error_sum);
      PIN(q_cmd) = LIMIT(PIN(q_cmd), PIN(pwm_volt));


      if(ABS(PIN(vel_fb)) > 0.1){
        float psi  = (PIN(uq_fb) - PIN(iq_fb) * PIN(r)) / (PIN(vel_fb) * PIN(pp));
        PIN(psi) = PIN(psi) * (1.0 - period / PIN(pi)) + psi * period / PIN(pi);
      }

      PIN(psi) = CLAMP(PIN(psi), 0.001, 1.0);

      PIN(timer) += period;
      if(PIN(timer) >= 5.0){
        PIN(timer) = 0.0;
        PIN(en) = 0.0;
        PIN(d_cmd) = 0.0;
        PIN(q_cmd) = 0.0;
        PIN(id_error_sum) = 0.0;
        PIN(iq_error_sum) = 0.0;
        PIN(torque_sum) = 0.0;

        PIN(state) = 3.3;
      }
    break;

    case 42: // j, d, f
      if(PIN(timer) < 120.0){
        PIN(freq) = PIN(max_vel) / (ABS(PIN(max_pos) - PIN(min_pos)) / 2.0 * 2.0 * M_PI);
        PIN(freq) = MIN(PIN(freq), sqrtf(PIN(max_acc) / (ABS(PIN(max_pos) - PIN(min_pos)) / 2.0 * 4.0 * M_PI * M_PI)));

        float phase = mod(PIN(timer) * 2.0 * M_PI * PIN(freq));
        PIN(pos) = PIN(amp) * sinf(phase);
        PIN(vel) = PIN(amp) * 2.0 * M_PI * PIN(freq) * cosf(phase);
        PIN(acc) = -PIN(amp) * 4.0 * M_PI * M_PI * PIN(freq) * PIN(freq) * sinf(phase);

        PIN(amp) = LIMIT(PIN(amp) + (PIN(max_pos) - PIN(min_pos)) / 2.0 / 30.0 * period, (PIN(max_pos) - PIN(min_pos)) / 2.0);
      }
      else{
        PIN(pos) += PIN(vel) * period + PIN(acc) * period * period / 2.0;
        PIN(vel) += PIN(acc) * period;
        float to_go = PIN(target) - PIN(pos);
        float time_to_go = sqrtf(2.0 * ABS(to_go) / PIN(max_acc));
        float acc = PIN(max_acc) * SIGN(to_go);
        float vel = acc * time_to_go;
        vel = LIMIT(vel, PIN(max_vel));
        acc = (vel - PIN(vel)) / period;
        PIN(acc) = LIMIT(acc, PIN(max_acc));

        if(ABS(PIN(max_pos) - PIN(pos)) < 0.1){
          PIN(target) = PIN(min_pos);
        }
        else if(ABS(PIN(min_pos) - PIN(pos)) < 0.1){
          PIN(target) = PIN(max_pos);
        }
      }
      
      float pos_p = PIN(pos_bw);
      float vel_p = PIN(vel_bw);
      float vel_i = PIN(vel_bw) * PIN(vel_bw);

      float pos_error = minus(mod(PIN(pos)), PIN(pos_fb));
      float vel_cmd = pos_p * pos_error + PIN(vel);
      vel_error = vel_cmd - PIN(vel_fb);
      PIN(fb_torque) = LIMIT(vel_p * vel_error * PIN(inertia), PIN(max_torque));
      PIN(torque_sum) = LIMIT(PIN(torque_sum) + vel_i * vel_error * period * PIN(inertia), PIN(max_torque) - PIN(fb_torque));
      PIN(acc_torque) = PIN(inertia) * PIN(acc);
      PIN(fb_torque) += PIN(torque_sum);
      PIN(ff_torque) = PIN(load) + PIN(damping) * vel_cmd + PIN(friction) * SIGN2(vel_cmd, PIN(max_vel) * 0.001) + PIN(acc_torque);
      PIN(torque) = LIMIT(PIN(fb_torque) + PIN(ff_torque), PIN(max_torque));

      //PIN(load) += period / PIN(li) * PIN(fb_torque) * period;
      PIN(damping) += period / PIN(di) * PIN(fb_torque) * PIN(vel) * period;
      PIN(friction) += period / PIN(fi) * PIN(fb_torque) * SIGN(PIN(vel)) * period;
      PIN(inertia) += period / PIN(ji) * PIN(fb_torque) * PIN(acc) * period;

      //PIN(load) = CLAMP(PIN(load), -100.0, 100.0);
      PIN(damping) = CLAMP(PIN(damping), 0.0, 100.0);
      PIN(friction) = CLAMP(PIN(friction), 0.0, 100.0);
      PIN(inertia) = CLAMP(PIN(inertia), 0.000005, 50.0);

      PIN(en) = 1.0;
      PIN(com_pos) = mod((PIN(pos_fb) + PIN(com_offset)) * PIN(pp));

      id_error = 0.0 - PIN(id_fb);
      PIN(d_cmd) = PIN(r) * PIN(id_fb) + PIN(cur_bw) * PIN(l) * id_error;
      PIN(id_error_sum) = PIN(id_error_sum) + PIN(cur_bw) * PIN(r) * id_error * period;
      PIN(d_cmd) += PIN(id_error_sum);
      PIN(d_cmd) = LIMIT(PIN(d_cmd), PIN(pwm_volt));

      float q_cmd = PIN(torque) / 3.0 * 2.0 / PIN(psi) / PIN(pp);
      iq_error = q_cmd - PIN(iq_fb);
      PIN(q_cmd) = PIN(r) * PIN(iq_fb) + PIN(cur_bw) * PIN(l) * iq_error;
      PIN(iq_error_sum) = PIN(iq_error_sum) + PIN(cur_bw) * PIN(r) * iq_error * period;
      PIN(q_cmd) += PIN(iq_error_sum);
      PIN(q_cmd) = LIMIT(PIN(q_cmd), PIN(pwm_volt));
      
      PIN(timer) += period;
      if(PIN(timer) > 180.0){
        PIN(timer) = 0.0;
        PIN(en) = 0.0;
        PIN(d_cmd) = 0.0;
        PIN(q_cmd) = 0.0;
        PIN(id_error_sum) = 0.0;
        PIN(iq_error_sum) = 0.0;
        PIN(acc) = 0.0;
        PIN(vel) = 0.0;
        PIN(amp) = 0.0;

        PIN(state) = 4.3;
      }
    break;

    case 100:
      PIN(com_pos) = mod((PIN(pos_fb) + PIN(com_offset)) * PIN(pp));
  }
}


hal_comp_t id_comp_struct = {
    .name      = "id",
    .nrt       = nrt,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = nrt_init,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = 0,//sizeof(struct id_ctx_t),
    .pin_count = sizeof(struct id_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};