#include "commands.h"
#include "common.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"

HAL_COMP(curpid);

// enable
HAL_PIN(en);
HAL_PIN(cmd_mode);

// current command
HAL_PIN(id_cmd);
HAL_PIN(iq_cmd);

// current feedback
HAL_PIN(id_fb);
HAL_PIN(iq_fb);

// HAL_PIN(ac_current);

// voltage output
HAL_PIN(ud);
HAL_PIN(uq);

// maximum output current and voltage
HAL_PIN(max_cur);
HAL_PIN(pwm_volt);

// d, q resistance and inductance
HAL_PIN(rd);
HAL_PIN(rq);
HAL_PIN(ld);
HAL_PIN(lq);

// torque constant
HAL_PIN(psi);

HAL_PIN(ff);  // r feed forward
HAL_PIN(kp);
HAL_PIN(ki);
HAL_PIN(kind);  // bemf feed forward

HAL_PIN(vel);  // velocity input

// current error outputs
HAL_PIN(id_error);
HAL_PIN(iq_error);

struct curpid_ctx_t {
  float id_error_sum;
  float iq_error_sum;
};

static void nrt_init(volatile void *ctx_ptr, volatile hal_pin_inst_t *pin_ptr) {
  // struct curpid_ctx_t * ctx = (struct curpid_ctx_t *)ctx_ptr;
  struct curpid_pin_ctx_t *pins = (struct curpid_pin_ctx_t *)pin_ptr;

  PIN(rd)  = 0.5;
  PIN(rq)  = 0.5;
  PIN(ld)  = 0.01;
  PIN(lq)  = 0.01;
  PIN(psi) = 0.05;
  PIN(kp)  = 0.1;
  PIN(ki)  = 0.005;
}

static void rt_func(float period, volatile void *ctx_ptr, volatile hal_pin_inst_t *pin_ptr) {
  struct curpid_ctx_t *ctx      = (struct curpid_ctx_t *)ctx_ptr;
  struct curpid_pin_ctx_t *pins = (struct curpid_pin_ctx_t *)pin_ptr;

  float rd = MAX(PIN(rd), 0.1);
  float rq = MAX(PIN(rq), 0.1);
  float ld = MAX(PIN(ld), 0.001);
  float lq = MAX(PIN(lq), 0.001);

  float ff   = PIN(ff);
  float kind = PIN(kind);
  float kpd  = ld * PIN(kp) / period / 2.0;
  float kid  = rd * PIN(ki) / ld;
  float kpq  = lq * PIN(kp) / period / 2.0;
  float kiq  = rq * PIN(ki) / lq;

  float max_cur = MAX(PIN(max_cur), 0.01);
  float idc     = PIN(id_cmd);
  float iqc     = PIN(iq_cmd);

  float max_volt = PIN(pwm_volt);

  float id = PIN(id_fb);
  float iq = PIN(iq_fb);

  // float ac_current = id * id + iq * iq;  //sqrtf(id * id + iq * iq);
  // PIN(ac_current)  = ac_current;

  float k;
  float abscur;
  
  if(PIN(cmd_mode) == VOLT_MODE) {
    abscur = id * id + iq * iq; // clamp over fb
    if(abscur > max_cur * max_cur) {
      idc = 0;
      iqc = 0;
    }
  }
  else{
    abscur = idc * idc + iqc * iqc; // clamp cmd
    if(abscur > max_cur * max_cur) {
      k = max_cur * max_cur / abscur;
      idc *= k;
      iqc *= k;
    }
  }



  float vel   = PIN(vel);
  float psi_d = ld * id + PIN(psi);
  float psi_q = lq * iq;
  float indd  = vel * psi_q;
  float indq  = vel * psi_d;

  float id_error = idc - id;
  float iq_error = iqc - iq;

  float ud = LIMIT(ff * rd * idc - kind * indd + kpd * id_error, max_volt);
  float uq = LIMIT(ff * rq * iqc + kind * indq + kpq * iq_error, max_volt);

  if(kpd * kid > 0.0 && kpq * kiq > 0.0) {
    ctx->id_error_sum = LIMIT(ctx->id_error_sum + kpd * kid * id_error, max_volt - ud);
    ctx->iq_error_sum = LIMIT(ctx->iq_error_sum + kpq * kiq * iq_error, max_volt - uq);
  } else {
    ctx->id_error_sum = 0.0;
    ctx->iq_error_sum = 0.0;
  }

  ud += ctx->id_error_sum;
  uq += ctx->iq_error_sum;

  if(PIN(cmd_mode) == VOLT_MODE) {
    ud                = LIMIT(idc, max_volt);
    uq                = LIMIT(iqc, max_volt);
    ctx->id_error_sum = 0.0;
    ctx->iq_error_sum = 0.0;
    id_error          = 0.0;
    iq_error          = 0.0;
  }

  if(PIN(en) <= 0.0) {
    ud                = 0.0;
    uq                = 0.0;
    ctx->id_error_sum = 0.0;
    ctx->iq_error_sum = 0.0;
  }

  PIN(ud) = ud;
  PIN(uq) = uq;

  PIN(id_error) = id_error;
  PIN(iq_error) = iq_error;
}

hal_comp_t curpid_comp_struct = {
    .name      = "curpid",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = nrt_init,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = sizeof(struct curpid_ctx_t),
    .pin_count = sizeof(struct curpid_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
