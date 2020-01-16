#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"

HAL_COMP(fanuc);

//fanuc encoder
HAL_PIN(C1);
HAL_PIN(C2);
HAL_PIN(C4);
HAL_PIN(C8);

//rotor position output
HAL_PIN(pos);

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  // struct fanuc_ctx_t * ctx = (struct fanuc_ctx_t *)ctx_ptr;
  struct fanuc_pin_ctx_t *pins = (struct fanuc_pin_ctx_t *)pin_ptr;

  //TODO: const...
  uint8_t tab[16];
  tab[0]  = 0;
  tab[1]  = 15;
  tab[2]  = 1;
  tab[3]  = 2;
  tab[4]  = 13;
  tab[5]  = 14;
  tab[6]  = 12;
  tab[7]  = 11;
  tab[8]  = 5;
  tab[9]  = 6;
  tab[10] = 4;
  tab[11] = 3;
  tab[12] = 8;
  tab[13] = 7;
  tab[14] = 9;
  tab[15] = 10;

  uint32_t t = (PIN(C1) > 0.0) + (PIN(C2) > 0.0) * 2 + (PIN(C4) > 0.0) * 4 + (PIN(C8) > 0.0) * 8;
  PIN(pos)   = mod((tab[t] / 15.0 * 2.0 * M_PI));
}

hal_comp_t fanuc_comp_struct = {
    .name      = "fanuc",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = 0,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = 0,
    .pin_count = sizeof(struct fanuc_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
