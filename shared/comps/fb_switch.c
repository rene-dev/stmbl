#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"

HAL_COMP(fb_switch);

HAL_PIN(polecount);

HAL_PIN(pos_fb);
HAL_PIN(vel_fb);
HAL_PIN(com_fb);
HAL_PIN(joint_fb);
HAL_PIN(state); // 0 = disabled, 1 = enabled

HAL_PIN(cmd_pos);

HAL_PIN(mot_pos);
HAL_PIN(mot_abs_pos);
HAL_PIN(mot_polecount);
HAL_PIN(mot_offset);
HAL_PIN(mot_state); // 0 = disabled, 1 = inc, 2 = start abs, 3 = abs

HAL_PIN(com_pos);
HAL_PIN(com_abs_pos);
HAL_PIN(com_polecount);
HAL_PIN(com_offset);
HAL_PIN(com_state);

HAL_PIN(joint_pos);
HAL_PIN(joint_abs_pos);
HAL_PIN(joint_offset);
HAL_PIN(joint_state);

HAL_PIN(mot_joint_ratio);

HAL_PIN(force_phase);
HAL_PIN(phase_time);
HAL_PIN(phase_cur);

HAL_PIN(current_com_pos);

HAL_PIN(en);

struct fb_switch_ctx_t{
   int32_t current_com_pos;
   float cmd_offset;
   float com_offset;
};

static void nrt_init(volatile void * ctx_ptr, volatile hal_pin_inst_t * pin_ptr){
   struct fb_switch_ctx_t * ctx = (struct fb_switch_ctx_t *)ctx_ptr;
   // struct fb_switch_pin_ctx_t * pins = (struct fb_switch_pin_ctx_t *)pin_ptr;
   ctx->current_com_pos = 10;
   ctx->cmd_offset = 0.0;
   ctx->com_offset = 0.0;
}

static void rt_func(float period, volatile void * ctx_ptr, volatile hal_pin_inst_t * pin_ptr){
   struct fb_switch_ctx_t * ctx = (struct fb_switch_ctx_t *)ctx_ptr;
   struct fb_switch_pin_ctx_t * pins = (struct fb_switch_pin_ctx_t *)pin_ptr;
   float mot_pos = PIN(mot_pos);
   PIN(pos_fb) = mod(mot_pos + ctx->cmd_offset);
   PIN(vel_fb) = mot_pos;
   
   if(PIN(en) <= 0.0){
      PIN(state) = 0.0;
      ctx->current_com_pos = 10;
      ctx->com_offset = 0.0;
      ctx->cmd_offset = minus(PIN(cmd_pos), mot_pos);
      PIN(pos_fb) = mod(mot_pos);
   }
   else{
      PIN(state) = 1.0;
      if(PIN(joint_state) >= 2.0 && ctx->current_com_pos > 3.0){
         ctx->current_com_pos = 3;
         ctx->com_offset = minus(mod(PIN(joint_abs_pos) * PIN(polecount) / PIN(mot_joint_ratio) + PIN(joint_offset)), mot_pos * PIN(polecount) / PIN(mot_polecount));
      }
      if(PIN(com_state) >= 2.0 && ctx->current_com_pos > 2.0){
         ctx->current_com_pos = 2;
         ctx->com_offset = minus(mod(PIN(com_abs_pos) * PIN(polecount) / PIN(com_polecount) + PIN(com_offset)), mot_pos * PIN(polecount) / PIN(mot_polecount));
      }
      if(PIN(mot_state) >= 2.0 && ctx->current_com_pos > 1.0){
         ctx->current_com_pos = 1;
         ctx->com_offset = 0.0;
      }
      if(ctx->current_com_pos > 4.0){
         ctx->current_com_pos = 4.0;
         // TODO cauto
         // ctx->com_offset = cauto
         PIN(state) = 0.0;
      }
      
      PIN(current_com_pos) = ctx->current_com_pos;
      
      switch(ctx->current_com_pos){
         case 4:
            PIN(com_fb) = mod(mot_pos * PIN(polecount) / PIN(mot_polecount) + ctx->com_offset);
         break;
         
         case 3:
            if(PIN(joint_state) != 3.0){
               PIN(com_fb) = mod(mot_pos * PIN(polecount) / PIN(mot_polecount) + ctx->com_offset);
            }
            else{
               PIN(com_fb) = mod(PIN(joint_abs_pos) * PIN(polecount) + PIN(joint_offset));
            }
         break;
         
         case 2:
            if(PIN(com_state) != 3.0){
               PIN(com_fb) = mod(mot_pos * PIN(polecount) / PIN(mot_polecount) + ctx->com_offset);
            }
            else{
               PIN(com_fb) = mod(PIN(com_abs_pos) * PIN(polecount) / PIN(com_polecount) + PIN(com_offset));
            }
         break;
         
         case 1:
            if(PIN(mot_state) != 3.0){
               PIN(state) = 0.0;
            }
            else{
               PIN(com_fb) = mod(PIN(mot_abs_pos) * PIN(polecount) / PIN(mot_polecount) + PIN(mot_offset));
            }
         break;
         
         default:
            PIN(state) = 0.0;
      }
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
