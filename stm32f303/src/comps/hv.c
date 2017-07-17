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

HAL_PIN(hv_temp);

//enable in
HAL_PIN(en);
//TODO: half bridge enable in
HAL_PIN(enu);
HAL_PIN(env);
HAL_PIN(enw);

//fault output
HAL_PIN(fault);

HAL_PIN(min_on); // min on time [s]
HAL_PIN(min_off); // min off time [s]

static void nrt_init(volatile void * ctx_ptr, volatile hal_pin_inst_t * pin_ptr){
  // struct hv_ctx_t * ctx = (struct hv_ctx_t *)ctx_ptr;
  struct hv_pin_ctx_t * pins = (struct hv_pin_ctx_t *)pin_ptr;
  PIN(enu) = 1.0;
  PIN(env) = 1.0;
  PIN(enw) = 1.0;
  PIN(min_on) = 0.00000035;
  PIN(min_off) = 0.000005;
  
  GPIO_InitTypeDef GPIO_InitStruct;
  //PA15 HV EN
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  //PB7 HV FAULT
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  
}

static void rt_func(float period, volatile void * ctx_ptr, volatile hal_pin_inst_t * pin_ptr){
  // struct hv_ctx_t * ctx = (struct hv_ctx_t *)ctx_ptr;
  struct hv_pin_ctx_t * pins = (struct hv_pin_ctx_t *)pin_ptr;

  float udc = MAX(PIN(udc), 0.1);
  //convert voltages to PWM output compare values
  int32_t u = (int32_t)(CLAMP(PIN(u), 0.0, udc) / udc * 4800.0);
  int32_t v = (int32_t)(CLAMP(PIN(v), 0.0, udc) / udc * 4800.0);
  int32_t w = (int32_t)(CLAMP(PIN(w), 0.0, udc) / udc * 4800.0);
  //convert on and off times to PWM output compare values
  int32_t min_on = (int32_t)(4800.0 * 15000.0 * PIN(min_on) + 0.5);
  int32_t min_off = (int32_t)(4800.0 * 15000.0 * PIN(min_off) + 0.5);
   
  if((u > 0 && u < min_on) || (v > 0 && v < min_on) || (w > 0 && w < min_on)){
    u += min_on;
    v += min_on;
    w += min_on;
  }
   
  if((u > 4800 - min_off) || (v > 4800 - min_off) || (w > 4800 - min_off)){
    u -= min_off;
    v -= min_off;
    w -= min_off;
  }

#ifdef PWM_INVERT
  PWM_U = 4800-CLAMP(u, 0, 4800 - min_off);
  PWM_V = 4800-CLAMP(v, 0, 4800 - min_off);
  PWM_W = 4800-CLAMP(w, 0, 4800 - min_off);
#else
  PWM_U = CLAMP(u, 0, 4800 - min_off);
  PWM_V = CLAMP(v, 0, 4800 - min_off);
  PWM_W = CLAMP(w, 0, 4800 - min_off);
#endif
  
  if(PIN(hv_temp) < 85.0){
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, PIN(en) > 0 ? GPIO_PIN_SET : GPIO_PIN_RESET);
  }
  else{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
  }
  //TODO: check enable timing on fault pin
  PIN(fault) = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7);
}

hal_comp_t hv_comp_struct = {
  .name = "hv",
  .nrt = 0,
  .rt = rt_func,
  .frt = 0,
  .nrt_init = nrt_init,
  .rt_start = 0,
  .frt_start = 0,
  .rt_stop = 0,
  .frt_stop = 0,
  .ctx_size = 0,
  .pin_count = sizeof(struct hv_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
