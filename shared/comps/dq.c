#include "commands.h"
#include "common.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"

HAL_COMP(dq);

HAL_PIN(mode);

//U V W inputs
HAL_PIN(u);
HAL_PIN(v);
HAL_PIN(w);

//rotor position
HAL_PIN(pos);
HAL_PIN(polecount);  //1

//a,b,gamma output
HAL_PIN(a);
HAL_PIN(b);
HAL_PIN(y);

//d,q output
HAL_PIN(d);
HAL_PIN(q);

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  // struct dq_ctx_t * ctx = (struct dq_ctx_t *)ctx_ptr;
  struct dq_pin_ctx_t *pins = (struct dq_pin_ctx_t *)pin_ptr;

  float u = PIN(u);
  float v = PIN(v);
  float w = PIN(w);

  //clarke transformation
  float a, b, y;

  switch((int)PIN(mode)) {
    case PHASE_90_3PH:  // 90°
      a = u - v;
      b = w - v;
      y = u / 3.0 + v / 3.0 + w / 3.0;
      break;

    case PHASE_120_3PH:  // 120°
      a = u * 2.0 / 3.0 - v / 3.0 - w / 3.0;
      b = v / M_SQRT3 - w / M_SQRT3;
      y = u / 3.0 + v / 3.0 + w / 3.0;
      break;

    case PHASE_180_2PH:  // 180°
      a = 0;
      b = (u - w) / 2.0;
      y = (u + w) / 2.0;
      break;

    case PHASE_180_3PH:  // 180°
      a = v;
      b = (u - w) / 2.0;
      y = (u + w) / 2.0;
      break;

    default:
      a = 0.0;
      b = 0.0;
      y = 0.0;
  }

  float p   = (int)MAX(PIN(polecount), 1.0);
  float pos = PIN(pos) * p;

  float si = 0.0;
  float co = 0.0;
  sincos_fast(pos, &si, &co);

  //park transformation
  float d = a * co + b * si;
  float q = -a * si + b * co;

  PIN(a) = a;
  PIN(b) = b;
  PIN(y) = y;

  PIN(d) = d;
  PIN(q) = q;
}

hal_comp_t dq_comp_struct = {
    .name      = "dq",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = 0,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = 0,
    .pin_count = sizeof(struct dq_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
