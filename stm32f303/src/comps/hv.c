#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"
#include "tim.h"
#include "f3hw.h"

HAL_COMP(hv);

//U V W input
HAL_PIN(a);
HAL_PIN(b);

//dclink input
HAL_PIN(udc);

HAL_PIN(hv_temp);

//enable in
HAL_PIN(en);

//TODO: half bridge enable in
HAL_PIN(ena);
HAL_PIN(enb);

//fault output
HAL_PIN(fault);

HAL_PIN(min_on); // min on time [s]
HAL_PIN(min_off); // min off time [s]

static void nrt_init(volatile void * ctx_ptr, volatile hal_pin_inst_t * pin_ptr){
  // struct hv_ctx_t * ctx = (struct hv_ctx_t *)ctx_ptr;
  struct hv_pin_ctx_t * pins = (struct hv_pin_ctx_t *)pin_ptr;
  PIN(ena) = 1.0;
  PIN(enb) = 1.0;
  PIN(min_on) = 0.000001;
  PIN(min_off) = 0.000001;
  
  GPIO_InitTypeDef GPIO_InitStruct;
  //enable
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  GPIO_InitStruct.Pin = GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
}

static void rt_func(float period, volatile void * ctx_ptr, volatile hal_pin_inst_t * pin_ptr){
  // struct hv_ctx_t * ctx = (struct hv_ctx_t *)ctx_ptr;
  struct hv_pin_ctx_t * pins = (struct hv_pin_ctx_t *)pin_ptr;

  float udc = MAX(PIN(udc), 0.1);
  //convert voltages to PWM output compare values
  int32_t a = (int32_t)(LIMIT(PIN(a), udc) / udc * 720.0) + 720;
  int32_t b = (int32_t)(LIMIT(PIN(b), udc) / udc * 720.0) + 720;

  //convert on and off times to PWM output compare values
  int32_t min_on = (int32_t)(1440.0 * 50000.0 * PIN(min_on) + 0.5);
  int32_t min_off = (int32_t)(1440.0 * 50000.0 * PIN(min_off) + 0.5);
   
  if((a > 0 && a < min_on) || (b > 0 && b < min_on)){
    a += min_on;
    b += min_on;
  }
   
  if((a > 1440 - min_off) || (b > 1440 - min_off)){
    a -= min_off;
    b -= min_off;
  }

// #ifdef PWM_INVERT
//   TIM1->CCR1 = 1440-CLAMP(a, 0, 1440 - min_off);
//   TIM1->CCR2 = 1440-CLAMP(b, 0, 1440 - min_off);
// #else
  TIM1->CCR1 = CLAMP(a, 0, 1440 - min_off);
  TIM1->CCR2 = CLAMP(b, 0, 1440 - min_off);
// #endif
  
  if(PIN(hv_temp) < 100.0 && PIN(en) > 0.0){
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, PIN(enb) > 0.0 ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, PIN(ena) > 0.0 ? GPIO_PIN_SET : GPIO_PIN_RESET);
  }
  else{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
  }

  //TODO: check enable timing on fault pin
  // PIN(fault) = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7);
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
