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
void set_bosch4();
void set_bosch1();
void set_kuka();
void set_festo();
void set_manutec();
void set_bergerlahr();
void set_sankyo();
void set_sanyo();
void set_mitsubishi();
void set_br();
