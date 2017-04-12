#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"

HAL_COMP(sim);

HAL_PIN(amp);
HAL_PIN(freq);
HAL_PIN(sin);
HAL_PIN(msin);
HAL_PIN(sin2);
HAL_PIN(msin2);//const vel, const max vel = amp
HAL_PIN(sin3);
HAL_PIN(msin3);//const max acc = amp
HAL_PIN(square);
HAL_PIN(vel);
HAL_PIN(res);
HAL_PIN(offset);

struct sim_ctx_t{
   float time;
   float amp;
   float freq;
   float vel;
};

static void rt_func(float period, volatile void * ctx_ptr, volatile hal_pin_inst_t * pin_ptr){
   struct sim_ctx_t * ctx = (struct sim_ctx_t *)ctx_ptr;
   struct sim_pin_ctx_t * pins = (struct sim_pin_ctx_t *)pin_ptr;
   
   ctx->amp = PIN(amp) * 0.001 + ctx->amp * 0.999;
   ctx->freq = PIN(freq) * 0.001 + ctx->freq * 0.999;
   float sin;
   float sin2;
   float sin3;
   float amp2;
   float r = MAX(PIN(res), 1);

   if(ABS(ctx->freq) > 0.01){
      amp2 = 1 / (ctx->freq * 2.0 * M_PI);
   }
   else{
      amp2 = 0;
   }

   ctx->time += period;

   if(ABS(ctx->freq * ctx->time) >= 1.0){ // TODO
      ctx->time = 0.0;
   }

   float co = 0.0;
   float si = 0.0;
   sincos_fast(ctx->freq * ctx->time * 2.0 * M_PI, &si, &co);

   sin = ctx->amp * si;
   sin2 = sin * amp2;
   sin3 = sin2 * amp2;

   float s = sin;
   float o = PIN(offset);
   ctx->vel += ctx->freq * 2.0 * M_PI * period;
   ctx->vel = mod(ctx->vel);

   PIN(sin) = s + o;
   PIN(sin2) = sin2 + o;
   PIN(sin3) = sin3 + o;
   PIN(msin) = ((int)(mod(s + o) * r)) / r;
   PIN(msin2) = ((int)(mod(sin2 + o) * r)) / r;
   PIN(msin3) = ((int)(mod(sin3 + o) * r)) / r;
   PIN(square) = (sin > 0.0) ? (ctx->amp + o) : (-ctx->amp + o);
   PIN(vel) = ctx->vel;//mod(((int)(vel * r)) / r + o);
}

hal_comp_t sim_comp_struct = {"sim", 0, rt_func, 0, 0, 0, 0, 0, 0, sizeof(struct sim_ctx_t), sizeof(struct sim_pin_ctx_t) / sizeof(struct hal_pin_inst_t)};
