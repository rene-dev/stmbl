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
#include "angle.h"
#include "stm32f4xx_conf.h"
#include "hw/hw.h"
#include "sserial.h"
#include "crc8.h"
#include "setup.h"
#include <string.h>

HAL_COMP(sserial);

// pins
HAL_PIN(dump_pd_vals);
HAL_PIN(error);
HAL_PIN(crc_error);  //counts crc errors
HAL_PIN(connected);  //connection status TODO: not stable during startup, needs link to pd
HAL_PIN(timeout);  // 20khz / 1khz * 2 reads = 40

HAL_PIN(pos_cmd);
HAL_PIN(pos_cmd_d);
HAL_PIN(pos_fb);

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

//TODO: move to ctx
struct sserial_ctx_t {
  uint32_t foo;
};

volatile uint8_t rxbuf[128];
volatile uint8_t txbuf[20];
uint16_t address;  //current address pointer
int rxpos;
memory_t memory;
uint8_t *heap_ptr;
uint32_t timeout;
pd_table_t pd_table;
float last_pos_cmd;
lbp_t lbp;
char name[] = LBPCardName;
int bufferpos;
int available;
unit_no_t unit;

uint8_t crc_reuest(uint8_t len) {
  uint8_t crc = crc8_init();
  for(int i = rxpos; i < rxpos + len; i++) {
    crc = crc8_update(crc, (void *)&(rxbuf[i % sizeof(rxbuf)]), 1);
  }
  crc8_finalize(crc);
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


uint16_t add_pd(char *name_string, char *unit_string, uint8_t data_size_in_bits, uint8_t data_type, uint8_t data_dir, float param_min, float param_max) {
  process_data_descriptor_t pdr;
  pdr.record_type    = RECORD_TYPE_PROCESS_DATA_RECORD;
  pdr.data_size      = data_size_in_bits;
  pdr.data_type      = data_type;
  pdr.data_direction = data_dir;
  pdr.param_min      = param_min;
  pdr.param_max      = param_max;
  pdr.data_addr      = MEMPTR(*heap_ptr);

  heap_ptr += NUM_BYTES(data_size_in_bits);
  // this aligns the heap pointer to 32bit.  Not doing this causes the floats in the pd to be misaligned, which crashes the arm.
  if((uint32_t)heap_ptr % 4) {
    heap_ptr += 4 - (uint32_t)heap_ptr % 4;
  }

  memcpy(heap_ptr, &pdr, sizeof(process_data_descriptor_t));
  // note that we don't store the names in the struct anymore.  The fixed-length struct is copied into memory, and then the nmaes go in directly behind it, so they'll read out properly

  uint16_t pd_ptr = MEMPTR(*heap_ptr);  // save off the ptr to return, before we modify the heap ptr

  heap_ptr = (uint8_t *)&(((process_data_descriptor_t *)heap_ptr)->names);

  // copy the strings in after the pd
  strcpy((char *)heap_ptr, unit_string);
  heap_ptr += strlen(unit_string) + 1;

  strcpy((char *)heap_ptr, name_string);
  heap_ptr += strlen(name_string) + 1;

  // moved this up to before the pd record
  /*
   // this aligns the heap pointer to 32bit.  Not doing this causes the floats in the pd to be misaligned, which crashes the arm.
   if((uint32_t)heap_ptr % 4){
   heap_ptr += 4 - (uint32_t)heap_ptr % 4;
   }
   */

  return pd_ptr;
}

uint16_t add_mode(char *name_string, uint8_t index, uint8_t type) {
  mode_descriptor_t mdr;
  mdr.record_type = RECORD_TYPE_MODE_DATA_RECORD;
  mdr.index       = index;
  mdr.type        = type;  //hw = 0, sw = 1
  mdr.unused      = 0x00;

  memcpy(heap_ptr, &mdr, sizeof(mode_descriptor_t));

  uint16_t md_ptr = MEMPTR(*heap_ptr);

  heap_ptr = (uint8_t *)&(((mode_descriptor_t *)heap_ptr)->names);

  strcpy((char *)heap_ptr, name_string);
  heap_ptr += strlen(name_string) + 1;

  return md_ptr;
}

void metadata(pd_metadata_t *pdm, process_data_descriptor_t *ptr) {
  pdm->ptr    = ptr;
  pdm->range  = ptr->data_type == DATA_TYPE_SIGNED ? MAX(ABS(ptr->param_min), ABS(ptr->param_max)) * 2 : ptr->param_max;
  pdm->bitmax = (1 << ptr->data_size) - 1;
}

//v3
//pb13 txen
//pc12 usart5 tx
//pa9 usart1 tx as rx
//USART5 TX DMA1 stream7 channel4
//USART1 RX DMA2 stream5 channel4

//v4
//normal enable: pb9
//mesa pinout enable: pb7(requires jumper)
//pa10 usart1 rx
//pa9 usart1 tx
//USART1 TX	DMA2 stream7 channel4
//USART1 RX	DMA2 stream5 channel4
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

  heap_ptr = memory.heap;

  uint16_t input_bits  = 8;  // this starts at 8 bits = 1 byte for the fault byte
  uint16_t output_bits = 0;

  // these are temp toc arrays that the macros will write pointers into.  the tocs get copied to main memory after everything else is written in
  uint16_t ptoc[32];
  uint16_t gtoc[32];

  uint16_t *ptocp = ptoc;
  uint16_t *gtocp = gtoc;
  process_data_descriptor_t *last_pd;

  ADD_PROCESS_VAR(("output_pins", "none", 4, DATA_TYPE_BITS, DATA_DIRECTION_OUTPUT, 0, 1));
  metadata(&(pd_table.output_pins), last_pd);
  ADD_PROCESS_VAR(("enable", "none", 1, DATA_TYPE_BOOLEAN, DATA_DIRECTION_OUTPUT, 0, 1));
  metadata(&(pd_table.enable), last_pd);
  ADD_PROCESS_VAR(("pos_cmd", "rad", 32, DATA_TYPE_FLOAT, DATA_DIRECTION_OUTPUT, -INFINITY, INFINITY));
  metadata(&(pd_table.pos_cmd), last_pd);

  ADD_PROCESS_VAR(("input_pins", "none", 4, DATA_TYPE_BITS, DATA_DIRECTION_INPUT, -100, 100));
  metadata(&(pd_table.input_pins), last_pd);
  ADD_PROCESS_VAR(("fault", "none", 1, DATA_TYPE_BOOLEAN, DATA_DIRECTION_INPUT, 0, 1));
  metadata(&(pd_table.fault), last_pd);
  ADD_PROCESS_VAR(("pos_fb", "rad", 32, DATA_TYPE_FLOAT, DATA_DIRECTION_INPUT, -INFINITY, INFINITY));
  metadata(&(pd_table.pos_fb), last_pd);
  //globals and modes are not working. https://github.com/LinuxCNC/linuxcnc/blob/2957cc5ad0a463c39fb35c10a0c14909c09a5fb7/src/hal/drivers/mesa-hostmot2/sserial.c#L1516
  // - globals need write support
  // - linuxcnc only supports globals of type DATA_TYPE_NONVOL_UNSIGNED or DATA_TYPE_NONVOL_SIGNED
  //ADD_GLOBAL_VAR(("swr", "non", 8, DATA_TYPE_NONVOL_UNSIGNED, DATA_DIRECTION_OUTPUT, 0, 0));

  //ADD_MODE(("foo", 0, 0));
  ADD_MODE(("Position mode", 0, 1));

  // automatically create padding pds based on the mod remainder of input/output bits
  if(input_bits % 8)
    ADD_PROCESS_VAR(("padding", "", 8 - (input_bits % 8), DATA_TYPE_PAD, DATA_DIRECTION_INPUT, 0, 0));
  if(output_bits % 8)
    ADD_PROCESS_VAR(("padding", "", 8 - (output_bits % 8), DATA_TYPE_PAD, DATA_DIRECTION_OUTPUT, 0, 0));

  // now that all the toc entries have been added, write out the tocs to memory and set up the toc pointers

  //calculate bytes from bits
  memory.discovery.input  = input_bits >> 3;
  memory.discovery.output = output_bits >> 3;

  memory.discovery.ptocp = MEMPTR(*heap_ptr);

  for(uint8_t i = 0; i < ptocp - ptoc; i++) {
    *heap_ptr++ = ptoc[i] & 0x00FF;
    *heap_ptr++ = (ptoc[i] & 0xFF00) >> 8;
  }
  // this is the ptoc end marker
  *heap_ptr++ = 0x00;
  *heap_ptr++ = 0x00;

  memory.discovery.gtocp = MEMPTR(*heap_ptr);

  for(uint8_t i = 0; i < gtocp - gtoc; i++) {
    *heap_ptr++ = gtoc[i] & 0x00FF;
    *heap_ptr++ = (gtoc[i] & 0xFF00) >> 8;
  }
  // this is the gtoc end marker
  *heap_ptr++ = 0x00;
  *heap_ptr++ = 0x00;
}

