#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"
#include "stm32f4xx_conf.h"
#include "hw/hw.h"

HAL_COMP(o_fb);

HAL_PIN(a);
HAL_PIN(b);

static void hw_init(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  // struct o_fb_ctx_t * ctx = (struct o_fb_ctx_t *)ctx_ptr;
  // struct o_fb_pin_ctx_t * pins = (struct o_fb_pin_ctx_t *)pin_ptr;

  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

  GPIO_InitStructure.GPIO_Pin = FB0_A_EN_PIN;
  GPIO_Init(FB0_A_EN_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = FB0_B_EN_PIN;
  GPIO_Init(FB0_B_EN_PORT, &GPIO_InitStructure);

  GPIO_SetBits(FB0_A_EN_PORT, FB0_A_EN_PIN);
  GPIO_SetBits(FB0_B_EN_PORT, FB0_B_EN_PIN);

  GPIO_InitStructure.GPIO_Pin = FB0_A_PIN;
  GPIO_Init(FB0_A_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = FB0_B_PIN;
  GPIO_Init(FB0_B_PORT, &GPIO_InitStructure);
}


static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  // struct o_fb_ctx_t * ctx = (struct o_fb_ctx_t *)ctx_ptr;
  struct o_fb_pin_ctx_t *pins = (struct o_fb_pin_ctx_t *)pin_ptr;

  if(PIN(a) > 0) {
    GPIO_SetBits(FB0_A_PORT, FB0_A_PIN);
  } else {
    GPIO_ResetBits(FB0_A_PORT, FB0_A_PIN);
  }

  if(PIN(b) > 0) {
    GPIO_SetBits(FB0_B_PORT, FB0_B_PIN);
  } else {
    GPIO_ResetBits(FB0_B_PORT, FB0_B_PIN);
  }
}

const hal_comp_t o_fb_comp_struct = {
    .name      = "o_fb",
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
    .pin_count = sizeof(struct o_fb_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
