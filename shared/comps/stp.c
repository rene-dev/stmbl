#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"

HAL_COMP(stp);

HAL_PIN(target);
HAL_PIN(pos_out);
HAL_PIN(vel_out);
HAL_PIN(acc_out);

HAL_PIN(max_vel);
HAL_PIN(max_acc);

HAL_PIN(dtg);

struct stp_ctx_t {
  float p0;
  float p;
  float v0;
  float pold;
  float vold;
};

static void rt_func(float period, volatile void *ctx_ptr, volatile hal_pin_inst_t *pin_ptr) {
  struct stp_ctx_t *ctx      = (struct stp_ctx_t *)ctx_ptr;
  struct stp_pin_ctx_t *pins = (struct stp_pin_ctx_t *)pin_ptr;

  float p1 = PIN(target);

  float vmax = PIN(max_vel);
  float amax = MAX(PIN(max_acc), 0.1);

  float vel = SIGN(p1 - ctx->p0) * amax * sqrtf(2.0 * ABS(p1 - ctx->p0) / amax);

  vel = LIMIT(vel, vmax);
  vel = CLAMP(vel, ctx->v0 - amax * period, ctx->v0 + amax * period);

  if(ABS(vel) > ABS(p1 - ctx->p0) / period) {
    vel = (p1 - ctx->p0) / period;
  }

  float acc = (vel - ctx->vold) / period;
  ctx->vold = vel;

  ctx->v0 = vel;
  ctx->p0 += ctx->v0 * period;
  ctx->p += ctx->p0 - ctx->pold;
  ctx->pold = ctx->p0;

  //p += vel * period;

  PIN(pos_out) = mod(ctx->p);
  PIN(vel_out) = vel;
  PIN(acc_out) = acc;
  PIN(dtg)     = p1 - ctx->p0;
}

const hal_comp_t stp_comp_struct = {
    .name      = "stp",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = 0,
    .hw_init   = 0,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = sizeof(struct stp_ctx_t),
    .pin_count = sizeof(struct stp_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
