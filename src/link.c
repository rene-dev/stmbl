#include "link.h"
#include "hal.h"
#include "misc.h"

void link_pid(){
  // cmd
  link_hal_pins("conf0.cmd_rev", "rev0.rev");

  link_hal_pins("rev0.out", "net0.cmd");
  link_hal_pins("net0.cmd", "pderiv0.in");
  link_hal_pins("pderiv0.out", "net0.cmd_d");

  link_hal_pins("conf0.max_vel", "pderiv0.vel_max");
  link_hal_pins("conf0.max_acc", "pderiv0.acc_max");

  link_hal_pins("net0.cmd","pid0.pos_ext_cmd");
  link_hal_pins("net0.cmd_d", "pid0.vel_ext_cmd");

  link_hal_pins("conf0.cmd_res", "pderiv0.in_res");


  // fb
  link_hal_pins("conf0.fb_rev", "rev1.rev");

  link_hal_pins("rev1.out", "cauto0.fb_in");
  link_hal_pins("cauto0.fb_out", "net0.fb");
  link_hal_pins("net0.fb", "pderiv1.in");
  link_hal_pins("pderiv1.out", "net0.fb_d");

  link_hal_pins("conf0.max_vel", "pderiv1.vel_max");
  link_hal_pins("conf0.max_acc", "pderiv1.acc_max");

  link_hal_pins("net0.fb","pid0.pos_fb");
  link_hal_pins("net0.fb_d", "pid0.vel_fb");

  link_hal_pins("conf0.fb_res", "pderiv1.res");


  // cauto
  link_hal_pins("conf0.pole_count", "cauto0.pole_count");
  set_hal_pin("cauto0.time", 0.5);
	set_hal_pin("cauto0.cur", 1.0);

  // pid
  link_hal_pins("conf0.r", "pid0.mot_r");
  link_hal_pins("conf0.j", "pid0.mot_j");
  link_hal_pins("conf0.km", "pid0.mot_km");
  link_hal_pins("conf0.pos_p", "pid0.pos_p");
  link_hal_pins("conf0.acc_p", "pid0.acc_p");
  link_hal_pins("conf0.acc_pi", "pid0.acc_pi");
  link_hal_pins("conf0.max_vel", "pid0.max_vel");
  link_hal_pins("conf0.max_vel", "pid0.vel_limit");
  link_hal_pins("conf0.max_acc", "pid0.max_acc");
  link_hal_pins("conf0.max_acc", "pid0.acc_limit");
  link_hal_pins("conf0.max_force", "pid0.max_force");
  link_hal_pins("conf0.max_force", "pid0.force_limit");
  link_hal_pins("conf0.max_cur", "pid0.max_cur");
  link_hal_pins("conf0.max_cur", "pid0.cur_limit");
  set_hal_pin("pid0.max_volt", 400.0);
  set_hal_pin("pid0.volt_limit", 400.0);
  set_hal_pin("pid0.max_pwm", 0.95);
  set_hal_pin("pid0.pwm_limit", 0.95);
  set_hal_pin("pid0.pos_lp", 4000.0);
	set_hal_pin("pid0.vel_p", 1.0);
	set_hal_pin("pid0.vel_lp", 4000.0);
	set_hal_pin("pid0.vel_fb_lp", 4000.0);
	set_hal_pin("pid0.acc_lp", 4000.0);
	set_hal_pin("pid0.force_p", 1.0);
	set_hal_pin("pid0.force_lp", 4000.0);
	set_hal_pin("pid0.cur_p", 1.0);

  link_hal_pins("pid0.enable", "net0.enable");
  set_hal_pin("net0.enable", 1.0);
  link_hal_pins("net0.vlt", "pid0.volt");


  // cur
  link_hal_pins("conf0.r", "cur0.r");
  link_hal_pins("conf0.l", "cur0.l");
  link_hal_pins("conf0.out_rev", "rev2.rev");
  link_hal_pins("conf0.max_cur", "cur0.cur_max");
  link_hal_pins("conf0.cur_lp", "cur0.lp");
  link_hal_pins("pid0.induction", "cur0.induction");
  link_hal_pins("cauto0.magpos", "rev2.in");
  link_hal_pins("rev2.out", "cur0.magpos");
  set_hal_pin("cur0.pwm_max", 0.95);
  set_hal_pin("cur0.ind_p", -1.0);
  link_hal_pins("net0.vlt", "cur0.volt");
  link_hal_pins("cur0.u", "pwm2uart0.u");
  link_hal_pins("cur0.v", "pwm2uart0.v");
  link_hal_pins("cur0.w", "pwm2uart0.w");
  link_hal_pins("pid0.cur_cmd", "cur0.iq");
  link_hal_pins("cauto0.i_q", "pid0.cur_ext_cmd");
  link_hal_pins("cauto0.i_d", "cur0.id");


  // pwm2uart
  link_hal_pins("net0.vlt", "pwm2uart0.volt");
  set_hal_pin("pwm2uart0.enable", 0.9);
  set_hal_pin("pwm2uart0.pwm_max", 0.9);


  // term
  link_hal_pins("net0.cmd", "term0.wave0");
  link_hal_pins("net0.fb", "term0.wave1");
  link_hal_pins("net0.cmd_d", "term0.wave2");
  link_hal_pins("net0.fb_d", "term0.wave3");
  set_hal_pin("term0.gain0", 10.0);
  set_hal_pin("term0.gain1", 10.0);
  set_hal_pin("term0.gain2", 1.0);
  set_hal_pin("term0.gain3", 1.0);

  link_hal_pins("conf0.sin_gain", "adc0.sin_gain");
  link_hal_pins("conf0.cos_gain", "adc0.cos_gain");
  link_hal_pins("conf0.sin_offset", "adc0.sin_offset");
  link_hal_pins("conf0.cos_offset", "adc0.cos_offset");


}

