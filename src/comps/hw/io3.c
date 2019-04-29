#include "commands.h"
#include "hal.h"
#include "defines.h"
#include <stdio.h>
#include <string.h>
#include "stm32f4xx_conf.h"
#include "hw/hw.h"
#include "main.h"


HAL_COMP(io);

HAL_PIN(fan);
HAL_PIN(state);
HAL_PIN(fault);
HAL_PIN(brake);

static void hw_init(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  // struct io_ctx_t * ctx = (struct io_ctx_t *)ctx_ptr;
  // struct io_pin_ctx_t *pins = (struct io_pin_ctx_t *)pin_ptr;

  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

  //fan
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  //brake
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
  TIM_TimeBaseStructure.TIM_ClockDivision     = TIM_CKD_DIV4;
  TIM_TimeBaseStructure.TIM_CounterMode       = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period            = 8400;  // 168e6 / 8400 = 20kHz
  TIM_TimeBaseStructure.TIM_Prescaler         = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);
  TIM_ARRPreloadConfig(TIM8, ENABLE);
  TIM_Cmd(TIM8, ENABLE);

  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM8);

  TIM_OCInitStructure.TIM_OCMode       = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState  = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
  TIM_OCInitStructure.TIM_Pulse        = 0;
  TIM_OCInitStructure.TIM_OCPolarity   = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState  = TIM_OCIdleState_Set;

  TIM_OC4Init(TIM8, &TIM_OCInitStructure);
  TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);
  TIM_CtrlPWMOutputs(TIM8, ENABLE);
}

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  // struct io_ctx_t * ctx = (struct io_ctx_t *)ctx_ptr;
  struct io_pin_ctx_t *pins = (struct io_pin_ctx_t *)pin_ptr;
  if(PIN(fan) > 0)
    GPIO_SetBits(GPIOC, GPIO_Pin_8);
  else
    GPIO_ResetBits(GPIOC, GPIO_Pin_8);

  TIM8->CCR4 = (int)CLAMP(PIN(brake) * 8400, 0, TIM8->ARR - 1);

  uint32_t red   = 0;
  uint32_t green = 0;

  switch((state_t)PIN(state)) {
    case DISABLED:
      green = BLINK(1);
      red   = 0;
      break;

    case ENABLED:
      green = 1;
      red   = 0;
      break;

    case LED_TEST:
    case PHASING:
      green = 1;
      red   = 1;
      break;

    case SOFT_FAULT:
      green = 0;
      red   = BLINK((int)PIN(fault));
      break;

    case HARD_FAULT:
      red   = BLINK((int)PIN(fault));
      green = BLINK((int)PIN(fault));
      break;
  }

  if(red > 0)
    GPIO_SetBits(GPIOC, GPIO_Pin_10);
  else
    GPIO_ResetBits(GPIOC, GPIO_Pin_10);

  if(green > 0)
    GPIO_SetBits(GPIOC, GPIO_Pin_11);
  else
    GPIO_ResetBits(GPIOC, GPIO_Pin_11);
}

hal_comp_t io_comp_struct = {
    .name      = "io",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = 0,
    .hw_init   = hw_init,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = 0,
    .pin_count = sizeof(struct io_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};