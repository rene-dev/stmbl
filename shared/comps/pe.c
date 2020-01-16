#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"

HAL_COMP(pe);

HAL_PIN(udc);
HAL_PIN(idc);
HAL_PIN(ud);
HAL_PIN(uq);
HAL_PIN(id);
HAL_PIN(iq);
HAL_PIN(torque);
HAL_PIN(vel);
HAL_PIN(r);
HAL_PIN(j);
HAL_PIN(cap);

HAL_PIN(e_el);
HAL_PIN(e_kin);
HAL_PIN(p_el_dc);
HAL_PIN(p_el_ac);
HAL_PIN(p_m);
HAL_PIN(p_t);

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  // struct pe_ctx_t * ctx = (struct pe_ctx_t *)ctx_ptr;
  struct pe_pin_ctx_t *pins = (struct pe_pin_ctx_t *)pin_ptr;

  float udc = PIN(udc);
  float vel = PIN(vel);
  float id  = PIN(id);
  float iq  = PIN(iq);
  float t   = PIN(torque);

  PIN(e_el)  = PIN(cap) * udc * udc / 2.0;
  PIN(e_kin) = PIN(j) * vel * vel / 2.0;

  PIN(p_el_dc) = udc * PIN(idc);
  PIN(p_el_ac) = PIN(ud) * PIN(id) + PIN(ud) * PIN(id);
  PIN(p_m)     = t * vel;
  PIN(p_t)     = 3 / 2 * PIN(r) * (id * id + iq * iq);
}

hal_comp_t pe_comp_struct = {
    .name      = "pe",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = 0,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = 0,
    .pin_count = sizeof(struct pe_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
