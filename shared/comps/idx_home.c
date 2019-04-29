#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"

HAL_COMP(idx_home);

HAL_PIN(mot_state);  // 0 = disabled, 1 = inc, 2 = start abs, 3 = abs
HAL_PIN(fb);
HAL_PIN(fb_abs);
HAL_PIN(index_en);
HAL_PIN(index_clear);
HAL_PIN(pos_out);

struct idx_home_ctx_t {
  int state;
  int lastq;
  int waitabs;
};

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct idx_home_ctx_t *ctx      = (struct idx_home_ctx_t *)ctx_ptr;
  struct idx_home_pin_ctx_t *pins = (struct idx_home_pin_ctx_t *)pin_ptr;

  uint8_t q = 0;

  if(PIN(index_en) > 0) {
    if(PIN(mot_state) == 3) {
      q = quadrant(PIN(fb_abs));
      if(((q == 1 && ctx->lastq == 4) || (q == 4 && ctx->lastq == 1)) || ctx->waitabs == 1) {
        PIN(index_clear) = 1;
        ctx->state       = 1;
      }
      ctx->lastq = q;
    } else {  //index requested, but index not seen
      ctx->waitabs = 1;
    }
  }

  if(!(PIN(index_en) > 0) && ctx->state == 1) {
    PIN(index_clear) = 0;
  }

  if(ctx->state == 1) {
    PIN(pos_out) = PIN(fb_abs);
  } else {
    PIN(pos_out) = PIN(fb);
  }
}

hal_comp_t idx_home_comp_struct = {
    .name      = "idx_home",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = 0,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = sizeof(struct idx_home_ctx_t),
    .pin_count = sizeof(struct idx_home_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
