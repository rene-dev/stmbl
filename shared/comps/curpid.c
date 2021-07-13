#include "curpid_comp.h"
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
HAL_PIN(r);
HAL_PIN(ld);
HAL_PIN(lq);

// torque constant
HAL_PIN(psi);

HAL_PIN(ff);  // r feed forward
HAL_PIN(cur_bw);
HAL_PIN(ksp); // predictor
HAL_PIN(kind);  // bemf feed forward
HAL_PIN(kci);

HAL_PIN(scale);

HAL_PIN(vel);  // velocity input

// current error outputs
HAL_PIN(id_error);
HAL_PIN(iq_error);

struct curpid_ctx_t {
  float id_error_sum;
  float iq_error_sum;
};

static void nrt_init(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  // struct curpid_ctx_t * ctx = (struct curpid_ctx_t *)ctx_ptr;
  struct curpid_pin_ctx_t *pins = (struct curpid_pin_ctx_t *)pin_ptr;

  PIN(r)  = 0.5;
  PIN(ld)  = 0.01;
  PIN(lq)  = 0.01;
  PIN(psi) = 0.05;
  PIN(cur_bw)  = 250.0;
  PIN(kci)  = 500.0;
  PIN(ksp)  = 1.0;
  PIN(scale) = 1.0;
}

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct curpid_ctx_t *ctx      = (struct curpid_ctx_t *)ctx_ptr;
  struct curpid_pin_ctx_t *pins = (struct curpid_pin_ctx_t *)pin_ptr;

  float r = MAX(PIN(r), 0.1);
  float ld = MAX(PIN(ld), 0.001);
  float lq = MAX(PIN(lq), 0.001);

  float ff   = PIN(ff);
  float kind = PIN(kind);

  float max_cur = MAX(PIN(max_cur), 0.01);
  float idc     = PIN(id_cmd);
  float iqc     = PIN(iq_cmd);

  float max_volt = PIN(pwm_volt);

  float id = PIN(id_fb);
  float iq = PIN(iq_fb);

  // float ac_current = id * id + iq * iq;  //sqrtf(id * id + iq * iq);
  // PIN(ac_current)  = ac_current;

  float abscur;
  float absvolt;

  if(PIN(cmd_mode) == VOLT_MODE) {
    absvolt = idc * idc + iqc * iqc; // clamp cmd
    PIN(scale) *= sqrtf(CLAMP(max_volt * max_volt / MAX(absvolt, max_volt * 0.1), 0.0, 1.0));

    abscur = id * id + iq * iq; // clamp over fb
    PIN(scale) += (max_cur * max_cur - abscur) * PIN(kci) * period;
  }
  else{
    abscur = idc * idc + iqc * iqc; // clamp cmd
    PIN(scale) = sqrtf(max_cur * max_cur / MAX(abscur, max_cur * 0.1));
  }
  PIN(scale) = CLAMP(PIN(scale), 0.0, 1.0);
  
  idc *= PIN(scale);
  iqc *= PIN(scale);

  float vel   = PIN(vel);
  float psi_d = ld * id + PIN(psi);
  float psi_q = lq * iq;
  float indd  = vel * psi_q;
  float indq  = vel * psi_d;

  // predictor to cancel pwm delay
  id += (PIN(ud) - r * id + indd) / ld * period * PIN(ksp);
  iq += (PIN(uq) - r * iq - indq) / lq * period * PIN(ksp);

  float id_error = idc - id;
  float iq_error = iqc - iq;

  float ud = LIMIT(ff * r * idc - kind * indd + PIN(cur_bw) * ld * id_error, max_volt);
  float uq = LIMIT(ff * r * iqc + kind * indq + PIN(cur_bw) * lq * iq_error, max_volt);

  if(PIN(cur_bw) * r > 0.0) {
    ctx->id_error_sum = LIMIT(ctx->id_error_sum + PIN(cur_bw) * r * id_error * period, max_volt - ud);
    ctx->iq_error_sum = LIMIT(ctx->iq_error_sum + PIN(cur_bw) * r * iq_error * period, max_volt - uq);
  } else {
    ctx->id_error_sum = 0.0;
    ctx->iq_error_sum = 0.0;
  }

  ud += ctx->id_error_sum;
  uq += ctx->iq_error_sum;

  if(PIN(cmd_mode) == VOLT_MODE) {
    ud                = idc;
    uq                = iqc;
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
