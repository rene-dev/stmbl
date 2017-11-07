#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"
#include "stm32f3xx_hal.h"

HAL_COMP(ppm);

HAL_PIN(pos);
HAL_PIN(a);
HAL_PIN(b);
HAL_PIN(cnt);
HAL_PIN(c1);
HAL_PIN(c2);
HAL_PIN(diff);
HAL_PIN(out);
HAL_PIN(zero);
HAL_PIN(max);
HAL_PIN(en);
HAL_PIN(gain);

float timeout = 100;

static void nrt_init(volatile void *ctx_ptr, volatile hal_pin_inst_t *pin_ptr) {
  // struct ppm_ctx_t * ctx = (struct ppm_ctx_t *)ctx_ptr;
  struct ppm_pin_ctx_t * pins = (struct ppm_pin_ctx_t *)pin_ptr;
  
  __HAL_RCC_TIM2_CLK_ENABLE();

  /**TIM1 GPIO Configuration    
  PB10,tx    ------> TIM2_CH3
  PA11,rx    ------> TIM2_CH4 
  */
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin       = GPIO_PIN_10 | GPIO_PIN_11;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  // TIM_encoder_InitTypeDef sConfig;
  // TIM_MasterConfigTypeDef sMasterConfig;
  TIM_HandleTypeDef htim2;
  htim2.Instance               = TIM2;
  htim2.Init.Prescaler         = 10;
  htim2.Init.CounterMode       = TIM_COUNTERMODE_UP;
  htim2.Init.Period            = 65535;
  htim2.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV4;
  htim2.Init.RepetitionCounter = 0;
  HAL_TIM_Base_Init(&htim2);
  
  TIM2->CCMR2 |= TIM_CCMR2_CC3S_0 | TIM_CCMR2_CC4S_1;
  TIM2->CCER |= TIM_CCER_CC3E | TIM_CCER_CC4E | TIM_CCER_CC4P;
  //TIM2->SMCR |= TIM_SMCR_TS_2 | TIM_SMCR_TS_0 | TIM_SMCR_SMS_2;
  TIM2->CR1 |= TIM_CR1_CEN;

  PIN(zero) = 32125;
  PIN(max) = 3350;
  PIN(gain) = 20;
}
//diff: 

// 28956.000000
// 32045.000000
// 35317.000000
static void rt_func(float period, volatile void *ctx_ptr, volatile hal_pin_inst_t *pin_ptr) {
  // struct ppm_ctx_t * ctx = (struct ppm_ctx_t *)ctx_ptr;
  struct ppm_pin_ctx_t *pins = (struct ppm_pin_ctx_t *)pin_ptr;
  PIN(cnt) = TIM2->CNT;
  if((TIM2->SR & TIM_SR_CC3IF) && (TIM2->SR & TIM_SR_CC4IF)){
    PIN(c1) = TIM2->CCR3;
    PIN(c2) = TIM2->CCR4;
    float diff = PIN(c2) - PIN(c1);
    if(diff < 0){
      diff += 65535;
    }
    if(diff > 28000 && diff < 36000){
      PIN(diff) = diff;
      PIN(out) = ((diff-PIN(zero))/PIN(max))*PIN(gain);
      timeout = 0;
      PIN(en) = 1;
    }
  }
  if(timeout > 0.5){
    PIN(en) = 0;
    PIN(out) = 0;
  }
  timeout += 0.0001;
}

hal_comp_t ppm_comp_struct = {
    .name      = "ppm",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = nrt_init,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = 0,
    .pin_count = sizeof(struct ppm_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
