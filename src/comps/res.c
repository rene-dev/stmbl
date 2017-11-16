#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"
#include "stm32f4xx_conf.h"
#include "hw/hw.h"

HAL_COMP(res);

HAL_PIN(pos);
HAL_PIN(amp);
HAL_PIN(quad);
HAL_PIN(poles);
HAL_PIN(min_amp);

HAL_PIN(vel);  // TODO: vel rev, fb,cmd -> vel0,1 -> rev

HAL_PIN(sin);
HAL_PIN(cos);

HAL_PIN(enable);
HAL_PIN(error);
HAL_PIN(state);
HAL_PIN(tim_oc);

// TODO: in hal stop, reset adc dma

struct res_ctx_t {
  int lastq;  // last quadrant
  int abspos;  // multiturn position
};

static void nrt_init(volatile void *ctx_ptr, volatile hal_pin_inst_t *pin_ptr) {
  // struct res_ctx_t *ctx      = (struct res_ctx_t *)ctx_ptr;
  struct res_pin_ctx_t *pins = (struct res_pin_ctx_t *)pin_ptr;
  PIN(poles)                 = 1.0;
  PIN(tim_oc)                = 0.85;
  PIN(min_amp)               = 0.15;
}
static void hw_init(volatile void *ctx_ptr, volatile hal_pin_inst_t *pin_ptr) {
  struct res_ctx_t *ctx = (struct res_ctx_t *)ctx_ptr;
  // struct res_pin_ctx_t *pins = (struct res_pin_ctx_t *)pin_ptr;

  ctx->abspos = 0;
  ctx->lastq  = 0;

  TIM_OCInitTypeDef TIM_OCInitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

#ifdef V4
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

  //timer init for v4, v3 uses slave timer
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  TIM_TimeBaseStructure.TIM_ClockDivision     = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode       = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period            = ADC_TR_COUNT - 1;  // 20kHz
  TIM_TimeBaseStructure.TIM_Prescaler         = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
  TIM_SelectSlaveMode(TIM4, TIM_SlaveMode_External1);  // Rising edges of the selected trigger (TRGI) clock the counter
  TIM_ITRxExternalClockConfig(TIM4, TIM_TS_ITR2);  // clk = TIM_MASTER(TIM2) trigger out
  TIM_ARRPreloadConfig(TIM4, ENABLE);
  TIM_Cmd(TIM4, ENABLE);
#endif
  // resolver reference signal OC
  TIM_OCInitStructure.TIM_OCMode       = TIM_OCMode_Toggle;
  TIM_OCInitStructure.TIM_OutputState  = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
  TIM_OCInitStructure.TIM_Pulse        = 0;
  TIM_OCInitStructure.TIM_OCPolarity   = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OCNPolarity  = TIM_OCNPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState  = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
  //ref is always OC3
  TIM_OC3Init(TIM_SLAVE, &TIM_OCInitStructure);
  TIM_OC3PreloadConfig(TIM_SLAVE, TIM_OCPreload_Enable);
  TIM_CtrlPWMOutputs(TIM_SLAVE, ENABLE);

  //resolver ref signal generation
  GPIO_InitStructure.GPIO_Pin   = FB0_RES_REF_PIN;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(FB0_RES_REF_PORT, &GPIO_InitStructure);
  GPIO_PinAFConfig(FB0_RES_REF_PORT, FB0_RES_REF_PIN_SOURCE, FB0_RES_REF_TIM_AF);

  // TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;
  // TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  // TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
  // TIM_OCInitStructure.TIM_Pulse = 10;
  // TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  // TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
  // TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  // TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
  //
  // TIM_OC3Init(TIM2, &TIM_OCInitStructure);
  // TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
  // TIM_CtrlPWMOutputs(TIM2, ENABLE);

  //txen
  GPIO_InitStructure.GPIO_Pin   = FB0_Z_TXEN_PIN;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(FB0_Z_TXEN_PORT, &GPIO_InitStructure);
  GPIO_SetBits(FB0_Z_TXEN_PORT, FB0_Z_TXEN_PIN);
}

static void rt_func(float period, volatile void *ctx_ptr, volatile hal_pin_inst_t *pin_ptr) {
  struct res_ctx_t *ctx      = (struct res_ctx_t *)ctx_ptr;
  struct res_pin_ctx_t *pins = (struct res_pin_ctx_t *)pin_ptr;
  //TODO: arr can change!
  FB0_RES_REF_TIM->CCR3 = (int)CLAMP(PIN(tim_oc) * FB0_RES_REF_TIM->ARR, 0, FB0_RES_REF_TIM->ARR - 1);

  float s = 0.0;
  float c = 0.0;
  float a = 0.0;

  s = PIN(sin);
  c = PIN(cos);
  a = sqrtf(s * s + c * c);

  float p = MAX(1.0, PIN(poles));

  float pos  = atan2f(s, c);
  float dpos = PIN(vel) * period / 2.0;

  if(a < PIN(min_amp)) {
    PIN(error) = 1.0;
    PIN(state) = 0.0;
  } else {
    PIN(error) = 0.0;
    PIN(state) = 3.0;
    if(p == 1.0f) {
      PIN(pos) = mod(pos + dpos);
    } else {
      int q = PIN(quad);  // current quadrant

      if(ctx->lastq == 2 && q == 3)
        ctx->abspos++;
      if(ctx->lastq == 3 && q == 2)
        ctx->abspos--;

      if(ctx->abspos >= p) {
        ctx->abspos = 0;
      }
      if(ctx->abspos <= -1) {
        ctx->abspos = p - 1;
      }

      ctx->lastq = q;
      //TODO: clamp ctx->abspos
      float absa = pos + ctx->abspos * M_PI * 2.0f;
      PIN(pos)   = mod(absa / p + dpos);
    }
  }
  PIN(amp) = a;
}

const hal_comp_t res_comp_struct = {
    .name      = "res",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = nrt_init,
    .hw_init   = hw_init,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = sizeof(struct res_ctx_t),
    .pin_count = sizeof(struct res_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
