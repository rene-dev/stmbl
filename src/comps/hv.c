#include "hv_comp.h"
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
HAL_PIN(cur_bw);
HAL_PIN(cur_ff);
HAL_PIN(cur_ind);
HAL_PIN(max_y);
HAL_PIN(max_cur);
HAL_PIN(dac);

// process data to LS
HAL_PIN(dc_volt);
HAL_PIN(id_fb);
HAL_PIN(iq_fb);
HAL_PIN(ud_fb);
HAL_PIN(uq_fb);
HAL_PIN(abs_cur);
HAL_PIN(abs_volt);
HAL_PIN(duty);

// state data to LS
HAL_PIN(hv_temp);
HAL_PIN(mot_temp);
HAL_PIN(core_temp);
HAL_PIN(fault);  //fault from hv
HAL_PIN(ignore_fault_pin);
HAL_PIN(y);
HAL_PIN(u_fb);
HAL_PIN(v_fb);
HAL_PIN(w_fb);

// misc
HAL_PIN(rev);
HAL_PIN(pwm_volt);
HAL_PIN(uart_sr);
HAL_PIN(uart_dr);
HAL_PIN(crc_error);  //total number of crc errors, never reset
HAL_PIN(scale);

HAL_PIN(state);
HAL_PIN(value);

struct hv_ctx_t {
  union {
    volatile packet_to_hv_t packet_to_hv;
    volatile packet_bootloader_t packet_to_hv_bootloader;
  } to_hv;
  union {
    volatile packet_from_hv_t packet_from_hv;
    volatile packet_bootloader_t packet_from_hv_bootloader;
  } from_hv;
  f3_config_data_t config;
  f3_state_data_t state;
  uint32_t addr;
  uint16_t timeout;
  uint8_t conf_addr;
  uint8_t send_state;
};

typedef enum {
  SLAVE_IN_APP,
  SEND_TO_BOOTLOADER,
  ERASE_FLASH,
  SEND_APP,
  CRC_CHECK,
  SEND_TO_APP,
  FLASH_FAILED,
} flash_state_t;

flash_state_t flash_state;

uint32_t send_to_bootloader;

extern uint8_t _binary_obj_hvf3_hvf3_bin_start;
extern uint8_t _binary_obj_hvf3_hvf3_bin_size;
extern uint8_t _binary_obj_hvf3_hvf3_bin_end;

struct ringbuf hv_rx_buf = RINGBUF(128);
struct ringbuf hv_tx_buf = RINGBUF(128);

void hv_send(char *ptr) {
  if(ptr) {  //TODO: check connection status
    rb_write(&hv_tx_buf, ptr, strlen(ptr));
    rb_write(&hv_tx_buf, "\n", 1);
  }
}
COMMAND("hv", hv_send, "send command to hv board");

