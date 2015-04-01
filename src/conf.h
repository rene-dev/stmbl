#pragma once

#define MAX_NAME 32
typedef char NAME[MAX_NAME];

enum pos_iface{
  MISC = 0,
  ENC0 = 1,
  ENC1 = 2,
  RES1 = 3,
  MITSU0 = 4,
  MITSU1 = 5,
  SINCOS1 = 6,
  STEPDIR0 = 7,
  STEPDIR1 = 8,
  SIN = 9,
  VEL = 10,
  SQUARE = 11
};

enum mot_type{
  1PH_DC = 1,
  1PH_SYNC = 2,
  1PH_ASYNC = 3,
  2PH_SYNC = 4,
  2PH_ASYNC = 5,
  3PH_SYNC = 6,
  3PH_ASYNC = 7
};

typedef struct mot{
  NAME name;
  NAME type;
  pos_iface fb;
  float fb_offset;
  float fb_reverse;
  float fb_pole_count;
  float fb_res;
  float pos_p;
  float acc_p;
  float acc_pi;
  float cur_lp;
  float j;
  float km;
  float r;
  float l;
  float pole_count;
  float max_vel;
  float max_acc;
  float max_force;
  float max_cur;
};

typedef struct conf{
  pos_iface cmd;
  float cmd_reverse;
  float cmd_res;
  mot motor;
};
