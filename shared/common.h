#pragma once
#include <stdint.h>

#if __GNUC__ < 5
#error gcc to old (< 5.0)
#endif

#define DATABAUD 3000000  //baudrate used for communication

//data from f3 to f4
#pragma pack(1)
typedef struct {
  float d_fb;
  float q_fb;
  float dc_volt;
  float pwm_volt;
  float value;
  uint16_t addr;
  union {
    uint16_t fault : 1;
    uint16_t foo;
  } flags;
  uint32_t crc;
} packet_from_hv_t;

//data from f4 to f3
#pragma pack(1)
typedef struct {
  float d_cmd;
  float q_cmd;
  float pos;
  float vel;
  float value;
  uint16_t addr;
  union {
    struct {
      uint16_t enable : 1;
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
    } flags;
    uint16_t foo;
  };
  uint32_t crc;
} packet_to_hv_t;

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

#pragma pack(1)
typedef union {
  struct f3_state_data_temp {
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

//check if structs can be send at 5kHz using DATABAUD
_Static_assert(sizeof(packet_to_hv_t) <= DATABAUD / 11 / 5000 - 1 - 5, "to_hv struct to large");
_Static_assert(sizeof(packet_from_hv_t) <= DATABAUD / 11 / 5000 - 1 - 5, "from_hv struct to large");
_Static_assert(!(sizeof(packet_to_hv_t) % 4), "to_hv struct not word aligned");
_Static_assert(!(sizeof(packet_from_hv_t) % 4), "from_hv struct not word aligned");