static void nrt_init(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
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
  DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t) & (ctx->to_hv.packet_to_hv);
  DMA_InitStructure.DMA_DIR                = DMA_DIR_MemoryToPeripheral;
  DMA_InitStructure.DMA_BufferSize         = MAX(sizeof(packet_to_hv_t), sizeof(packet_bootloader_t));
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
  DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t) & (ctx->from_hv.packet_from_hv);
  DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize         = MAX(sizeof(packet_from_hv_t), sizeof(packet_bootloader_t));
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
  ctx->timeout       = 0;
  PIN(dac)           = 1560;
  send_to_bootloader = 0;
  flash_state        = SLAVE_IN_APP;
  ctx->send_state = 0;
  PIN(ignore_fault_pin) = 1;
}

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct hv_ctx_t *ctx      = (struct hv_ctx_t *)ctx_ptr;
  struct hv_pin_ctx_t *pins = (struct hv_pin_ctx_t *)pin_ptr;
  float e   = PIN(en);
  float pos = PIN(pos);
  float vel = PIN(vel);

  ctx->config.pins.r       = PIN(r);
  ctx->config.pins.l       = PIN(l);
  ctx->config.pins.psi     = PIN(psi);
  ctx->config.pins.cur_bw  = PIN(cur_bw);
  ctx->config.pins.cur_ff  = PIN(cur_ff);
  ctx->config.pins.cur_ind = PIN(cur_ind);
  ctx->config.pins.max_y   = PIN(max_y);
  ctx->config.pins.max_cur = PIN(max_cur) * PIN(scale);
  ctx->config.pins.dac     = PIN(dac);

  uint32_t dma_count = MAX(sizeof(packet_from_hv_t), sizeof(packet_bootloader_t)) - DMA_GetCurrDataCounter(UART_DRV_RX_DMA);

  PIN(value) = 0.0;

  if(dma_count >= sizeof(stmbl_talk_header_t)) {
    // PIN(value) = 0.5;
    if(dma_count >= sizeof(stmbl_talk_header_t) + ctx->from_hv.packet_from_hv.header.len * 4) {
      PIN(value) = 0.75;

      CRC_ResetDR();
      uint32_t crc = CRC_CalcBlockCRC((uint32_t *)&(ctx->from_hv.packet_from_hv.header.slave_addr), sizeof(stmbl_talk_header_t) / 4 + ctx->from_hv.packet_from_hv.header.len - 1);
      if(ctx->from_hv.packet_from_hv.header.crc == crc) {
        switch(flash_state) {
          case SLAVE_IN_APP:
            if(ctx->from_hv.packet_from_hv.header.slave_addr == 0 && ctx->from_hv.packet_from_hv.header.len == (sizeof(packet_from_hv_t) - sizeof(stmbl_talk_header_t)) / 4) {
              // from f3 app
              PIN(id_fb)    = ctx->from_hv.packet_from_hv.id_fb;
              PIN(iq_fb)    = ctx->from_hv.packet_from_hv.iq_fb;
              PIN(ud_fb)    = ctx->from_hv.packet_from_hv.ud_fb;
              PIN(uq_fb)    = ctx->from_hv.packet_from_hv.uq_fb;
              if(PIN(rev) > 0.0) {
                PIN(uq_fb) *= -1.0;
                PIN(iq_fb) *= -1.0;
              }
              PIN(fault)   = ctx->from_hv.packet_from_hv.fault;
              PIN(abs_cur) = sqrtf(PIN(id_fb) * PIN(id_fb) + PIN(iq_fb) * PIN(iq_fb));
              PIN(abs_volt) = sqrtf(PIN(ud_fb) * PIN(ud_fb) + PIN(uq_fb) * PIN(uq_fb));
              if(PIN(pwm_volt) > 0.0){
                PIN(duty) = PIN(abs_volt) / PIN(pwm_volt);
              }

              uint16_t a         = ctx->from_hv.packet_from_hv.header.conf_addr;
              a                  = CLAMP(a, 0, sizeof(f3_state_data_t) / 4);
              ctx->state.data[a] = ctx->from_hv.packet_from_hv.header.config.f32;

              PIN(dc_volt)   = ctx->state.pins.dc_volt;
              PIN(pwm_volt)  = ctx->state.pins.pwm_volt;
              PIN(u_fb)      = ctx->state.pins.u_fb;
              PIN(v_fb)      = ctx->state.pins.v_fb;
              PIN(w_fb)      = ctx->state.pins.w_fb;
              PIN(hv_temp)   = ctx->state.pins.hv_temp;
              PIN(mot_temp)  = ctx->state.pins.mot_temp;
              PIN(core_temp) = ctx->state.pins.core_temp;
              PIN(y)         = ctx->state.pins.y;

              PIN(value) = 1.0;

              ctx->timeout = 0;

              if(ctx->from_hv.packet_from_hv.buf != 0x0) {
                rb_write(&hv_rx_buf, (void *)&(ctx->from_hv.packet_from_hv.buf), 1);
              }
            } else {
              // wrong packet len or slave addr
            }
            break;
          case SEND_TO_BOOTLOADER:

            break;
          case ERASE_FLASH:
            if(ctx->from_hv.packet_from_hv.header.slave_addr == 255 && ctx->from_hv.packet_from_hv.header.len == (sizeof(packet_bootloader_t) - sizeof(stmbl_talk_header_t)) / 4) {
              // from f3 bootloader
              if(ctx->from_hv.packet_from_hv_bootloader.state == BOOTLOADER_STATE_OK && ctx->from_hv.packet_from_hv_bootloader.cmd == BOOTLOADER_OPCODE_PAGEERASE) {
                ctx->timeout = 0;
                flash_state  = SEND_APP;
              }
            } else {
              // wrong packet len or slave addr
              PIN(value) = 3.0;
            }
            break;
          case SEND_APP:
            if(ctx->from_hv.packet_from_hv.header.slave_addr == 255 && ctx->from_hv.packet_from_hv.header.len == (sizeof(packet_bootloader_t) - sizeof(stmbl_talk_header_t)) / 4) {
              // from f3 bootloader
              if(ctx->from_hv.packet_from_hv_bootloader.state == BOOTLOADER_STATE_OK && ctx->from_hv.packet_from_hv_bootloader.cmd == BOOTLOADER_OPCODE_WRITE && ctx->from_hv.packet_from_hv_bootloader.addr == 0x08004000 + ctx->addr * 4 && ctx->from_hv.packet_from_hv_bootloader.value == ((uint32_t *)&(_binary_obj_hvf3_hvf3_bin_start))[ctx->addr]) {
                ctx->timeout = 0;
                ctx->addr++;
              }
              if(ctx->addr > ((uint32_t) & (_binary_obj_hvf3_hvf3_bin_size)) / 4) {
                flash_state = CRC_CHECK;
                // flash_state = SEND_TO_APP;
              }
            } else {
              // wrong packet len or slave addr
              PIN(value) = 3.0;
            }
            break;
          case CRC_CHECK:
            if(ctx->from_hv.packet_from_hv.header.slave_addr == 255 && ctx->from_hv.packet_from_hv.header.len == (sizeof(packet_bootloader_t) - sizeof(stmbl_talk_header_t)) / 4) {
              // from f3 bootloader
              if(ctx->from_hv.packet_from_hv_bootloader.state == BOOTLOADER_STATE_OK && ctx->from_hv.packet_from_hv_bootloader.cmd == BOOTLOADER_OPCODE_CRCCHECK) {
                ctx->timeout = 0;
                flash_state  = SEND_TO_APP;
              }
            } else {
              // wrong packet len or slave addr
              PIN(value) = 3.0;
            }
            break;
          case SEND_TO_APP:

            break;
          case FLASH_FAILED:

            break;
        }


      } else {
        // CRC fault
        PIN(crc_error)
        ++;
        // PIN(fault) = HV_CRC_ERROR;
        // PIN(value) = 4.0;
      }
    }
  }

  if(ctx->timeout > 2) {
    PIN(fault) = HV_TIMEOUT_ERROR;
  }
  ctx->timeout++;

  float d_cmd = PIN(d_cmd);
  float q_cmd = PIN(q_cmd);

  if(PIN(rev) > 0.0) {
    q_cmd *= -1.0;
    pos = minus(0, pos);
  }


  uint32_t tx_size = 0;

  switch(flash_state) {
    case SLAVE_IN_APP:
      if(e > 0.0) {
        ctx->to_hv.packet_to_hv.d_cmd        = d_cmd;
        ctx->to_hv.packet_to_hv.q_cmd        = q_cmd;
        ctx->to_hv.packet_to_hv.flags.enable = 1;
      } else {
        ctx->to_hv.packet_to_hv.d_cmd        = 0.0;
        ctx->to_hv.packet_to_hv.q_cmd        = 0.0;
        ctx->to_hv.packet_to_hv.flags.enable = 0;
      }
      ctx->to_hv.packet_to_hv.flags.ignore_fault_pin = PIN(ignore_fault_pin) > 0.0;
      ctx->to_hv.packet_to_hv.flags.cmd_type   = PIN(cmd_mode);
      ctx->to_hv.packet_to_hv.flags.phase_type = PIN(phase_mode);
      ctx->to_hv.packet_to_hv.pos              = pos;
      ctx->to_hv.packet_to_hv.vel              = vel;

      ctx->to_hv.packet_to_hv.header.slave_addr = 0;
      ctx->to_hv.packet_to_hv.header.flags.cmd  = WRITE_CONF;
      ctx->to_hv.packet_to_hv.header.flags.counter++;
      ctx->to_hv.packet_to_hv.header.len        = (sizeof(packet_to_hv_t) - sizeof(stmbl_talk_header_t)) / 4;
      ctx->to_hv.packet_to_hv.header.conf_addr  = ctx->conf_addr;
      ctx->to_hv.packet_to_hv.header.config.f32 = ctx->config.data[ctx->conf_addr++];

      uint8_t buf[1];
      if(rb_read(&hv_tx_buf, buf, 1)) {
        ctx->to_hv.packet_to_hv.flags.buf = buf[0];
      } else {
        ctx->to_hv.packet_to_hv.flags.buf = 0x0;
      }

      tx_size = sizeof(packet_to_hv_t);

      ctx->conf_addr %= sizeof(f3_config_data_t) / 4;

      if(send_to_bootloader) {
        send_to_bootloader = 0;
        flash_state        = SEND_TO_BOOTLOADER;
        ctx->timeout       = 0;
        // TODO: check f3 crc, size, ...
      }
      break;

    case SEND_TO_BOOTLOADER:  // fix
      ctx->to_hv.packet_to_hv.header.flags.cmd = BOOTLOADER;
      ctx->to_hv.packet_to_hv.flags.buf        = 0x0;
      ctx->to_hv.packet_to_hv.header.flags.counter++;
      ctx->to_hv.packet_to_hv.d_cmd        = 0.0;
      ctx->to_hv.packet_to_hv.q_cmd        = 0.0;
      ctx->to_hv.packet_to_hv.flags.enable = 0;

      tx_size = sizeof(packet_to_hv_t);

      if(ctx->timeout > 10) {
        ctx->timeout = 0;
        flash_state  = ERASE_FLASH;
      }
      break;

    case ERASE_FLASH:
      ctx->to_hv.packet_to_hv.header.slave_addr = 255;
      ctx->to_hv.packet_to_hv.header.flags.cmd  = NO_CMD;
      ctx->to_hv.packet_to_hv.header.flags.counter++;
      ctx->to_hv.packet_to_hv.header.len        = (sizeof(packet_bootloader_t) - sizeof(stmbl_talk_header_t)) / 4;
      ctx->to_hv.packet_to_hv.header.conf_addr  = 0;
      ctx->to_hv.packet_to_hv.header.config.f32 = 0;
      ctx->to_hv.packet_to_hv_bootloader.addr   = 0;
      ctx->to_hv.packet_to_hv_bootloader.value  = 0;
      ctx->to_hv.packet_to_hv_bootloader.cmd    = BOOTLOADER_OPCODE_PAGEERASE;

      tx_size = sizeof(packet_bootloader_t);

      ctx->addr = 0;

      // flash_state = SLAVE_IN_APP;

      if(ctx->timeout > 20000) {
        ctx->timeout = 0;
        flash_state  = FLASH_FAILED;
      }
      break;

    case SEND_APP:
      ctx->to_hv.packet_to_hv.header.flags.counter++;
      ctx->to_hv.packet_to_hv.header.len       = (sizeof(packet_bootloader_t) - sizeof(stmbl_talk_header_t)) / 4;
      ctx->to_hv.packet_to_hv_bootloader.addr  = 0x08004000 + ctx->addr * 4;
      ctx->to_hv.packet_to_hv_bootloader.value = ((uint32_t *)&_binary_obj_hvf3_hvf3_bin_start)[ctx->addr];
      ctx->to_hv.packet_to_hv_bootloader.cmd   = BOOTLOADER_OPCODE_WRITE;

      tx_size = sizeof(packet_bootloader_t);

      if(ctx->timeout > 10) {
        ctx->timeout = 0;
        flash_state  = FLASH_FAILED;
      }
      break;

    case CRC_CHECK:
      ctx->to_hv.packet_to_hv.header.flags.counter++;
      ctx->to_hv.packet_to_hv.header.len     = (sizeof(packet_bootloader_t) - sizeof(stmbl_talk_header_t)) / 4;
      ctx->to_hv.packet_to_hv_bootloader.cmd = BOOTLOADER_OPCODE_CRCCHECK;

      tx_size = sizeof(packet_bootloader_t);

      if(ctx->timeout > 2000) {
        ctx->timeout = 0;
        flash_state  = FLASH_FAILED;
      }
      break;
    case SEND_TO_APP:
      ctx->to_hv.packet_to_hv.header.flags.cmd = DO_RESET;
      ctx->to_hv.packet_to_hv.header.flags.counter++;
      ctx->to_hv.packet_to_hv.header.len     = (sizeof(packet_bootloader_t) - sizeof(stmbl_talk_header_t)) / 4;
      ctx->to_hv.packet_to_hv_bootloader.cmd = BOOTLOADER_OPCODE_NOP;

      tx_size = sizeof(packet_bootloader_t);

      if(ctx->timeout > 2000) {
        ctx->timeout = 0;
        flash_state  = SLAVE_IN_APP;
      }
      break;
    case FLASH_FAILED:
      if(ctx->timeout > 10) {
        ctx->timeout = 0;
        flash_state  = SLAVE_IN_APP;
      }
      break;
  }

  if(ctx->send_state > 1){
    if(flash_state != SLAVE_IN_APP){
      tx_size = 0;
    }
    ctx->send_state = 0;
  }
  ctx->send_state++;

  if(tx_size) {
    CRC_ResetDR();
    ctx->to_hv.packet_to_hv.header.crc = CRC_CalcBlockCRC((uint32_t *)&(ctx->to_hv.packet_to_hv.header.slave_addr), tx_size / 4 - 1);

    //start DMA TX transfer
    DMA_Cmd(UART_DRV_TX_DMA, DISABLE);
    DMA_ClearFlag(UART_DRV_TX_DMA, UART_DRV_TX_DMA_TCIF);
    UART_DRV_TX_DMA->NDTR = tx_size;
    DMA_Cmd(UART_DRV_TX_DMA, ENABLE);

    // clear uart faults
    PIN(uart_sr) = UART_DRV->SR;
    PIN(uart_dr) = UART_DRV->DR;

    //start DMA RX transfer
    DMA_Cmd(UART_DRV_RX_DMA, DISABLE);
    DMA_ClearFlag(UART_DRV_RX_DMA, UART_DRV_RX_DMA_TCIF);
    DMA_Cmd(UART_DRV_RX_DMA, ENABLE);
  }


  PIN(state) = flash_state;
}

