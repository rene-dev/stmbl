#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"

HAL_COMP(vel_int);

HAL_PIN(pos_in);
HAL_PIN(pos_out);

HAL_PIN(vel_in);
HAL_PIN(vel_out);

HAL_PIN(wd);
HAL_PIN(error);

struct vel_int_ctx_t{
   float pos;
   float counter;
};

static void nrt_init(volatile void * ctx_ptr, volatile hal_pin_inst_t * pin_ptr){
  struct vel_int_ctx_t * ctx = (struct vel_int_ctx_t *)ctx_ptr;
  struct vel_int_pin_ctx_t * pins = (struct vel_int_pin_ctx_t *)pin_ptr;
  ctx->pos = 0.0;
  ctx->counter = 0.0;
  PIN(pos_in) = 0.0;
  PIN(pos_out) = 0.0;

  PIN(vel_in) = 0.0;
  PIN(vel_out) = 0.0;

  PIN(wd) = 0.002;
  PIN(error) = 0.0;
}

static void rt_func(float period, volatile void * ctx_ptr, volatile hal_pin_inst_t * pin_ptr){
   struct vel_int_ctx_t * ctx = (struct vel_int_ctx_t *)ctx_ptr;
   struct vel_int_pin_ctx_t * pins = (struct vel_int_pin_ctx_t *)pin_ptr;

   float p = PIN(pos_in);
   float v = PIN(vel_in);

   if(ctx->counter > PIN(wd) && v != 0.0){
      v = 0;
      PIN(error) = 1.0;
   }
   else{
      PIN(error) = 0.0;
      ctx->counter += period;
   }
   
   if(EDGE(p)){
      ctx->counter = 0.0;
      ctx->pos = p;
   }
   else{
      ctx->pos += v * period;
   }
   
   ctx->pos = mod(ctx->pos);
   
   PIN(pos_out) = ctx->pos;
   PIN(vel_out) = v;
}

hal_comp_t vel_int_comp_struct = {
  .name = "vel_int",
  .nrt = 0,
  .rt = rt_func,
  .frt = 0,
  .nrt_init = nrt_init,
  .rt_start = 0,
  .frt_start = 0,
  .rt_stop = 0,
  .frt_stop = 0,
  .ctx_size = sizeof(struct vel_int_ctx_t),
  .pin_count = sizeof(struct vel_int_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
