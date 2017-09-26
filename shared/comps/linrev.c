//Calculate motor angle from position in machine units

/*

      |
  Q2  |  Q1
      |
-------------
      |
  Q3  |  Q4
      |

*/


#include "hal.h"
#include "angle.h"
#include <math.h>

HAL_COMP(linrev);

HAL_PIN(scale);

HAL_PIN(cmd_in);
HAL_PIN(cmd_out);

HAL_PIN(cmd_d_in);
HAL_PIN(cmd_d_out);

HAL_PIN(fb_in);
HAL_PIN(fb_out);

struct linrev_ctx_t {
  uint8_t lastq;  //last quadrant
  int32_t rev;  //current multiturn
};

static void rt_func(float period, volatile void *ctx_ptr, volatile hal_pin_inst_t *pin_ptr) {
  struct linrev_ctx_t *ctx      = (struct linrev_ctx_t *)ctx_ptr;
  struct linrev_pin_ctx_t *pins = (struct linrev_pin_ctx_t *)pin_ptr;

  float s = PIN(scale);
  if(s < 0.01) {
    s = 0.01;
  }

  PIN(cmd_out)   = mod((PIN(cmd_in) / s) * 2.0 * M_PI);
  PIN(cmd_d_out) = PIN(cmd_d_in) / s * 2.0 * M_PI;

  uint8_t q = 0;

  if(PIN(fb_in) > M_PI / 2.0) {
    q = 2;
  }
  if(PIN(fb_in) < -M_PI / 2.0) {
    q = 3;
  }

  if(q != 0 && q == 3 && ctx->lastq == 2) {
    ctx->rev++;
  }

  if(q != 0 && q == 2 && ctx->lastq == 3) {
    ctx->rev--;
  }

  ctx->lastq = q;

  PIN(fb_out) = ((PIN(fb_in) + ctx->rev * M_PI * 2.0) * s) / (2.0 * M_PI);
}

const hal_comp_t linrev_comp_struct = {
    .name      = "linrev",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = 0,
    .hw_init   = 0,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = sizeof(struct linrev_ctx_t),
    .pin_count = sizeof(struct linrev_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
