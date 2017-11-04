#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"
#include "tim.h"
#include "f3hw.h"

HAL_COMP(hv);

//U V W input
HAL_PIN(u);
HAL_PIN(v);
HAL_PIN(w);

//dclink input
HAL_PIN(udc);
HAL_PIN(ac_current);
HAL_PIN(hv_temp);

//enable in
HAL_PIN(en);

//TODO: half bridge enable in
HAL_PIN(enu);
HAL_PIN(env);
HAL_PIN(enw);

//fault output
HAL_PIN(fault);

HAL_PIN(min_on);  // min on time [s]
HAL_PIN(min_off);  // min off time [s]

HAL_PIN(dac);

//comperator outputs
HAL_PIN(cu);
HAL_PIN(cv);
HAL_PIN(cw);
//master out enable
HAL_PIN(moe_r);
HAL_PIN(moe_w);

struct hv_ctx_t{
  uint32_t fault;
  float overtemp_error;
  float overvoltage_error;
  float overcurrent_error;
  float fault_pin_error;
};

static void nrt_init(volatile void *ctx_ptr, volatile hal_pin_inst_t *pin_ptr) {
  // struct hv_ctx_t * ctx = (struct hv_ctx_t *)ctx_ptr;
  struct hv_pin_ctx_t *pins = (struct hv_pin_ctx_t *)pin_ptr;

  PIN(enu)     = 1.0;
  PIN(env)     = 1.0;
  PIN(enw)     = 1.0;
  PIN(min_on)  = 0.00000035;
  PIN(min_off) = 0.000005;
  PIN(dac)     = 1100;

  GPIO_InitTypeDef GPIO_InitStruct;

#ifdef HV_EN_PIN
  GPIO_InitStruct.Pin   = HV_EN_PIN;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(HV_EN_PORT, &GPIO_InitStruct);
#endif

#ifdef HV_FAULT_PIN
  GPIO_InitStruct.Pin  = HV_FAULT_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(HV_FAULT_PORT, &GPIO_InitStruct);
#endif
}

static void rt_func(float period, volatile void *ctx_ptr, volatile hal_pin_inst_t *pin_ptr) {
  struct hv_ctx_t * ctx = (struct hv_ctx_t *)ctx_ptr;
  struct hv_pin_ctx_t *pins = (struct hv_pin_ctx_t *)pin_ptr;

  float udc = MAX(PIN(udc), 0.1);
  //convert voltages to PWM output compare values
  int32_t u = (int32_t)(CLAMP(PIN(u), 0.0, udc) / udc * (float)(PWM_RES));
  int32_t v = (int32_t)(CLAMP(PIN(v), 0.0, udc) / udc * (float)(PWM_RES));
  int32_t w = (int32_t)(CLAMP(PIN(w), 0.0, udc) / udc * (float)(PWM_RES));
  //convert on and off times to PWM output compare values
  int32_t min_on  = (int32_t)((float)(PWM_RES)*15000.0 * PIN(min_on) + 0.5);
  int32_t min_off = (int32_t)((float)(PWM_RES)*15000.0 * PIN(min_off) + 0.5);

  if((u > 0 && u < min_on) || (v > 0 && v < min_on) || (w > 0 && w < min_on)) {
    u += min_on;
    v += min_on;
    w += min_on;
  }

  if((u > PWM_RES - min_off) || (v > PWM_RES - min_off) || (w > PWM_RES - min_off)) {
    u -= min_off;
    v -= min_off;
    w -= min_off;
  }

  #ifdef PWM_INVERT
    PWM_U = PWM_RES - CLAMP(u, 0, PWM_RES - min_off);
    PWM_V = PWM_RES - CLAMP(v, 0, PWM_RES - min_off);
    PWM_W = PWM_RES - CLAMP(w, 0, PWM_RES - min_off);
  #else
    PWM_U = CLAMP(u, 0, PWM_RES - min_off);
    PWM_V = CLAMP(v, 0, PWM_RES - min_off);
    PWM_W = CLAMP(w, 0, PWM_RES - min_off);
  #endif

  float i = PIN(ac_current);
  float t = PIN(hv_temp);

  if(err_filter(&(ctx->overtemp_error), 5.0, 0.001, t > ABS_MAX_TEMP)) {
    ctx->fault = 1;
  }

  if(err_filter(&(ctx->overvoltage_error), 5.0, 0.001, udc > ABS_MAX_VOLT)) {
    ctx->fault = 2;
  }

  if(err_filter(&(ctx->overcurrent_error), 5.0, 0.001, i * i > MAX_CURRENT)) {
    ctx->fault = 3;
  }

  if(i * i > ABS_MAX_CURRENT) {
    ctx->fault = 4;
  }

  // #ifdef HV_FAULT_PIN
  //   //TODO: check enable timing on fault pin
  //   if(err_filter(&(ctx->fault_pin_error), 45.0, 0.01, HAL_GPIO_ReadPin(HV_FAULT_PORT, HV_FAULT_PIN) <= 0.0)){
  //     //ctx->fault = 5;
  //   }
  // #endif

  PIN(fault) = ctx->fault;

  if(PIN(en) > 0.0){
    if(ctx->fault == 0){
      #ifdef HV_EN_PIN
        HAL_GPIO_WritePin(HV_EN_PORT, HV_EN_PIN, GPIO_PIN_SET);
      #endif
    }
    else{
      #ifdef HV_EN_PIN
      HAL_GPIO_WritePin(HV_EN_PORT, HV_EN_PIN, GPIO_PIN_RESET);
      #endif
    }
  }
  else{
    ctx->fault = 0;
    #ifdef HV_EN_PIN
      HAL_GPIO_WritePin(HV_EN_PORT, HV_EN_PIN, GPIO_PIN_RESET);
    #endif
  }

  //dac output for comperators
  DAC1->DHR12R1 = (uint32_t)PIN(dac);

  //comperator outputs for debugging
  PIN(cu) = (COMP1->CSR & COMP_CSR_COMPxOUT) > 0;
  PIN(cv) = (COMP2->CSR & COMP_CSR_COMPxOUT) > 0;
  PIN(cw) = (COMP4->CSR & COMP_CSR_COMPxOUT) > 0;

  //master out enable
  PIN(moe_r) = (TIM8->BDTR & TIM_BDTR_MOE) > 0;
  if(PIN(moe_w) > 0.0) {
    PIN(moe_w) = 0.0;
    TIM8->BDTR |= TIM_BDTR_MOE;
  }
}

hal_comp_t hv_comp_struct = {
    .name      = "hv",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = nrt_init,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = sizeof(struct hv_ctx_t),
    .pin_count = sizeof(struct hv_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
