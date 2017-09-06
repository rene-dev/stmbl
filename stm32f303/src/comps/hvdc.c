#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"
#include "tim.h"
#include "f3hw.h"

HAL_COMP(hvdc);

//dc voltage input
HAL_PIN(uq);
//dclink input
HAL_PIN(udc);


static void rt_func(float period, volatile void *ctx_ptr, volatile hal_pin_inst_t *pin_ptr) {
  // struct hv_ctx_t * ctx = (struct hv_ctx_t *)ctx_ptr;
  struct hvdc_pin_ctx_t *pins = (struct hvdc_pin_ctx_t *)pin_ptr;

  float udc     = MAX(PIN(udc), 1.0);
  int32_t dcpwm = (int32_t)(PIN(uq) / 2.0 / udc * 4800.0);
  PWM_U         = CLAMP(2400 + dcpwm, 50, 4750);
  PWM_V         = CLAMP(2400 - dcpwm, 50, 4750);
}

hal_comp_t hvdc_comp_struct = {
    .name      = "hvdc",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = 0,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = 0,
    .pin_count = sizeof(struct hvdc_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
