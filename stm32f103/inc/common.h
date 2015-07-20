#pragma once

#define STATIC_ASSERT(COND, MSG) extern char MSG[(COND)?1:-1]

#define DATABAUD 2000000

#define TOFIXED(a) ((int16_t)(a*64))
#define TOFLOAT(a) ((float)(a/64.0))

typedef struct{
   int16_t amp;
   int16_t volt;
   int16_t temp;
} from_hv_t;


typedef struct{
   int16_t a;
   int16_t b;
} to_hv_t;

STATIC_ASSERT(sizeof(to_hv_t) <= DATABAUD / 11 / 5000 - 1 - 5, to_hv_struct_to_large);
STATIC_ASSERT(sizeof(from_hv_t) <= DATABAUD / 11 / 5000 - 1 - 5, from_hv_struct_to_large);

//#error foo bar

// struct f1tof4{
//   int16 ia;
//   int16 ib;
//   int16 ua;
//   int16 ub;
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
//       u/i mode
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