void set_cmd_enc(){
  link_hal_pins("enc_cmd0.pos", "rev0.in");
  link_hal_pins("conf0.cmd_res", "enc_cmd0.res");
}

void set_cmd_const(){
  set_hal_pin("rev0.in", 0.0);
}


void set_cmd_sin(){
  link_hal_pins("sim0.msin", "rev0.in");
  link_hal_pins("conf0.cmd_res", "enc_cmd0.res");
}

void set_cmd_square(){
  link_hal_pins("sim0.square", "rev0.in");
  link_hal_pins("conf0.cmd_res", "enc_cmd0.res");
}

void set_cmd_vel(){
  link_hal_pins("sim0.vel", "rev0.in");
  link_hal_pins("conf0.cmd_res", "enc_cmd0.res");
}

void set_cmd_stp(){
  link_hal_pins("stp0.pos_out", "rev0.in");
  link_hal_pins("net0.fb", "stp0.pos_in");
  link_hal_pins("net0.fb_d", "stp0.vel_in");
  //link_hal_pins("conf0.max_vel", "stp0.max_vel");
  set_hal_pin("stp0.max_vel", RPM(200));
  set_hal_pin("stp0.max_acc", RPM(1000) / 0.05);
}

void set_cmd_stpff(){
  link_hal_pins("stp0.pos_out", "rev0.in");
  link_hal_pins("stp0.vel_out", "net0.cmd_d");
  link_hal_pins("stp0.acc_out", "pid0.acc_ext_cmd");
  link_hal_pins("net0.fb", "stp0.pos_in");
  link_hal_pins("net0.fb_d", "stp0.vel_in");
  //link_hal_pins("conf0.max_vel", "stp0.max_vel");
  set_hal_pin("stp0.max_vel", RPM(200));
  set_hal_pin("stp0.max_acc", RPM(1000) / 0.05);
}

void set_fb_enc(){
  link_hal_pins("enc_fb0.pos", "rev1.in");
  link_hal_pins("conf0.fb_res", "enc_fb0.res");
}

void set_fb_sincos(){
  link_hal_pins("adc0.sin3", "enc_fb0.sin");
  link_hal_pins("adc0.cos3", "enc_fb0.cos");

  link_hal_pins("enc_fb0.ipos", "rev1.in");
  link_hal_pins("conf0.fb_res", "enc_fb0.res");
  link_hal_pins("enc_fb0.error", "net0.fb_error");

  set_hal_pin("enc_fb0.ires", 1024.0);
  set_hal_pin("pderiv1.res", 524288.0);
}

