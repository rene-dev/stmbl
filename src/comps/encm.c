#include "encm_comp.h"
#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"
#include "stm32f4xx_conf.h"
#include "hw/hw.h"

HAL_COMP(encm);

HAL_PIN(pos);
HAL_PIN(error);
HAL_PIN(cmd_error);
HAL_PIN(crc_error);
HAL_PIN(dma_error);
HAL_PIN(state);

HAL_PIN(cmd);
HAL_PIN(req);
HAL_PIN(full_duplex);
HAL_PIN(bytes);
HAL_PIN(id);

HAL_PINA(buf, 15);


struct encm_ctx_t {
  uint32_t error;
  uint8_t rxbuf[15];
};

static void hw_init(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct encm_ctx_t *ctx = (struct encm_ctx_t *)ctx_ptr;
  struct encm_pin_ctx_t * pins = (struct encm_pin_ctx_t *)pin_ptr;
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

  USART_InitStruct.USART_BaudRate            = 2500000;
  USART_InitStruct.USART_WordLength          = USART_WordLength_8b;
  USART_InitStruct.USART_StopBits            = USART_StopBits_1;
  USART_InitStruct.USART_Parity              = USART_Parity_No;
  USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStruct.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART6, &USART_InitStruct);

  /* Enable the USART */
  USART_Cmd(USART6, ENABLE);

  if(PIN(full_duplex) > 0.0){
    //USART RX
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_7;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOC, &GPIO_InitStruct);
  }
  else{
    USART_HalfDuplexCmd(USART6, ENABLE);
  }

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
  struct encm_ctx_t *ctx      = (struct encm_ctx_t *)ctx_ptr;
  struct encm_pin_ctx_t *pins = (struct encm_pin_ctx_t *)pin_ptr;
  // for(int i = 0; i < ARRAY_SIZE(ctx->rxbuf); i++){
  //    PIN_ARRAY(d,i) = ctx->rxbuf[i];
  // }
  //position request: 0x32
  //reply:
  //0: request
  //1: unknown
  //2: low 1 bit
  //3: mid 8 bit
  //4: hi 8 bit
  //5: 8 bit full turns
  //6: 255 turns... how many bits?
  //7: unknown
  //8: checksum: xor byte 0-8 = 0

  //request: 0x02
  //0: request
  //1: unknown
  //2: low 8 bit
  //3: mid 8 bit 
  //4: hi 1 + 7 bit mt
  //5: crc

  //request: 0x92
  //0: request
  //1: unknown
  //2: id
  //3: crc or id ...

  uint8_t cmd = 0;
  uint8_t expected_bytes = 0;

  if(PIN(cmd) > 0){ // cmd overwrite
    cmd = PIN(cmd);
  }
  else{
    switch((int) PIN(id)){
      case 0: // no id
        cmd = 0x92; // request id
        expected_bytes = 0;
      break;

      case 32:
        cmd = 0x2; // request singel turn data
        expected_bytes = 7;
      break;

      case 61:
      case 65:
        cmd = 0x32; // request singel turn data
        expected_bytes = 9;
      break;
      
      default:
        cmd = 0x32;
        expected_bytes = 9;
      break;
    }
  }

  PIN(req) = cmd;

  PIN(error) = 0;
  PIN(crc_error) = 0;
  PIN(dma_error) = 0;
  PIN(cmd_error) = 0;  
  PIN(state) = 0;

  // number of received bytes
  uint8_t bytes = sizeof(ctx->rxbuf) - DMA_GetCurrDataCounter(DMA2_Stream1);
  PIN(bytes) = bytes;

  // check crc (xor all == 0)
  uint8_t crc = 0;
  for(int i = 0; i < bytes; i++){
    crc = crc ^ ctx->rxbuf[i];
    PINA(buf, i) = ctx->rxbuf[i];
  }
  
  if(crc){
    PIN(error) = 1;
    PIN(crc_error) = 1;
  }
  else{
    // check #bytes
    if(bytes < 3 || ((expected_bytes > 0) & (bytes != expected_bytes))){
      PIN(error) = 1;
      PIN(dma_error) = 1;
    }
    else{
      // check cmd
      if(ctx->rxbuf[0] != cmd){
        PIN(error) = 1;
        PIN(cmd_error) = 1;
      }
      else{
        uint32_t ipos = 0;
        switch(cmd){
          case 0x2: // single turn data
            ipos = (ctx->rxbuf[2] << 11) + ((ctx->rxbuf[3] & 0x1f) << 19); // 13 bit
            PIN(state) = 3;
          break;

          case 0x32: // single turn data
            ipos = (ctx->rxbuf[2] & 0x80) + (ctx->rxbuf[3] << 8) + (ctx->rxbuf[4] << 16); // 17 bit
            PIN(state) = 3;
          break;

          case 0x92: // encoder id
            PIN(id) = ctx->rxbuf[2];
          break;
        }
        PIN(pos) = (ipos * M_PI * 2.0 / 16777216.0) - M_PI;
      }
    }
  }


  //TODO: irq here will cause problems
  GPIO_SetBits(GPIOD, GPIO_Pin_15);  //tx enable
  USART_SendData(USART6, cmd);
  while(USART_GetFlagStatus(USART6, USART_FLAG_TC) == RESET)
    ;
  GPIO_ResetBits(GPIOD, GPIO_Pin_15);  //tx disable
  //start rx dma
  DMA_Cmd(DMA2_Stream1, DISABLE);
  DMA_ClearFlag(DMA2_Stream1, DMA_FLAG_TCIF1);
  DMA_Cmd(DMA2_Stream1, ENABLE);
}

hal_comp_t encm_comp_struct = {
    .name      = "encm",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = 0,
    .hw_init   = hw_init,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = sizeof(struct encm_ctx_t),
    .pin_count = sizeof(struct encm_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
