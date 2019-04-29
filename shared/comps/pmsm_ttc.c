#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"

HAL_COMP(pmsm_ttc);

// motor values
HAL_PIN(psi);
HAL_PIN(polecount);

// cogging torque ripple, constant
HAL_PIN(ac);  // amplitude
HAL_PIN(pc);  // phase
HAL_PIN(nc);  // frequency

// electrical torque ripple, linear with current
HAL_PIN(ae);  // amplitude
HAL_PIN(pe);  // phase
HAL_PIN(ne);  // frequency

HAL_PIN(pos_in);
HAL_PIN(pos_out);
HAL_PIN(t);           // compensation torque
HAL_PIN(g);           // compensation gain
HAL_PIN(block_gain);  // block commutation gain


// torque cmd in
HAL_PIN(torque);

// cur cmd out
HAL_PIN(cur);

static void nrt_init(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  // struct sim_ctx_t * ctx = (struct sim_ctx_t *)ctx_ptr;
  struct pmsm_ttc_pin_ctx_t *pins = (struct pmsm_ttc_pin_ctx_t *)pin_ptr;

  PIN(nc)         = 1.0;
  PIN(ne)         = 1.0;
  PIN(block_gain) = 0.0;
}

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  // struct pmsm_ttc_ctx_t * ctx = (struct pmsm_ttc_ctx_t *)ctx_ptr;
  struct pmsm_ttc_pin_ctx_t *pins = (struct pmsm_ttc_pin_ctx_t *)pin_ptr;

  float p      = MAX(PIN(polecount), 1.0);
  float psi_m  = MAX(PIN(psi), 0.01);
  float torque = PIN(torque);
  float pos    = PIN(pos_in);

  float tc = PIN(ac) * sinf(PIN(pc) + pos * PIN(nc));
  float te = torque * PIN(ae) * sinf(PIN(pe) + pos * PIN(ne));

  PIN(pos_out) = pos * (1.0 - PIN(block_gain)) + (((int)((pos / 2.0 / M_PI + 0.5) * 6 + 0.5)) / 6.0 * 2.0 * M_PI - M_PI) * PIN(block_gain);
  PIN(t)       = tc + te;
  PIN(cur)     = (torque + PIN(g) * (tc + te)) / 3.0 * 2.0 / p / psi_m;
}

hal_comp_t pmsm_ttc_comp_struct = {
    .name      = "pmsm_ttc",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = nrt_init,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = 0,
    .pin_count = sizeof(struct pmsm_ttc_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
