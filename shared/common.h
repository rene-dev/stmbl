#pragma once
#include <stdint.h>

#if __GNUC__ < 5
   #error gcc to old (< 5.0)
#endif

#define DATABAUD 2250000 //baudrate used for communication

//fixed point calculations signed bit, 9 bit predecimal, 6 bit decimal
#define TOFIXED(a) ((int16_t)((a) * 64))
#define TOFLOAT(a) ((float)((a) / 64.0))

#define PWM_RES 4800

//TODO: CRC
typedef struct{
   uint8_t start; // 255
   uint8_t key;
} packet_header_t;

//data from f1 to f4
#pragma pack(1)
typedef struct{
   float d_fb;
   float q_fb;
   float dc_volt;
   float pwm_volt;
   float value;
   uint8_t addr    : 7;
   uint8_t fault : 1;
} from_hv_t;

//data from f4 to f1
#pragma pack(1)
typedef struct{
   float d_cmd;
   float q_cmd;
   float pos;
   float vel;
   float value;
   uint8_t addr    : 7;
   uint8_t enable  : 1;
} to_hv_t;

#pragma pack(1)
typedef union {
   struct f3_config_data_temp{
      float mode;
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

#pragma pack(1)
typedef union {
   struct f3_state_data_temp{
      float u_fb;
      float v_fb;
      float w_fb;
      float hv_temp;
      float mot_temp;
      float core_temp;
      float fault;
      float y;
   } pins;
   float data[sizeof(struct f3_state_data_temp) / 4];
} f3_state_data_t;


#pragma pack(1)
typedef struct{
   packet_header_t head;
   to_hv_t data;
} packet_to_hv_t;

#pragma pack(1)
typedef struct{
   packet_header_t head;
   from_hv_t data;
} packet_from_hv_t;

void buff_packet(packet_header_t* p, uint8_t size);
void unbuff_packet(packet_header_t* p, uint8_t size);

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
