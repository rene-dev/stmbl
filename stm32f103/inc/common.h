#pragma once
#include <stdint.h>

//#define TROLLER

#define STATIC_ASSERT(COND, MSG) extern char MSG[(COND)?1:-1]

#define DATABAUD 2000000 //baudrate used for communication

//fixed point calculations
#define TOFIXED(a) ((int16_t)((a) * 64))
#define TOFLOAT(a) ((float)((a) / 64.0))

#define TOSFLOAT(a) (ABS(a) >= 4.0) ? (((int16_t)((a) * 64)) | 0x4000) : (((int16_t)((a) * 4096.0)) & 0xBFFFF)
#define TOFLOATS(a) ((a) & 0x4000) ? ((float)(((a) & 0xBFFFF) / 64.0)) : ((float)(((a) & 0xBFFFF) / 4096.0))

#define CLAMP(x, low, high)  (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))

#define PWM_RES 2400

typedef struct{
   uint8_t start; // 255
   uint8_t key;
} packet_header_t;

//data from f1 to f4
typedef struct{
   int16_t dc_cur;
   int16_t dc_volt;
   int16_t hv_temp;
#ifdef TROLLER
   int16_t a;
   int16_t b;
   int16_t c;
#endif
} from_hv_t;

//data from f4 to f1
typedef struct{
   int16_t a;
   int16_t b;
} to_hv_t;

typedef struct{
   packet_header_t head;
   to_hv_t data;
} packet_to_hv_t;

typedef struct{
   packet_header_t head;
   from_hv_t data;
} packet_from_hv_t;

void buff_packet(packet_header_t* p, uint8_t size);
void unbuff_packet(packet_header_t* p, uint8_t size);

//check if structs can be send at 5kHz using DATABAUD
STATIC_ASSERT(sizeof(to_hv_t) <= DATABAUD / 11 / 5000 - 1 - 5, to_hv_struct_to_large);
STATIC_ASSERT(sizeof(from_hv_t) <= DATABAUD / 11 / 5000 - 1 - 5, from_hv_struct_to_large);


// struct f1tof4{
//   int16 a;
//   int16 b;
//   int16 udc;
//   int16 value;
//   struct {
//     bool enabled;
//     bool temp_limit;
//     bool current_limit;
//     bool watchdog; // toggle
//     int4 misc;
//   }flags;
//   int8 crc;
// };
//
// struct f4tof1{
//   int16 a; // u/i
//   int16 b; // u/i
//   int16 value;
//   int6 address;
//   /*
//     rw:
//       r
//       l
//       max_i
//       max_temp
//       max_pwm
//       phase_offset_uv
//       phase_offset_vw
//       svm_mode (centered, svm, bottom low)
//       u/i cmd mode
//       u/i fb mode
//
//     r:
//       temp0
//       temp1
//       temp2
//       temp3
//       hw version
//   */
//   bool enable;
//   bool watchdog; // toggle in hal
//   int8 crc;
// };
