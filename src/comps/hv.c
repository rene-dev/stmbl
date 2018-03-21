#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"
#include "stm32f4xx_conf.h"
#include "hw/hw.h"
#include "common.h"
#include "main.h"
#include "ringbuf.h"

HAL_COMP(hv);

//process data from LS
HAL_PIN(d_cmd);
HAL_PIN(q_cmd);
HAL_PIN(pos);
HAL_PIN(vel);
HAL_PIN(en);

// config data from LS
HAL_PIN(phase_mode);
HAL_PIN(cmd_mode);
HAL_PIN(r);
HAL_PIN(l);
HAL_PIN(psi);
HAL_PIN(cur_p);
HAL_PIN(cur_i);
HAL_PIN(cur_ff);
HAL_PIN(cur_ind);
HAL_PIN(max_y);
HAL_PIN(max_cur);

// process data to LS
HAL_PIN(dc_volt);
HAL_PIN(d_fb);
HAL_PIN(q_fb);
HAL_PIN(abs_cur);

// state data to LS
HAL_PIN(hv_temp);
HAL_PIN(mot_temp);
HAL_PIN(core_temp);
HAL_PIN(fault);
HAL_PIN(y);
HAL_PIN(u_fb);
HAL_PIN(v_fb);
HAL_PIN(w_fb);

// misc
HAL_PIN(rev);
HAL_PIN(com_error);
HAL_PIN(pwm_volt);
HAL_PIN(uart_sr);
HAL_PIN(uart_dr);
HAL_PIN(crc_error);
HAL_PIN(timeout);
HAL_PIN(scale);

struct hv_ctx_t {
  volatile packet_to_hv_t packet_to_hv;
  volatile packet_from_hv_t packet_from_hv;
  f3_config_data_t config;
  f3_state_data_t state;
  uint16_t addr;
  uint16_t timeout;
  uint8_t frt_slot;
};

struct ringbuf hv_rx_buf = RINGBUF(128);
struct ringbuf hv_tx_buf = RINGBUF(128);

void hv_send(char *ptr) {
  if(ptr){//TODO: check connection status
    rb_write(&hv_tx_buf, ptr, strlen(ptr));
    rb_write(&hv_tx_buf, "\n", 1);
  }
}
COMMAND("hv", hv_send, "send command to hv board");

static void nrt_init(volatile void *ctx_ptr, volatile hal_pin_inst_t *pin_ptr) {
  struct hv_ctx_t *ctx      = (struct hv_ctx_t *)ctx_ptr;
  struct hv_pin_ctx_t *pins = (struct hv_pin_ctx_t *)pin_ptr;

  //setup uart to f1. uses DMA to transfer to_hv struct.
  GPIO_InitTypeDef GPIO_InitStruct;
  USART_InitTypeDef USART_InitStruct;
  DMA_InitTypeDef DMA_InitStructure;

  UART_DRV_CLOCK_COMMAND(UART_DRV_RCC, ENABLE);

  //USART TX
  GPIO_PinAFConfig(UART_DRV_TX_PORT, UART_DRV_TX_PIN_SOURCE, UART_DRV_TX_AF_SOURCE);
  GPIO_InitStruct.GPIO_Pin   = UART_DRV_TX_PIN;
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(UART_DRV_TX_PORT, &GPIO_InitStruct);

  //USART RX
  GPIO_PinAFConfig(UART_DRV_RX_PORT, UART_DRV_RX_PIN_SOURCE, UART_DRV_RX_AF_SOURCE);
  GPIO_InitStruct.GPIO_Pin = UART_DRV_RX_PIN;
  GPIO_Init(UART_DRV_RX_PORT, &GPIO_InitStruct);

  USART_OverSampling8Cmd(UART_DRV, ENABLE);
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
  DMA_InitStructure.DMA_Priority           = DMA_Priority_Medium;
  DMA_InitStructure.DMA_FIFOMode           = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold      = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst        = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;
  DMA_Init(UART_DRV_RX_DMA, &DMA_InitStructure);


  USART_DMACmd(UART_DRV, USART_DMAReq_Rx, ENABLE);
  DMA_Cmd(UART_DRV_RX_DMA, DISABLE);
  DMA_ClearFlag(UART_DRV_RX_DMA, UART_DRV_RX_DMA_TCIF);
  DMA_Cmd(UART_DRV_RX_DMA, ENABLE);

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);
  ctx->timeout = 0;
  PIN(timeout) = 0;
}

