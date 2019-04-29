#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"

HAL_COMP(reslimit);

HAL_PIN(pos_in);
HAL_PIN(pos_out);

HAL_PIN(res);

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  // struct reslimit_ctx_t * ctx = (struct reslimit_ctx_t *)ctx_ptr;
  struct reslimit_pin_ctx_t *pins = (struct reslimit_pin_ctx_t *)pin_ptr;
  //TODO: offset at zerocross
  uint32_t r   = ABS(PIN(res));  //TODO: div by zero
  PIN(pos_out) = ((int)(PIN(pos_in) * r / 2.0 / M_PI + 0.5)) / (float)r * 2.0 * M_PI;
}

hal_comp_t reslimit_comp_struct = {
    .name      = "reslimit",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = 0,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = 0,
    .pin_count = sizeof(struct reslimit_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
