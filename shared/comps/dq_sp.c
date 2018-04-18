#include "commands.h"
#include "common.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"

HAL_COMP(dq_sp);

HAL_PIN(mode);

//rotor position
HAL_PIN(pos);
HAL_PIN(polecount);  //1

//a,b input
HAL_PIN(a);
HAL_PIN(b);

//d,q output
HAL_PIN(d);
HAL_PIN(q);

static void rt_func(float period, volatile void *ctx_ptr, volatile hal_pin_inst_t *pin_ptr) {
  // struct dq_ctx_t * ctx = (struct dq_ctx_t *)ctx_ptr;
  struct dq_pin_ctx_t *pins = (struct dq_pin_ctx_t *)pin_ptr;

  //clarke transformation
  float a = PIN(a);
  float b = PIN(b);

  float p   = (int)MAX(PIN(polecount), 1.0);
  float pos = PIN(pos) * p;

  float si = 0.0;
  float co = 0.0;
  sincos_fast(pos, &si, &co);

  //park transformation
  float d = a * co + b * si;
  float q = -a * si + b * co;

  PIN(d) = d;
  PIN(q) = q;
}

hal_comp_t dq_comp_struct = {
    .name      = "dq_sp",
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
