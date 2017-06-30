#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"

HAL_COMP(fb_switch);

HAL_PIN(mot_polecount);

HAL_PIN(pos_fb);
HAL_PIN(vel_fb);
HAL_PIN(com_fb);
HAL_PIN(joint_fb);
HAL_PIN(state); // 0 = disabled, 1 = enabled

HAL_PIN(cmd_pos);

HAL_PIN(mot_pos0);
HAL_PIN(mot_abs_pos0);
HAL_PIN(mot_polecount0);
HAL_PIN(mot_offset0);
HAL_PIN(mot_state0); // 0 = disabled, 1 = inc, 2 = start abs, 3 = abs

HAL_PIN(mot_pos1);
HAL_PIN(mot_abs_pos1);
HAL_PIN(mot_polecount1);
HAL_PIN(mot_offset1);
HAL_PIN(mot_state1);

HAL_PIN(joint_pos);
HAL_PIN(joint_abs_pos);
HAL_PIN(joint_offset);
HAL_PIN(joint_state);

HAL_PIN(mot_joint_ratio);

HAL_PIN(force_phase);
HAL_PIN(phase_time);
HAL_PIN(phase_cur);

HAL_PIN(en);

struct fb_switch_ctx_t{
   int32_t current_com_pos;
   float cmd_offset;
   float com_offset;
};

static void nrt_init(volatile void * ctx_ptr, volatile hal_pin_inst_t * pin_ptr){
   struct fb_switch_ctx_t * ctx = (struct fb_switch_ctx_t *)ctx_ptr;
   struct fb_switch_pin_ctx_t * pins = (struct fb_switch_pin_ctx_t *)pin_ptr;
   ctx->current_com_pos = 10;
   ctx->cmd_offset = 0.0;
   ctx->com_offset = 0.0;
}

static void rt_func(float period, volatile void * ctx_ptr, volatile hal_pin_inst_t * pin_ptr){
   struct fb_switch_ctx_t * ctx = (struct fb_switch_ctx_t *)ctx_ptr;
   struct fb_switch_pin_ctx_t * pins = (struct fb_switch_pin_ctx_t *)pin_ptr;
   float mot_pos0 = PIN(mot_pos0);
   PIN(pos_fb) = mod(mot_pos0 + ctx->cmd_offset);

   if(PIN(en) <= 0.0){
      PIN(state) = 0.0;
      ctx->current_com_pos = 10;
      ctx->com_offset = 0.0;
      ctx->cmd_offset = minus(PIN(cmd_pos), mot_pos0);
   }
   else{
      PIN(state) = 1.0;
      if(PIN(joint_state) >= 2.0 && ctx->current_com_pos > 3.0){
         ctx->current_com_pos = 3;
         ctx->com_offset = minus(mod(PIN(joint_abs_pos) * PIN(mot_polecount) / PIN(mot_joint_ratio) + PIN(joint_offset)), mot_pos0 * PIN(mot_polecount) / PIN(mot_polecount0));
      }
      if(PIN(mot_state1) >= 2.0 && ctx->current_com_pos > 2.0){
         ctx->current_com_pos = 2;
         ctx->com_offset = minus(mod(PIN(mot_abs_pos1) * PIN(mot_polecount) / PIN(mot_polecount1) + PIN(mot_offset1)), mot_pos0 * PIN(mot_polecount) / PIN(mot_polecount0));
      }
      if(PIN(mot_state0) >= 2.0 && ctx->current_com_pos > 1.0){
         ctx->current_com_pos = 1;
         ctx->com_offset = 0.0;
      }
      if(ctx->current_com_pos > 4.0){
         ctx->current_com_pos = 4.0;
         // TODO cauto
         // ctx->com_offset = cauto
         PIN(state) = 0.0;
      }
      
      switch(ctx->current_com_pos){
         case 4:
            PIN(com_fb) = mod(mot_pos0 * PIN(mot_polecount) / PIN(mot_polecount0) + ctx->com_offset);
         break;
         
         case 3:
            if(PIN(joint_state) != 2.0){
               PIN(com_fb) = mod(mot_pos0 * PIN(mot_polecount) / PIN(mot_polecount0) + ctx->com_offset);
            }
            else{
               PIN(com_fb) = mod(PIN(joint_abs_pos) * PIN(mot_polecount) + PIN(joint_offset));
            }
         break;
         
         case 2:
            if(PIN(mot_state1) != 2.0){
               PIN(com_fb) = mod(mot_pos0 * PIN(mot_polecount) / PIN(mot_polecount0) + ctx->com_offset);
            }
            else{
               PIN(com_fb) = mod(PIN(mot_abs_pos1) * PIN(mot_polecount) / PIN(mot_polecount1) + PIN(mot_offset1));
            }
         break;
         
         case 1:
            if(PIN(mot_state1) != 2.0){
               PIN(state) = 0.0;
            }
            else{
               PIN(com_fb) = mod(PIN(mot_abs_pos0) * PIN(mot_polecount) / PIN(mot_polecount0) + PIN(mot_offset0));
            }
         break;
         
         default:
            PIN(state) = 0.0;
      }
         
      PIN(vel_fb) = mot_pos0;
   }
}

hal_comp_t fb_switch_comp_struct = {
  .name = "fb_switch",
  .nrt = 0,
  .rt = rt_func,
  .frt = 0,
  .nrt_init = nrt_init,
  .rt_start = 0,
  .frt_start = 0,
  .rt_stop = 0,
  .frt_stop = 0,
  .ctx_size = sizeof(struct fb_switch_ctx_t),
  .pin_count = sizeof(struct fb_switch_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