static void rt_func(float period, volatile void *ctx_ptr, volatile hal_pin_inst_t *pin_ptr) {
  struct hv_ctx_t *ctx      = (struct hv_ctx_t *)ctx_ptr;
  ctx->frt_slot = 0;
}

static void frt_func(float period, volatile void *ctx_ptr, volatile hal_pin_inst_t *pin_ptr) {
  struct hv_ctx_t *ctx      = (struct hv_ctx_t *)ctx_ptr;
  struct hv_pin_ctx_t *pins = (struct hv_pin_ctx_t *)pin_ptr;
  ctx->frt_slot++;
  if(ctx->frt_slot == 3){
    float e   = PIN(en);
    float pos = PIN(pos);
    float vel = PIN(vel);

    ctx->config.pins.r       = PIN(r);
    ctx->config.pins.l       = PIN(l);
    ctx->config.pins.psi     = PIN(psi);
    ctx->config.pins.cur_p   = PIN(cur_p);
    ctx->config.pins.cur_i   = PIN(cur_i);
    ctx->config.pins.cur_ff  = PIN(cur_ff);
    ctx->config.pins.cur_ind = PIN(cur_ind);
    ctx->config.pins.max_y   = PIN(max_y);
    ctx->config.pins.max_cur = PIN(max_cur) * PIN(scale);

    uint32_t dma_pos = DMA_GetCurrDataCounter(UART_DRV_RX_DMA);
    if(dma_pos == 0) {
      CRC_ResetDR();
      uint32_t crc = CRC_CalcBlockCRC((uint32_t *)&(ctx->packet_from_hv.header.slave_addr), sizeof(packet_from_hv_t) / 4 - 1);

      if(crc == ctx->packet_from_hv.header.crc) {
        PIN(d_fb)     = ctx->packet_from_hv.d_fb;
        PIN(q_fb)     = ctx->packet_from_hv.q_fb;
        PIN(fault)     = ctx->packet_from_hv.fault;

        PIN(abs_cur)  = sqrtf(PIN(d_fb) * PIN(d_fb) + PIN(q_fb) * PIN(q_fb));

        uint16_t a         = ctx->packet_from_hv.header.conf_addr;
        a                  = CLAMP(a, 0, sizeof(f3_state_data_t) / 4);
        ctx->state.data[a] = ctx->packet_from_hv.header.config.f32;

        PIN(dc_volt)  = ctx->state.pins.dc_volt;
        PIN(pwm_volt) = ctx->state.pins.pwm_volt;
        PIN(u_fb)      = ctx->state.pins.u_fb;
        PIN(v_fb)      = ctx->state.pins.v_fb;
        PIN(w_fb)      = ctx->state.pins.w_fb;
        PIN(hv_temp)   = ctx->state.pins.hv_temp;
        PIN(mot_temp)  = ctx->state.pins.mot_temp;
        PIN(core_temp) = ctx->state.pins.core_temp;
        PIN(y)         = ctx->state.pins.y;
        if(ctx->packet_from_hv.fault > 0.0) {
          PIN(com_error) = HV_FAULT_ERROR;
        } else {
          PIN(com_error) = 0.0;
        }
        ctx->timeout = 0;
        if(ctx->packet_from_hv.buf != 0xff){
          rb_write(&hv_rx_buf, (void*)&(ctx->packet_from_hv.buf), 1);
        }
      } else {
        PIN(crc_error)++;
        PIN(com_error) = HV_CRC_ERROR;
      }
    }

    if(ctx->timeout > 2) {
      PIN(timeout)++;
      PIN(com_error) = HV_TIMEOUT_ERROR;
    }
    ctx->timeout++;

    float d_cmd = PIN(d_cmd);
    float q_cmd = PIN(q_cmd);

    if(PIN(rev) > 0.0) {
      q_cmd *= -1.0;
      pos = minus(0, pos);
    }

    if(e > 0.0) {
      ctx->packet_to_hv.d_cmd        = d_cmd;
      ctx->packet_to_hv.q_cmd        = q_cmd;
      ctx->packet_to_hv.flags.enable = 1;
    } else {
      ctx->packet_to_hv.d_cmd        = 0.0;
      ctx->packet_to_hv.q_cmd        = 0.0;
      ctx->packet_to_hv.flags.enable = 0;
    }
    ctx->packet_to_hv.flags.cmd_type   = PIN(cmd_mode);
    ctx->packet_to_hv.flags.phase_type = PIN(phase_mode);
    ctx->packet_to_hv.pos              = pos;
    ctx->packet_to_hv.vel              = vel;

    ctx->packet_to_hv.header.slave_addr = 0;
    ctx->packet_to_hv.header.len = (sizeof(packet_to_hv_t) - sizeof(stmbl_talk_header_t)) / 4;
    ctx->packet_to_hv.header.conf_addr = ctx->addr;
    ctx->packet_to_hv.header.config.f32 = ctx->config.data[ctx->addr++];

    ctx->addr %= sizeof(f3_config_data_t) / 4;
    uint8_t buf[1];
    if(rb_read(&hv_tx_buf, buf, 1)){
      ctx->packet_to_hv.flags.buf = buf[0];
    }else{
      ctx->packet_to_hv.flags.buf = 0xff;
    }

    CRC_ResetDR();
    ctx->packet_to_hv.header.crc = CRC_CalcBlockCRC((uint32_t *)&(ctx->packet_to_hv.header.slave_addr), sizeof(packet_to_hv_t) / 4 - 1);

    PIN(uart_sr) = UART_DRV->SR;
    PIN(uart_dr) = UART_DRV->DR;
    //start DMA RX transfer
    DMA_Cmd(UART_DRV_RX_DMA, DISABLE);
    DMA_ClearFlag(UART_DRV_RX_DMA, UART_DRV_RX_DMA_TCIF);
    DMA_Cmd(UART_DRV_RX_DMA, ENABLE);

    //start DMA TX transfer
    DMA_Cmd(UART_DRV_TX_DMA, DISABLE);
    DMA_ClearFlag(UART_DRV_TX_DMA, UART_DRV_TX_DMA_TCIF);
    DMA_Cmd(UART_DRV_TX_DMA, ENABLE);
  }
  // PIN(power) = PIN(dc_cur) * PIN(dc_volt);
  // if(PIN(pwm_volt) > 0.0){
  //   PIN(dc_cur_sim) = ABS(PIN(iq)) / PIN(pwm_volt) * sqrtf(a*a + b*b)*0.5 + PIN(dc_cur_sim)*0.5;
  // }
  // if(ABS(a*b) > 0.01){
  //   PIN(ac_cur_sim) = PIN(dc_cur) / sqrtf(a*a+b*b) * PIN(pwm_volt);
  // }
}

static void nrt_func(volatile void *ctx_ptr, volatile hal_pin_inst_t *pin_ptr) {
  // struct hv_ctx_t *ctx      = (struct hv_ctx_t *)ctx_ptr;
  // struct hv_pin_ctx_t *pins = (struct hv_pin_ctx_t *)pin_ptr;
  char c;
  while(rb_getc(&hv_rx_buf, &c)){
    printf("%c",c);
  }
}

hal_comp_t hv_comp_struct = {
    .name      = "hv",
    .nrt       = nrt_func,
    .rt        = rt_func,
    .frt       = frt_func,
    .nrt_init  = nrt_init,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = sizeof(struct hv_ctx_t),
    .pin_count = sizeof(struct hv_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
