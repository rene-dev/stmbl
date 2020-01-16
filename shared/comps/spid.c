#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"

HAL_COMP(spid);

// input
HAL_PIN(cmd);  // command
HAL_PIN(fb);   // feedback
HAL_PIN(en);   // enalbe

// gains
HAL_PIN(kp);      // proportional
HAL_PIN(ki);      // integator
HAL_PIN(kd);      // differential
HAL_PIN(ksd);     // scaled differential
HAL_PIN(kdi);     // differential integrator
HAL_PIN(ksdi);    // scaled differential integrator
HAL_PIN(kff0);    // feedforward 0
HAL_PIN(kff1);    // feedforward 1
HAL_PIN(offset);  // 0 offset

HAL_PIN(min_output);
HAL_PIN(max_output);
HAL_PIN(max_error);

// output
HAL_PIN(output);
HAL_PIN(error);
HAL_PIN(sat);

struct spid_ctx_t {
  float error_sum;
  float last_error;
  float last_cmd;
};

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct spid_ctx_t *ctx      = (struct spid_ctx_t *)ctx_ptr;
  struct spid_pin_ctx_t *pins = (struct spid_pin_ctx_t *)pin_ptr;

  float offset    = PIN(offset);
  float min       = PIN(min_output) - offset;
  float max       = PIN(max_output) - offset;
  float max_error = PIN(max_error);

  float cmd   = PIN(cmd);
  float cmd_d = (cmd - ctx->last_cmd) / period;
  float error = cmd - PIN(fb);
  if(max_error > 0.0) {
    error = LIMIT(error, max_error);
  }
  float error_d = (error - ctx->last_error) / period;

  float output = 0.0;
  output += cmd * PIN(kff0);    // feedforward 0
  output += cmd_d * PIN(kff1);  // feedforward 1
  output += error * PIN(kp);    // porportional
  output += error_d * PIN(kd);  // differential
  if(PIN(ksd) != 0.0 && ABS(error) > (max - min) / PIN(ksd) * 0.001) {
    ctx->error_sum += error_d / ABS(error) * PIN(ksd);  // scalded differential
  }
  output = CLAMP(output, min, max);

  ctx->error_sum += error * PIN(ki) * period;     // integrator
  ctx->error_sum += error_d * PIN(kdi) * period;  // differential integrator
  if(PIN(ksdi) != 0.0 && ABS(error) > (max - min) / PIN(ksdi) * 0.001) {
    ctx->error_sum += error_d / ABS(error) * PIN(ksdi) * period;  // scalded differential integrator
  }
  ctx->error_sum = CLAMP(ctx->error_sum, min - output, max - output);  // dynamic anti windup

  output += ctx->error_sum;

  if(PIN(en) <= 0.0) {
    output         = 0.0;
    ctx->error_sum = 0.0;
  }

  if(output <= min * 0.99 || output >= max * 0.99) {
    PIN(sat) += period;
  } else {
    PIN(sat) = 0.0;
  }

  output += offset;

  PIN(output) = output;

  PIN(error)      = error;
  ctx->last_error = error;
  ctx->last_cmd   = cmd;
}

hal_comp_t spid_comp_struct = {
    .name      = "spid",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = 0,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = sizeof(struct spid_ctx_t),
    .pin_count = sizeof(struct spid_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