void process_data_rpc(uint8_t fault, volatile uint8_t *input, volatile uint8_t *output) {
  uint16_t *ptocp      = (uint16_t *)(memory.bytes + memory.discovery.ptocp);
  uint32_t local_rxpos = rxpos;
  *(input++)           = fault;
  *input               = 0x00;

  // data needs to be packed and unpacked based on its type and size
  // input is a pointer to the data that gets sent back to the host
  // need a bit pointer to keep track of partials

  uint8_t output_bit_ptr = 0;
  uint8_t input_bit_ptr  = 0;

  while(*ptocp != 0x0000) {
    process_data_descriptor_t *pd = (process_data_descriptor_t *)(memory.bytes + *ptocp++);

    if(IS_INPUT(pd)) {
      uint16_t data_addr   = pd->data_addr;
      uint8_t data_size    = pd->data_size;
      uint8_t data_bit_ptr = 0;
      while(data_size > 0) {
        uint8_t bits_to_pack = data_size < BITSLEFT(input_bit_ptr) ? data_size : BITSLEFT(input_bit_ptr);
        if(BITSLEFT(data_bit_ptr) < bits_to_pack) {
          bits_to_pack = BITSLEFT(data_bit_ptr);
        }

        uint8_t mask = ((1 << bits_to_pack) - 1) << (data_bit_ptr);

        *input |= ((MEMU8(data_addr) & mask) >> data_bit_ptr) << input_bit_ptr;

        input_bit_ptr += bits_to_pack;
        data_bit_ptr += bits_to_pack;
        data_size -= bits_to_pack;
        if((input_bit_ptr %= 8) == 0)
          *(++input) = 0x00;  // make sure we clear the input buffer whenever we increment bytes
        if((data_bit_ptr %= 8) == 0)
          data_addr++;
      }
    }
    if(IS_OUTPUT(pd)) {
      uint16_t data_addr = pd->data_addr;
      uint8_t data_size  = pd->data_size;

      uint8_t val_bits_remaining = 8;
      uint8_t val                = 0x00;

      while(data_size > 0) {
        // the number of bits to unpack this iteration is the number of bits remaining in the pd, or the number of bits remaining in the output byte,
        // whichever is smaller.  Then, it can be even smaller if we have less room in the current val.

        uint8_t bits_to_unpack = data_size < BITSLEFT(output_bit_ptr) ? data_size : BITSLEFT(output_bit_ptr);
        if(val_bits_remaining < bits_to_unpack) {
          bits_to_unpack = val_bits_remaining;
        }

        // create a bitmask the width of the bits to read, shifted to the position in the output byte that we're pointing to
        uint8_t mask = ((1 << bits_to_unpack) - 1) << (output_bit_ptr);

        // val is what we get when we mask off output and then shift it to the proper place.
        val = val | ((rxbuf[(local_rxpos + 1) % sizeof(rxbuf)] & mask) >> (output_bit_ptr)) << (8 - val_bits_remaining);

        val_bits_remaining -= bits_to_unpack;
        data_size -= bits_to_unpack;
        output_bit_ptr += bits_to_unpack;
        // rxpos is a ringbuf and wraps around
        // note: this replaces the output argument
        if((output_bit_ptr %= 8) == 0) {
          local_rxpos++;
          local_rxpos = local_rxpos % sizeof(rxbuf);
        }


        if(val_bits_remaining == 0 || data_size == 0) {
          MEMU8(data_addr++) = val;
          val_bits_remaining = 8;
          val                = 0x00;
        }
      }
      // now we've finished unpacking it and storing it in memory, but we have to fix up the high bits if it wasn't a byte-aligned datasize.
      // for instance, if we receive 0xFFF in a 12 bit field, that is a negative number, but we stored it as 0x0FFF in memory.
      // strategy is to set the most significant n bits of the MSB to the most significant bit of the output value, iff the pd is defined as signed.
      if(SIGNED(pd) && pd->data_size % 8 != 0) {
        //printf("in output fixup.  data_addr %h  data_size %i num_bytes %i\n", pd->data_addr, pd->data_size, NUM_BYTES(pd->data_size));
        uint8_t msb_addr = pd->data_addr + NUM_BYTES(pd->data_size) - 1;
        //printf("in output fixup.  MSB (at %h): %h\n", msb_addr, MEMU8(msb_addr));

        // these two masks use data_size%8, this is the number of bits in the most significant byte, and since we tested for %8!=0 above, we know it's a partial byte
        if(MEMU8(msb_addr) & 1 << (pd->data_size % 8 - 1)) {  // this test uses a mask that is 1 in the most significant bit position, we only need to fixup the val if it's 1 (ie negative)
          // this mask is all the unused high bits set
          uint8_t mask = 0xFF ^ ((1 << pd->data_size % 8) - 1);
          //printf("applying mask: %h\n", mask);
          MEMU8(msb_addr) |= mask;
        }

        //printf("fixed up val: %h\n", MEMU8(msb_addr));
      }
    }
  }
}