void send_boot(char *ptr) {
  send_to_bootloader = 1;
}
COMMAND("hv_update", send_boot, "try hv update");

static void nrt_func(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct hv_ctx_t *ctx = (struct hv_ctx_t *)ctx_ptr;
  // struct hv_pin_ctx_t *pins = (struct hv_pin_ctx_t *)pin_ptr;
  char c;
  while(rb_getc(&hv_rx_buf, &c)) {
    printf("%c", c);
  }

  static flash_state_t last_flash_state = SLAVE_IN_APP;
  static uint32_t last_addr             = 0;

  if(ctx->addr >= last_addr + 1024) {
    printf("hv_update: status: %i%%\n", (int)(100.0 * ctx->addr * 4. / (float)((uint32_t) & (_binary_obj_hvf3_hvf3_bin_size))));
    last_addr = ctx->addr;
  }

  if(last_flash_state != flash_state) {
    switch(flash_state) {
      case SLAVE_IN_APP:
        printf("hv_update: SLAVE_IN_APP\n");
        break;
      case SEND_TO_BOOTLOADER:
        printf("hv_update: SEND_TO_BOOTLOADER\n");
        last_addr = 0;
        break;
      case ERASE_FLASH:
        printf("hv_update: ERASE_FLASH\n");
        last_addr = 0;
        break;
      case SEND_APP:
        printf("hv_update: SEND_APP\n");
        break;
      case CRC_CHECK:
        printf("hv_update: CRC_CHECK\n");
        last_addr = 0;
        break;
      case SEND_TO_APP:
        printf("hv_update: SEND_TO_APP\n");
        last_addr = 0;
        break;
      case FLASH_FAILED:
        printf("hv_update: FLASH_FAILED\n");
        last_addr = 0;
        break;
    }
    last_flash_state = flash_state;
  }
}

hal_comp_t hv_comp_struct = {
    .name      = "hv",
    .nrt       = nrt_func,
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
