#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"
#include "stm32f4xx_conf.h"
#include "hw/hw.h"

HAL_COMP(enc_cmd);

HAL_PIN(res);
HAL_PIN(pos);
HAL_PIN(a);
HAL_PIN(b);
HAL_PIN(mode); // 0 = quad, 1 = step/dir, 2 = dir/step, 3 = up/down

struct enc_cmd_ctx_t{
   int e_res;
};

static void nrt_init(volatile void * ctx_ptr, volatile hal_pin_inst_t * pin_ptr){
   struct enc_cmd_ctx_t * ctx = (struct enc_cmd_ctx_t *)ctx_ptr;
   struct enc_cmd_pin_ctx_t * pins = (struct enc_cmd_pin_ctx_t *)pin_ptr;
   ctx->e_res = 0;
   PIN(res) = 4096.0;
}

static void hw_init(volatile void * ctx_ptr, volatile hal_pin_inst_t * pin_ptr){
   struct enc_cmd_ctx_t * ctx = (struct enc_cmd_ctx_t *)ctx_ptr;
   struct enc_cmd_pin_ctx_t * pins = (struct enc_cmd_pin_ctx_t *)pin_ptr;
   
   ctx->e_res = (int)PIN(res);
   if(ctx->e_res < 1){
       ctx->e_res = 1;
   }
     
   //RCC_AHB1PeriphClockCmd(CMD_|ENC0_B_IO_RCC, ENABLE);    //Enable needed Clocks for IOs
   GPIO_InitTypeDef GPIO_InitStruct;
   GPIO_StructInit(&GPIO_InitStruct);
   GPIO_InitStruct.GPIO_Pin   = CMD_A_PIN;
   GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
   GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
   GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
   GPIO_Init(CMD_A_PORT, &GPIO_InitStruct);

   GPIO_InitStruct.GPIO_Pin   = CMD_B_PIN;
   GPIO_Init(CMD_B_PORT, &GPIO_InitStruct);
   
   //Bind pins to Timer
   GPIO_PinAFConfig(CMD_A_PORT, CMD_A_PIN_SOURCE, CMD_ENC_TIM_AF);
   GPIO_PinAFConfig(CMD_B_PORT, CMD_B_PIN_SOURCE, CMD_ENC_TIM_AF);

   //Enable Timer clock
   RCC_APB2PeriphClockCmd(CMD_ENC_TIM_RCC, ENABLE);
   TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
   TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
   TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
   TIM_TimeBaseStruct.TIM_Prescaler = 1;
   TIM_TimeBaseStruct.TIM_RepetitionCounter = 0;
   TIM_TimeBaseInit(CMD_ENC_TIM, &TIM_TimeBaseStruct);
   
   TIM_EncoderInterfaceConfig (CMD_ENC_TIM, TIM_EncoderMode_TI1, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
   
   TIM_ICInitTypeDef TIM_ICInitStruct;
   TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
   TIM_ICInitStruct.TIM_ICFilter = 0x0f;                         //Digital filtering @ 1/32 fDTS
   TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Falling;     //Just trigger at the rising edge, because its the  clock
   TIM_ICInitStruct.TIM_ICPrescaler = 1;                        //no prescaler, capture is done each time an edge is detected on the capture input
   TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI; //IC1 mapped to TI1
   TIM_ICInit(CMD_ENC_TIM, &TIM_ICInitStruct);
   
   TIM_ICInitStruct.TIM_Channel = TIM_Channel_2;
   TIM_ICInitStruct.TIM_ICFilter = 0x0f;                         //Digital filtering @ 1/32 fDTS
   TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_BothEdge;   //Trigger at every edge, because its the direction
   TIM_ICInitStruct.TIM_ICPrescaler = 1;                        //no prescaler, capture is done each time an edge is detected on the capture input
   TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_IndirectTI; //IC2 mapped to TI1
   TIM_ICInit(CMD_ENC_TIM, &TIM_ICInitStruct);

   TIM_SetAutoreload(CMD_ENC_TIM, ctx->e_res - 1);
   
   TIM_Cmd(CMD_ENC_TIM, ENABLE);
}

static void rt_func(float period, volatile void * ctx_ptr, volatile hal_pin_inst_t * pin_ptr){
   struct enc_cmd_ctx_t * ctx = (struct enc_cmd_ctx_t *)ctx_ptr;
   struct enc_cmd_pin_ctx_t * pins = (struct enc_cmd_pin_ctx_t *)pin_ptr;
   
  int32_t tim = TIM_GetCounter(CMD_ENC_TIM);//TODO: interrupt here?

  PIN(a) = CMD_A_PORT->IDR & CMD_A_PIN > 0;
  PIN(b) = CMD_B_PORT->IDR & CMD_B_PIN > 0;
  
  float p = 0.0;
  p = mod(tim * 2.0f * M_PI / (float)ctx->e_res);
  
  int r = (int)PIN(res);
  if(r < 1){
       r = 1;
  }
  
  if(ctx->e_res != r){
    ctx->e_res = r;
    TIM_SetAutoreload(FB0_ENC_TIM, ctx->e_res - 1);
  }
}

const hal_comp_t enc_cmd_comp_struct = {
  .name = "enc_cmd",
  .nrt = 0,
  .rt = rt_func,
  .frt = 0,
  .nrt_init = nrt_init,
  .hw_init = hw_init,
  .rt_start = 0,
  .frt_start = 0,
  .rt_stop = 0,
  .frt_stop = 0,
  .ctx_size = sizeof(struct enc_cmd_ctx_t),
  .pin_count = sizeof(struct enc_cmd_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
