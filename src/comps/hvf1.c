#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"
#include "stm32f4xx_conf.h"
#include "hw/hw.h"
#include "common_f1.h"


HAL_COMP(hv);

HAL_PIN(a);
HAL_PIN(b);
HAL_PIN(mode);

HAL_PIN(enable);
HAL_PIN(error);
HAL_PIN(over_cur);
HAL_PIN(over_temp);
HAL_PIN(hv_fault);

HAL_PIN(dc_cur);
HAL_PIN(dc_volt);
HAL_PIN(pwm_volt);
HAL_PIN(hv_temp);
HAL_PIN(power);
HAL_PIN(dc_cur_sim);
HAL_PIN(ac_cur_sim);
HAL_PIN(iq);

HAL_PIN(rev);

struct hv_ctx_t {
  volatile packet_to_hv_t packet_to_hv;
  volatile packet_from_hv_t packet_from_hv;
};

static void nrt_init(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct hv_ctx_t *ctx = (struct hv_ctx_t *)ctx_ptr;
  // struct hv_pin_ctx_t *pins = (struct hv_pin_ctx_t *)pin_ptr;

  //setup uart to f1. uses DMA to transfer to_hv struct.
  GPIO_InitTypeDef GPIO_InitStruct;
  USART_InitTypeDef USART_InitStruct;
  UART_DRV_CLOCK_COMMAND(UART_DRV_RCC, ENABLE);

  //USART TX
  GPIO_PinAFConfig(UART_DRV_TX_PORT, UART_DRV_TX_PIN_SOURCE, UART_DRV_TX_AF_SOURCE);
  GPIO_InitStruct.GPIO_Pin   = UART_DRV_TX_PIN;
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(UART_DRV_TX_PORT, &GPIO_InitStruct);

  //USART RX
  GPIO_PinAFConfig(UART_DRV_RX_PORT, UART_DRV_RX_PIN_SOURCE, UART_DRV_RX_AF_SOURCE);
  GPIO_InitStruct.GPIO_Pin = UART_DRV_RX_PIN;
  GPIO_Init(UART_DRV_RX_PORT, &GPIO_InitStruct);

  USART_InitStruct.USART_BaudRate            = DATABAUD;
  USART_InitStruct.USART_WordLength          = USART_WordLength_8b;
  USART_InitStruct.USART_StopBits            = USART_StopBits_1;
  USART_InitStruct.USART_Parity              = USART_Parity_No;
  USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStruct.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(UART_DRV, &USART_InitStruct);

  /* Enable the USART */
  USART_Cmd(UART_DRV, ENABLE);

  // DMA-Disable
  DMA_Cmd(UART_DRV_TX_DMA, DISABLE);
  DMA_DeInit(UART_DRV_TX_DMA);

  // DMA2-Config
  DMA_InitTypeDef DMA_InitStructure;
  DMA_InitStructure.DMA_Channel            = UART_DRV_TX_DMA_CHAN;
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (UART_DRV->DR);
  DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t) & (ctx->packet_to_hv);
  DMA_InitStructure.DMA_DIR                = DMA_DIR_MemoryToPeripheral;
  DMA_InitStructure.DMA_BufferSize         = sizeof(packet_to_hv_t);
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
  DMA_Init(UART_DRV_TX_DMA, &DMA_InitStructure);

  //DMA_Cmd(UART_DRV_TX_DMA, ENABLE);

  USART_DMACmd(UART_DRV, USART_DMAReq_Tx, ENABLE);


  // DMA-Disable
  DMA_Cmd(UART_DRV_RX_DMA, DISABLE);
  DMA_DeInit(UART_DRV_RX_DMA);

  // DMA2-Config
  DMA_InitStructure.DMA_Channel            = UART_DRV_RX_DMA_CHAN;
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (UART_DRV->DR);
  DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t) & (ctx->packet_from_hv);
  DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize         = sizeof(packet_from_hv_t);
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
  DMA_Init(UART_DRV_RX_DMA, &DMA_InitStructure);


  USART_DMACmd(UART_DRV, USART_DMAReq_Rx, ENABLE);

  ctx->packet_to_hv.head.start   = 255;
  ctx->packet_to_hv.head.key     = 0;
  ctx->packet_from_hv.head.start = 0;
  ctx->packet_from_hv.head.key   = 0;
}

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct hv_ctx_t *ctx      = (struct hv_ctx_t *)ctx_ptr;
  struct hv_pin_ctx_t *pins = (struct hv_pin_ctx_t *)pin_ptr;

  float e = PIN(enable);

  if(ctx->packet_from_hv.head.start == 255) {
    unbuff_packet((packet_header_t *)&(ctx->packet_from_hv), sizeof(from_hv_t));
    ctx->packet_from_hv.head.start = 0;
    PIN(dc_cur)                    = TOFLOAT(ctx->packet_from_hv.data.dc_cur) * 0.5 + PIN(dc_cur) * 0.5;
    PIN(dc_volt)                   = TOFLOAT(ctx->packet_from_hv.data.dc_volt);
    if(PIN(mode) == 0) {  //AC
      PIN(pwm_volt) = PIN(dc_volt) / 2.0 * 0.95 * 1.15;
    } else if(PIN(mode) == 1) {  //DC
      PIN(pwm_volt) = PIN(dc_volt) * 0.95;
    } else if(PIN(mode) == 2) {  //2phase ac
      PIN(pwm_volt) = PIN(dc_volt) * 0.7 * 0.95;
    } else {
      PIN(pwm_volt) = 0.0;
    }
    PIN(hv_temp)   = TOFLOAT(ctx->packet_from_hv.data.hv_temp);
    PIN(over_cur)  = ctx->packet_from_hv.data.over_cur;   //hardware cur limit
    PIN(over_temp) = ctx->packet_from_hv.data.over_temp;  //hardware temp limit
    PIN(hv_fault)  = ctx->packet_from_hv.data.hv_fault;   //iramx fault

    PIN(error) = 0.0;  //TODO: link to fault
  } else {
    PIN(error) = 1.0;
  }

  float a = PIN(a);
  float b = PIN(b);

  if(PIN(rev) > 0.0) {  //TODO: rev DC
    b *= -1.0;
  }

  ctx->packet_to_hv.data.mode = CLAMP(PIN(mode), 0, 16);
  if(e > 0.0) {
    ctx->packet_to_hv.data.a      = a;
    ctx->packet_to_hv.data.b      = b;
    ctx->packet_to_hv.data.enable = 1;
  } else {
    ctx->packet_to_hv.data.a      = 0;
    ctx->packet_to_hv.data.b      = 0;
    ctx->packet_to_hv.data.enable = 0;
  }
  buff_packet((packet_header_t *)&(ctx->packet_to_hv), sizeof(to_hv_t));

  //start DMA TX transfer
  DMA_Cmd(UART_DRV_TX_DMA, DISABLE);
  DMA_ClearFlag(UART_DRV_TX_DMA, UART_DRV_TX_DMA_TCIF);
  DMA_Cmd(UART_DRV_TX_DMA, ENABLE);

  //start DMA RX transfer
  DMA_Cmd(UART_DRV_RX_DMA, DISABLE);
  DMA_ClearFlag(UART_DRV_RX_DMA, UART_DRV_RX_DMA_TCIF);
  DMA_Cmd(UART_DRV_RX_DMA, ENABLE);

  PIN(power) = PIN(dc_cur) * PIN(dc_volt);
  if(PIN(pwm_volt) > 0.0) {
    PIN(dc_cur_sim) = ABS(PIN(iq)) / PIN(pwm_volt) * sqrtf(a * a + b * b) * 0.5 + PIN(dc_cur_sim) * 0.5;
  }
  if(ABS(a * b) > 0.01) {
    PIN(ac_cur_sim) = PIN(dc_cur) / sqrtf(a * a + b * b) * PIN(pwm_volt);
  }
}


hal_comp_t hv_comp_struct = {
    .name      = "hv",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = nrt_init,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = sizeof(struct hv_ctx_t),
    .pin_count = sizeof(struct hv_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};