void set_fb_res(){
  link_hal_pins("res0.pos", "rev1.in");

  link_hal_pins("adc0.sin", "res0.sin");
  link_hal_pins("adc0.cos", "res0.cos");
  link_hal_pins("res0.error", "net0.fb_error");
  set_hal_pin("adc0.res_en", 1.0);
}

void set_fb_mitsu(){
  link_hal_pins("encm0.pos", "rev1.in");
  link_hal_pins("encm0.error", "net0.fb_error");
}

void set_rexroth(){
	set_hal_pin("conf0.pole_count", 3.0);
	set_hal_pin("conf0.max_vel", RPM(9000));
	set_hal_pin("conf0.max_acc", RPM(9000) / 0.05);
	set_hal_pin("conf0.max_force", 1.8);
	set_hal_pin("conf0.max_cur", 6.8);

	set_hal_pin("conf0.fb_type", SINCOS1);
	set_hal_pin("conf0.fb_rev", 1.0);
	set_hal_pin("conf0.fb_res", 512.0);
	set_hal_pin("conf0.r", 7.5);//mess
	set_hal_pin("conf0.l", 0.008);//mess
	set_hal_pin("conf0.j", 0.000013);//data
	set_hal_pin("conf0.km", 0.29);//data

	set_hal_pin("conf0.pos_p", 80.0);
	set_hal_pin("conf0.acc_p", 0.15);
	set_hal_pin("conf0.acc_pi", 50.0);
	set_hal_pin("conf0.cur_lp", 0.4);
	set_hal_pin("enc_fb0.res", 512.0);
  set_fb_sincos();
}

void set_baldor(){ // TODO
   set_hal_pin("conf0.pole_count", 2.0);
   set_hal_pin("conf0.max_vel", RPM(6000));
   set_hal_pin("conf0.max_acc", RPM(6000) / 0.01);
   set_hal_pin("conf0.max_force", 4.52);
   set_hal_pin("conf0.max_cur", 30.0);

   set_hal_pin("conf0.fb_type", RES1);
   set_hal_pin("conf0.fb_rev", 1.0);
   set_hal_pin("conf0.fb_res", 4096.0);
   set_hal_pin("conf0.r", 0.64);//measured
   set_hal_pin("conf0.l", 0.0016);//measured
   set_hal_pin("conf0.j", 0.0005);//estimated
   set_hal_pin("conf0.km", 0.36);//calculated

   set_hal_pin("conf0.pos_p", 80.0);
   set_hal_pin("conf0.acc_p", 0.15);
   set_hal_pin("conf0.acc_pi", 50.0);
   set_hal_pin("conf0.cur_lp", 0.4);
  set_fb_res();
}


void set_bosch4(){ // TODO
	set_hal_pin("conf0.pole_count", 4.0);
	set_hal_pin("conf0.max_vel", RPM(1000));
	set_hal_pin("conf0.max_acc", RPM(1000) / 0.01);
	set_hal_pin("conf0.max_force", 1.5);
	set_hal_pin("conf0.max_cur", 5.3);

	set_hal_pin("conf0.fb_type", RES1);
	set_hal_pin("conf0.fb_rev", 1.0);
	set_hal_pin("conf0.fb_res", 4096.0);
	set_hal_pin("conf0.r", 15.0);//typenschild
	set_hal_pin("conf0.l", 0.002);//unknown
	set_hal_pin("conf0.j", 0.000141);//typenschild
	set_hal_pin("conf0.km", 0.2727);//typenschild

	set_hal_pin("conf0.pos_p", 80.0);
	set_hal_pin("conf0.acc_p", 0.15);
	set_hal_pin("conf0.acc_pi", 50.0);
	set_hal_pin("conf0.cur_lp", 0.4);
	set_fb_res();
}

