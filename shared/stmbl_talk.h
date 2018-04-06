#pragma once
#include <stdint.h>

#pragma pack(push, 1)
typedef struct{
    uint32_t crc; // can be 0 for eth comm
    uint8_t slave_addr;   // calc from uid, overwrite in config
    uint8_t len; // #data in byte
    uint8_t conf_addr;
    struct {
        uint8_t read_same_addr : 1;
        uint8_t write_to_conf : 1;
        uint8_t packet_to_master : 1;
        uint8_t error : 1;
        uint8_t counter : 4; // packet counter, mosi: ++, miso: same as request
    } flags;
    union{
      float f32;
      uint32_t u32;
      int32_t i32;
    } config;
} stmbl_talk_header_t;
#pragma pack(pop)


/*
conf[0]
.
.
.
conf[] uint32_t protocol_features {

}
conf[] uint32_t api_key = hash(descr)
conf[] uint32_t *descr = {
    enum {
        FLOAT = "f",
        UINT = "u",
        INT = "i",
    } type : 8;
    enum {
        SIZE_32 = "5",
        SIZE_16 = "4",
        SIZE_8 = "3",
        SIZE_4 = "2",
        SIZE_2 = "1",
        SIZE_1 = "0",
    } size : 8;
    enum {
        COUNT_32 = "5",
        COUNT_16 = "4",
        COUNT_8 = "3",
        COUNT_4 = "2",
        COUNT_2 = "1",
        COUNT_1 = "0",
    } count : 8;
    enum {
        READ = "r",
        WRITE = "w",
    }  read_write : 8;
    char name[32];
}
conf[] uint32_t conf {
    uint8_t min_addr;
    uint8_t max_addr;
    uint4_t reply_div;
    uint4_t reply_phase;
    uint8_t max_reply_len;
}
conf[] uint32_t error {
    enum {
        NO_ERROR,
        API_KEY_MISMATCH,
        LEN_MISMATCH,
        FW_ERROR,
    } error_type : 8;
    uint16_t fw_error;
}
conf[] = cmd{
    RESET,
    START_BOOTLOADER,
}
*/
