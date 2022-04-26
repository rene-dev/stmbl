#include "fault_comp.h"
#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"
#include "main.h"
#include "common.h"

HAL_COMP(fault);

HAL_PIN(en);
HAL_PIN(state);
HAL_PIN(fault);
HAL_PIN(last_fault);
HAL_PIN(en_out);
HAL_PIN(en_fb);
HAL_PIN(en_pid);

HAL_PIN(fb_ready);

HAL_PIN(cmd_error);
HAL_PIN(mot_fb_error);
HAL_PIN(com_fb_error);
HAL_PIN(joint_fb_error);
HAL_PIN(hv_error);

HAL_PIN(hv_temp);
HAL_PIN(mot_temp);
HAL_PIN(max_hv_temp);
HAL_PIN(max_mot_temp);
HAL_PIN(high_hv_temp);
HAL_PIN(high_mot_temp);
HAL_PIN(fan_hv_temp);
HAL_PIN(fan_mot_temp);

HAL_PIN(scale);

HAL_PIN(dc_volt);
HAL_PIN(min_dc_volt);
HAL_PIN(high_dc_volt);
HAL_PIN(max_dc_volt);

HAL_PIN(dc_cur);
HAL_PIN(max_dc_cur);

HAL_PIN(ac_cur);
HAL_PIN(max_ac_cur);

HAL_PIN(pos_error);
HAL_PIN(max_pos_error);

HAL_PIN(sat);
HAL_PIN(max_sat);

HAL_PIN(mot_brake);
HAL_PIN(dc_brake);

HAL_PIN(hv_fan);
HAL_PIN(mot_fan);

HAL_PIN(print);

HAL_PIN(brake_release);

HAL_PIN(warn_timer);
HAL_PIN(error_timer);

//fault strings for fault_t form common.h
static const char* fault_string[] = {
  "no error",
  "CMD error",
  "mot FB error",
  "com FB error",
  "joint FB error",
  "position error",
  "saturation error",
  "Motor overtemperture",
  "HV crc error",
  "HV timeout error",
  "HV overtemperture",
  "HV volt error",
  "HV fault error",
  "Current offset fault",
  "Motor overcurrent rms",
  "Motor overcurrent peak",
  "Motor overcurrent hw limit",
};

struct fault_ctx_t {
  state_t state;
  fault_t fault;
  float cmd_error;
  float mot_fb_error;
  float com_fb_error;
  float joint_fb_error;
  float hv_temp_error;
  float dc_volt_error;
  float mot_temp_error;
};

void enable(char *ptr) {
  hal_parse("fault0.en = 1");
}

COMMAND("enable", enable, "enable");

void disable(char *ptr) {
  hal_parse("fault0.en = 0");
}

COMMAND("disable", disable, "disable");


