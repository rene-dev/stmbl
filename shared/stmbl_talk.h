#pragma once
#include <stdint.h>

#pragma pack(1)
typedef struct{
    uint32_t crc;   // can be 0 for eth comm
    uint8_t slave_addr;   // calc from uid, overwrite in config
    uint8_t len;    // #data in byte
    uint8_t conf_addr;
    struct {
        uint8_t incr_read_addr : 1;
        uint8_t write_to_conf : 1;
        uint8_t packet_to_master : 1;
        uint8_t error : 1;  // mosi: clear error, miso: error set
        uint8_t counter : 4;    // packet counter, mosi: ++, miso: same as request
    } flags;
    union{
      float f32;
      uint32_t u32;
    } config;
} stmbl_talk_header_t;