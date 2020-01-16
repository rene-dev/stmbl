#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"

HAL_COMP(wobl);

HAL_PIN(home_pos);
HAL_PIN(amp);
HAL_PIN(freq);
HAL_PIN(freq_diff);
HAL_PIN(duty);
HAL_PIN(en);

HAL_PIN(pos);
HAL_PIN(out);
HAL_PIN(out_freq);

struct wobl_ctx_t {
  float ang;
  float ang2;
};

static void frt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct wobl_ctx_t *ctx      = (struct wobl_ctx_t *)ctx_ptr;
  struct wobl_pin_ctx_t *pins = (struct wobl_pin_ctx_t *)pin_ptr;
    
  
  if(PIN(en) <= 0.0){
    ctx->ang = 0.0;
    ctx->ang2 = 0.0;
  }
  else{
    ctx->ang += period * PIN(freq) * 2.0 * M_PI; 
    ctx->ang = mod(ctx->ang);
    ctx->ang2 += period * (PIN(freq) + PIN(freq_diff)) * 2.0 * M_PI; 
    ctx->ang2 = mod(ctx->ang2);
  }

  PIN(pos) = PIN(home_pos) + sinf(ctx->ang) * PIN(amp);
  PIN(out_freq) = PIN(freq) + PIN(freq_diff);
  PIN(out) = ((sinf(ctx->ang2) / 2 - 0.5 + PIN(duty)) > 0.0) ? 1.0 : 0.0;

}

hal_comp_t wobl_comp_struct = {
    .name      = "wobl",
    .nrt       = 0,
    .rt        = 0,
    .frt       = frt_func,
    .nrt_init  = 0,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = sizeof(struct wobl_ctx_t),
    .pin_count = sizeof(struct wobl_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
