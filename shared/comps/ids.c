#include "ids_comp.h"
#include "hal.h"
#include "defines.h"
#include "angle.h"

HAL_COMP(ids);

HAL_PIN(en);

HAL_PIN(state);
HAL_PIN(param);
HAL_PIN(step);
HAL_PIN(rep);

HAL_PIN(freq);
HAL_PIN(amp);
HAL_PIN(min_pos);
HAL_PIN(max_pos);
HAL_PIN(max_vel);
HAL_PIN(max_acc);

HAL_PIN(pos);
HAL_PIN(vel);
HAL_PIN(acc);
HAL_PIN(pos_cmd);
HAL_PIN(vel_cmd);
HAL_PIN(acc_cmd);

HAL_PIN(pos_error);
HAL_PIN(vel_error);

HAL_PIN(pos_bw);
HAL_PIN(vel_bw);
HAL_PIN(vel_d);
HAL_PIN(cur_bw);

HAL_PIN(ff);
HAL_PIN(kp);
HAL_PIN(ks);
HAL_PIN(kv);
HAL_PIN(kt);
HAL_PIN(kd);

HAL_PINA(params, 3);
HAL_PINA(max_params, 3);

HAL_PIN(target);
HAL_PIN(cost);
HAL_PIN(min_cost);
HAL_PIN(auto_step);

HAL_PIN(timer);

static void nrt_init(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  //struct ids_ctx_t * ctx = (struct ids_ctx_t *)ctx_ptr;
  struct ids_pin_ctx_t *pins = (struct ids_pin_ctx_t *)pin_ptr;
  PIN(pos_bw) = 10.0;
  PIN(vel_bw) = 100.0;
  PIN(vel_d) = 10.0;
  PINA(params, 0) = PIN(vel_bw);
  PINA(params, 1) = 1.0 / PIN(vel_d);
  PINA(params, 2) = PIN(pos_bw);

  PIN(kp) = 1.0;
  PIN(ks) = 0.0;
  PIN(kv) = 1.0;
  PIN(kt) = 1.2;
  PIN(kd) = 0.7;

  PIN(ff) = 1.0;

  PIN(max_vel) = 100.0;
  PIN(max_acc) = 1000.0;
  PIN(min_pos) = -10.0;
  PIN(max_pos) = 10.0;

  PIN(param) = 0.0;
  PIN(step) = 0.25;
  PIN(rep) = 2.0;

  PIN(auto_step) = 1.0;
}

