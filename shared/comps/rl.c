#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"

HAL_COMP(rl);

HAL_PIN(ra);
HAL_PIN(rb);
HAL_PIN(ld);
HAL_PIN(lq);
HAL_PIN(la);
HAL_PIN(lb);

HAL_PIN(max_cur);
HAL_PIN(udc);
HAL_PIN(start);
HAL_PIN(state);

HAL_PIN(ki);
HAL_PIN(time);
HAL_PIN(t);

HAL_PIN(ia_fb);
HAL_PIN(ib_fb);
HAL_PIN(ua_fb);
HAL_PIN(ub_fb);
HAL_PIN(ua);
HAL_PIN(ub);

struct rl_ctx_t{
  uint32_t state;
  float error_sum;
  float time;
  float ra;
  float rb;
};

static void nrt_init(volatile void *ctx_ptr, volatile hal_pin_inst_t *pin_ptr) {
  struct rl_ctx_t * ctx = (struct rl_ctx_t *)ctx_ptr;
  struct rl_pin_ctx_t *pins = (struct rl_pin_ctx_t *)pin_ptr;

  PIN(time) = 1.0;
  PIN(ki) = 100.0;
  PIN(max_cur) = 1.0;
  ctx->ra = 0.0;
  ctx->rb = 0.0;
  ctx->state = 0;
  ctx->time = 0.0;
  ctx->error_sum = 0.0;
}

static void rt_func(float period, volatile void *ctx_ptr, volatile hal_pin_inst_t *pin_ptr) {
  struct rl_ctx_t * ctx = (struct rl_ctx_t *)ctx_ptr;
  struct rl_pin_ctx_t *pins = (struct rl_pin_ctx_t *)pin_ptr;

  switch(ctx->state){
    case 0:
      if(PIN(start) > 0.0){
        ctx->state = 1;
        ctx->error_sum = 0.0;
        ctx->time = 0.0;
      }
      PIN(ua) = 0.0;
      PIN(ub) = 0.0;
    break;

    case 1: // measure ra
      ctx->time += period;
      ctx->error_sum += (PIN(max_cur) - PIN(ia_fb)) * period * PIN(ki);
      PIN(ua) = ctx->error_sum;

      if(ABS(PIN(ia_fb) > 0.1)){
        ctx->ra = ctx->ra * 0.95 + PIN(ua_fb) / PIN(ia_fb) * 0.05;
      }
      
      if(ctx->time >= PIN(time)){
        ctx->time = 0.0;
        ctx->state = 2;
        ctx->error_sum = 0.0;
        PIN(ua) = 0.0;
        PIN(ub) = 0.0;
      }
    break;

    case 2: // measure rb
      ctx->time += period;
      ctx->error_sum += (PIN(max_cur) - PIN(ib_fb)) * period * PIN(ki);
      PIN(ub) = ctx->error_sum;

      if(ABS(PIN(ib_fb) > 0.1)){
        ctx->rb = ctx->rb * 0.95 + PIN(ub_fb) / PIN(ib_fb) * 0.05;
      }
      
      if(ctx->time >= PIN(time)){
        ctx->time = 0.0;
        ctx->state = 3;
        ctx->error_sum = 0.0;
        PIN(ua) = 0.0;
        PIN(ub) = 0.0;
      }
    break;

    default:
      if(PIN(start) <= 0.0){
        ctx->state = 0;
      }
      ctx->time = 0.0;
      PIN(ua) = 0.0;
      PIN(ub) = 0.0;
    break;
  }

  PIN(state) = ctx->state;
  PIN(t) = ctx->time;
  PIN(ra) = ctx->ra;
  PIN(rb) = ctx->rb;
}

hal_comp_t rl_comp_struct = {
    .name      = "rl",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = nrt_init,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = 0,
    .pin_count = sizeof(struct rl_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