//TODO: div by zero
float scale_out(pd_metadata_t pd, int32_t val) {
  return val * pd.range / (float)pd.bitmax;
}
//TODO: div by zero
int32_t scale_in(pd_metadata_t pd, float val) {
  return CLAMP(val, pd.ptr->param_min, pd.ptr->param_max) * pd.bitmax / pd.range;
}

static void rt_func(float period, volatile void *ctx_ptr, volatile hal_pin_inst_t *pin_ptr) {
  // struct res_ctx_t * ctx = (struct res_ctx_t *)ctx_ptr;
  struct sserial_pin_ctx_t *pins = (struct sserial_pin_ctx_t *)pin_ptr;
  // update all hal pins with values from their associated pds


  // uint16_t foo = MEMU16(pd_table.pos_cmd.ptr->data_addr);
  // scale_out(pd_table.pos_cmd, *(int16_t*)&foo);
  PIN(pos_cmd)    = MEMFLOAT(pd_table.pos_cmd.ptr->data_addr);
  uint8_t outpins = MEMU8(pd_table.output_pins.ptr->data_addr);
  PIN(out0)       = outpins >> 0 & 1 ? 1.0 : 0.0;
  PIN(out1)       = outpins >> 1 & 1 ? 1.0 : 0.0;
  PIN(out2)       = outpins >> 2 & 1 ? 1.0 : 0.0;
  PIN(out3)       = outpins >> 3 & 1 ? 1.0 : 0.0;
  uint8_t enable  = MEMU8(pd_table.enable.ptr->data_addr);
  PIN(enable)     = enable << 0 & 1 ? 1.0 : 0.0;
  //TODO: how to handle bidirectional pins properly?

  //*((uint16_t *)&(memory.bytes[pd_table.pos_fb.ptr->data_addr])) = (uint16_t)scale_in(pd_table.pos_fb, PIN(pos_fb));

  //uint32_t tmp = MEMU32(ptr);*((float*)&tmp);}))

  //MEMFLOAT(pd_table.pos_fb.ptr->data_addr) = PIN(pos_fb);
  *((float *)&(memory.bytes[pd_table.pos_fb.ptr->data_addr])) = PIN(pos_fb);

  MEMU8(pd_table.fault.ptr->data_addr)      = BOOLPIN(fault);
  MEMU8(pd_table.input_pins.ptr->data_addr) = BOOLPIN(in0) << 0 | BOOLPIN(in1) << 1 | BOOLPIN(in2) << 2 | BOOLPIN(in3) << 3;
}


