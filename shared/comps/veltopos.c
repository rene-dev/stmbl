#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"

HAL_COMP(veltopos);

HAL_PIN(vel);
HAL_PIN(vel_fb);
HAL_PIN(max_acc);
HAL_PIN(max_vel);
HAL_PIN(polecount);
HAL_PIN(pos);


struct veltopos_ctx_t {
  float vel;
};

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct veltopos_ctx_t *ctx      = (struct veltopos_ctx_t *)ctx_ptr;
  struct veltopos_pin_ctx_t *pins = (struct veltopos_pin_ctx_t *)pin_ptr;

  //ctx->vel = CLAMP(PIN(vel), ctx->vel - PIN(max_acc) * period, ctx->vel + PIN(max_acc) * period);
  ctx->vel = LIMIT(ctx->vel, PIN(max_vel));
  ctx->vel = CLAMP(PIN(vel), PIN(vel_fb) - PIN(max_acc) * period, PIN(vel_fb) + PIN(max_acc) * period);
  PIN(pos) += ctx->vel * PIN(polecount) * period;
  PIN(pos) = mod(PIN(pos));
}

hal_comp_t veltopos_comp_struct = {
    .name      = "veltopos",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = 0,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = sizeof(struct veltopos_ctx_t),
    .pin_count = sizeof(struct veltopos_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
