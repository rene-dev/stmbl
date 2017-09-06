#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"

HAL_COMP(acim_ttc);

HAL_PIN(mode); // 0 = u/f, 1 = mtpa

// motor values
HAL_PIN(torque_n);
HAL_PIN(cur_n);
HAL_PIN(slip_n);
HAL_PIN(polecount);

// torque cmd in
HAL_PIN(torque);
HAL_PIN(vel);

// cur cmd out
HAL_PIN(id);
HAL_PIN(iq);
HAL_PIN(freq);
HAL_PIN(slip)
HAL_PIN(pos);

static void rt_func(float period, volatile void * ctx_ptr, volatile hal_pin_inst_t * pin_ptr){
   // struct acim_ttc_ctx_t * ctx = (struct acim_ttc_ctx_t *)ctx_ptr;
   struct acim_ttc_pin_ctx_t * pins = (struct acim_ttc_pin_ctx_t *)pin_ptr;

   float poles = MAX(PIN(polecount), 1.0);
   float torque = PIN(torque);
   float vel = PIN(vel);
   float slip_n = PIN(slip_n);
   float torque_n = MAX(PIN(torque_n), 0.001);
   float cur_n = PIN(cur_n);

   float id = 0.0;
   float iq = 0.0;
   float freq = vel * poles / 2.0 / M_PI;
   float slip = 0.0;

   switch((int)PIN(mode)){
      case 0: // slip control
         id = cur_n / sqrtf(2.0); // constant flux
         iq = cur_n / sqrtf(2.0) / torque_n * torque;
         slip = slip_n / torque_n * torque; 
      break;

      case 1: // mtpa
         id = 0.0;
         iq = cur_n / torque_n * torque;
         slip = slip_n * SIGN(torque); // constant slip
         break;

      default:
         id = 0;
         iq = 0;
         slip = 0.0;
   }

   freq += slip;

   PIN(id) = id;
   PIN(iq) = iq;
   PIN(freq) = freq;
   PIN(slip) = slip;
   PIN(pos) = mod(PIN(pos) + freq * period * 2.0 * M_PI);
}

hal_comp_t acim_ttc_comp_struct = {
  .name = "acim_ttc",
  .nrt = 0,
  .rt = rt_func,
  .frt = 0,
  .nrt_init = 0,
  .rt_start = 0,
  .frt_start = 0,
  .rt_stop = 0,
  .frt_stop = 0,
  .ctx_size = 0,
  .pin_count = sizeof(struct acim_ttc_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
