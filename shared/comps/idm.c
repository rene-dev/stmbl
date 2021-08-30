#include "idm_comp.h"
#include "hal.h"
#include "defines.h"
#include "angle.h"

HAL_COMP(idm);

HAL_PIN(en);

HAL_PIN(state);
HAL_PIN(timer);

HAL_PIN(freq);
HAL_PIN(amp);
HAL_PIN(min_pos);
HAL_PIN(max_pos);
HAL_PIN(max_vel);
HAL_PIN(max_acc);

HAL_PIN(pos);
HAL_PIN(pos_cmd);
HAL_PIN(vel_cmd);
HAL_PIN(acc_cmd);

HAL_PIN(pos_error);
HAL_PIN(vel_error);

HAL_PIN(ji);
HAL_PIN(fi);
HAL_PIN(di);
HAL_PIN(li);

HAL_PIN(fb_torque);

HAL_PIN(inertia);
HAL_PIN(damping);
HAL_PIN(friction);
HAL_PIN(load);

HAL_PIN(pos_bw);
HAL_PIN(vel_bw);
HAL_PIN(vel_d);

HAL_PIN(sys);

HAL_PIN(target);
HAL_PIN(auto_step);

static void nrt_init(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  //struct idm_ctx_t * ctx = (struct idm_ctx_t *)ctx_ptr;
  struct idm_pin_ctx_t *pins = (struct idm_pin_ctx_t *)pin_ptr;
  PIN(fi) = 0.001;
  PIN(li) = 10.0;
  PIN(ji) = 200.0;
  PIN(di) = 10.0;
  PIN(pos_bw) = 5.0;
  PIN(vel_bw) = 25.0;
  PIN(vel_d) = 1.0;
  PIN(max_vel) = 50.0;
  PIN(max_acc) = 1000.0;
  PIN(min_pos) = -10.0;
  PIN(max_pos) = 10.0;
  PIN(auto_step) = 1.4;
  PIN(inertia) = 0.0001;
}

static void nrt(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  //struct idm_ctx_t * ctx = (struct idm_ctx_t *)ctx_ptr;
  struct idm_pin_ctx_t *pins = (struct idm_pin_ctx_t *)pin_ptr;

  switch((int)(PIN(state) * 10.0 + 0.5)){
    case 0:
      //PIN(inertia) = 0.0001;
    break;
    
    case 10:
      PIN(state) = 1.1;
      PIN(timer) = 0.0;
      PIN(target) = PIN(min_pos);

      if(PIN(auto_step) >= 1){
        PIN(state) = 1.2;
      }
      else{
        printf("Measure friction, damping and inertia\n");
        printf("the motor will move\n");
        printf("idm0.state = 1.2 <font color='green'>to start</font>\n");
      }
    break;

    case 13:
      if(PIN(sys) > 0.0){
        printf("conf0.j_sys = %f <font color='green'># append to config</font>\n", PIN(inertia));
      }
      else{
        printf("conf0.j = %f <font color='green'># append to config</font>\n", PIN(inertia));
      }
      printf("conf0.d = %f <font color='green'># append to config</font>\n", PIN(damping));
      printf("conf0.f = %f <font color='green'># append to config</font>\n", PIN(friction));
      printf("done\n");
      //printf("pid0.l = %f\n", PIN(load));
      PIN(state) = 1.4;
    break;
  }
}

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  //struct idm_ctx_t * ctx = (struct idm_ctx_t *)ctx_ptr;
  struct idm_pin_ctx_t *pins = (struct idm_pin_ctx_t *)pin_ptr;

  if(PIN(en) <= 0.0){
    PIN(state) = 0.0;
  }

  switch((int)(PIN(state) * 10.0 + 0.5)){
    case 0:
      PIN(timer) = 0.0;
      PIN(acc_cmd) = 0.0;
      PIN(vel_cmd) = 0.0;
      PIN(amp) = 0.0;

      if(PIN(en) > 0.0){
        PIN(state) = 1.0;
      }
    break;

    case 12: // j, d, f
      if(PIN(timer) < 120.0){
        PIN(freq) = PIN(max_vel) / (ABS(PIN(max_pos) - PIN(min_pos)) / 2.0 * 2.0 * M_PI);
        PIN(freq) = MIN(PIN(freq), sqrtf(PIN(max_acc) / (ABS(PIN(max_pos) - PIN(min_pos)) / 2.0 * 4.0 * M_PI * M_PI)));

        float phase = mod(PIN(timer) * 2.0 * M_PI * PIN(freq));
        PIN(pos) = PIN(amp) * sinf(phase);
        PIN(vel_cmd) = PIN(amp) * 2.0 * M_PI * PIN(freq) * cosf(phase);
        PIN(acc_cmd) = -PIN(amp) * 4.0 * M_PI * M_PI * PIN(freq) * PIN(freq) * sinf(phase);

        PIN(amp) = LIMIT(PIN(amp) + (PIN(max_pos) - PIN(min_pos)) / 2.0 / 30.0 * period, (PIN(max_pos) - PIN(min_pos)) / 2.0);
      }
      else{
        PIN(pos) += PIN(vel_cmd) * period + PIN(acc_cmd) * period * period / 2.0;
        PIN(vel_cmd) += PIN(acc_cmd) * period;
        float to_go = PIN(target) - PIN(pos);
        float time_to_go = sqrtf(2.0 * ABS(to_go) / PIN(max_acc));
        float acc = PIN(max_acc) * SIGN(to_go);
        float vel = acc * time_to_go;
        vel = LIMIT(vel, PIN(max_vel));
        acc = (vel - PIN(vel_cmd)) / period;
        PIN(acc_cmd) = LIMIT(acc, PIN(max_acc));

        if(ABS(PIN(max_pos) - PIN(pos)) < 0.1){
          PIN(target) = PIN(min_pos);
        }
        else if(ABS(PIN(min_pos) - PIN(pos)) < 0.1){
          PIN(target) = PIN(max_pos);
        }
      }

      PIN(pos_cmd) = mod(PIN(pos));
      

      PIN(inertia) += period / PIN(ji) * PIN(fb_torque) * PIN(acc_cmd) * period;
      PIN(damping) += period / PIN(di) * PIN(fb_torque) * PIN(vel_cmd) * period;
      PIN(friction) += period / PIN(fi) * PIN(fb_torque) * SIGN(PIN(vel_cmd)) * period;
      PIN(load) += period / PIN(li) * PIN(fb_torque) * period;

      PIN(inertia) = CLAMP(PIN(inertia), 0.000005, 50.0);
      PIN(damping) = CLAMP(PIN(damping), 0.0, 100.0);
      PIN(friction) = CLAMP(PIN(friction), 0.0, 100.0);
      PIN(load) = CLAMP(PIN(load), -100.0, 100.0);
      

      PIN(timer) += period;
      if(PIN(timer) > 180.0){
        PIN(timer) = 0.0;
        PIN(acc_cmd) = 0.0;
        PIN(vel_cmd) = 0.0;
        PIN(amp) = 0.0;

        PIN(state) = 1.3;
      }
    break;
  }
}


hal_comp_t idm_comp_struct = {
    .name      = "idm",
    .nrt       = nrt,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = nrt_init,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = 0,//sizeof(struct idm_ctx_t),
    .pin_count = sizeof(struct idm_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};