void set_baldor_klein(){ // TODO
	set_hal_pin("brake0.brake", 1.0);
	set_hal_pin("conf0.pole_count", 2.0);//hauser 3
	set_hal_pin("conf0.max_vel", RPM(1000));
	set_hal_pin("conf0.max_acc", RPM(1000) / 0.01);
	set_hal_pin("conf0.max_force", 5);
	set_hal_pin("conf0.max_cur", 20);

	set_hal_pin("conf0.fb_type", RES1);
	set_hal_pin("conf0.fb_rev", 1.0);
	set_hal_pin("conf0.fb_res", 4096.0);
	set_hal_pin("conf0.r", 4.7);//typenschild
	set_hal_pin("conf0.l", 0.002);//unknown
	set_hal_pin("conf0.j", 0.000141);//typenschild
	set_hal_pin("conf0.km", 0.2727);//typenschild

	set_hal_pin("conf0.pos_p", 80.0);
	set_hal_pin("conf0.acc_p", 0.15);
	set_hal_pin("conf0.acc_pi", 50.0);
	set_hal_pin("conf0.cur_lp", 0.4);
	set_fb_res();
}


void set_bosch1(){ // TODO
	set_hal_pin("conf0.pole_count", 4.0);
	set_hal_pin("conf0.max_vel", RPM(8000));
	set_hal_pin("conf0.max_acc", RPM(8000) / 0.01);
	set_hal_pin("conf0.max_force", 4.8);
	set_hal_pin("conf0.max_cur", 13.0);
	set_hal_pin("conf0.fb_type", RES1);
	set_hal_pin("conf0.fb_rev", 1.0);
	set_hal_pin("conf0.fb_res", 4096.0);
	set_hal_pin("conf0.r", 4.9);//typenschild
	set_hal_pin("conf0.l", 0.0006);//gemessen
	set_hal_pin("conf0.j", KGCM2(0.39));//typenschild
	set_hal_pin("conf0.km", 0.366);//typenschild

	set_hal_pin("conf0.pos_p", 60.0);
	set_hal_pin("conf0.acc_p", 0.01);
	set_hal_pin("conf0.acc_pi", 70.0);
	set_hal_pin("conf0.cur_lp", 1.0);
  set_fb_res();
}

void set_kuka(){ // TODO
	set_hal_pin("conf0.pole_count", 1.0);
	set_hal_pin("conf0.max_vel", RPM(3000));
	set_hal_pin("conf0.max_acc", RPM(3000) / 0.01);
	set_hal_pin("conf0.max_force", 4.8);
	set_hal_pin("conf0.max_cur", 13.0);

	set_hal_pin("conf0.fb_type", RES1);
	set_hal_pin("conf0.fb_rev", 1.0);
	set_hal_pin("conf0.fb_res", 4096.0);
	set_hal_pin("conf0.r", 2.0);
	set_hal_pin("conf0.l", 0.0006);
	set_hal_pin("conf0.j", KGCM2(0.39));
	set_hal_pin("conf0.km", 0.366);

	set_hal_pin("conf0.pos_p", 60.0);
	set_hal_pin("conf0.acc_p", 0.01);
	set_hal_pin("conf0.acc_pi", 70.0);
	set_hal_pin("conf0.cur_lp", 1.0);
  set_fb_res();
}

void set_festo(){ // TODO
	set_hal_pin("conf0.pole_count", 4.0);
	set_hal_pin("conf0.max_vel", RPM(8090));
	set_hal_pin("conf0.max_acc", RPM(8090) / 0.01);
	set_hal_pin("conf0.max_force", 2.8);
	set_hal_pin("conf0.max_cur", 6.4);

	set_hal_pin("conf0.fb_type", RES1);
	set_hal_pin("conf0.fb_rev", 1.0);
	set_hal_pin("conf0.fb_res", 4096.0);
	set_hal_pin("conf0.r", 9.6);//typenschild
	set_hal_pin("conf0.l", 0.0133);//gemessen
	set_hal_pin("conf0.j", KGCM2(0.26));//typenschild
	set_hal_pin("conf0.km", 0.457);//typenschild

	set_hal_pin("conf0.pos_p", 60.0);
	set_hal_pin("conf0.acc_p", 0.02);
	set_hal_pin("conf0.acc_pi", 70.0);
	set_hal_pin("conf0.cur_lp", 0.0);

	set_hal_pin("brake0.brake", 1.0);
  set_fb_res();
}

