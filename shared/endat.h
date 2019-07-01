#include "stdint.h"

#ifndef ENDAT_H
#define ENDAT_h

typedef enum{
  ENDAT_READ_POS   = 0b00000111, // 0b11100000,// 0b000111,
  ENDAT_SELECT_MEM = 0b00001110, // 0b01110000,// 0b001110,
  ENDAT_READ_ADDR  = 0b00100011, // 0b11000100,// 0b100011,
  ENDAT_WRITE_ADDR = 0b00011100, // 0b00111000,// 0b011100,
  ENDAT_RESET      = 0b00101010, // 0b01010100,// 0b101010,
} endat_cmd_t; 

typedef enum{
  ENDAT_MEM_STATE  = 0b10111001, // 0b10011101,// 0b10111001,
  ENDAT_MEM_PARAM0 = 0b10100001, // 0b10000101,// 0b10100001,
  ENDAT_MEM_PARAM1 = 0b10100011, // 0b11000101,// 0b10100011,
  ENDAT_MEM_PARAM2 = 0b10100101, // 0b10100101,// 0b10100101,
  // ENDAT_MEM_PARAM_3 = 0b10100111,
} endat_mem_t;

typedef enum{
  ENDAT_ADDR_ERROR = 0,
  ENDAT_ADDR_WARNING = 1,
  ENDAT_ADDR_POS_LEN = 13,
  ENDAT_ADDR_TYPE = 14,
  ENDAT_ADDR_MULTITURN = 1,
  ENDAT_ADDR_RES_LOW = 4,
  ENDAT_ADDR_RES_HIGH = 5,
  ENDAT_ADDR_MAX_VEL = 0,
} endat_mem_addr_t;

extern const uint64_t bitmask[];

typedef union{
  struct{
    uint16_t ilumination : 1;
    uint16_t amplitude : 1;
    uint16_t pos : 1;
    uint16_t over_voltage : 1;
    uint16_t under_voltage : 1;
    uint16_t over_current : 1;
    uint16_t battery : 1;
  };
  uint16_t reg;
} endat_state_error_t;

typedef union{
  struct{
    uint16_t freq : 1;
    uint16_t temp : 1;
    uint16_t ilumination : 1;
    uint16_t battery : 1;
    uint16_t ref : 1;
  };
  uint16_t reg;
} endat_state_warning_t;

typedef struct{
  uint64_t pos;
  uint64_t mpos;
  uint32_t pos_bits;
  uint32_t pos_len; // param 13
  uint32_t mpos_bits; // param 17
  uint32_t fb_type; // param 14
  uint32_t pos_res; // param 20, 21
  uint32_t max_vel; // param 32
  uint32_t error_bit; // error bit
  // state mem 0
  endat_state_error_t error;
  // state mem 1
  endat_state_warning_t warning;

  endat_mem_t current_mem;
  endat_cmd_t current_cmd;
  uint8_t current_addr;
  uint8_t crc;
  uint16_t current_value;
} endat_data_t;

uint32_t endat_tx(endat_cmd_t cmd, uint8_t p1, uint16_t p2, uint8_t* buf, endat_data_t* data);
uint32_t endat_rx(uint8_t* buf, uint32_t max_len, endat_data_t* data);

inline uint8_t flip8(uint8_t d){
  uint8_t r = d & 1;
  for(int i = 0; i < sizeof(r) * 8 - 1; i++){
    r <<= 1;
    d >>= 1;
    r |= d & 1;
  }

  return(r);
}

inline uint16_t flip16(uint16_t d){
  uint16_t r = d & 1;
  for(int i = 0; i < sizeof(r) * 8 - 1; i++){
    r <<= 1;
    d >>= 1;
    r |= d & 1;
  }

  return(r);
}

inline uint32_t flip32(uint32_t d){
  uint32_t r = d & 1;
  for(int i = 0; i < sizeof(r) * 8 - 1; i++){
    r <<= 1;
    d >>= 1;
    r |= d & 1;
  }

  return(r);
}

inline uint64_t flip64(uint64_t d){
  uint64_t r = d & 1;
  for(int i = 0; i < sizeof(r) * 8 - 1; i++){
    r <<= 1;
    d >>= 1;
    r |= d & 1;
  }

  return(r);
}
#endif