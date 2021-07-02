#include "motsim_comp.h"
#include "hal.h"
#include "defines.h"
#include "angle.h"

HAL_COMP(motsim);


// sim inputs
HAL_PIN(d_cmd);
HAL_PIN(q_cmd);

HAL_PIN(cmd_mode);
HAL_PIN(en);

HAL_PIN(com_pos);
HAL_PIN(com_vel);

HAL_PIN(r);
HAL_PIN(l);
HAL_PIN(psi);

HAL_PIN(load_torque); // load


// sim outputs
HAL_PIN(id_fb);
HAL_PIN(ud_fb);
HAL_PIN(iq_fb);
HAL_PIN(uq_fb);

HAL_PIN(pos_fb);

// sim state
HAL_PIN(pos);
HAL_PIN(vel);
HAL_PIN(acc);
HAL_PIN(temp);
HAL_PIN(id);
HAL_PIN(iq);

// sim config
HAL_PIN(mot_r); // resistance
HAL_PIN(mot_l); // inductance
HAL_PIN(mot_psi); // torque constant

HAL_PIN(mot_pp); // pole pairs
HAL_PIN(mot_j); // inertia
HAL_PIN(mot_f); // friction
HAL_PIN(mot_d); // damping

HAL_PIN(thermal_r); // thermal resistance
HAL_PIN(thermal_mass); // thermal mass

HAL_PIN(deadtime); // pwm deadtime
HAL_PIN(drop); // pwm drop

HAL_PIN(pwm_volt); // dc volt
HAL_PIN(pwm_res); // pwm res

HAL_PIN(cur_res); // current res
HAL_PIN(cur_noise); // current noise

HAL_PIN(fb_res); // fb res
HAL_PIN(fb_noise); // fb noise
HAL_PIN(fb_delay); // fb delay

HAL_PIN(curpid_mult); // curpid iterations / cycle
HAL_PIN(motsim_mult); // motsim iterations / curpid cycle


HAL_PIN(id_error_sum);
HAL_PIN(iq_error_sum);
HAL_PIN(cur_bw);


static void nrt_init(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  //struct motsim_ctx_t * ctx = (struct motsim_ctx_t *)ctx_ptr;
  struct motsim_pin_ctx_t *pins = (struct motsim_pin_ctx_t *)pin_ptr;
  PIN(r) = 1.0;
  PIN(l) = 0.001;
  PIN(psi) = 0.055;
  
  PIN(mot_r) = 0.75;
  PIN(mot_l) = 0.0015;
  PIN(mot_psi) = 0.05;

  PIN(mot_pp) = 3;
  PIN(mot_j) = 0.000025;
  PIN(mot_f) = 0.01;
  PIN(mot_d) = 0.003;

  PIN(thermal_r) = 0.1;
  PIN(thermal_mass) = 0.1;

  PIN(deadtime) = 0.0;
  PIN(drop) = 0.0;

  PIN(pwm_volt) = 320.0;
  PIN(pwm_res) = 4800;

  PIN(cur_res) = 0.01;
  PIN(cur_noise) = 0.0;

  PIN(fb_res) = 4096;
  PIN(fb_noise) = 0.0;
  PIN(fb_delay) = 0.0;

  PIN(curpid_mult) = 3;
  PIN(motsim_mult) = 3;

  PIN(cur_bw) = 500.0;

  PIN(temp) = 20.0;
}


