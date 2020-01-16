#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"

HAL_COMP(vel);

HAL_PIN(pos_in);
HAL_PIN(pos_out);
HAL_PIN(vel);
HAL_PIN(acc);
HAL_PIN(w);
HAL_PIN(d);
HAL_PIN(g);
HAL_PIN(h);
HAL_PIN(j);
HAL_PIN(lp);
HAL_PIN(torque);
HAL_PIN(vel_ff);
HAL_PIN(en);
HAL_PIN(pos_error);

struct vel_ctx_t {
  float last_acc;
  float acc_sum;
  float vel_sum;
};

static void nrt_init(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  // struct vel_ctx_t * ctx = (struct vel_ctx_t *)ctx_ptr;
  struct vel_pin_ctx_t *pins = (struct vel_pin_ctx_t *)pin_ptr;

  PIN(w)  = 1000.0;
  PIN(d)  = 0.9;
  PIN(g)  = 1.0;
  PIN(h)  = 1.0;
  PIN(j)  = 0.00001;
  PIN(lp) = 50.0;
  PIN(en) = 1.0;
}

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct vel_ctx_t *ctx      = (struct vel_ctx_t *)ctx_ptr;
  struct vel_pin_ctx_t *pins = (struct vel_pin_ctx_t *)pin_ptr;

  if(PIN(en) == 0.0) {
    ctx->vel_sum = PIN(pos_in);
    ctx->acc_sum = 0.0;
  }
  float vel_ff = PIN(vel_ff) * PIN(h);
  ctx->vel_sum += (ctx->acc_sum + vel_ff) * period;  // ff

  float pos_error = minus(PIN(pos_in), ctx->vel_sum);
  float w         = PIN(w);
  float d         = PIN(d);
  float g         = PIN(g);
  float lp        = LP_HZ(PIN(lp));
  float j         = MAX(PIN(j), 0.0000001);
  float acc       = g * PIN(torque) / j;

  ctx->last_acc = acc * lp + (1.0 - lp) * ctx->last_acc;

  float acc_ff = acc - ctx->last_acc;

  acc_ff += pos_error * w * w;

  ctx->acc_sum += acc_ff * period;

  PIN(vel) = ctx->acc_sum + vel_ff;
  PIN(acc) = acc_ff;

  vel_ff = 2.0 * d * w * pos_error;

  ctx->vel_sum += vel_ff * period;
  ctx->vel_sum = mod(ctx->vel_sum);

  PIN(pos_out)   = ctx->vel_sum;
  PIN(pos_error) = minus(PIN(pos_in), ctx->vel_sum);
}

hal_comp_t vel_comp_struct = {
    .name      = "vel",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = nrt_init,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = sizeof(struct vel_ctx_t),
    .pin_count = sizeof(struct vel_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
