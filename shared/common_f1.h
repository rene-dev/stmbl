#pragma once
#include <stdint.h>

#if __GNUC__ < 5
#error gcc to old (< 5.0)
#endif

//#define TROLLER

#define DATABAUD 2250000  //baudrate used for communication

//fixed point calculations signed bit, 9 bit predecimal, 6 bit decimal
#define TOFIXED(a) ((int16_t)((a)*64))
#define TOFLOAT(a) ((float)((a) / 64.0))

#define PWM_RES 2400

//TODO: CRC
typedef struct {
  uint8_t start;  // 255
  uint8_t key;
} packet_header_t;

//data from f1 to f4
#pragma pack(push, 1)
typedef struct {
  int16_t dc_cur;
  int16_t dc_volt;
  int16_t hv_temp;
  uint8_t high_volt : 1;  //hardware hi limit
  uint8_t low_volt : 1;   //hardware low limit
  uint8_t over_cur : 1;   //hardware cur limit
  uint8_t over_temp : 1;  //hardware temp limit
  uint8_t hv_fault : 1;   //iramx fault
  uint8_t sys_fault : 1;  //sys fault, crc error, clock error, watchdog bit, startup failure...
  uint8_t padding : 2;
#ifdef TROLLER
  int16_t a;
  int16_t b;
  int16_t c;
#endif
} from_hv_t;

//data from f4 to f1
typedef struct {
  float a;
  float b;
  float pos;
  uint8_t mode : 4;  //TODO: change to enum
  uint8_t enable : 1;
  uint8_t padding : 3;
} to_hv_t;

typedef struct {
  packet_header_t head;
  to_hv_t data;
} packet_to_hv_t;

typedef struct {
  packet_header_t head;
  from_hv_t data;
} packet_from_hv_t;
#pragma pack(pop)

void buff_packet(packet_header_t *p, uint8_t size);
void unbuff_packet(packet_header_t *p, uint8_t size);

//check if structs can be send at 5kHz using DATABAUD
_Static_assert(sizeof(packet_to_hv_t) <= DATABAUD / 11 / 5000 - 1 - 5, "to_hv struct to large");
_Static_assert(sizeof(packet_from_hv_t) <= DATABAUD / 11 / 5000 - 1 - 5, "from_hv struct to large");


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