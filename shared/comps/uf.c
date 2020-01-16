#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"

HAL_COMP(uf);

HAL_PIN(un);
HAL_PIN(fn);
HAL_PIN(f_cmd);
HAL_PIN(f_fb);
HAL_PIN(pos);
HAL_PIN(ud);
HAL_PIN(max_acc);
HAL_PIN(min_acc);

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  // struct uf_ctx_t * ctx = (struct uf_ctx_t *)ctx_ptr;
  struct uf_pin_ctx_t *pins = (struct uf_pin_ctx_t *)pin_ptr;

  if(PIN(f_fb) < PIN(f_cmd)) {
    PIN(f_fb) += PIN(max_acc) * period;
  }
  if(PIN(f_fb) > PIN(f_cmd)) {
    PIN(f_fb) -= PIN(min_acc) * period;
  }
  PIN(pos) += PIN(f_fb) * 2.0 * M_PI * period;
  PIN(pos) = mod(PIN(pos));
  PIN(ud)  = PIN(un) / MAX(PIN(fn), 1.0) * PIN(f_fb);
}

hal_comp_t uf_comp_struct = {
    .name      = "uf",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = 0,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = 0,
    .pin_count = sizeof(struct uf_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
