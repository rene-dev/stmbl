#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"

#define POLES 50.0

HAL_COMP(map);

HAL_PIN(pos_in);
HAL_PIN(pos_out);
HAL_PIN(start);
HAL_PIN(freq);
HAL_PIN(over);
HAL_PIN(print);
HAL_PIN(state);
HAL_PIN(counter);
HAL_PIN(index);


struct map_ctx_t{
   float map[(int)POLES];
   float rmap[(int)POLES];
   int state;
   int index;
   int counter;
   float value;
   float pos;
};

static void nrt_init(volatile void * ctx_ptr, volatile hal_pin_inst_t * pin_ptr){
   struct map_ctx_t * ctx = (struct map_ctx_t *)ctx_ptr;
   struct map_pin_ctx_t * pins = (struct map_pin_ctx_t *)pin_ptr;
   PIN(over) = 500.0;
   for(int i = 0; i < POLES; i++){
      ctx->map[i] = 0.0;
      ctx->rmap[i] = 0.0;
   }
}


static void rt_func(float period, volatile void * ctx_ptr, volatile hal_pin_inst_t * pin_ptr){
   struct map_ctx_t * ctx = (struct map_ctx_t *)ctx_ptr;
   struct map_pin_ctx_t * pins = (struct map_pin_ctx_t *)pin_ptr;
   
   PIN(state) = ctx->state;
   PIN(counter) = ctx->counter;
   PIN(index) = ctx->index;
   
   int index;
   float min, max, p, min_p, max_p, k;

   switch(ctx->state){
      case 0: // pipe through
         //PIN(pos_out) = PIN(pos_in);
         PIN(pos_out) = 0.0;
         
         if(PIN(start) > 0.0){
            PIN(pos_out) = 0.0;
            ctx->counter++;
            if(ctx->counter > 1000){
               ctx->state = 1;
               ctx->index = 0;
               ctx->pos = 0.0;
            }
         }
      break;
      
      case 1: // move motor
         ctx->pos += 2.0 * M_PI * PIN(freq) * period;
         PIN(pos_out) = mod(ctx->pos);
         
         if(ctx->pos >= (float)ctx->index * 2.0 * M_PI / POLES){
            ctx->value = 0.0;
            ctx->counter = 0;
            ctx->state = 2;
            //PIN(pos_out) = mod((float)ctx->index / POLES * 2.0 * M_PI);
         }
         
         if(ctx->index >= POLES){
            ctx->state = 3;
         }
      break;
      
      case 2: // measure
         ctx->value += PIN(pos_in) / PIN(over);
         ctx->counter++;
         if(ctx->counter > PIN(over)){
            ctx->map[ctx->index] = mod(ctx->value);
            ctx->state = 1;
            ctx->index++;
         }
      break;
      
      case 3: // remap
      break;
      
      case 4: // map
         index = (int)((mod(PIN(pos_in)) + M_PI) / 2.0 / M_PI * POLES);
         min = ctx->rmap[index];
         max = ctx->rmap[(index + 1) % (int)POLES];
         p = mod(PIN(pos_in));
         min_p = mod(index * 2.0 * M_PI / POLES);
         max_p = mod((index + 1) * 2.0 * M_PI / POLES);
         k = minus(p, min_p) / minus(max_p, min_p);
         
         PIN(pos_out) = mod(min + minus(max, min) * k);
      
         if(PIN(start) <= 0.0){
            ctx->state = 0;
         }
      break;
   }
}

static void nrt_func(volatile void * ctx_ptr, volatile hal_pin_inst_t * pin_ptr){
   struct map_ctx_t * ctx = (struct map_ctx_t *)ctx_ptr;
   struct map_pin_ctx_t * pins = (struct map_pin_ctx_t *)pin_ptr;

   if(PIN(print) > 0.0){
      PIN(print) = 0.0;
      printf("\nmap(mot_pos) -> fb_pos\n");
      printf("index, map[index], pos(index)\n");
      for(int i = 0; i < POLES; i++){
         printf("%i, %f, %f\n", i, ctx->map[i], (float)i / POLES * 2.0 * M_PI);
      }
      printf("\nrmap(fb_pos) -> mot_pos\n");
      printf("index, map[index], pos(index)\n");
      for(int i = 0; i < POLES; i++){
         printf("%i, %f, %f\n", i, ctx->rmap[i], (float)i / POLES * 2.0 * M_PI);
      }
   }
   
   if(ctx->state == 3){ // remap
      
      float min, max, p;
      int min_index, max_index;
      
      for(int i = 0; i < POLES; i++){
         min = -10.0;
         max = 10.0;
         min_index = 0;
         max_index = 0;
         p = (float)i * 2.0 * M_PI / POLES;
         
         for(int j = 0; j < POLES; j++){
            if(minus(p, ctx->map[j]) <= min && minus(p, ctx->map[j]) >= 0.0){
               min = minus(p, ctx->map[j]);
               min_index = j;
            }
            if(minus(ctx->map[j], p) <= max && minus(ctx->map[j], p) >= 0.0){
               max = minus(ctx->map[j], p);
               max_index = j;
            }
         }
         p = mod(p);
         min = mod(min);
         max = mod(max);
         
         ctx->rmap[i] = min_index * 2.0 * M_PI / POLES
                      + minus(max_index * 2.0 * M_PI / POLES, min_index * 2.0 * M_PI / POLES) * min / (min + max);
         
         //ctx->rmap[i] = min_index * 2.0 * M_PI / POLES * minus(p, min) / minus(max, min) + max_index * 2.0 * M_PI / POLES * minus(max, p) / minus(max, min);
      }
      
      // for(int i = 0; i < POLES; i++){
      //    ctx->map[i] = ctx->rmap[i];
      // }
      
      ctx->state = 4;
   }
}

hal_comp_t map_comp_struct = {
  .name = "map",
  .nrt = nrt_func,
  .rt = rt_func,
  .frt = 0,
  .nrt_init = nrt_init,
  .rt_start = 0,
  .frt_start = 0,
  .rt_stop = 0,
  .frt_stop = 0,
  .ctx_size = sizeof(struct map_ctx_t),
  .pin_count = sizeof(struct map_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
