#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"
#include "stm32f3xx_hal.h"

HAL_COMP(enc);

HAL_PIN(pos);
HAL_PIN(a);
HAL_PIN(b);

TIM_HandleTypeDef htim1;

static void nrt_init(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  // struct enc_ctx_t * ctx = (struct enc_ctx_t *)ctx_ptr;
  // struct enc_pin_ctx_t * pins = (struct enc_pin_ctx_t *)pin_ptr;
  __HAL_RCC_TIM1_CLK_ENABLE();

  /**TIM1 GPIO Configuration    
  PA8     ------> TIM1_CH1
  PA9     ------> TIM1_CH2 
  */
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin       = GPIO_PIN_8 | GPIO_PIN_9;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF6_TIM1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  TIM_Encoder_InitTypeDef sConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim1.Instance               = TIM1;
  htim1.Init.Prescaler         = 0;
  htim1.Init.CounterMode       = TIM_COUNTERMODE_UP;
  htim1.Init.Period            = 2000;
  htim1.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  // htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode  = TIM_ENCODERMODE_TI12;  //TIM_ENCODERMODE_TI1??
  sConfig.IC1Polarity  = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter    = 0;
  sConfig.IC2Polarity  = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter    = 0;
  HAL_TIM_Encoder_Init(&htim1, &sConfig);

  sMasterConfig.MasterOutputTrigger  = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode      = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig);

  HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_1 | TIM_CHANNEL_2);
}

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  // struct enc_ctx_t * ctx = (struct enc_ctx_t *)ctx_ptr;
  struct enc_pin_ctx_t *pins = (struct enc_pin_ctx_t *)pin_ptr;

  PIN(a)   = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8);
  PIN(b)   = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9);
  PIN(pos) = mod(TIM1->CNT * 2.0f * M_PI / 2000.0f);
}

hal_comp_t enc_comp_struct = {
    .name      = "enc",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = nrt_init,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = 0,
    .pin_count = sizeof(struct enc_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
