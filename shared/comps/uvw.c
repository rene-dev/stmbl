#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"

HAL_COMP(uvw);

//u,v,w inputs
HAL_PIN(u);
HAL_PIN(v);
HAL_PIN(w);

//rotor position output
HAL_PIN(pos);

static void rt_func(float period, volatile void * ctx_ptr, volatile hal_pin_inst_t * pin_ptr){
   // struct uvw_ctx_t * ctx = (struct uvw_ctx_t *)ctx_ptr;
   struct uvw_pin_ctx_t * pins = (struct uvw_pin_ctx_t *)pin_ptr;

   uint32_t rpos = (PIN(u) > 0.0) * 1.0 + (PIN(v) > 0.0) * 2.0 + (PIN(w) > 0.0) * 4.0;
   //TODO: make this const, fault output
   uint32_t t[8];
   t[0] = 0;//fault
   t[1] = 0;//u
   t[2] = 2;//v
   t[3] = 1;//u + v
   t[4] = 4;//w
   t[5] = 5;//u + w
   t[6] = 3;//v + w
   t[7] = 0;//fault
   PIN(pos) = mod((float)t[rpos]/6.0 * 2.0 * M_PI);
}

hal_comp_t uvw_comp_struct = {
  .name = "uvw",
  .nrt = 0,
  .rt = rt_func,
  .frt = 0,
  .nrt_init = 0,
  .rt_start = 0,
  .frt_start = 0,
  .rt_stop = 0,
  .frt_stop = 0,
  .ctx_size = 0,
  .pin_count = sizeof(struct uvw_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