static void nrt(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  //struct ids_ctx_t * ctx = (struct ids_ctx_t *)ctx_ptr;
  struct ids_pin_ctx_t *pins = (struct ids_pin_ctx_t *)pin_ptr;

  switch((int)(PIN(state) * 10.0 + 0.5)){
    case 0:
    break;
    
    case 10:
      PIN(state) = 1.1;
      PIN(target) = PIN(max_pos);

      if(PIN(auto_step) >= 1){
        PIN(state) = 1.2;
      }
      else{
        printf("Tune PPI bandwidth and damping\n");
        printf("the motor will move\n");
        printf("ids0.state = 1.2 <font color='green'>to start</font>\n");
      }
    break;

    case 13:
      printf("conf0.pos_bw = %f <font color='green'># append to config</font>\n", PIN(pos_bw));
      printf("conf0.vel_bw = %f <font color='green'># append to config</font>\n", PIN(vel_bw));
      printf("conf0.vel_d = %f <font color='green'># append to config</font>\n", PIN(vel_d));
      printf("done\n");
      PIN(state) = 1.4;
    break;
  }
}

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  //struct ids_ctx_t * ctx = (struct ids_ctx_t *)ctx_ptr;
  struct ids_pin_ctx_t *pins = (struct ids_pin_ctx_t *)pin_ptr;

  if(PIN(en) <= 0.0){
    PIN(state) = 0.0;
  }

  switch((int)(PIN(state) * 10.0 + 0.5)){
    case 0:
      PIN(acc_cmd) = 0.0;
      PIN(vel_cmd) = 0.0;
      PIN(acc) = 0.0;
      PIN(vel) = 0.0;

      PIN(param) = 0.0;
      PIN(step) = 0.1;
      PIN(rep) = 1.0;

      PIN(pos_bw) = 10.0;
      PIN(vel_bw) = 100.0;
      PIN(vel_d) = 10.0;
      PINA(params, 0) = PIN(vel_bw);
      PINA(params, 1) = 1.0 / PIN(vel_d);
      PINA(params, 2) = PIN(pos_bw);

      PINA(max_params, 0) = PIN(cur_bw) / 2.0;
      PINA(max_params, 1) = 1.0;

      PIN(min_cost) = (PIN(max_pos) - PIN(min_pos)) * (PIN(max_pos) - PIN(min_pos)) / PIN(max_vel) * 100.0;
      PIN(cost) = PIN(min_cost);

      if(PIN(en) > 0.0){
        PIN(state) = 1.0;
      }
    break;

    case 12:
      PIN(pos) += PIN(vel) * period + PIN(acc) * period * period / 2.0;
      PIN(vel) += PIN(acc) * period;
      float to_go = PIN(target) - PIN(pos);
      float time_to_go = sqrtf(2.0 * ABS(to_go) / PIN(max_acc));
      float acc = PIN(max_acc) * SIGN(to_go);
      float vel = acc * time_to_go;
      vel = LIMIT(vel, PIN(max_vel));
      acc = (vel - PIN(vel)) / period;

      if(time_to_go < period){
        time_to_go = 0.0;
        to_go = 0.0;
        vel = 0.0;
        acc = 0.0;
        PIN(pos) = PIN(target);
        PIN(vel) = 0.0;
        PIN(acc) = 0.0;
      }

      PIN(acc) = LIMIT(acc, PIN(max_acc));

      PIN(pos_cmd) = mod(PIN(pos));
      PIN(vel_cmd) = PIN(vel) * PIN(ff);
      PIN(acc_cmd) = PIN(acc) * PIN(ff);
      
      PIN(cost) += ABS(PIN(pos_error)) * PIN(kp) * period;
      PIN(cost) += PIN(pos_error) * PIN(pos_error) * PIN(ks) * period;
      PIN(cost) += PIN(vel_error) * PIN(vel_error) * PIN(kv) * period;
      

      PIN(timer) += period;
      if(PIN(timer) < (ABS(PIN(max_pos) - PIN(min_pos)) / PIN(max_vel) + 2.0 * PIN(max_vel) / PIN(max_acc))){
        PIN(target) = PIN(max_pos);
      }
      else{
        PIN(target) = PIN(min_pos);
      }
      if(PIN(timer) > 2.0 * (ABS(PIN(max_pos) - PIN(min_pos)) / PIN(max_vel) + 2.0 * PIN(max_vel) / PIN(max_acc))){
        PIN(timer) = 0.0;
      }
      
      if(PIN(timer) == 0.0){
        PIN(min_cost) = MIN(PIN(cost), PIN(min_cost));

        PINA(max_params, 2) = PINA(params, 0) * 2.0;

        if(PINA(params, (int)PIN(param)) > PINA(max_params, (int)PIN(param))){
          PINA(params, (int)PIN(param)) = PINA(max_params, (int)PIN(param));
          PIN(min_cost) = PIN(cost) * 10.0;
          PIN(param)++;
        }
        else if(PIN(cost) > PIN(min_cost) * PIN(kt)){
          PINA(params, (unsigned int)PIN(param)) *= PIN(kd);
          PIN(min_cost) = PIN(cost) * 10.0;
          PIN(param)++;
        }
        else{
          PINA(params, (unsigned int)PIN(param)) *= 1.0 + PIN(step);
        }

        PIN(cost) = 0.0;
      }
      
      PIN(pos_bw) = PINA(params, 2);
      PIN(vel_bw) = PINA(params, 0);
      PIN(vel_d) = 1.0 / PINA(params, 1);
      
      if(PIN(param) > 2.0){
        PIN(acc_cmd) = 0.0;
        PIN(vel_cmd) = 0.0;
        PIN(param) = 0.0;

        PIN(state) = 1.3;
      }
    break;
  }
}


hal_comp_t ids_comp_struct = {
    .name      = "ids",
    .nrt       = nrt,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = nrt_init,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = 0,//sizeof(struct ids_ctx_t),
    .pin_count = sizeof(struct ids_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};