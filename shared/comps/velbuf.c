#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"

HAL_COMP(velbuf);

HAL_PIN(pos);
HAL_PIN(vel);
HAL_PIN(vel_lp);
HAL_PIN(min_pos_diff);

HAL_PIN(lpf)

HAL_PIN(diff_time);
HAL_PIN(diff_pos);

#define velbuf_size  20

struct velbuf_ctx_t {
  float buf[velbuf_size];
  uint32_t ptr;
  float frt_period;
};

static void nrt_init(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct velbuf_ctx_t *ctx      = (struct velbuf_ctx_t *)ctx_ptr;
  struct velbuf_pin_ctx_t *pins = (struct velbuf_pin_ctx_t *)pin_ptr;
  
  PIN(min_pos_diff) = 0.01;

  for(int i = 0; i < velbuf_size; i++){
      ctx->buf[i] = 0;
  }

  PIN(lpf) = 1000.0;
}


static void frt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct velbuf_ctx_t *ctx      = (struct velbuf_ctx_t *)ctx_ptr;
  struct velbuf_pin_ctx_t *pins = (struct velbuf_pin_ctx_t *)pin_ptr;

  ctx->buf[ctx->ptr++] = PIN(pos);
  ctx->ptr %= velbuf_size;
  ctx->frt_period = period;
}


static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct velbuf_ctx_t *ctx      = (struct velbuf_ctx_t *)ctx_ptr;
  struct velbuf_pin_ctx_t *pins = (struct velbuf_pin_ctx_t *)pin_ptr;

  int p = ctx->ptr;
  float pos = ctx->buf[(p - 1 + velbuf_size) % velbuf_size];
   
  float old_pos = ctx->buf[p];
  float diff_time = ctx->frt_period * velbuf_size;
  
  float min_diff = PIN(min_pos_diff);
   
  for(int i = 0; i < velbuf_size; i++){
    if(ABS(minus(pos, ctx->buf[(p + i) % velbuf_size])) > min_diff){
        old_pos = ctx->buf[(p + i) % velbuf_size];
        diff_time = (velbuf_size - i - 1) * ctx->frt_period;
    }else{
        break;
    }
  }
  
  float diff_pos = minus(pos, old_pos);
  PIN(diff_pos) = diff_pos;
  PIN(diff_time) = diff_time;
  PIN(vel) = diff_pos / diff_time;

  PIN(vel_lp) = PIN(vel) * LP_HZ(PIN(lpf)) + PIN(vel_lp) * (1.0 - LP_HZ(PIN(lpf)));
}

hal_comp_t velbuf_comp_struct = {
    .name      = "velbuf",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = frt_func,
    .nrt_init  = nrt_init,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = sizeof(struct velbuf_ctx_t),
    .pin_count = sizeof(struct velbuf_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};