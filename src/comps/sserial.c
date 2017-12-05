/*
* This file is part of the stmbl project.
*
* Copyright (C) 2013 Rene Hopf <renehopf@mac.com>
* Copyright (C) 2015 Ian McMahon <facetious.ian@gmail.com>
* Copyright (C) 2013 Nico Stute <crinq@crinq.de>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "stm32f4xx_conf.h"
#include "hw/hw.h"
#include "sserial.h"
#include "crc8.h"
#include "setup.h"
#include <string.h>

HAL_COMP(sserial);

// pins
HAL_PIN(error);
HAL_PIN(crc_error);  //counts crc errors, is never reset
HAL_PIN(connected);  //connection status TODO: not stable during startup, needs link to pd
HAL_PIN(timeout);  // 20khz / 1khz * 2 reads = 40

HAL_PIN(pos_cmd);
HAL_PIN(pos_cmd_d);
HAL_PIN(pos_fb);
HAL_PIN(vel_fb);
HAL_PIN(current);

HAL_PIN(in0);
HAL_PIN(in1);
HAL_PIN(in2);
HAL_PIN(in3);
HAL_PIN(fault);

HAL_PIN(out0);
HAL_PIN(out1);
HAL_PIN(out2);
HAL_PIN(out3);
HAL_PIN(enable);
HAL_PIN(index_clear);
HAL_PIN(index_out);
HAL_PIN(pos_advance);

//TODO: move to ctx
struct sserial_ctx_t {
  uint32_t foo;
};

volatile uint8_t rxbuf[128];
volatile uint8_t txbuf[128];
uint16_t address;  //current address pointer
int rxpos;
discovery_rpc_t discovery;
uint32_t timeout;
lbp_t lbp;
char name[] = LBPCardName;
int bufferpos;
int available;
unit_no_t unit;
uint32_t max_waste_ticks;
uint32_t block_bytes;

#pragma pack(1)
//*****************************************************************************
uint8_t sserial_slave[] = {
    0x0B, 0x09, 0x7F, 0x01, 0x99, 0x01, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xA0, 0x20, 0x10, 0x80,
    0x00, 0x00, 0x80, 0xFF, 0x00, 0x00, 0x80, 0x7F,
    0x08, 0x00, 0x72, 0x61, 0x64, 0x00, 0x70, 0x6F,
    0x73, 0x5F, 0x63, 0x6D, 0x64, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xA0, 0x20, 0x10, 0x80,
    0x00, 0x00, 0x80, 0xFF, 0x00, 0x00, 0x80, 0x7F,
    0x26, 0x00, 0x72, 0x61, 0x64, 0x00, 0x76, 0x65,
    0x6C, 0x5F, 0x63, 0x6D, 0x64, 0x00, 0x00, 0x00,
    0xA0, 0x04, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x80, 0x3F, 0x46, 0x00, 0x6E, 0x6F,
    0x6E, 0x65, 0x00, 0x6F, 0x75, 0x74, 0x70, 0x75,
    0x74, 0x5F, 0x70, 0x69, 0x6E, 0x73, 0x00, 0x00,
    0xA0, 0x01, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x80, 0x3F, 0x67, 0x00, 0x6E, 0x6F,
    0x6E, 0x65, 0x00, 0x65, 0x6E, 0x61, 0x62, 0x6C,
    0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xA0, 0x20, 0x10, 0x00, 0x00, 0x00, 0x80, 0xFF,
    0x00, 0x00, 0x80, 0x7F, 0x82, 0x00, 0x72, 0x61,
    0x64, 0x00, 0x70, 0x6F, 0x73, 0x5F, 0x66, 0x62,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xA0, 0x20, 0x10, 0x00, 0x00, 0x00, 0x80, 0xFF,
    0x00, 0x00, 0x80, 0x7F, 0xA1, 0x00, 0x72, 0x61,
    0x64, 0x00, 0x76, 0x65, 0x6C, 0x5F, 0x66, 0x62,
    0x00, 0x00, 0x00, 0x00, 0xA0, 0x08, 0x03, 0x00,
    0x00, 0x00, 0xF0, 0xC1, 0x00, 0x00, 0xF0, 0x41,
    0xC1, 0x00, 0x41, 0x00, 0x63, 0x75, 0x72, 0x72,
    0x65, 0x6E, 0x74, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xA0, 0x04, 0x01, 0x00, 0x00, 0x00, 0xC8, 0xC2,
    0x00, 0x00, 0xC8, 0x42, 0xDC, 0x00, 0x6E, 0x6F,
    0x6E, 0x65, 0x00, 0x69, 0x6E, 0x70, 0x75, 0x74,
    0x5F, 0x70, 0x69, 0x6E, 0x73, 0x00, 0x00, 0x00,
    0xA0, 0x01, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x80, 0x3F, 0xFE, 0x00, 0x6E, 0x6F,
    0x6E, 0x65, 0x00, 0x66, 0x61, 0x75, 0x6C, 0x74,
    0x00, 0x00, 0x00, 0x00, 0xA0, 0x01, 0x07, 0x40,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3F,
    0x19, 0x01, 0x6E, 0x6F, 0x6E, 0x65, 0x00, 0x69,
    0x6E, 0x64, 0x65, 0x78, 0x5F, 0x65, 0x6E, 0x61,
    0x62, 0x6C, 0x65, 0x00, 0xB0, 0x00, 0x01, 0x00,
    0x50, 0x6F, 0x73, 0x69, 0x74, 0x69, 0x6F, 0x6E,
    0x20, 0x6D, 0x6F, 0x64, 0x65, 0x00, 0x00, 0x00,
    0xA0, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x4E, 0x01, 0x00, 0x70,
    0x61, 0x64, 0x64, 0x69, 0x6E, 0x67, 0x00, 0x00,
    0xA0, 0x02, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x67, 0x01, 0x00, 0x70,
    0x61, 0x64, 0x64, 0x69, 0x6E, 0x67, 0x00, 0x0C,
    0x00, 0x2C, 0x00, 0x48, 0x00, 0x68, 0x00, 0x88,
    0x00, 0xA8, 0x00, 0xC4, 0x00, 0xE0, 0x00, 0x00,
    0x01, 0x1C, 0x01, 0x50, 0x01, 0x68, 0x01, 0x00,
    0x00, 0x3C, 0x01, 0x00, 0x00,
};
uint16_t sserial_ptocp = 0x017F;
uint16_t sserial_gtocp = 0x0199;

typedef struct {
  float pos_cmd;
  float vel_cmd;
  uint32_t output_pins_0 : 1;
  uint32_t output_pins_1 : 1;
  uint32_t output_pins_2 : 1;
  uint32_t output_pins_3 : 1;
  uint32_t enable : 1;
  uint32_t index_enable : 1;
  uint32_t padding : 2;
} sserial_out_process_data_t;  //size:9 bytes
_Static_assert(sizeof(sserial_out_process_data_t) == 9, "sserial_out_process_data_t size error!");

typedef struct {
  float pos_fb;
  float vel_fb;
  int8_t current;
  uint32_t input_pins_0 : 1;
  uint32_t input_pins_1 : 1;
  uint32_t input_pins_2 : 1;
  uint32_t input_pins_3 : 1;
  uint32_t fault : 1;
  uint32_t index_enable : 1;
  uint32_t padding : 2;
} sserial_in_process_data_t;  //size:10 bytes
_Static_assert(sizeof(sserial_in_process_data_t) == 10, "sserial_in_process_data_t size error!");
//******************************************************************************
sserial_out_process_data_t data_out;
sserial_in_process_data_t data_in;

uint8_t crc_reuest(uint8_t len) {
  uint8_t crc = crc8_init();
  for(int i = rxpos; i < rxpos + len; i++) {
    crc = crc8_update(crc, (void *)&(rxbuf[i % sizeof(rxbuf)]), 1);
  }
  crc = crc8_finalize(crc);
  return crc == rxbuf[(rxpos + len) % sizeof(rxbuf)];
}

uint8_t crc8(uint8_t *addr, uint8_t len) {
  uint8_t crc = crc8_init();
  crc         = crc8_update(crc, addr, len);
  return crc8_finalize(crc);
}

void send(uint8_t len, uint8_t docrc) {
  timeout = 0;
  if(docrc) {
    txbuf[len] = crc8((uint8_t *)txbuf, len);
    DMA_SetCurrDataCounter(DMA1_Stream4, len + 1);
  } else {
    DMA_SetCurrDataCounter(DMA1_Stream4, len);
  }
  DMA_Cmd(DMA1_Stream4, DISABLE);
  DMA_ClearFlag(DMA1_Stream4, DMA_FLAG_TCIF4);
  DMA_Cmd(DMA1_Stream4, ENABLE);
}

//v3
//pb13 txen
//pc12 usart5 tx
//pa9 usart1 tx as rx
//USART5 TX DMA1 stream7 channel4
//USART1 RX DMA2 stream5 channel4

//v4.1
//pa0 usart4 tx DMA1 stream4 channel4
//pa10 usart1 rx DMA2 stream5 channel4

//TODO: lbp command 0xe6 to set mode

static void hw_init(volatile void *ctx_ptr, volatile hal_pin_inst_t *pin_ptr) {
  // struct sserial_ctx_t * ctx = (struct sserial_ctx_t *)ctx_ptr;
  struct sserial_pin_ctx_t *pins = (struct sserial_pin_ctx_t *)pin_ptr;

  PIN(timeout) = 100.0;  // 20khz / 1khz * 2 reads = 40

  GPIO_InitTypeDef GPIO_InitStruct;
  USART_InitTypeDef USART_InitStruct;
  DMA_InitTypeDef DMA_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
  //USART TX
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_UART4);
  GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_0;
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStruct);

  //USART RX
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
  GPIO_Init(GPIOA, &GPIO_InitStruct);

  USART_InitStruct.USART_BaudRate            = 2500000;
  USART_InitStruct.USART_WordLength          = USART_WordLength_8b;
  USART_InitStruct.USART_StopBits            = USART_StopBits_1;
  USART_InitStruct.USART_Parity              = USART_Parity_No;
  USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStruct.USART_Mode                = USART_Mode_Rx;
  USART_Init(USART1, &USART_InitStruct);
  USART_InitStruct.USART_Mode = USART_Mode_Tx;
  USART_Init(UART4, &USART_InitStruct);
  //USART_HalfDuplexCmd(USART1,ENABLE);

  //USART_InitStruct.USART_Mode = USART_Mode_Tx;
  //USART_Init(USART1, &USART_InitStruct);

  //USART_Cmd(USART1, ENABLE);
  USART_Cmd(USART1, ENABLE);
  USART_Cmd(UART4, ENABLE);

  //RX DMA

  DMA_Cmd(DMA2_Stream5, DISABLE);
  DMA_DeInit(DMA2_Stream5);

  // DMA2-Config
  DMA_InitStructure.DMA_Channel            = DMA_Channel_4;
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (USART1->DR);
  DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)&rxbuf;
  DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize         = sizeof(rxbuf);
  DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize     = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority           = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode           = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold      = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst        = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream5, &DMA_InitStructure);

  DMA_Cmd(DMA2_Stream5, ENABLE);

  USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);

  //TX DMA

  DMA_Cmd(DMA1_Stream4, DISABLE);
  DMA_DeInit(DMA1_Stream4);

  // DMA2-Config
  DMA_InitStructure.DMA_Channel            = DMA_Channel_4;
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (UART4->DR);
  DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)&txbuf;
  DMA_InitStructure.DMA_DIR                = DMA_DIR_MemoryToPeripheral;
  DMA_InitStructure.DMA_BufferSize         = sizeof(txbuf);
  DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize     = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_Mode               = DMA_Priority_Low;
  DMA_InitStructure.DMA_Priority           = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode           = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold      = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst        = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;
  DMA_Init(DMA1_Stream4, &DMA_InitStructure);

  //DMA_Cmd(DMA1_Stream7, ENABLE);

  USART_DMACmd(UART4, USART_DMAReq_Tx, ENABLE);

  //tx enable
  GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_7;
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_SetBits(GPIOB, GPIO_Pin_7);

  //generate unit number from 96bit unique chip ID
  unit.unit = U_ID[0] ^ U_ID[1] ^ U_ID[2];

  rxpos   = 0;
  timeout = 1000;  //make sure we start in timeout

  discovery.input  = sizeof(sserial_in_process_data_t) + 1;  //+1 for fault byte
  discovery.output = sizeof(sserial_out_process_data_t);
  discovery.ptocp  = sserial_ptocp;
  discovery.gtocp  = sserial_gtocp;

  //bytes to wait before expected end of transmission to prevent timeouts
  block_bytes = 3;
  //calculate timeout in systicks for block_bytes
  max_waste_ticks = (1.0 / 2500000.0) * 11.0 * (float)block_bytes / (1.0f / (float)hal_get_systick_freq());
}


static void frt_func(float period, volatile void *ctx_ptr, volatile hal_pin_inst_t *pin_ptr) {
  // struct res_ctx_t * ctx = (struct res_ctx_t *)ctx_ptr;
  struct sserial_pin_ctx_t *pins = (struct sserial_pin_ctx_t *)pin_ptr;
  for(int j = 0; j < 1; j++) {
    //next received packet will be written to bufferpos
    bufferpos = sizeof(rxbuf) - DMA_GetCurrDataCounter(DMA2_Stream5);
    //how many packets we have the the rx buffer for processing
    available = (bufferpos - rxpos + sizeof(rxbuf)) % sizeof(rxbuf);

    if(available >= 1) {
      lbp.byte = rxbuf[rxpos];

      if(lbp.ct == CT_LOCAL && lbp.wr == 0) {  //local read, cmd+crc = 2b
        timeout = 0;
        if(available >= 2) {
          switch(lbp.byte) {
            case LBPCookieCMD:
              txbuf[0] = LBPCookie;
              break;
            case LBPStatusCMD:  //TODO: return status
              txbuf[0] = 0x00;
              break;
            case LBPCardName0Cmd ... LBPCardName3Cmd:
              txbuf[0] = name[lbp.byte - LBPCardName0Cmd];
              break;
            default:  //TODO: handle unknown command condition
              txbuf[0] = 0x00;
          }
          send(1, 1);
          rxpos += 2;
        } else {
          continue;
        }
      } else if(lbp.ct == CT_LOCAL && lbp.wr == 1) {  //local write, cmd+data+crc = 3b
        timeout = 0;
        //0xFF and 0xFC are not followed by crc
        if(rxbuf[rxpos] == 0xFF) {
          // reset parser
          rxpos += 1;
        } else if(rxbuf[rxpos] == 0xFC) {
          // todo
          rxpos += 1;
        } else if(available >= 3) {  //writes do not expect crc in reply
          txbuf[0] = 0x00;
          send(1, 0);
          rxpos += 3;
        } else {
          continue;
        }
      } else if(lbp.ct == CT_RPC) {  //RPC TODO: check for ct should not required for rpc
        timeout = 0;
        if(lbp.byte == UnitNumberRPC && available >= 2) {  //unit number, cmd+crc = 2b
          txbuf[0] = unit.byte[0];
          txbuf[1] = unit.byte[1];
          txbuf[2] = unit.byte[2];
          txbuf[3] = unit.byte[3];
          send(4, 1);
          rxpos += 2;
        } else if(lbp.byte == DiscoveryRPC && available >= 2) {  //discovery, cmd+crc = 2b
          memcpy((void *)txbuf, ((uint8_t *)&discovery), sizeof(discovery));
          send(sizeof(discovery), 1);
          rxpos += 2;
        } else if(lbp.byte == ProcessDataRPC && available >= discovery.output + 2 - block_bytes) {  //process data, requires cmd+output bytes+crc
          uint32_t t1         = hal_get_systick_value();
          uint32_t wait_ticks = 0;
          //wait with timeout until rest of process data is received
          do {
            uint32_t t2 = hal_get_systick_value();
            if(t1 < t2) {
              t1 += hal_get_systick_reload();
            }
            wait_ticks = t1 - t2;
            //next received packet will be written to bufferpos
            bufferpos = sizeof(rxbuf) - DMA_GetCurrDataCounter(DMA2_Stream5);
            //how many packets we have the the rx buffer for processing
            available = (bufferpos - rxpos + sizeof(rxbuf)) % sizeof(rxbuf);
          } while(available < discovery.output + 2 && wait_ticks <= max_waste_ticks);
          //TODO: fault handling on timeout...
          //set input pins
          data_in.pos_fb       = PIN(pos_fb) + PIN(vel_fb) * PIN(pos_advance);
          data_in.vel_fb       = PIN(vel_fb);
          data_in.current      = CLAMP(PIN(current) / (30.0f / 128.0f), -127, 127);
          data_in.input_pins_0 = (PIN(in0) > 0) ? 1 : 0;
          data_in.input_pins_1 = (PIN(in1) > 0) ? 1 : 0;
          data_in.input_pins_2 = (PIN(in2) > 0) ? 1 : 0;
          data_in.input_pins_3 = (PIN(in3) > 0) ? 1 : 0;
          data_in.fault        = (PIN(fault) > 0) ? 1 : 0;

          //copy output pins from rx buffer
          for(int i = 0; i < discovery.output; i++) {
            ((uint8_t *)(&data_out))[i] = rxbuf[(rxpos + i + 1) % sizeof(rxbuf)];
          }

          //set bidirectional pins
          PIN(index_out)       = data_out.index_enable;
          data_in.index_enable = (PIN(index_clear) > 0) ? 0 : data_out.index_enable;

          //copy input pins to tx buffer
          txbuf[0] = 0x00;  //fault byte
          for(int i = 0; i < (discovery.input - 1); i++) {
            txbuf[i + 1] = ((uint8_t *)(&data_in))[i];
          }

          //send buffer
          DMA_SetCurrDataCounter(DMA1_Stream4, discovery.input + 1);
          DMA_Cmd(DMA1_Stream4, DISABLE);
          DMA_ClearFlag(DMA1_Stream4, DMA_FLAG_TCIF4);
          DMA_Cmd(DMA1_Stream4, ENABLE);
          txbuf[discovery.input] = crc8((uint8_t *)txbuf, discovery.input);
          //send(discovery.input, 1);

          //we cannot send the reply based on crc, as this causes timeouts TODO: still valid?
          if(crc_reuest(discovery.output + 1)) {
            timeout = 0;
            //set output pins
            PIN(pos_cmd)   = data_out.pos_cmd;
            PIN(pos_cmd_d) = data_out.vel_cmd;
            PIN(out0)      = data_out.output_pins_0;
            PIN(out1)      = data_out.output_pins_1;
            PIN(out2)      = data_out.output_pins_2;
            PIN(out3)      = data_out.output_pins_3;
            PIN(enable)    = data_out.enable;
          } else {
            PIN(crc_error)
            ++;
            PIN(connected) = 0;
            PIN(error)     = 1;
            PIN(pos_cmd)   = 0;
            PIN(pos_cmd_d) = 0;
            PIN(out0)      = 0;
            PIN(out1)      = 0;
            PIN(out2)      = 0;
            PIN(out3)      = 0;
            PIN(enable)    = 0;
          }
          rxpos += discovery.output + 2;
        } else {
          continue;
        }
      } else if(lbp.ct == CT_RW && lbp.wr == 0) {  //read TODO: implement write.
        timeout = 0;
        if(available >= 2) {
          if(lbp.as == 1) {  //address included in command = cmd+addr+addr+crc
            if(available >= 4) {
              address = rxbuf[(rxpos + 1) % sizeof(rxbuf)] + (rxbuf[(rxpos + 2) % sizeof(rxbuf)] << 8);
              rxpos += 4;
            } else {
              continue;
            }
          } else {  //address not included in command = cmd+crc
            rxpos += 2;
          }
          //TODO: check if address is valid
          memcpy((void *)txbuf, &sserial_slave[address], (1 << lbp.ds));
          send((1 << lbp.ds), 1);
          if(lbp.ai == 1) {  //auto increment address by datasize
            address += (1 << lbp.ds);
          }
        } else {
          continue;
        }
      } else {
        //TODO: handle unkown packet
      }
    }

    if(timeout > PIN(timeout)) {  //TODO: clamping
      PIN(connected) = 0;
      PIN(error)     = 1;
      PIN(pos_cmd)   = 0;
      PIN(pos_cmd_d) = 0;
      PIN(out0)      = 0;
      PIN(out1)      = 0;
      PIN(out2)      = 0;
      PIN(out3)      = 0;
      PIN(enable)    = 0;
      rxpos          = bufferpos;
    } else {
      PIN(connected) = 1;
      PIN(error)     = 0;
    }
    rxpos = rxpos % sizeof(rxbuf);
  }
  timeout++;
}

const hal_comp_t sserial_comp_struct = {
    .name      = "sserial",
    .nrt       = 0,
    .rt        = 0,
    .frt       = frt_func,
    .nrt_init  = 0,
    .hw_init   = hw_init,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = sizeof(struct sserial_ctx_t),
    .pin_count = sizeof(struct sserial_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
