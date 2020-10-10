#pragma once
#include <stdint.h>

#pragma pack(push, 1)
typedef struct {
  uint32_t crc;        // can be 0 for eth comm
  uint8_t slave_addr;  // calc from uid, overwrite in config, bootloader = 255
  uint8_t len;         // #data in words
  uint8_t conf_addr;
  struct {
    enum stmbl_talk_cmd_t {
      NO_CMD,
      WRITE_CONF,  // conf[rx.header.conf_addr] = rx.header.config.u32, tx.header.config.u32 = conf[tx.header.conf_addr++]
      READ_CONF,   // tx.header.config.u32 = conf[rx.header.conf_addr]
      DO_RESET,
      BOOTLOADER
    } cmd : 4;
    uint8_t counter : 4;  // packet counter, master: ++, slave: same as request
  } flags;
  union {
    float f32;
    uint32_t u32;
    int32_t i32;
  } config;
} stmbl_talk_header_t;
#pragma pack(pop)
