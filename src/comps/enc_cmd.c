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
HAL_PIN(remap); // 0 = cmd, 1 = fb0, 2 = fb1

struct enc_cmd_ctx_t{
   int e_res;
   uint32_t a_pin, b_pin, a_pin_source, b_pin_source, tim_af, tim_rcc;
   GPIO_TypeDef * a_port, * b_port;
   TIM_TypeDef * tim;
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
     
   //RCC_AHB1PeriphClockCmd(FB1_|ENC0_B_IO_RCC, ENABLE);    //Enable needed Clocks for IOs
   GPIO_InitTypeDef GPIO_InitStruct;
   GPIO_StructInit(&GPIO_InitStruct);
   GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
   GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
   GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;

   
   

   switch((int)PIN(remap)){
      case 0:
         ctx->a_pin = CMD_A_PIN;
         ctx->b_pin = CMD_B_PIN;
         ctx->a_port = CMD_A_PORT;
         ctx->b_port = CMD_B_PORT;
         ctx->a_pin_source = CMD_A_PIN_SOURCE;
         ctx->b_pin_source = CMD_B_PIN_SOURCE;
         ctx->tim_af = CMD_ENC_TIM_AF;
         ctx->tim_rcc = CMD_ENC_TIM_RCC;
         ctx->tim = CMD_ENC_TIM;
      break;
      case 1:
         ctx->a_pin = FB0_A_PIN;
         ctx->b_pin = FB0_B_PIN;
         ctx->a_port = FB0_A_PORT;
         ctx->b_port = FB0_B_PORT;
         ctx->a_pin_source = FB0_A_PIN_SOURCE;
         ctx->b_pin_source = FB0_B_PIN_SOURCE;
         ctx->tim_af = FB0_ENC_TIM_AF;
         ctx->tim_rcc = FB0_ENC_TIM_RCC;
         ctx->tim = FB0_ENC_TIM;
      break;
      case 2:
         ctx->a_pin = FB1_A_PIN;
         ctx->b_pin = FB1_B_PIN;
         ctx->a_port = FB1_A_PORT;
         ctx->b_port = FB1_B_PORT;
         ctx->a_pin_source = FB1_A_PIN_SOURCE;
         ctx->b_pin_source = FB1_B_PIN_SOURCE;
         ctx->tim_af = FB1_ENC_TIM_AF;
         ctx->tim_rcc = FB1_ENC_TIM_RCC;
         ctx->tim = FB1_ENC_TIM;
      break;
      default:
      return;
   }
   GPIO_InitStruct.GPIO_Pin   = ctx->a_pin;
   GPIO_Init(ctx->a_port, &GPIO_InitStruct);

   GPIO_InitStruct.GPIO_Pin   = ctx->b_pin;
   GPIO_Init(ctx->b_port, &GPIO_InitStruct);
   
   //Bind pins to Timer
   GPIO_PinAFConfig(ctx->a_port, ctx->a_pin_source, ctx->tim_af);
   GPIO_PinAFConfig(ctx->b_port, ctx->b_pin_source, ctx->tim_af);

   RCC_APB1PeriphClockCmd(ctx->tim_rcc, ENABLE);

   TIM_SetAutoreload(ctx->tim, ctx->e_res - 1);
   // quad
   TIM_Cmd(ctx->tim, DISABLE);
   TIM_EncoderInterfaceConfig(ctx->tim, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
   TIM_ICInitTypeDef TIM_ICInitStruct;
   TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
   TIM_ICInitStruct.TIM_ICFilter = 0x0f;                         //Digital filtering @ 1/32 fDTS
   TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;     //Just trigger at the rising edge, because its the  clock
   TIM_ICInitStruct.TIM_ICPrescaler = 1;                        //no prescaler, capture is done each time an edge is detected on the capture input
   TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI; //IC1 mapped to TI1
   TIM_ICInit(ctx->tim, &TIM_ICInitStruct);

   TIM_ICInitStruct.TIM_Channel = TIM_Channel_2;
   TIM_ICInitStruct.TIM_ICFilter = 0x0f;                         //Digital filtering @ 1/32 fDTS
   TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_BothEdge;   //Trigger at every edge, because its the direction
   TIM_ICInitStruct.TIM_ICPrescaler = 1;                        //no prescaler, capture is done each time an edge is detected on the capture input
   TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_IndirectTI; //IC2 mapped to TI1
   TIM_ICInit(ctx->tim, &TIM_ICInitStruct);
   TIM_Cmd(ctx->tim, ENABLE);
}

static void rt_func(float period, volatile void * ctx_ptr, volatile hal_pin_inst_t * pin_ptr){
   struct enc_cmd_ctx_t * ctx = (struct enc_cmd_ctx_t *)ctx_ptr;
   struct enc_cmd_pin_ctx_t * pins = (struct enc_cmd_pin_ctx_t *)pin_ptr;
   
  int32_t tim = TIM_GetCounter(ctx->tim);

  PIN(a) = GPIO_ReadInputDataBit(ctx->a_port, ctx->a_pin);
  PIN(b) = GPIO_ReadInputDataBit(ctx->b_port, ctx->b_pin);
  
  float p = 0.0;
  p = mod(tim * 2.0f * M_PI / (float)ctx->e_res);
  
  PIN(pos) = p;
  
  int r = (int)PIN(res);
  if(r < 1){
       r = 1;
  }
  
  if(ctx->e_res != r){
    ctx->e_res = r;
    TIM_SetAutoreload(ctx->tim, ctx->e_res - 1);
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
