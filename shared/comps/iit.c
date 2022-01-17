#include "iit_comp.h"
#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"

HAL_COMP(iit);

// conf
HAL_PIN(amb_temp);
HAL_PIN(high_temp);
HAL_PIN(max_temp);
HAL_PIN(max_cur);
HAL_PIN(cur_boost);
HAL_PIN(max_time);

// out
HAL_PIN(temp);
// in
HAL_PIN(cur);

struct iit_ctx_t {
  float e;
};

static void nrt_init(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct iit_ctx_t *ctx      = (struct iit_ctx_t *)ctx_ptr;
  struct iit_pin_ctx_t *pins = (struct iit_pin_ctx_t *)pin_ptr;
  PIN(amb_temp) = 30.0;
  PIN(high_temp) = 80.0;
  PIN(max_temp) = 100.0;
  PIN(max_time) = 10.0;
  PIN(cur_boost) = 3.0;
  ctx->e = 0.0;
}

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct iit_ctx_t *ctx      = (struct iit_ctx_t *)ctx_ptr;
  struct iit_pin_ctx_t *pins = (struct iit_pin_ctx_t *)pin_ptr;

  float cur_n = PIN(max_cur) / MAX(PIN(cur_boost), 1.0);
  float max_e = PIN(max_cur) * PIN(max_cur) * MAX(PIN(max_time), 0.1);

  float temp = ctx->e / max_e * (PIN(max_temp) - PIN(amb_temp)) + PIN(amb_temp);

  float pin = PIN(cur) * PIN(cur);
  float pout = (temp - PIN(amb_temp)) * cur_n * cur_n / (PIN(high_temp) - PIN(amb_temp));

  ctx->e += (pin - pout) * period;

  PIN(temp) = temp;
}

hal_comp_t iit_comp_struct = {
  .name      = "iit",
  .nrt       = 0,
  .rt        = rt_func,
  .frt       = 0,
  .nrt_init  = nrt_init,
  .rt_start  = 0,
  .frt_start = 0,
  .rt_stop   = 0,
  .frt_stop  = 0,
  .ctx_size  = sizeof(struct iit_ctx_t),
  .pin_count = sizeof(struct iit_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
