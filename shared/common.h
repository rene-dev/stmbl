/*
* This file is part of the stmbl project.
*
* Copyright (C) 2013-2017 Rene Hopf <renehopf@mac.com>
* Copyright (C) 2013-2017 Nico Stute <crinq@crinq.de>
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

#pragma once
#include <stdint.h>
#include "stmbl_talk.h"

#if __GNUC__ < 5
#error gcc to old (< 5.0)
#endif

#define DATABAUD 3000000  //baudrate used for communication

#pragma pack(1)
typedef struct {
  stmbl_talk_header_t header;
  uint32_t addr;
  uint32_t value;
  enum{
    BOOTLOADER_OPCODE_READ = 0,
    BOOTLOADER_OPCODE_WRITE, // MEM[addr] = value
    BOOTLOADER_OPCODE_PAGEERASE, // clear PAGE[addr]
    BOOTLOADER_OPCODE_RESET,
    BOOTLOADER_OPCODE_CRCCHECK, // CRC(APP) == 0
  } cmd : 8;
  enum {
    BOOTLOADER_STATE_OK = 0,
    BOOTLOADER_STATE_NAK,
  } state : 8;
  uint16_t padding;
} packet_bootloader_t;

// #pragma pack(1)
// typedef struct {
//   stmbl_talk_header_t header;
//   uint32_t addr;
//   uint32_t value;
//   enum{
//     BOOTLOADER_OPCODE_READ = 0, // value = MEM[addr]
//     BOOTLOADER_OPCODE_WRITE, // value = MEM[addr]
//     BOOTLOADER_OPCODE_PAGEERASE,
//     BOOTLOADER_OPCODE_RESET,
//     BOOTLOADER_OPCODE_CRCCHECK, // state = CRC(APP) != 0
//   } cmd : 8; // same as request
//   enum {
//     BOOTLOADER_STATE_OK = 0,
//     BOOTLOADER_STATE_NAK,
//   } state;
// } packet_to_bootloader_t; 

//process data from f3 to f4
#pragma pack(1)
typedef struct {
  stmbl_talk_header_t header;
  float d_fb;
  float q_fb;
  uint8_t fault;
  uint8_t buf;
  uint16_t padding;
} packet_from_hv_t;

//process data from f4 to f3
#pragma pack(1)
typedef struct {
  stmbl_talk_header_t header;
  float d_cmd;
  float q_cmd;
  float pos;
  float vel;
  union {
    struct {
      uint8_t enable : 1;
      enum packet_to_hv_cmd_type_t {
        VOLT_MODE = 0,
        CURRENT_MODE,
      } cmd_type : 1;
      enum packet_to_hv_phase_type_t {
        PHASE_90_3PH = 0,
        PHASE_90_4PH,
        PHASE_120_3PH,
        PHASE_180_2PH,
        PHASE_180_3PH,
      } phase_type : 3;
      enum packet_to_hv_opcode_t {
        PACKET_TO_HV_OPCODE_NOP = 0,
        PACKET_TO_HV_OPCODE_RESET,
        PACKET_TO_HV_OPCODE_BOOTLOADER,
      } opcode : 3;
      uint8_t buf;
    } flags;
    uint32_t padding;
  };
} packet_to_hv_t;

//config data for f3
#pragma pack(1)
typedef union {
  struct f3_config_data_temp {
    float r;
    float l;
    float psi;
    float cur_p;
    float cur_i;
    float cur_ff;
    float cur_ind;
    float max_y;
    float max_cur;
  } pins;
  float data[sizeof(struct f3_config_data_temp) / 4];
} f3_config_data_t;

//status reply from f3
#pragma pack(1)
typedef union {
  struct f3_state_data_temp {
    float u_fb;
    float v_fb;
    float w_fb;
    float hv_temp;
    float mot_temp;
    float core_temp;
    float dc_volt;
    float pwm_volt;
    float y;
  } pins;
  float data[sizeof(struct f3_state_data_temp) / 4];
} f3_state_data_t;

//check if structs can be send at 5kHz with DATABAUD
_Static_assert(sizeof(packet_to_hv_t) <= DATABAUD / 11 / 5000 - 1 - 5, "to_hv struct to large");
_Static_assert(sizeof(packet_from_hv_t) <= DATABAUD / 11 / 5000 - 1 - 5, "from_hv struct to large");
//check if structs are word aligned
_Static_assert(!(sizeof(packet_to_hv_t) % 4), "to_hv struct not word aligned");
_Static_assert(!(sizeof(packet_from_hv_t) % 4), "from_hv struct not word aligned");
