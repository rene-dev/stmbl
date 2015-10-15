#pragma once

enum{
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

void link_pid();
void set_rexroth();
void set_bosch1();
void set_bosch2();
void set_bosch3();
void set_bosch4();
void set_baldor();
void set_hauser();
void set_hauser_stefan();
void set_kuka();
void set_festo();
void set_manutec();
void set_bergerlahr();
void set_sankyo();
void set_sanyo();
void set_sanyo_r2();
void set_mitsubishi();
void set_br();
void set_e240();
void set_kollmorgen();

void set_cmd_enc();
void set_cmd_const();
void set_cmd_sin();
void set_cmd_square();
void set_cmd_vel();
void set_cmd_stp();
void set_cmd_stpff();
void set_cmd_lcnc();
void set_cur_cmd();
