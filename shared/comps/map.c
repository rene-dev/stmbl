#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"

#define POLES 50.0

HAL_COMP(map);

HAL_PIN(pos_in);
HAL_PIN(pos_out);
HAL_PIN(pos_out2);
HAL_PIN(start);
HAL_PIN(freq);
HAL_PIN(over);
HAL_PIN(print);
HAL_PIN(state);
HAL_PIN(counter);
HAL_PIN(index);

float interp(float value, float* array, uint32_t array_size){
  value = CLAMP(value, 0.0, 1.0);
  array_size = MAX(array_size, 1);

  uint32_t min_i = (uint32_t)(value * array_size) % array_size;
  uint32_t max_i = (uint32_t)(min_i + 1.0) % array_size;
  
  return(array[min_i] + (value - (float)min_i / array_size) * minus(array[max_i], array[min_i]) / ((float)max_i / array_size - (float)min_i / array_size));
}

float interpd(float value, float* array, uint32_t array_size){
  value = CLAMP(value, 0.0, 1.0);
  array_size = MAX(array_size, 1);

  uint32_t min_i = (uint32_t)(value * array_size) % array_size;
  uint32_t max_i = (uint32_t)(min_i + 1.0) % array_size;
  
  return(minus(array[max_i], array[min_i]) / ((float)max_i / array_size - (float)min_i / array_size));
}

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
         ctx->value = PIN(pos_in) ;// PIN(over);
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
        PIN(pos_out2) = interp(PIN(pos_in) / 2.0 / M_PI + 0.5, ctx->rmap, POLES);
      
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
    float p = 0.0;
    float pp = 0.0;
    float error = 0.0;
    int j = 0;

    for(int i = 0; i < POLES; i++){
      p = (float)i * 2.0 * M_PI / POLES - M_PI;
      j = 0;
      do{
        pp += error * 0.6 / interpd(pp / 2.0 / M_PI + 0.5, ctx->map, POLES);
        pp = mod(pp);
        error = minus(p, interp(pp / 2.0 / M_PI + 0.5, ctx->map, POLES));
        j++;
      }
      while(j < 20000 && ABS(error) >= 2.0 * M_PI / 32768.0 * 20.0);
      printf("index %u, error %f, it %u\n", i, error, j);
      error = 0.0;

      ctx->rmap[i] = pp;
    }

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
