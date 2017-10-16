#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"

HAL_COMP(ramp);

HAL_PIN(in);
HAL_PIN(out);
HAL_PIN(ramp);

HAL_PIN(min);
HAL_PIN(max);
HAL_PIN(min_acc);
HAL_PIN(max_acc);
HAL_PIN(brake_scale);

struct ramp_ctx_t{
  float out;
};

static void nrt_init(volatile void *ctx_ptr, volatile hal_pin_inst_t *pin_ptr) {
  struct ramp_pin_ctx_t *pins = (struct ramp_pin_ctx_t *)pin_ptr;  
  PIN(brake_scale) = 1.0;
}

static void rt_func(float period, volatile void *ctx_ptr, volatile hal_pin_inst_t *pin_ptr) {
  struct ramp_ctx_t * ctx = (struct ramp_ctx_t *)ctx_ptr;
  struct ramp_pin_ctx_t *pins = (struct ramp_pin_ctx_t *)pin_ptr;

  float min = PIN(min);
  float max = MAX(min + 0.0001, PIN(max));

  float in = CLAMP(PIN(in), min, max);
  float brake_scale = (ABS(in) - ABS(ctx->out)) <= 0.0 ? PIN(brake_scale) : 1.0;
  float scale = (ABS(ctx->out) - MAX(0.0, min)) / (MAX(0.0001, max) - MAX(0.0, min));
  float acc = PIN(min_acc) + (PIN(max_acc) - PIN(min_acc)) * scale;
  acc *= brake_scale;

  ctx->out += LIMIT((in - ctx->out) * acc * period, acc * period);

  if((in - ctx->out) / (max - min) < 0.001){
    PIN(ramp) = 0.0;
  }
  else{
    PIN(ramp) = 1.0;
  }

  ctx->out = CLAMP(ctx->out, min, max);
  PIN(out) = ctx->out;
}

hal_comp_t ramp_comp_struct = {
    .name      = "ramp",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = nrt_init,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = sizeof(struct ramp_ctx_t),
    .pin_count = sizeof(struct ramp_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
