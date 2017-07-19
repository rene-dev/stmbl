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

HAL_PIN(p0);
HAL_PIN(p1);
HAL_PIN(p2);
HAL_PIN(p3);
HAL_PIN(p4);
HAL_PIN(p5);
HAL_PIN(p6);
HAL_PIN(p7);

//rotor position output
HAL_PIN(pos);
HAL_PIN(rpos);

static void nrt_init(volatile void * ctx_ptr, volatile hal_pin_inst_t * pin_ptr){
  // struct uvw_ctx_t * ctx = (struct io_ctx_t *)ctx_ptr;
  struct uvw_pin_ctx_t * pins = (struct uvw_pin_ctx_t *)pin_ptr;
  PIN(p0) = 0;//fault
  PIN(p1) = 0;//u      = 0
  PIN(p2) = 2;//v      = 2.094395
  PIN(p3) = 1;//u + v  = 1.047198
  PIN(p4) = 4;//w      = -2.094395
  PIN(p5) = 5;//u + w  = -1.047198
  PIN(p6) = 3;//v + w  = -3.141593
  PIN(p7) = 0;//fault
}

static void rt_func(float period, volatile void * ctx_ptr, volatile hal_pin_inst_t * pin_ptr){
   // struct uvw_ctx_t * ctx = (struct uvw_ctx_t *)ctx_ptr;
   struct uvw_pin_ctx_t * pins = (struct uvw_pin_ctx_t *)pin_ptr;

   uint32_t rpos = (PIN(u) > 0.0) * 1.0 + (PIN(v) > 0.0) * 2.0 + (PIN(w) > 0.0) * 4.0;
   //TODO: make this const, fault output
   uint32_t t[8];
   t[0] = PIN(p0);
   t[1] = PIN(p0);
   t[2] = PIN(p0);
   t[3] = PIN(p0);
   t[4] = PIN(p0);
   t[5] = PIN(p0);
   t[6] = PIN(p0);
   t[7] = PIN(p0);
   PIN(rpos) = rpos;
   PIN(pos) = mod((float)t[rpos]/6.0 * 2.0 * M_PI);
}

hal_comp_t uvw_comp_struct = {
  .name = "uvw",
  .nrt = 0,
  .rt = rt_func,
  .frt = 0,
  .nrt_init = nrt_init,
  .rt_start = 0,
  .frt_start = 0,
  .rt_stop = 0,
  .frt_stop = 0,
  .ctx_size = 0,
  .pin_count = sizeof(struct uvw_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
