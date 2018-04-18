#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"
#include "stmsp_f3hw.h"
#include "stm32f3xx_hal.h"

HAL_COMP(hv);

//U V W input
HAL_PIN(a);
HAL_PIN(b);

HAL_PIN(a_fb);
HAL_PIN(b_fb);

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

HAL_PIN(min_on);  // min on time [s]
HAL_PIN(min_off);  // min off time [s]

static void nrt_init(volatile void *ctx_ptr, volatile hal_pin_inst_t *pin_ptr) {
  // struct hv_ctx_t * ctx = (struct hv_ctx_t *)ctx_ptr;
  struct hv_pin_ctx_t *pins = (struct hv_pin_ctx_t *)pin_ptr;
  PIN(ena)                  = 1.0;
  PIN(enb)                  = 1.0;
  PIN(min_on)               = 0.000001;
  PIN(min_off)              = 0.000001;

  GPIO_InitTypeDef GPIO_InitStruct;
  //enable
  GPIO_InitStruct.Pin   = GPIO_PIN_15;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  GPIO_InitStruct.Pin   = GPIO_PIN_10;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

static void rt_func(float period, volatile void *ctx_ptr, volatile hal_pin_inst_t *pin_ptr) {
  // struct hv_ctx_t * ctx = (struct hv_ctx_t *)ctx_ptr;
  struct hv_pin_ctx_t *pins = (struct hv_pin_ctx_t *)pin_ptr;

  float udc = MAX(PIN(udc), 0.1);
  //convert voltages to PWM output compare values
  int32_t a = (int32_t)(PIN(a) / udc * PWM_RES / 2.0) + PWM_RES / 2;
  int32_t b = (int32_t)(PIN(b) / udc * PWM_RES / 2.0) + PWM_RES / 2;

  //convert on and off times to PWM output compare values
  int32_t min  = (int32_t)(PWM_RES * RT_FREQ * PIN(min_on) + 0.5);
  int32_t min_off = (int32_t)(PWM_RES * RT_FREQ * PIN(min_off) + 0.5);

  a = CLAMP(a, min, PWM_RES - min_off);
  b = CLAMP(b, min, PWM_RES - min_off);

  TIM1->CCR1 = a;
  TIM1->CCR2 = b;
  // GPIOC->BSRR |= GPIO_PIN_15 << 16;
  
  PIN(a_fb) = (a - PWM_RES / 2.0) * 2.0 * udc / PWM_RES;
  PIN(b_fb) = (b - PWM_RES / 2.0) * 2.0 * udc / PWM_RES;

  if(PIN(udc) < 52.0 && PIN(hv_temp) < 100.0 && PIN(en) > 0.0) {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, PIN(enb) > 0.0 ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, PIN(ena) > 0.0 ? GPIO_PIN_SET : GPIO_PIN_RESET);
  } else {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
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
    .ctx_size  = 0,
    .pin_count = sizeof(struct hv_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
