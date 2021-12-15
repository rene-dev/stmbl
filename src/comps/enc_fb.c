#include "enc_fb_comp.h"
#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"
#include "stm32f4xx_conf.h"
#include "hw/hw.h"

HAL_COMP(enc_fb);

HAL_PIN(res);
HAL_PIN(ires);
HAL_PIN(pos);
HAL_PIN(abs_pos);
HAL_PIN(state);
HAL_PIN(index);  //TODO:
HAL_PIN(a);
HAL_PIN(b);
HAL_PIN(ipos);
HAL_PIN(sin);
HAL_PIN(cos);
HAL_PIN(quad);
HAL_PIN(oquad);
HAL_PIN(oquadoff);
HAL_PIN(qdiff);
HAL_PIN(error);
HAL_PIN(amp);
HAL_PIN(vel);
HAL_PIN(ccr3);
HAL_PIN(en_index);
HAL_PIN(indexprint);


struct enc_fb_ctx_t {
  int e_res;
  float absoffset;
};

static int indexpos   = 0;
static int indexprint = 0;

static void nrt_init(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct enc_fb_ctx_t *ctx      = (struct enc_fb_ctx_t *)ctx_ptr;
  struct enc_fb_pin_ctx_t *pins = (struct enc_fb_pin_ctx_t *)pin_ptr;
  ctx->e_res                    = 0;
  ctx->absoffset                = 0.0;
  PIN(res)                      = 2048.0;
  PIN(ires)                     = 1024.0;
}