static void nrt_init(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct fault_ctx_t *ctx      = (struct fault_ctx_t *)ctx_ptr;
  struct fault_pin_ctx_t *pins = (struct fault_pin_ctx_t *)pin_ptr;

  ctx->state         = DISABLED;
  ctx->fault         = NO_ERROR;
  PIN(last_fault)    = NO_ERROR;
  PIN(min_dc_volt)   = 20.0;
  PIN(high_dc_volt)  = 370.0;
  PIN(max_dc_volt)   = 390.0;
  PIN(max_hv_temp)   = 90.0;
  PIN(max_mot_temp)  = 100.0;
  PIN(high_hv_temp)  = 70.0;
  PIN(high_mot_temp) = 80.0;
  PIN(fan_hv_temp)   = 60.0;
  PIN(fan_mot_temp)  = 60.0;
}

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct fault_ctx_t *ctx      = (struct fault_ctx_t *)ctx_ptr;
  struct fault_pin_ctx_t *pins = (struct fault_pin_ctx_t *)pin_ptr;

  switch(ctx->state) {
    case DISABLED:
      if(PIN(en) > 0.0) {
        ctx->state = PHASING;
      }
      break;

    case ENABLED:
      if(PIN(en) <= 0.0) {
        ctx->state = DISABLED;
      }
      break;

    case PHASING:
      if(PIN(fb_ready) > 0.0) {
        ctx->state = ENABLED;
      }

      if(PIN(en) <= 0.0) {
        ctx->state = DISABLED;
      }
      break;

    case SOFT_FAULT:
      if(PIN(en) <= 0.0) {
        ctx->state = DISABLED;
      }
      break;

    case LED_TEST:
    case HARD_FAULT:
      break;
  }

  if(err_filter(&(ctx->cmd_error), 5.0, 0.001, PIN(cmd_error) > 0.0)) {
    ctx->fault      = CMD_ERROR;
    PIN(last_fault) = ctx->fault;
    ctx->state      = SOFT_FAULT;
  }

  if(err_filter(&(ctx->mot_fb_error), 5.0, 0.001, PIN(mot_fb_error) > 0.0)) {
    ctx->fault      = MOT_FB_ERROR;
    PIN(last_fault) = ctx->fault;
    ctx->state      = SOFT_FAULT;
  }

  if(err_filter(&(ctx->com_fb_error), 5.0, 0.001, PIN(com_fb_error) > 0.0)) {
    ctx->fault      = COM_FB_ERROR;
    PIN(last_fault) = ctx->fault;
    ctx->state      = SOFT_FAULT;
  }

  if(err_filter(&(ctx->joint_fb_error), 5.0, 0.001, PIN(joint_fb_error) > 0.0)) {
    ctx->fault      = JOINT_FB_ERROR;
    PIN(last_fault) = ctx->fault;
    ctx->state      = SOFT_FAULT;
  }

  if(PIN(max_pos_error) > 0 && (ABS(PIN(pos_error)) > PIN(max_pos_error))) {
    ctx->fault      = POS_ERROR;
    PIN(last_fault) = ctx->fault;
    ctx->state      = SOFT_FAULT;
  }

  if(PIN(max_sat) > 0 && (PIN(sat) > PIN(max_sat))) {
    ctx->fault      = SAT_ERROR;
    PIN(last_fault) = ctx->fault;
    ctx->state      = SOFT_FAULT;
  }

  if(err_filter(&(ctx->hv_temp_error), 5.0, 0.001, PIN(hv_temp) > PIN(max_hv_temp))) {
    ctx->fault      = HV_TEMP_ERROR;
    PIN(last_fault) = ctx->fault;
    ctx->state      = SOFT_FAULT;
  }

  if(err_filter(&(ctx->dc_volt_error), 5.0, 0.001, PIN(dc_volt) > PIN(max_dc_volt) || PIN(dc_volt) < PIN(min_dc_volt))) {
    ctx->fault      = HV_VOLT_ERROR;
    PIN(last_fault) = ctx->fault;
    ctx->state      = SOFT_FAULT;
  }

  if(err_filter(&(ctx->mot_temp_error), 5.0, 0.001, PIN(mot_temp) > PIN(max_mot_temp))) {
    ctx->fault      = MOT_TEMP_ERROR;
    PIN(last_fault) = ctx->fault;
    ctx->state      = SOFT_FAULT;
  }

  float hv_error = PIN(hv_error);
  if(hv_error > 0.0) {
    ctx->fault      = hv_error;
    PIN(last_fault) = ctx->fault;
    ctx->state      = SOFT_FAULT;
  }

  float scale = 1.0;
  scale       = MIN(scale, SCALE(PIN(hv_temp), PIN(high_hv_temp), PIN(max_hv_temp)));
  scale       = MIN(scale, SCALE(PIN(dc_volt), PIN(high_dc_volt), PIN(max_dc_volt)));
  scale       = MIN(scale, SCALE(PIN(mot_temp), PIN(high_mot_temp), PIN(max_mot_temp)));
  scale       = MIN(scale, SCALE(PIN(ac_cur), PIN(max_ac_cur), PIN(max_ac_cur) * 1.1));
  scale       = MIN(scale, SCALE(PIN(dc_cur), PIN(max_dc_cur), PIN(max_dc_cur) * 1.1));

  PIN(dc_brake) = SCALE(PIN(dc_volt), PIN(max_dc_volt), PIN(high_dc_volt));

  if(PIN(hv_temp) >= PIN(fan_hv_temp)) {
    PIN(hv_fan) = 1.0;
  }

  if(PIN(hv_temp) < PIN(fan_hv_temp) * 0.9) {
    PIN(hv_fan) = 0.0;
  }

  if(PIN(mot_temp) >= PIN(fan_mot_temp)) {
    PIN(mot_fan) = 1.0;
  }

  if(PIN(mot_temp) < PIN(fan_mot_temp) * 0.9) {
    PIN(mot_fan) = 0.0;
  }

  switch(ctx->state) {
    case DISABLED:
      ctx->fault     = NO_ERROR;
      /* FALLTHRU */
    case SOFT_FAULT:
    case LED_TEST:
    case HARD_FAULT:
      PIN(mot_brake) = 0.0;
      PIN(en_out)    = 0.0;
      PIN(en_fb)     = 0.0;
      PIN(en_pid)    = 0.0;
      break;

    case ENABLED:
      PIN(mot_brake)  = 1.0;
      PIN(en_out)     = 1.0;
      PIN(en_fb)      = 1.0;
      PIN(en_pid)     = 1.0;
      ctx->fault      = NO_ERROR;
      PIN(last_fault) = NO_ERROR;
      break;

    case PHASING:
      PIN(mot_brake) = 1.0;
      ctx->fault     = NO_ERROR;
      PIN(en_pid)    = 0.0;
      PIN(en_fb)     = 1.0;
      PIN(en_out)    = 1.0;
      break;
  }

  PIN(fault) = ctx->fault;
  PIN(state) = ctx->state;
  PIN(scale) = scale;

  if(PIN(brake_release) > 0.0) {
    PIN(mot_brake) = 1.0;
  }

  PIN(warn_timer) = MAX(PIN(warn_timer) - period, 0.0);
  PIN(error_timer) = MAX(PIN(error_timer) - period, 0.0);
}


