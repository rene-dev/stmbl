#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"
#include "stm32f4xx_conf.h"
#include "hw/hw.h"

HAL_COMP(smartabs);

HAL_PIN(pos);
HAL_PIN(error);
HAL_PIN(state);
HAL_PIN(dma);  //dma transfers left
HAL_PIN(a0);
HAL_PIN(a1);
HAL_PIN(a2);
HAL_PIN(a3);
HAL_PIN(a4);
HAL_PIN(a5);
//TODO: state, error

struct smartabs_ctx_t {
  uint32_t error;
  uint8_t rxbuf[15];
};

static void hw_init(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct smartabs_ctx_t *ctx = (struct smartabs_ctx_t *)ctx_ptr;
  // struct smartabs_pin_ctx_t * pins = (struct smartabs_pin_ctx_t *)pin_ptr;
  GPIO_InitTypeDef GPIO_InitStruct;
  USART_InitTypeDef USART_InitStruct;
  DMA_InitTypeDef DMA_InitStructure;

  //TX enable
  GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_15;
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStruct);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);

  //USART TX
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);
  GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_6;
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(GPIOC, &GPIO_InitStruct);

  //USART RX
  // GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART6);
  // GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
  // GPIO_Init(GPIOB, &GPIO_InitStruct);

  USART_InitStruct.USART_BaudRate            = 2500000;
  USART_InitStruct.USART_WordLength          = USART_WordLength_8b;
  USART_InitStruct.USART_StopBits            = USART_StopBits_1;
  USART_InitStruct.USART_Parity              = USART_Parity_No;
  USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStruct.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART6, &USART_InitStruct);

  /* Enable the USART */
  USART_Cmd(USART6, ENABLE);
  USART_HalfDuplexCmd(USART6, ENABLE);

  // DMA-Disable
  DMA_Cmd(DMA2_Stream1, DISABLE);
  DMA_DeInit(DMA2_Stream1);

  // DMA2-Config
  DMA_InitStructure.DMA_Channel            = DMA_Channel_5;
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (USART6->DR);
  DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)&ctx->rxbuf;
  DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize         = ARRAY_SIZE(ctx->rxbuf);
  DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize     = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority           = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode           = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold      = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst        = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream1, &DMA_InitStructure);

  USART_DMACmd(USART6, USART_DMAReq_Rx, ENABLE);
}

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct smartabs_ctx_t *ctx      = (struct smartabs_ctx_t *)ctx_ptr;
  struct smartabs_pin_ctx_t *pins = (struct smartabs_pin_ctx_t *)pin_ptr;
  PIN(dma)                    = DMA_GetCurrDataCounter(DMA2_Stream1);
  if((sizeof(ctx->rxbuf) - DMA_GetCurrDataCounter(DMA2_Stream1)) == 6) {
    PIN(a0) = ctx->rxbuf[0];
    PIN(a1) = ctx->rxbuf[1];
    PIN(a2) = ctx->rxbuf[2];
    PIN(a3) = ctx->rxbuf[3];
    PIN(a4) = ctx->rxbuf[4];
    PIN(a5) = ctx->rxbuf[5];
    uint32_t tpos = ((ctx->rxbuf[4] & 0x01) << 16) + (ctx->rxbuf[3] << 8) + ctx->rxbuf[2];
    PIN(pos) = (tpos * M_PI * 2.0 / 131072.0) - M_PI;
    PIN(state) = 3;
    PIN(error) = 0;
  } else {
    ctx->error++;  //TODO: overflow...
    PIN(error) = ctx->error;
    PIN(state) = 0.0;
  }

  //TODO: irq here will cause problems
  __disable_irq();
  GPIO_SetBits(GPIOD, GPIO_Pin_15);  //tx enable
  USART_SendData(USART6, 0x02);
  while(USART_GetFlagStatus(USART6, USART_FLAG_TC) == RESET)
    ;
  GPIO_ResetBits(GPIOD, GPIO_Pin_15);  //tx disable
  //start rx dma
  DMA_Cmd(DMA2_Stream1, DISABLE);
  DMA_ClearFlag(DMA2_Stream1, DMA_FLAG_TCIF1);
  DMA_Cmd(DMA2_Stream1, ENABLE);
  __enable_irq();
}

hal_comp_t smartabs_comp_struct = {
    .name      = "smartabs",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = 0,
    .hw_init   = hw_init,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = sizeof(struct smartabs_ctx_t),
    .pin_count = sizeof(struct smartabs_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
