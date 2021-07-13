#include "svm_comp.h"
#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"

HAL_COMP(svm);

//U V W inputs
HAL_PIN(u);
HAL_PIN(v);
HAL_PIN(w);

//dclink input
HAL_PIN(udc);

//U V W outputs
HAL_PIN(su);
HAL_PIN(sv);
HAL_PIN(sw);

//commutation mode
HAL_PIN(cmode);

//modulation mode
HAL_PIN(mode);

//half bridge enable out
HAL_PIN(enu);
HAL_PIN(env);
HAL_PIN(enw);

static void nrt_init(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  // struct svm_ctx_t * ctx = (struct svm_ctx_t *)ctx_ptr;
  struct svm_pin_ctx_t *pins = (struct svm_pin_ctx_t *)pin_ptr;

  PIN(mode) = 1.0;
  PIN(enu)  = 1.0;
  PIN(env)  = 1.0;
  PIN(enw)  = 1.0;
}

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  // struct svm_ctx_t * ctx = (struct svm_ctx_t *)ctx_ptr;
  struct svm_pin_ctx_t *pins = (struct svm_pin_ctx_t *)pin_ptr;

  float offset = 0;
  float udc    = PIN(udc);

  float u = PIN(u);
  float v = PIN(v);
  float w = PIN(w);

  switch((int)PIN(mode)) {
    default:
    case 0:  // sine modulation
      offset = (u + v + w) / 3.0 - udc / 2.0;
      break;

    case 1:  // space vector modulation
      offset = (MIN3(u, v, w) + MAX3(u, v, w)) / 2.0 - udc / 2.0;
      break;

    case 2:  // flat bottom space vector modulation
      offset = MIN3(u, v, w);
      break;

    case 3:  // flat top space vector modulation
      offset = MAX3(u, v, w) - udc;
      break;
  }

  PIN(enu) = 1.0;
  PIN(env) = 1.0;
  PIN(enw) = 1.0;

  switch((int)PIN(cmode)) {
    case 1:  // block
      if(u > v && u < w) {
        PIN(enu) = 0.0;
      }
      if(v > u && v < w) {
        PIN(env) = 0.0;
      }
      if(w > u && w < v) {
        PIN(enw) = 0.0;
      }
      break;

    default:  // sine
      break;
  }

  PIN(su) = u - offset;
  PIN(sv) = v - offset;
  PIN(sw) = w - offset;
}

hal_comp_t svm_comp_struct = {
    .name      = "svm",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = nrt_init,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = 0,
    .pin_count = sizeof(struct svm_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
