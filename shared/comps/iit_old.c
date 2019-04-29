#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"

HAL_COMP(iit);

HAL_PIN(brake_r);
HAL_PIN(brake);
HAL_PIN(air_temp);
HAL_PIN(mot_air_res);
HAL_PIN(mot_cap);
HAL_PIN(mot_tau);
HAL_PIN(mot_temp);
HAL_PIN(mot_max_temp);
HAL_PIN(mot_res);
HAL_PIN(mot_psi);
HAL_PIN(mot_real_res);
HAL_PIN(mot_real_psi);
HAL_PIN(mot_cont_cur);
HAL_PIN(id);
HAL_PIN(iq);
HAL_PIN(flow);

struct iit_ctx_t {
  float mot_temp;
};

static void nrt_init(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct iit_ctx_t *ctx      = (struct iit_ctx_t *)ctx_ptr;
  struct iit_pin_ctx_t *pins = (struct iit_pin_ctx_t *)pin_ptr;
  ctx->mot_temp              = 25.0;
  PIN(brake_r)               = 50.0;
  PIN(brake)                 = 2.0;
  PIN(air_temp)              = 20.0;
  PIN(mot_air_res)           = 0.0;
  PIN(mot_cap)               = 0.0;
  PIN(mot_tau)               = 300.0;
  PIN(mot_temp)              = 25.0;
  PIN(mot_max_temp)          = 130.0;
  PIN(mot_res)               = 1.0;
  PIN(mot_psi)               = 1.0;
  PIN(mot_real_res)          = 1.0;
  PIN(mot_real_psi)          = 1.0;
  PIN(mot_cont_cur)          = 2.15;
  PIN(id)                    = 0.0;
  PIN(iq)                    = 0.0;
  PIN(flow)                  = 0.0;
}

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct iit_ctx_t *ctx      = (struct iit_ctx_t *)ctx_ptr;
  struct iit_pin_ctx_t *pins = (struct iit_pin_ctx_t *)pin_ptr;

  float mot_real_res = PIN(mot_res) * (1 + 0.004 * (ctx->mot_temp - PIN(air_temp)));

  float mot_max_res = PIN(mot_res) * (1 + 0.004 * (PIN(mot_max_temp) - PIN(air_temp)));
  float mot_air_res = (PIN(mot_max_temp) - PIN(air_temp)) / (PIN(mot_cont_cur) * PIN(mot_cont_cur) * mot_max_res);
  ;
  float mot_cap = PIN(mot_tau) / mot_air_res;

  float brake_flow = 24.0 * 24.0 / PIN(brake_r) * PIN(brake);
  float flow       = (PIN(id) * PIN(id) + PIN(iq) * PIN(iq)) * 3.0 / 2.0 * mot_real_res - (ctx->mot_temp - PIN(air_temp)) / mot_air_res;

  flow += brake_flow;

  ctx->mot_temp += flow / mot_cap * period;

  PIN(flow)         = flow;
  PIN(mot_air_res)  = mot_air_res;
  PIN(mot_cap)      = mot_cap;
  PIN(mot_temp)     = ctx->mot_temp;
  PIN(mot_real_res) = mot_real_res;
  PIN(mot_real_psi) = PIN(mot_psi) * (1 - 0.0013 * (ctx->mot_temp - PIN(air_temp)));
  ;
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