static void nrt_func(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct fault_ctx_t *ctx      = (struct fault_ctx_t *)ctx_ptr;
  struct fault_pin_ctx_t *pins = (struct fault_pin_ctx_t *)pin_ptr;

  if(PIN(warn_timer) <= 0.0){
    if(PIN(dc_volt) > PIN(high_dc_volt)){
      printf("<font color='orange'>WARNING:</font> over voltage current clamping active\n");
      PIN(warn_timer) = 1.0;
    }

    if(PIN(mot_temp) > PIN(high_mot_temp)){
      printf("<font color='orange'>WARNING:</font> over temperature (motor) current clamping active\n");
      PIN(warn_timer) = 1.0;
    }

    if(PIN(hv_temp) > PIN(high_hv_temp)){
      printf("<font color='orange'>WARNING:</font> over temperature (driver) current clamping active\n");
      PIN(warn_timer) = 1.0;
    }
  }

  //TODO: fix EDGE
  if(EDGE(ctx->state) || PIN(print) > 0.0) {
    PIN(print) = 0.0;
    switch((state_t)ctx->state) {
      case DISABLED:
        printf("INFO: Disabled \n");
        break;

      case ENABLED:
        printf("INFO: Enabled \n");
        break;

      case PHASING:
        printf("INFO: Phasing \n");
        break;

      case SOFT_FAULT:
        printf("ERROR: Fault %lu: %s\n",(uint32_t)ctx->fault,fault_string[(uint32_t)ctx->fault]);
        break;

      case HARD_FAULT:
        printf("ERROR: Hard fault: \n");
        break;

      default:
        break;
    }
  }
}

hal_comp_t fault_comp_struct = {
    .name      = "fault",
    .nrt       = nrt_func,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = nrt_init,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = sizeof(struct fault_ctx_t),
    .pin_count = sizeof(struct fault_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