void set_manutec(){ // TODO
	set_hal_pin("conf0.pole_count", 3.0);
	set_hal_pin("conf0.max_vel", RPM(8000));
	set_hal_pin("conf0.max_acc", RPM(8000) / 0.002);
	set_hal_pin("conf0.max_force", 4.8);
	set_hal_pin("conf0.max_cur", 13.0);

	set_hal_pin("conf0.fb_type", ENC1);
	set_hal_pin("conf0.fb_rev", 1.0);
	set_hal_pin("conf0.fb_res", 2400.0);
	set_hal_pin("conf0.r", 4.9);
	set_hal_pin("conf0.l", 0.0006);
	set_hal_pin("conf0.j", KGCM2(0.39));
	set_hal_pin("conf0.km", 0.366);

	set_hal_pin("conf0.pos_p", 50.0);
	set_hal_pin("conf0.acc_p", 0.01);
	set_hal_pin("conf0.acc_pi", 70.0);
	set_hal_pin("conf0.cur_lp", 1.0);
  set_fb_enc();
}

void set_bergerlahr(){ // TODO
	set_hal_pin("conf0.pole_count", 3.0);
	set_hal_pin("conf0.max_vel", RPM(12000));
	set_hal_pin("conf0.max_acc", RPM(12000) / 0.01);
	set_hal_pin("conf0.max_force", 3.0);
	set_hal_pin("conf0.max_cur", 6.0);

	set_hal_pin("conf0.fb_type", ENC1); //SINCOS1
	set_hal_pin("conf0.fb_rev", 1.0);
	set_hal_pin("conf0.fb_res", 4096.0);
	set_hal_pin("conf0.r", 23.7);//datenblatt
	set_hal_pin("conf0.l", 0.053);//datenblatt
	set_hal_pin("conf0.j", KGCM2(0.26));//datenblatt
	set_hal_pin("conf0.km", 0.3724);//datenblatt

	set_hal_pin("conf0.pos_p", 60.0);
	set_hal_pin("conf0.acc_p", 0.01);
	set_hal_pin("conf0.acc_pi", 70.0);
	set_hal_pin("conf0.cur_lp", 0.5);

	// set_hal_pin("conf0.pos_p", 80.0);
	// set_hal_pin("conf0.acc_p", 0.15);
	// set_hal_pin("conf0.acc_pi", 50.0);
	// set_hal_pin("conf0.cur_lp", 0.4);
  set_fb_sincos();
}

//mc101ns
//100w 3000rpm 0.8A
void set_sankyo(){ // TODO
	set_hal_pin("conf0.pole_count", 4.0);
	set_hal_pin("conf0.max_vel", RPM(3000));
	set_hal_pin("conf0.max_acc", RPM(3000) / 0.01);
	set_hal_pin("conf0.max_force", 3.0);
	set_hal_pin("conf0.max_cur", 6.0);

	set_hal_pin("conf0.fb_type", ENC1);
	set_hal_pin("conf0.fb_rev", 1.0);
	set_hal_pin("conf0.fb_res", 8192.0);
	set_hal_pin("conf0.r", 14.0);
	set_hal_pin("conf0.l", 0.01);
	set_hal_pin("conf0.j", KGCM2(0.26));
	set_hal_pin("conf0.km", 0.12);

	set_hal_pin("conf0.pos_p", 60.0);
	set_hal_pin("conf0.acc_p", 0.01);
	set_hal_pin("conf0.acc_pi", 70.0);
	set_hal_pin("conf0.cur_lp", 0.5);
  set_fb_enc();
}

//P50B08100DXS
void set_sanyo(){ // TODO
	set_hal_pin("conf0.pole_count", 2.0);
	set_hal_pin("conf0.max_vel", RPM(4500));
	set_hal_pin("conf0.max_acc", RPM(4500) / 0.01);
	set_hal_pin("conf0.max_force", 11.76);
	set_hal_pin("conf0.max_cur", 25.7);

	set_hal_pin("conf0.fb_type", ENC1);
	set_hal_pin("conf0.fb_rev", 1.0);
	set_hal_pin("conf0.fb_res", 16384.0);
	set_hal_pin("conf0.r", 0.8);
	set_hal_pin("conf0.l", 0.01);
	set_hal_pin("conf0.j", KGCM2(2.65));
	set_hal_pin("conf0.km", 0.553);

	set_hal_pin("conf0.pos_p", 60.0);
	set_hal_pin("conf0.acc_p", 0.01);
	set_hal_pin("conf0.acc_pi", 70.0);
	set_hal_pin("conf0.cur_lp", 0.5);
  set_fb_enc();
}

