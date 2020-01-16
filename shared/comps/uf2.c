#include "hal.h"
#include "defines.h"
#include "angle.h"

HAL_COMP(uf);

HAL_PIN(u_n);
HAL_PIN(vel_n);
HAL_PIN(polecount);
HAL_PIN(cur_n);
HAL_PIN(slip_n);


HAL_PIN(scale);

HAL_PIN(vel_cmd);
HAL_PIN(cur);

HAL_PIN(u_cmd);
HAL_PIN(vel_cmd_out);
HAL_PIN(com_pos);

HAL_PIN(load);

struct uf_ctx_t {
  float cur;
};

static void nrt_init(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct uf_ctx_t *ctx      = (struct uf_ctx_t *)ctx_ptr;
  struct uf_pin_ctx_t *pins = (struct uf_pin_ctx_t *)pin_ptr;

  PIN(u_n) = 220.0 * sqrtf(2.0) / 2.0;
  PIN(vel_n) = 3000.0 / 60.0 * 2.0 * M_PI;
  PIN(com_pos) = 0.0;
  PIN(slip_n) = 0.01;
  ctx->cur = 0.0;
}

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct uf_ctx_t *ctx      = (struct uf_ctx_t *)ctx_ptr;
  struct uf_pin_ctx_t *pins = (struct uf_pin_ctx_t *)pin_ptr;

  ctx->cur = PIN(cur) * 0.01 + ctx->cur * 0.99;

  float vel_cmd = PIN(vel_cmd) + ctx->cur / MAX(PIN(cur_n), 0.1) * PIN(slip_n) * PIN(vel_n);
  PIN(vel_cmd_out) = vel_cmd;
  PIN(u_cmd) = vel_cmd / MAX(PIN(vel_n), 0.1) * PIN(u_n) * PIN(scale);
  PIN(com_pos) = mod(PIN(com_pos) + vel_cmd * PIN(polecount) * period);
  PIN(load) = ctx->cur / MAX(PIN(cur_n), 0.1);
}

const hal_comp_t uf_comp_struct = {
    .name      = "uf",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = nrt_init,
    .hw_init   = 0,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = sizeof(struct uf_ctx_t),
    .pin_count = sizeof(struct uf_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
