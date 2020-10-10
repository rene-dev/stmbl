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

//dump:
//sigrok-cli --driver fx2lafw --config samplerate=24m --time 1s --channels 3=rx,2=tx -t rx=f -o sserial.sr
//decode:
//sigrok-cli -i sserial.sr -P uart:baudrate=2500000:tx=tx:rx=rx:format=hex -A uart=tx-data:rx-data
//decode more:
//sigrok-cli -i sserial.sr -P uart:baudrate=2500000:tx=tx:rx=rx,sserial
//sudo mesaflash --device 5i25 --sserial --verbose

#define UnitNumberRPC 0xbc
#define DiscoveryRPC 0xbb
#define ProcessDataRPC 0xbd

//local commands
#define LBPCookieCMD 0xdf
#define LBPCookie 0x5a

#define LBPStatusCMD 0xc1

#define LBPCardName "stbl"
#define LBPCardName0Cmd 0xd0
#define LBPCardName1Cmd 0xd1
#define LBPCardName2Cmd 0xd2
#define LBPCardName3Cmd 0xd3

#define CT_RW 1     // 01b
#define CT_RPC 2    // 10b
#define CT_LOCAL 3  // 11b

#define MEM_SIZE 2048

#define RECORD_TYPE_PROCESS_DATA_RECORD 0xA0
#define RECORD_TYPE_MODE_DATA_RECORD 0xB0

//process data
#define DATA_TYPE_PAD 0x00  //padding, done automaticly at the end
#define DATA_TYPE_BITS 0x01
#define DATA_TYPE_UNSIGNED 0x02
#define DATA_TYPE_SIGNED 0x03
#define DATA_TYPE_NONVOL_UNSIGNED 0x04
#define DATA_TYPE_NONVOL_SIGNED 0x05
#define DATA_TYPE_STREAM 0x06
#define DATA_TYPE_BOOLEAN 0x07
#define DATA_TYPE_ENCODER 0x08
#define DATA_TYPE_FLOAT 0x10  // New for STMBL
#define DATA_TYPE_ENCODER_H 0x18
#define DATA_TYPE_ENCODER_L 0x28

#define DATA_DIRECTION_INPUT 0x00
#define DATA_DIRECTION_BI_DIRECTIONAL 0x40
#define DATA_DIRECTION_OUTPUT 0x80

#define MEMPTR(p) ((uint32_t)&p - (uint32_t)&memory)

#define MEMU8(ptr) (memory.bytes[ptr])
#define MEMU16(ptr) (memory.bytes[ptr] | memory.bytes[ptr + 1] << 8)
#define MEMU32(ptr) (memory.bytes[ptr] | memory.bytes[ptr + 1] << 8 | memory.bytes[ptr + 2] << 16 | memory.bytes[ptr + 3] << 24)
#define MEMFLOAT(ptr) ((float)({uint32_t tmp = MEMU32(ptr);*((float*)&tmp); }))

#define NUM_BYTES(bits) (bits / 8 + (bits % 8 > 0 ? 1 : 0))

#define IS_INPUT(pdr) (pdr->data_direction != 0x80)
#define IS_OUTPUT(pdr) (pdr->data_direction != 0x00)

#define SIGNED(pdr) (pdr->data_type == DATA_TYPE_SIGNED || pdr->data_type == DATA_TYPE_NONVOL_SIGNED)
#define UNSIGNED(pdr) (pdr->data_type == DATA_TYPE_UNSIGNED || pdr->data_type == DATA_TYPE_NONVOL_UNSIGNED)

#define INDIRECT_PD(pd_ptr) ((process_data_descriptor_t *)(memory.bytes + *pd_ptr))
#define DATA_DIR(pd_ptr) INDIRECT_PD(pd_ptr)->data_direction
#define DATA_SIZE(pd_ptr) INDIRECT_PD(pd_ptr)->data_size

#define ADD_PROCESS_VAR(args)                                          \
  *ptocp = add_pd args;                                                \
  input_bits += IS_INPUT(INDIRECT_PD(ptocp)) ? DATA_SIZE(ptocp) : 0;   \
  output_bits += IS_OUTPUT(INDIRECT_PD(ptocp)) ? DATA_SIZE(ptocp) : 0; \
  last_pd = INDIRECT_PD(ptocp++)
#define ADD_GLOBAL_VAR(args) *gtocp++ = add_pd args
#define ADD_MODE(args) *gtocp++ = add_mode args

#define BITSLEFT(ptr) (8 - ptr)

#define BOOLPIN(pin) (uint8_t)(PIN(pin) > 0.0)

typedef union {
  struct {
    uint8_t ds : 2;   //DataSize: Specifies data size, 00b = 1 bytes, 01b = 2 bytes, 10 b= 4 bytes, 11b = 8 bytes.
    uint8_t as : 1;   //AddressSize: 0 to specify current address, 1 to specify 2 byte address.
    uint8_t ai : 1;   //AutoInc: 0 leaves address unchanged, 1 specifies that address is post incremented by data size in bytes.
    uint8_t rid : 1;  //RPCIncludesData: 0 specifies that data is from stream, 1, that data is from RPC (RPC only, ignored for non RPC commands)
    uint8_t wr : 1;   //Write: 1 to specify write, 0 to specify read
    uint8_t ct : 2;   //CommandType: Must be 01b to specify data read/write command
    // 01:rw   11:local command   10:rpc
  };
  struct {
    uint8_t rpc : 6;
    uint8_t dummy : 2;
    //01:rw 11: local command 10:rpc
  };
  uint8_t byte;
} lbp_t;

#pragma pack(push, 1)
typedef union {
  uint8_t byte[4];
  uint32_t unit;
} unit_no_t;

typedef struct {
  uint8_t record_type;  //0xa0
  uint8_t data_size;
  uint8_t data_type;
  uint8_t data_direction;
  float param_min;
  float param_max;
  uint16_t data_addr;
  char names;
} process_data_descriptor_t;

typedef struct {
  uint8_t record_type;  //0xb0
  uint8_t index;
  uint8_t type;
  uint8_t unused;
  char names;
} mode_descriptor_t;

typedef struct {
  uint8_t input;   //process data input bytes
  uint8_t output;  //process data output bytes
  uint16_t ptocp;  //pointer to process data table start
  uint16_t gtocp;  //pointer to mode data table start
} discovery_rpc_t;

typedef union {
  struct {
    discovery_rpc_t discovery;  //6 bytes
    uint16_t foo;               //padding, so heap is word aligned
    uint8_t heap[MEM_SIZE - sizeof(discovery_rpc_t)];
  };

  uint8_t bytes[MEM_SIZE];
} memory_t;

typedef struct {
  process_data_descriptor_t *ptr;
  float range;
  uint32_t bitmax;
} pd_metadata_t;

typedef struct {
  pd_metadata_t pos_cmd;
  pd_metadata_t pos_fb;
  pd_metadata_t vel_cmd;
  pd_metadata_t vel_fb;
  pd_metadata_t input_pins;
  pd_metadata_t output_pins;
  pd_metadata_t fault;
  pd_metadata_t enable;
  pd_metadata_t index_enable;
  pd_metadata_t current;
} pd_table_t;
#pragma pack(pop)