static void hw_init(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct enc_fb_ctx_t *ctx      = (struct enc_fb_ctx_t *)ctx_ptr;
  struct enc_fb_pin_ctx_t *pins = (struct enc_fb_pin_ctx_t *)pin_ptr;
  GPIO_InitTypeDef GPIO_InitStructure;
  TIM_ICInitTypeDef TIM_ICInitStructure;
  TIM_ICInitStructure.TIM_Channel     = TIM_Channel_1 | TIM_Channel_2;
  TIM_ICInitStructure.TIM_ICPolarity  = TIM_ICPolarity_BothEdge;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter    = 0xF;
  TIM_ICInit(FB0_ENC_TIM, &TIM_ICInitStructure);

  /***************** port 1, quadrature , sin/cos or resolver *********************/
  ctx->e_res = (int)PIN(res);
  if(ctx->e_res < 1) {
    ctx->e_res = 1;
  }
  // enable clocks
  RCC_APB1PeriphClockCmd(FB0_ENC_TIM_RCC, ENABLE);

  // pin mode: af
  GPIO_InitStructure.GPIO_Pin   = FB0_A_PIN;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(FB0_A_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = FB0_B_PIN;
  GPIO_Init(FB0_B_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = FB0_Z_PIN;
  GPIO_Init(FB0_Z_PORT, &GPIO_InitStructure);

  // pin af -> tim
  GPIO_PinAFConfig(FB0_A_PORT, FB0_A_PIN_SOURCE, FB0_ENC_TIM_AF);
  GPIO_PinAFConfig(FB0_B_PORT, FB0_B_PIN_SOURCE, FB0_ENC_TIM_AF);
  GPIO_PinAFConfig(FB0_Z_PORT, FB0_Z_PIN_SOURCE, FB0_ENC_TIM_AF);

  // enc res / turn
  TIM_SetAutoreload(FB0_ENC_TIM, ctx->e_res - 1);

  // quad
  TIM_Cmd(FB0_ENC_TIM, DISABLE);
  TIM_EncoderInterfaceConfig(FB0_ENC_TIM, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Falling);
  TIM_Cmd(FB0_ENC_TIM, ENABLE);
  FB0_ENC_TIM->CCMR2 |= TIM_CCMR2_CC3S_0;  //CC3 channel is configured as input, IC3 is mapped on CH3
  FB0_ENC_TIM->CCER |= TIM_CCER_CC3E;      //Capture enabled
}


// static void frt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
//   struct enc_fb_ctx_t *ctx      = (struct enc_fb_ctx_t *)ctx_ptr;
//   struct enc_fb_pin_ctx_t *pins = (struct enc_fb_pin_ctx_t *)pin_ptr;

//   float p  = mod(TIM_GetCounter(FB0_ENC_TIM) * 2.0f * M_PI / (float)ctx->e_res);
//   PIN(pos) = p;
//   //TODO: this gets triggered by wire saving abs encoders. add timeout?
//   if(RISING_EDGE(!GPIO_ReadInputDataBit(FB0_Z_PORT, FB0_Z_PIN))) {
//     // TODO: fix
//     ctx->absoffset = -p;
//     PIN(state)     = 3.0;

//   }
//   PIN(index)  = GPIO_ReadInputDataBit(FB0_Z_PORT, FB0_Z_PIN);
//   PIN(abs_pos) = mod(p + ctx->absoffset);
// }

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct enc_fb_ctx_t *ctx      = (struct enc_fb_ctx_t *)ctx_ptr;
  struct enc_fb_pin_ctx_t *pins = (struct enc_fb_pin_ctx_t *)pin_ptr;

  //sample timer value and timer pins together, so we can calculate the quadrant of the timer
  int32_t tim     = TIM_GetCounter(FB0_ENC_TIM);  //TODO: interrupt here?
  uint32_t scgpio = FB0_A_PORT->IDR;

  float p = 0.0;
  int r   = (int)PIN(res);
  if(r < 1) {
    r = 1;
  }

  float ir = PIN(ires);
  if(ir < 1) {
    ir = 1;
  }

  float s = PIN(sin);
  float c = PIN(cos);

  int q;

  //calculate quadrant of timer
  if((scgpio & FB0_A_PIN)) {  //TODO: invert for v3... check: plot oquad vs quad
    if(scgpio & FB0_B_PIN) {
      q = 1;
    } else {
      q = 2;
    }
  } else {
    if(scgpio & FB0_B_PIN) {
      q = 4;
    } else {
      q = 3;
    }
  }
  //TODO: sincos stuff at speed
  //analog quadrant is calculated by adc component
  int qdiff = PIN(quad) - q;

  switch(qdiff) {
    case 1:
    case -3:
      tim++;
      break;
    case -1:
    case 3:
      tim--;
      break;
    default:
      break;
  }

  if(tim >= ctx->e_res) {
    tim = 0;
  } else if(tim < 0) {
    tim = ctx->e_res - 1;
  }

  PIN(qdiff) = qdiff;

  PIN(a) = (scgpio & FB0_A_PIN) > 0;  //TODO: invert for v3
  PIN(b) = (scgpio & FB0_B_PIN) > 0;

  PIN(oquad) = q;

  p        = mod(tim * 2.0f * M_PI / (float)ctx->e_res);
  PIN(pos) = p;

  if((PIN(en_index) > 0.0) && (FB0_ENC_TIM->SR & TIM_SR_CC3IF)) {
    int cc         = FB0_ENC_TIM->CCR3;
    PIN(state)     = 3.0;
    ctx->absoffset = mod(cc * 2.0f * M_PI / (float)ctx->e_res);
    if(PIN(indexprint) > 0.0) {
      indexpos   = cc;
      indexprint = 1;
    }
  }
  PIN(abs_pos) = minus(p, ctx->absoffset);
  PIN(index)   = GPIO_ReadInputDataBit(FB0_Z_PORT, FB0_Z_PIN);

  if(PIN(amp) > 0.25 || ABS(PIN(vel)) > 0.15) {
    PIN(error) = 0.0;
    PIN(state) = MAX(PIN(state), 1.0);
    PIN(ipos)  = mod(p + ((int)(ir * mod(atan2f(s, c) * 4.0 + M_PI) / M_PI)) / ir * M_PI / (float)ctx->e_res);
  } else {
    PIN(error) = 1.0;
    PIN(state) = 0.0;
  }

  if(ctx->e_res != r) {
    ctx->e_res = r;
    TIM_SetAutoreload(FB0_ENC_TIM, ctx->e_res - 1);
  }
}

static void nrt_func(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  // struct enc_fb_ctx_t *ctx      = (struct enc_fb_ctx_t *)ctx_ptr;
  // struct enc_fb_pin_ctx_t *pins = (struct enc_fb_pin_ctx_t *)pin_ptr;
  if(indexprint == 1) {
    indexprint = 0;
    printf("cnt = %i\n", indexpos);
  }
}

const hal_comp_t enc_fb_comp_struct = {
    .name      = "enc_fb",
    .nrt       = nrt_func,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = nrt_init,
    .hw_init   = hw_init,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = sizeof(struct enc_fb_ctx_t),
    .pin_count = sizeof(struct enc_fb_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
