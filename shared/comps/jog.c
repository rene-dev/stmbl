#include "commands.h"
#include "hal.h"
#include "defines.h"

HAL_COMP(jog);

HAL_PIN(jog);

static volatile float jog;
static volatile float jog_timeout;

void jog_left(char *ptr) {
  jog         = -1.0;
  jog_timeout = 0.0;
}

void jog_right(char *ptr) {
  jog         = 1.0;
  jog_timeout = 0.0;
}

void jog_stop(char *ptr) {
  jog         = 0.0;
  jog_timeout = 0.0;
}

COMMAND("jogl", jog_left, "Jog left");
COMMAND("jogr", jog_right, "Jog right");
COMMAND("jogx", jog_stop, "Stop jog");

static void nrt_init(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  jog_timeout = 0.0;
  jog         = 0.0;
}

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct jog_pin_ctx_t *pins = (struct jog_pin_ctx_t *)pin_ptr;

  if(jog_timeout < 0.75) {
    jog_timeout += period;
  } else {
    jog = 0.0;
  }
  PIN(jog) = jog;
}

hal_comp_t jog_comp_struct = {
    .name      = "jog",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = nrt_init,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = 0,
    .pin_count = sizeof(struct jog_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
