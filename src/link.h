#pragma once

typedef enum{
  ENC = 1,
  RES,
  SINCOS,
  STEPDIR,
  UPDOWN,
  PWM,
  FREQ,
  MITSU,
  YASKAWA,
  HYPER,
  SANYO,
  SSERIAL,
  ENDAT,
  BISS,
  SSI
} protocol_t;

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
void set_siemens();
void set_manutec();
void set_bergerlahr();
void set_sankyo();
void set_sanyo();
void set_sanyo_r2();
void set_yaskawa();
void set_mitsubishi();
void set_br();
void set_8MSA3M();
void set_e240();
void set_kollmorgen();
void set_gas();

int update_cmd();
int update_fb();