static void frt_func(float period, volatile void *ctx_ptr, volatile hal_pin_inst_t *pin_ptr) {
  // struct res_ctx_t * ctx = (struct res_ctx_t *)ctx_ptr;
  struct sserial_pin_ctx_t *pins = (struct sserial_pin_ctx_t *)pin_ptr;

  for(int j = 0; j < 2; j++) {
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
          memcpy((void *)txbuf, ((uint8_t *)&memory.discovery), sizeof(memory.discovery));
          send(sizeof(memory.discovery), 1);
          rxpos += 2;
        } else if(lbp.byte == ProcessDataRPC && available >= memory.discovery.output + 2) {  //process data, requires cmd+output bytes+crc
          //TODO: maybe packing and unpacking can be moved to RT
          process_data_rpc(0x00, txbuf, &(rxbuf[rxpos + 1]));  // todo: send a proper fault byte?
          send(memory.discovery.input, 1);
          //uint16_t foo = MEMU16(pd_table.pos_cmd.ptr->data_addr);
          //float p = scale_out(pd_table.pos_cmd, *(int16_t*)&foo);
          float p        = MEMFLOAT(pd_table.pos_cmd.ptr->data_addr);
          PIN(pos_cmd_d) = minus(p, last_pos_cmd) * 1000.0f;  //TODO: only valid for 1khz servo thread
          last_pos_cmd   = p;
          //we cannot send the reply based on crc, as this causes timeouts
          //instead we should check for errors in RT
          if(!crc_reuest(memory.discovery.output + 1)) {
            PIN(crc_error)
            ++;
          }
          rxpos += memory.discovery.output + 2;
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
          memcpy((void *)txbuf, &memory.bytes[address], (1 << lbp.ds));
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

    timeout++;
    if(timeout > PIN(timeout)) {  //TODO: clamping
      PIN(connected) = 0;
      PIN(error)     = 1;
      rxpos          = bufferpos;
    } else {
      PIN(connected) = 1;
      PIN(error)     = 0;
    }
    rxpos = rxpos % sizeof(rxbuf);
  }
}


// NRT(
//   if(PIN(dump_pd_vals) != 0.0) {
//     if(PIN(error) != 0) {
//       printf("Error count: %f\n", PIN(error));
//     }
//     PIN(dump_pd_vals) = 0.0;
//
//     printf("pos_cmd: %f\n", PIN(pos_cmd));
//
//     uint16_t *ptocp = (uint16_t *)(memory.bytes + memory.discovery.ptocp);
//     while(*ptocp != 0x0000) {
//       process_data_descriptor_t *pd = (process_data_descriptor_t *)(memory.bytes + *ptocp++);
//       printf("pd has data at %x with value %x\n", pd->data_addr, MEMU16(pd->data_addr));
//     }
//   }
// );

const hal_comp_t sserial_comp_struct = {
    .name      = "sserial",
    .nrt       = 0,
    .rt        = rt_func,
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