static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  //struct motsim_ctx_t * ctx = (struct motsim_ctx_t *)ctx_ptr;
  struct motsim_pin_ctx_t *pins = (struct motsim_pin_ctx_t *)pin_ptr;

  float ud = 0.0;
  float uq = 0.0;

  float si = 0.0;
  float co = 0.0; 
  sincos_fast(mod(PIN(pos) * PIN(mot_pp)), &si, &co);
  
  // inverse park transformation
  float ia = PIN(id) * co - PIN(iq) * si;
  float ib = PIN(id) * si + PIN(iq) * co;

  sincos_fast(PIN(com_pos), &si, &co);

  // park transformation
  PIN(id_fb) = ia * co + ib * si;
  PIN(iq_fb) = -ia * si + ib * co;
  
  float kp;
  float ki;

  float id_error;
  float iq_error;
  
  float psi_d;
  float psi_q;

  float ind_d;
  float ind_q;

  switch((int)PIN(cmd_mode)){
    case 0: // voltage mode
      ud = PIN(d_cmd);
      uq = PIN(q_cmd);
    break;

    case 1: // current mode
      kp = PIN(cur_bw) * PIN(l);
      ki = PIN(cur_bw) * PIN(r);

      id_error = PIN(d_cmd) - PIN(id_fb);
      iq_error = PIN(q_cmd) - PIN(iq_fb);
      
      psi_d = PIN(l) * PIN(id_fb) + PIN(psi);
      psi_q = PIN(l) * PIN(iq_fb);

      ind_d = PIN(com_vel) * psi_q;
      ind_q = PIN(com_vel) * psi_d;

      ud = LIMIT(PIN(r) * PIN(id_fb) - ind_d + kp * id_error, PIN(pwm_volt));
      uq = LIMIT(PIN(r) * PIN(iq_fb) + ind_q + kp * iq_error, PIN(pwm_volt));

      PIN(id_error_sum) = LIMIT(PIN(id_error_sum) + ki * id_error * period, PIN(pwm_volt) - ud);
      PIN(iq_error_sum) = LIMIT(PIN(iq_error_sum) + ki * iq_error * period, PIN(pwm_volt) - uq);
      ud += PIN(id_error_sum);
      uq += PIN(iq_error_sum);
    break;
  }

  // pwm resolution, limit
  ud = LIMIT((int)(ud / PIN(pwm_volt) * PIN(pwm_res)) / PIN(pwm_res) * PIN(pwm_volt), PIN(pwm_volt));
  uq = LIMIT((int)(uq / PIN(pwm_volt) * PIN(pwm_res)) / PIN(pwm_res) * PIN(pwm_volt), PIN(pwm_volt));

  PIN(ud_fb) = ud;
  PIN(uq_fb) = uq;

  // inverse park transformation
  float ua = ud * co - uq * si;
  float ub = ud * si + uq * co;

  si = 0.0;
  co = 0.0;
  sincos_fast(mod(PIN(pos) * PIN(mot_pp)), &si, &co);

  // park transformation
  ud = ua * co + ub * si;
  uq = -ua * si + ub * co;

  // pwm distortion, deadtime, drop
  //

  if(PIN(en) <= 0.0){
    ud = 0.0;
    uq = 0.0;
  }

  psi_d = PIN(mot_l) * PIN(id) + PIN(mot_psi);
  psi_q = PIN(mot_l) * PIN(iq);

  ind_d = PIN(vel) * PIN(mot_pp) * psi_q;
  ind_q = PIN(vel) * PIN(mot_pp) * psi_d;

  PIN(id) += (ud - PIN(mot_r) * PIN(id) + ind_d) / PIN(mot_l) * period;
  PIN(iq) += (uq - PIN(mot_r) * PIN(iq) - ind_q) / PIN(mot_l) * period;

  float e_torque = 3.0 / 2.0 * PIN(mot_psi) * PIN(iq) * PIN(mot_pp);
  float m_torque = e_torque + PIN(load_torque) - PIN(vel) * PIN(mot_d) - SIGN2(PIN(vel), 0.1) * PIN(mot_f);
  PIN(acc) = m_torque / PIN(mot_j);
  PIN(pos) += PIN(vel) * period + PIN(acc) * period * period / 2.0;
  PIN(pos) = mod(PIN(pos));
  PIN(vel) += PIN(acc) * period;  

  PIN(pos_fb) = ((int)(PIN(pos) / 2.0 / M_PI * PIN(fb_res))) * 2.0 * M_PI / PIN(fb_res);
}


hal_comp_t motsim_comp_struct = {
    .name      = "motsim",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = nrt_init,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = 0,//sizeof(struct motsim_ctx_t),
    .pin_count = sizeof(struct motsim_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};