//Mitsubishi HA-FF38-UE-S1
void set_mitsubishi(){ // TODO
  link_hal_pins("encm0.pos", "rev1.in");

	set_hal_pin("conf0.pole_count", 2.0);
	set_hal_pin("conf0.max_vel", RPM(8000));
	set_hal_pin("conf0.max_acc", RPM(8000) / 0.01);
	set_hal_pin("conf0.max_force", 2.0);
	set_hal_pin("conf0.max_cur", 6.0);

//	set_hal_pin("conf0.fb_type", MITSU1);
	set_hal_pin("conf0.fb_rev", 0.0);
  set_hal_pin("conf0.out_rev", 0.0);
	set_hal_pin("conf0.fb_res", 16384.0);
	set_hal_pin("conf0.r", 1.5);
	set_hal_pin("conf0.l", 0.0025);
	set_hal_pin("conf0.j", KGCM2(0.5));
	set_hal_pin("conf0.km", 0.22);

	set_hal_pin("conf0.pos_p", 50.0);
	set_hal_pin("conf0.acc_p", 0.15);
	set_hal_pin("conf0.acc_pi", 50.0);
	set_hal_pin("conf0.cur_lp", 0.5);
	set_hal_pin("encm0.enable", 1.0);

  set_hal_pin("cur0.induction", 0.0);
  set_hal_pin("cauto0.cur", 3.0);
  set_hal_pin("cauto0.mag_offset", 1.638);
  set_hal_pin("conf0.autophase", 0.0);
  set_fb_mitsu();
}

void set_br(){
	set_hal_pin("conf0.pole_count", 3.0);
	set_hal_pin("conf0.max_vel", RPM(6000));
	set_hal_pin("conf0.max_acc", RPM(6000) / 0.01);
	set_hal_pin("conf0.max_force", 1.6);
	set_hal_pin("conf0.max_cur", 2.9);

	set_hal_pin("conf0.fb_type", RES1);
	set_hal_pin("conf0.fb_rev", 1.0);
	set_hal_pin("conf0.fb_res", 16384.0);
	set_hal_pin("conf0.r", 51.0);
	set_hal_pin("conf0.l", 0.02);
	set_hal_pin("conf0.j", KGCM2(0.008));
	set_hal_pin("conf0.km", 0.6);

	set_hal_pin("conf0.pos_p", 60.0);
	set_hal_pin("conf0.acc_p", 0.1);
	set_hal_pin("conf0.acc_pi", 50.0);
	set_hal_pin("conf0.cur_lp", 0.0);
  set_fb_res();
}
//linear 25 ohm
//e250 15
void set_e240(){ // TODO
	set_hal_pin("conf0.pole_count", 1.0);
	set_hal_pin("conf0.max_vel", RPM(8000));
	set_hal_pin("conf0.max_acc", RPM(8000) / 0.01);
	set_hal_pin("conf0.max_force", 4.8);
	set_hal_pin("conf0.max_cur", 13.0);

	set_hal_pin("conf0.fb_type", ENC1);
	set_hal_pin("conf0.fb_rev", 0.0);
	set_hal_pin("conf0.fb_res", 2048.0);
	//set_hal_pin("conf0.r", 5.4);
	//set_hal_pin("conf0.l", 0.0082);
	//set_hal_pin("conf0.j", KGCM2(0.268));
	//set_hal_pin("conf0.km", 0.135);
	set_hal_pin("conf0.r", 25.0);
	set_hal_pin("conf0.l", 0.003);
	set_hal_pin("conf0.j", KGCM2(0.268));
	set_hal_pin("conf0.km", 0.068);

	set_hal_pin("conf0.pos_p", 45.0);
	set_hal_pin("conf0.acc_p", 0.025);
	set_hal_pin("conf0.acc_pi", 10.0);
	set_hal_pin("conf0.cur_lp", 0.0);
	set_hal_pin("cur0.magpos", 0.0);
	//set_hal_pin("enc0.quad_en0", 1.0);
  set_fb_enc();
}
