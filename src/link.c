#include "link.h"
#include "hal.h"
#include "misc.h"

void link_pid(){
   // cmd
   link_hal_pins("conf0.cmd_rev", "rev0.rev");

   link_hal_pins("rev0.out", "pderiv0.in");
   link_hal_pins("rev0.out", "net0.cmd");

   link_hal_pins("net0.cmd", "pderiv0.in");
   link_hal_pins("pderiv0.out", "net0.cmd_d");

   link_hal_pins("conf0.max_vel", "stp0.max_vel");
   link_hal_pins("conf0.max_acc", "stp0.max_acc");

   link_hal_pins("conf0.max_vel", "pderiv0.vel_max");
   link_hal_pins("conf0.max_acc", "pderiv0.acc_max");
   set_hal_pin("pderiv0.in_lp", 0.8);


   link_hal_pins("net0.cmd","pid0.pos_ext_cmd");
   link_hal_pins("net0.cmd_d", "pid0.vel_ext_cmd");

   link_hal_pins("conf0.cmd_res", "pderiv0.res");


   // fb
   link_hal_pins("conf0.fb_rev", "rev1.rev");

   link_hal_pins("rev1.out", "vel0.pos_in");
   link_hal_pins("fault0.enable_pid", "vel0.en");

   //link_hal_pins("cauto0.fb_out", "net0.fb");
   //link_hal_pins("net0.fb", "pderiv1.in");
   //link_hal_pins("pderiv1.out", "net0.fb_d");
   link_hal_pins("vel0.pos_out", "cauto0.fb_in");

   link_hal_pins("conf0.j", "vel0.j");
   link_hal_pins("pmsm0.torque", "vel0.torque");
   link_hal_pins("vel0.vel", "net0.fb_d");
   link_hal_pins("cauto0.fb_out", "net0.fb");

   //link_hal_pins("conf0.max_vel", "pderiv1.vel_max");
   //link_hal_pins("conf0.max_acc", "pderiv1.acc_max");

   link_hal_pins("net0.fb","pid0.pos_fb");
   link_hal_pins("net0.fb_d", "pid0.vel_fb");

   //link_hal_pins("conf0.fb_res", "pderiv1.res");

   // fault
   link_hal_pins("conf0.max_dc_cur", "fault0.max_dc_cur");
   link_hal_pins("conf0.max_ac_cur", "fault0.max_ac_cur");
   link_hal_pins("conf0.max_dc_volt", "fault0.max_dc_volt");
   link_hal_pins("conf0.max_hv_temp", "fault0.max_hv_temp");
   link_hal_pins("conf0.max_core_temp", "fault0.max_core_temp");
   link_hal_pins("conf0.max_motor_temp", "fault0.max_motor_temp");
   link_hal_pins("conf0.max_pos_error", "fault0.max_pos_error");
   link_hal_pins("conf0.high_dc_volt", "fault0.high_dc_volt");
   link_hal_pins("conf0.high_hv_temp", "fault0.high_hv_temp");
   link_hal_pins("conf0.high_motor_temp", "fault0.high_motor_temp");
   link_hal_pins("conf0.low_dc_volt", "fault0.low_dc_volt");
   link_hal_pins("conf0.fan_hv_temp", "fault0.fan_hv_temp");
   link_hal_pins("conf0.fan_core_temp", "fault0.fan_core_temp");
   link_hal_pins("conf0.fan_motor_temp", "fault0.fan_motor_temp");
   link_hal_pins("conf0.autophase", "fault0.phase_on_start");
   link_hal_pins("conf0.max_sat", "fault0.max_sat");

   set_hal_pin("fault0.reset", 0.0);

   link_hal_pins("fault0.phase_start", "cauto0.start");
   link_hal_pins("cauto0.ready", "fault0.phase_ready");

   link_hal_pins("pid0.pos_error", "fault0.pos_error");
   link_hal_pins("pid0.saturated", "fault0.sat");
   link_hal_pins("pwm2uart0.dc_volt", "fault0.dc_volt");
   link_hal_pins("pwm2uart0.hv_temp", "fault0.hv_temp");
   link_hal_pins("net0.core_temp0", "fault0.core_temp0");
   link_hal_pins("net0.core_temp1", "fault0.core_temp1");
   link_hal_pins("net0.motor_temp", "fault0.motor_temp");
   link_hal_pins("pwm2uart0.dc_cur", "fault0.dc_cur");
   //link_hal_pins("net0.ac_cur", "fault0.ac_cur");
   link_hal_pins("net0.fb_error", "fault0.fb_error");
   link_hal_pins("net0.cmd", "fault0.cmd");
   link_hal_pins("rev1.out", "fault0.fb");
   link_hal_pins("fault0.start_offset", "cauto0.start_offset");

   link_hal_pins("fault0.cur", "pid0.max_cur");
   link_hal_pins("fault0.cur", "curpid0.max_cur");

   link_hal_pins("fault0.brake", "brake0.brake");
   link_hal_pins("fault0.hv_fan", "fan0.fan");

   link_hal_pins("fault0.enable_out", "pwm2uart0.enable");
   link_hal_pins("fault0.enable_pid", "pid0.enable");

   link_hal_pins("net0.enable", "fault0.enable");

   link_hal_pins("fault0.led_green", "led0.g");
   link_hal_pins("fault0.led_red", "led0.r");


   //

   // cauto
   set_hal_pin("cauto0.time", 0.5);
   set_hal_pin("cauto0.cur", 1.0);
   link_hal_pins("conf0.fb_offset", "cauto0.mag_offset");

   // pid
   link_hal_pins("conf0.j", "pid0.j");
   link_hal_pins("conf0.p", "pid0.p");
   link_hal_pins("conf0.pos_p", "pid0.pos_p");
   link_hal_pins("conf0.vel_p", "pid0.vel_p");
   link_hal_pins("conf0.acc_p", "pid0.acc_p");
   link_hal_pins("conf0.acc_pi", "pid0.acc_pi");
   link_hal_pins("conf0.max_vel", "pid0.max_usr_vel");
   link_hal_pins("conf0.max_acc", "pid0.max_usr_acc");
   link_hal_pins("conf0.max_force", "pid0.max_usr_torque");
   set_hal_pin("pid0.pos_lp", 4000.0);
   set_hal_pin("pid0.vel_lp", 4000.0);
   set_hal_pin("pid0.acc_lp", 4000.0);
   set_hal_pin("pid0.force_p", 1.0);
   set_hal_pin("pid0.force_lp", 4000.0);
   set_hal_pin("pid0.force_ff", 1.0);
   set_hal_pin("pid0.vel_ff", 1.0);
   set_hal_pin("pid0.acc_ff", 1.0);
   set_hal_pin("net0.enable", 1.0);
   link_hal_pins("pid0.torque_cor_cmd", "t2c0.torque");

   // t2c
   link_hal_pins("conf0.polecount", "t2c0.polecount");
   link_hal_pins("conf0.psi", "t2c0.psi");

   // curpid
   link_hal_pins("cauto0.i_d", "curpid0.id_cmd");
   link_hal_pins("t2c0.cur", "curpid0.iq_cmd");
   link_hal_pins("pwm2uart0.dc_volt", "curpid0.dc_volt");
   link_hal_pins("pwm2uart0.ac_volt", "curpid0.ac_volt");
   link_hal_pins("conf0.r", "curpid0.rd");
   link_hal_pins("conf0.r", "curpid0.rq");
   link_hal_pins("conf0.l", "curpid0.ld");
   link_hal_pins("conf0.l", "curpid0.lq");
   link_hal_pins("conf0.cur_ff", "curpid0.ff");
   link_hal_pins("conf0.cur_p", "curpid0.kp");
   link_hal_pins("conf0.cur_i", "curpid0.ki");
   link_hal_pins("conf0.cur_ind", "curpid0.kind");

   // pmsm
   link_hal_pins("curpid0.ud", "pmsm0.ud");
   link_hal_pins("curpid0.uq", "pmsm0.uq");
   link_hal_pins("net0.fb_d", "pmsm0.vel");
   link_hal_pins("conf0.r", "pmsm0.r");
   link_hal_pins("conf0.l", "pmsm0.ld");
   link_hal_pins("conf0.l", "pmsm0.lq");
   link_hal_pins("conf0.psi", "pmsm0.psi");
   link_hal_pins("pmsm0.id", "curpid0.id_fb");
   link_hal_pins("pmsm0.iq", "curpid0.iq_fb");
   link_hal_pins("pmsm0.indd", "curpid0.indd_fb");
   link_hal_pins("pmsm0.indq", "curpid0.indq_fb");
   link_hal_pins("conf0.polecount", "pmsm0.polecount");

   // pmsm_limits
   link_hal_pins("conf0.r", "pmsm_limits0.r");
   link_hal_pins("conf0.l", "pmsm_limits0.ld");
   link_hal_pins("conf0.l", "pmsm_limits0.lq");
   link_hal_pins("conf0.psi", "pmsm_limits0.psi");
   link_hal_pins("conf0.polecount", "pmsm_limits0.polecount");
   link_hal_pins("pwm2uart0.ac_volt", "pmsm_limits0.ac_volt");
   link_hal_pins("pmsm0.indq", "pmsm_limits0.indq");
   link_hal_pins("pmsm0.iq", "pmsm_limits0.iq");
   link_hal_pins("pmsm_limits0.next_max_torque", "pid0.max_torque");
   link_hal_pins("pmsm_limits0.next_min_torque", "pid0.min_torque");
   link_hal_pins("pmsm_limits0.abs_max_vel", "pid0.max_vel");

   // idq, dq
   link_hal_pins("conf0.polecount", "dq0.polecount");
   link_hal_pins("cauto0.pos", "dq0.pos");
   link_hal_pins("pwm2uart0.iu", "dq0.u");
   link_hal_pins("pwm2uart0.iv", "dq0.v");
   link_hal_pins("pwm2uart0.iw", "dq0.w");
   link_hal_pins("curpid0.ud", "idq0.d");
   link_hal_pins("curpid0.uq", "idq0.q");
   link_hal_pins("cauto0.pos", "idq0.pos");
   link_hal_pins("conf0.polecount", "idq0.polecount");
   link_hal_pins("idq0.a", "pwm2uart0.a");
   link_hal_pins("idq0.b", "pwm2uart0.b");


   // misc
   link_hal_pins("conf0.out_rev", "rev2.rev");
   link_hal_pins("cauto0.pos", "rev2.in");
   link_hal_pins("cauto0.i_q", "pid0.cur_ext_cmd");
   link_hal_pins("conf0.cmd_res", "sim0.res");


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
   set_hal_pin("conf0.cmd_res", 40000.0);
   link_hal_pins("enc_cmd0.pos", "rev0.in");
   link_hal_pins("conf0.cmd_res", "enc_cmd0.res");
   set_hal_pin("en0.txen", 0.0);
}

void set_cmd_const(){
   set_hal_pin("rev0.in", 0.0);
}


void set_cmd_sin(){
   link_hal_pins("sim0.msin", "rev0.in");
   link_hal_pins("conf0.cmd_res", "sim0.res");
}

void set_cmd_square(){
   link_hal_pins("sim0.square", "rev0.in");
   link_hal_pins("conf0.cmd_res", "sim0.res");
}

void set_cmd_vel(){
   link_hal_pins("sim0.vel", "rev0.in");
   link_hal_pins("conf0.cmd_res", "sim0.res");
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

void set_cmd_lcnc(){
   link_hal_pins("enc_cmd0.pos", "rev0.in");
   link_hal_pins("conf0.cmd_res", "enc_cmd0.res");
   set_hal_pin("conf0.cmd_res", 14400.0);
   set_hal_pin("conf0.cmd_rev", 1.0);
   link_hal_pins("fault0.enable_out", "en0.en");
   set_hal_pin("en0.txen", 1.0);
}

void set_cur_cmd(){
   set_hal_pin("pwm2uart0.enable", 1.0);
   link_hal_pins("curpid0.iq_cmd", "term0.wave0");
   link_hal_pins("curpid0.iq_fb", "term0.wave1");
   link_hal_pins("curpid0.uq", "term0.wave2");
   link_hal_pins("net0.volt", "term0.wave3");
   link_hal_pins("net0.fb_d", "term0.wave4");
   link_hal_pins("pmsm0.torque", "term0.wave5");
   set_hal_pin("term0.gain0", 100.0);
   set_hal_pin("term0.gain1", 100.0);
   set_hal_pin("term0.gain2", 3.0);
   set_hal_pin("term0.gain3", 3.0);
   set_hal_pin("term0.gain4", 1.0);
   set_hal_pin("term0.gain5", 200.0);
   set_hal_pin("conf0.cur_p", 0.5);
   set_hal_pin("conf0.cur_i", 0.001);
   set_hal_pin("sim0.amp", 0.0);
   link_hal_pins("sim0.square", "curpid0.iq_cmd");
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
   //set_hal_pin("pderiv1.res", 524288.0);
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

void set_fb_sanyo(){
   link_hal_pins("encs0.pos", "rev1.in");
}

void set_rexroth(){
   set_hal_pin("conf0.polecount", 3.0);
   set_hal_pin("conf0.max_vel", RPM(9000));
   set_hal_pin("conf0.max_acc", RPM(9000) / 0.05);
   set_hal_pin("conf0.max_force", 1.8);
   set_hal_pin("conf0.max_ac_cur", 6.8);

   set_hal_pin("conf0.fb_type", SINCOS1);
   set_hal_pin("conf0.fb_rev", 1.0);
   set_hal_pin("conf0.fb_res", 512.0);
   set_hal_pin("conf0.r", 7.5);//mess
   set_hal_pin("conf0.l", 0.008);//mess
   set_hal_pin("conf0.j", 0.000013);//data, 0.000090
   set_hal_pin("conf0.psi", 0.29 / 2.0 / M_PI);//data

   set_hal_pin("conf0.pos_p", 100.0);
   set_hal_pin("conf0.acc_p", 0.2);
   set_hal_pin("conf0.acc_pi", 50.0);
   set_hal_pin("conf0.cur_p", 0.5);
   set_hal_pin("conf0.cur_i", 0.001);
   set_hal_pin("conf0.cur_ff", 1.0);
   set_hal_pin("enc_fb0.res", 512.0);
   set_fb_sincos();
}

void set_baldor(){ // TODO
   set_hal_pin("conf0.polecount", 2.0);
   set_hal_pin("conf0.max_vel", RPM(6000));
   set_hal_pin("conf0.max_acc", RPM(6000) / 0.01);
   set_hal_pin("conf0.max_force", 4.52);
   set_hal_pin("conf0.max_ac_cur", 30.0);

   set_hal_pin("conf0.fb_type", RES1);
   set_hal_pin("conf0.fb_rev", 1.0);
   set_hal_pin("conf0.fb_res", 4096.0);
   set_hal_pin("conf0.fb_offset", 2.395);
   set_hal_pin("conf0.autophase", 0.0);
   set_hal_pin("conf0.r", 0.64);//measured
   set_hal_pin("conf0.l", 0.0016);//measured
   set_hal_pin("conf0.j", 0.0002);//estimated
   set_hal_pin("conf0.psi", 0.36 / 2.0 / M_PI);//calculated

   set_hal_pin("conf0.pos_p", 100.0);
   set_hal_pin("conf0.acc_p", 0.15);
   set_hal_pin("conf0.acc_pi", 150.0);
   set_hal_pin("conf0.cur_p", 0.2);
   set_hal_pin("conf0.cur_ff", 1.0);
   set_hal_pin("conf0.cur_i", 0.0);
   set_hal_pin("conf0.cur_ind", 0.9);
   set_hal_pin("vel0.w", 1000.0);
   set_fb_res();
}

void set_hauser(){ // TODO
   set_hal_pin("conf0.polecount", 3.0);
   set_hal_pin("conf0.max_vel", RPM(3000));
   set_hal_pin("conf0.max_acc", RPM(3000)/0.05);
   set_hal_pin("conf0.max_force", 20.0);
   set_hal_pin("conf0.max_ac_cur", 20.0);

   set_hal_pin("conf0.fb_offset", 0.35);
   set_hal_pin("conf0.autophase", 0.0);
   set_hal_pin("conf0.fb_type", RES1);
   set_hal_pin("conf0.fb_res", 4096.0);
   set_hal_pin("conf0.fb_rev", 0.0);
   set_hal_pin("conf0.r", 0.8);//measured
   set_hal_pin("conf0.l", 0.0041);//measured
   set_hal_pin("conf0.j", 0.0005);//estimated
   set_hal_pin("conf0.psi", 0.52 / 2.0 / M_PI);//calculated

   set_hal_pin("conf0.pos_p", 80.0);
   set_hal_pin("conf0.acc_p", 0.15);
   set_hal_pin("conf0.acc_pi", 100.0);
   set_hal_pin("conf0.cur_p", 0.2);
   set_hal_pin("conf0.cur_i", 0.001);

   set_hal_pin("stp0.max_vel", 50.0);
   set_hal_pin("stp0.max_acc", 100.0);

   set_fb_res();
}

void set_hauser_stefan(){ // TODO
   set_hal_pin("conf0.polecount", 3.0);
   set_hal_pin("conf0.max_vel", RPM(3000));
   set_hal_pin("conf0.max_acc", 10000.0);
   set_hal_pin("conf0.max_force", 20.0);
   set_hal_pin("conf0.max_ac_cur", 20.0);

   set_hal_pin("conf0.fb_offset", 1.05);
   set_hal_pin("conf0.autophase", 0.0);
   set_hal_pin("conf0.fb_type", RES1);
   set_hal_pin("conf0.fb_res", 4096.0);
   set_hal_pin("conf0.fb_rev", 0.0);
   set_hal_pin("conf0.r", 0.8);//measured
   set_hal_pin("conf0.l", 0.0041);//measured
   set_hal_pin("conf0.j", 0.0005);//estimated
   set_hal_pin("conf0.psi", 0.52 / 2.0 / M_PI);//calculated

   set_hal_pin("conf0.pos_p", 80.0);
   set_hal_pin("conf0.acc_p", 0.15);
   set_hal_pin("conf0.acc_pi", 100.0);
   set_hal_pin("conf0.cur_p", 0.2);
   set_hal_pin("conf0.cur_i", 0.001);

   set_hal_pin("stp0.max_vel", 50.0);
   set_hal_pin("stp0.max_acc", 100.0);

   set_fb_res();
}

void set_bosch1(){ // TODO
   set_hal_pin("conf0.polecount", 4.0);
   set_hal_pin("conf0.max_vel", RPM(8000));
   set_hal_pin("conf0.max_acc", RPM(8000) / 0.05);
   set_hal_pin("conf0.max_force", 4.1);
   set_hal_pin("conf0.max_ac_cur", 11.25);

   set_hal_pin("conf0.fb_type", RES1);
   set_hal_pin("conf0.fb_rev", 1.0);
   set_hal_pin("conf0.fb_res", 4096.0);
   set_hal_pin("conf0.r", 2.45);//typenschild
   set_hal_pin("conf0.l", 0.001);//unknown
   set_hal_pin("conf0.j", KGCM2(0.39));//typenschild
   set_hal_pin("conf0.psi", 0.3666 / 2.0 / M_PI);//typenschild

   set_hal_pin("conf0.pos_p", 80.0);
   set_hal_pin("conf0.acc_p", 0.1);
   set_hal_pin("conf0.acc_pi", 50.0);
   set_hal_pin("conf0.cur_p", 0.2);
   set_hal_pin("conf0.cur_i", 0.001);
   set_hal_pin("conf0.autophase", 0.0);
   set_hal_pin("conf0.fb_offset", 2.359);
   set_hal_pin("stp0.max_vel", 50.0);
   set_hal_pin("stp0.max_acc", 500.0);

   set_hal_pin("conf0.sin_offset", -17205.0);
   set_hal_pin("conf0.cos_offset", -17475.0);
   set_hal_pin("conf0.sin_gain", 0.00015);
   set_hal_pin("conf0.cos_gain", 0.00015);
   set_fb_res();
}

void set_bosch2(){ // TODO
   set_hal_pin("conf0.polecount", 4.0);
   set_hal_pin("conf0.max_vel", RPM(11500));
   set_hal_pin("conf0.max_acc", RPM(11500) / 0.05);
   set_hal_pin("conf0.max_force", 0.45);
   set_hal_pin("conf0.max_ac_cur", 2.5);

   set_hal_pin("conf0.fb_type", RES1);
   set_hal_pin("conf0.fb_rev", 1.0);
   set_hal_pin("conf0.fb_res", 4096.0);
   set_hal_pin("conf0.r", 25.5);//typenschild
   set_hal_pin("conf0.l", 0.001);//unknown
   set_hal_pin("conf0.j", KGCM2(0.11));//typenschild
   set_hal_pin("conf0.psi", 0.27 / 2.0 / M_PI);//typenschild

   set_hal_pin("conf0.pos_p", 10.0); // 80
   set_hal_pin("conf0.acc_p", 0.1);
   set_hal_pin("conf0.acc_pi", 10.0); // 20
   set_hal_pin("conf0.cur_p", 0.2); // 0
   set_hal_pin("conf0.cur_i", 0.001); // 0
   set_hal_pin("conf0.autophase", 0.0);
   set_hal_pin("conf0.fb_offset", 2.359);
   set_hal_pin("stp0.max_vel", 150.0);
   set_hal_pin("stp0.max_acc", 750.0);

   set_hal_pin("conf0.sin_offset", -17315.0);
   set_hal_pin("conf0.cos_offset", -17620.0);
   set_hal_pin("conf0.sin_gain", 0.00016);
   set_hal_pin("conf0.cos_gain", 0.00015);
   set_fb_res();
}

void set_bosch3(){ // TODO
   set_hal_pin("conf0.polecount", 4.0);
   set_hal_pin("conf0.max_vel", RPM(11500));
   set_hal_pin("conf0.max_acc", RPM(11500) / 0.05);
   set_hal_pin("conf0.max_force", 0.45);
   set_hal_pin("conf0.max_ac_cur", 2.5);

   set_hal_pin("conf0.fb_type", RES1);
   set_hal_pin("conf0.fb_rev", 1.0);
   set_hal_pin("conf0.fb_res", 4096.0);
   set_hal_pin("conf0.r", 25.5);//typenschild
   set_hal_pin("conf0.l", 0.001);//unknown
   set_hal_pin("conf0.j", KGCM2(0.11));//typenschild
   set_hal_pin("conf0.psi", 0.27 / 2.0 / M_PI);//typenschild

   set_hal_pin("conf0.pos_p", 20.0);
   set_hal_pin("conf0.acc_p", 0.1);
   set_hal_pin("conf0.acc_pi", 20.0);
   set_hal_pin("conf0.cur_p", 0.0);
   set_hal_pin("conf0.cur_i", 0.0);
   set_hal_pin("conf0.autophase", 0.0);
   set_hal_pin("conf0.fb_offset", 2.359);
   set_hal_pin("stp0.max_vel", 150.0);
   set_hal_pin("stp0.max_acc", 2000.0);

   set_hal_pin("conf0.sin_offset", -17295.0);
   set_hal_pin("conf0.cos_offset", -17555.0);
   set_hal_pin("conf0.sin_gain", 0.00015);
   set_hal_pin("conf0.cos_gain", 0.00015);
   set_fb_res();
}

void set_bosch4(){ // TODO
   set_hal_pin("conf0.polecount", 4.0);
   set_hal_pin("conf0.max_vel", RPM(1000));
   set_hal_pin("conf0.max_acc", RPM(1000) / 0.01);
   set_hal_pin("conf0.max_force", 1.5);
   set_hal_pin("conf0.max_ac_cur", 5.3);

   set_hal_pin("conf0.fb_type", RES1);
   set_hal_pin("conf0.fb_rev", 1.0);
   set_hal_pin("conf0.fb_res", 4096.0);
   set_hal_pin("conf0.r", 7.6);//typenschild
   set_hal_pin("conf0.l", 0.001);//unknown
   set_hal_pin("conf0.j", 0.0004);//typenschild 0.000141
   set_hal_pin("conf0.psi", 0.29 / 2.0 / M_PI);//typenschild

   set_hal_pin("conf0.pos_p", 30.0); // 20
   set_hal_pin("conf0.acc_p", 0.1); // 0.15
   set_hal_pin("conf0.acc_pi", 30.0); // 20
   set_hal_pin("conf0.cur_p", 0.1);
   set_hal_pin("conf0.cur_i", 0.001);
   set_hal_pin("conf0.autophase", 0.0);
   set_hal_pin("conf0.fb_offset", 2.359);

   set_hal_pin("conf0.sin_offset", -17470.0);
   set_hal_pin("conf0.cos_offset", -17570.0);
   set_hal_pin("conf0.sin_gain", 0.00015);
   set_hal_pin("conf0.cos_gain", 0.00015);

   set_fb_res();
}

void set_baldor_klein(){ // TODO
   set_hal_pin("brake0.brake", 1.0);
   set_hal_pin("conf0.polecount", 2.0);//hauser 3
   set_hal_pin("conf0.max_vel", RPM(1000));
   set_hal_pin("conf0.max_acc", RPM(1000) / 0.01);
   set_hal_pin("conf0.max_force", 5);
   set_hal_pin("conf0.max_ac_cur", 20);

   set_hal_pin("conf0.fb_type", RES1);
   set_hal_pin("conf0.fb_rev", 1.0);
   set_hal_pin("conf0.fb_res", 4096.0);
   set_hal_pin("conf0.r", 4.7);//typenschild
   set_hal_pin("conf0.l", 0.002);//unknown
   set_hal_pin("conf0.j", 0.000141);//typenschild
   set_hal_pin("conf0.psi", 0.2727 / 2.0 / M_PI);//typenschild

   set_hal_pin("conf0.pos_p", 80.0);
   set_hal_pin("conf0.acc_p", 0.15);
   set_hal_pin("conf0.acc_pi", 50.0);
   set_hal_pin("conf0.cur_lp", 0.4);
   set_fb_res();
}



void set_kuka(){ // TODO
   set_hal_pin("conf0.polecount", 1.0);
   set_hal_pin("conf0.max_vel", RPM(3000));
   set_hal_pin("conf0.max_acc", RPM(3000) / 0.01);
   set_hal_pin("conf0.max_force", 4.8);
   set_hal_pin("conf0.max_ac_cur", 13.0);

   set_hal_pin("conf0.fb_type", RES1);
   set_hal_pin("conf0.fb_rev", 1.0);
   set_hal_pin("conf0.fb_res", 4096.0);
   set_hal_pin("conf0.r", 2.0);
   set_hal_pin("conf0.l", 0.0006);
   set_hal_pin("conf0.j", KGCM2(0.39));
   set_hal_pin("conf0.psi", 0.366 / 2.0 / M_PI);

   set_hal_pin("conf0.pos_p", 60.0);
   set_hal_pin("conf0.acc_p", 0.01);
   set_hal_pin("conf0.acc_pi", 70.0);
   set_hal_pin("conf0.cur_lp", 1.0);
   set_fb_res();
}

void set_festo(){ // TODO
   set_hal_pin("conf0.polecount", 4.0);
   set_hal_pin("conf0.max_vel", RPM(8090));
   set_hal_pin("conf0.max_acc", RPM(8090) / 0.01);
   set_hal_pin("conf0.max_force", 2.8);
   set_hal_pin("conf0.max_ac_cur", 6.4);

   set_hal_pin("conf0.fb_type", RES1);
   set_hal_pin("conf0.fb_rev", 0.0);
   set_hal_pin("conf0.fb_res", 4096.0);
   set_hal_pin("conf0.r", 9.6);//typenschild
   set_hal_pin("conf0.l", 0.0133);//gemessen
   set_hal_pin("conf0.j", KGCM2(0.26));//typenschild
   set_hal_pin("conf0.psi", 0.35 / 2.0 / M_PI);//typenschild

   set_hal_pin("conf0.pos_p", 150.0);
   set_hal_pin("conf0.acc_p", 0.2);
   set_hal_pin("conf0.acc_pi", 100.0);
   set_hal_pin("conf0.cur_p", 0.5);
   set_hal_pin("conf0.cur_i", 0.001);
   set_hal_pin("conf0.cur_ff", 1.0);
   set_hal_pin("conf0.cur_ind", 0.9);
   set_hal_pin("conf0.autophase", 0.0);
   set_hal_pin("conf0.fb_offset", 1.324);
   set_fb_res();
}

void set_sanyo_r2(){ // TODO
   set_hal_pin("conf0.polecount", 5.0);
   set_hal_pin("conf0.max_vel", RPM(6000));
   set_hal_pin("conf0.max_acc", RPM(6000) / 0.01);
   set_hal_pin("conf0.max_force", 0.37);
   set_hal_pin("conf0.max_ac_cur", 2.15);

   set_hal_pin("conf0.fb_rev", 0.0);
   set_hal_pin("conf0.fb_res", 4096.0);
   set_hal_pin("conf0.r", 12.0);//typenschild
   set_hal_pin("conf0.l", 0.0066);//gemessen
   set_hal_pin("conf0.j", 0.0000028);//typenschild
   set_hal_pin("conf0.psi", 0.01904519332);//typenschild

   set_hal_pin("conf0.pos_p", 150.0);
   set_hal_pin("conf0.acc_p", 0.05);
   set_hal_pin("conf0.acc_pi", 100.0);
   set_hal_pin("conf0.cur_p", 0.5);
   set_hal_pin("conf0.cur_i", 0.001);
   set_hal_pin("conf0.cur_ff", 1.0);
   set_hal_pin("conf0.cur_ind", 0.9);
   set_hal_pin("conf0.autophase", 0.0);
   set_hal_pin("conf0.fb_offset", -2.513331);
   set_fb_sanyo();
}


void set_manutec(){ // TODO
   set_hal_pin("conf0.polecount", 3.0);
   set_hal_pin("conf0.max_vel", RPM(8000));
   set_hal_pin("conf0.max_acc", RPM(8000) / 0.002);
   set_hal_pin("conf0.max_force", 4.8);
   set_hal_pin("conf0.max_ac_cur", 13.0);

   set_hal_pin("conf0.fb_type", ENC1);
   set_hal_pin("conf0.fb_rev", 1.0);
   set_hal_pin("conf0.fb_res", 2400.0);
   set_hal_pin("conf0.r", 4.9);
   set_hal_pin("conf0.l", 0.0006);
   set_hal_pin("conf0.j", KGCM2(0.39));
   set_hal_pin("conf0.psi", 0.366 / 2.0 / M_PI);

   set_hal_pin("conf0.pos_p", 50.0);
   set_hal_pin("conf0.acc_p", 0.01);
   set_hal_pin("conf0.acc_pi", 70.0);
   set_hal_pin("conf0.cur_lp", 1.0);
   set_fb_enc();
}

void set_bergerlahr(){ // TODO
   set_hal_pin("conf0.polecount", 3.0);
   set_hal_pin("conf0.max_vel", RPM(12000));
   set_hal_pin("conf0.max_acc", RPM(12000) / 0.01);
   set_hal_pin("conf0.max_force", 3.0);
   set_hal_pin("conf0.max_ac_cur", 6.0);

   set_hal_pin("conf0.fb_type", ENC1); //SINCOS1
   set_hal_pin("conf0.fb_rev", 1.0);
   set_hal_pin("conf0.fb_res", 4096.0);
   set_hal_pin("conf0.r", 23.7);//datenblatt
   set_hal_pin("conf0.l", 0.053);//datenblatt
   set_hal_pin("conf0.j", KGCM2(0.26));//datenblatt
   set_hal_pin("conf0.psi", 0.3724 / 2.0 / M_PI);//datenblatt

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
   set_hal_pin("conf0.polecount", 4.0);
   set_hal_pin("conf0.max_vel", RPM(3000));
   set_hal_pin("conf0.max_acc", RPM(3000) / 0.01);
   set_hal_pin("conf0.max_force", 3.0);
   set_hal_pin("conf0.max_ac_cur", 6.0);

   set_hal_pin("conf0.fb_type", ENC1);
   set_hal_pin("conf0.fb_rev", 1.0);
   set_hal_pin("conf0.fb_res", 8192.0);
   set_hal_pin("conf0.r", 7.0);
   set_hal_pin("conf0.l", 0.013);
   set_hal_pin("conf0.j", KGCM2(0.1));
   set_hal_pin("conf0.psi", 0.04);

   set_hal_pin("conf0.pos_p", 60.0);
   set_hal_pin("conf0.acc_p", 0.01);
   set_hal_pin("conf0.acc_pi", 50.0);
   set_hal_pin("conf0.cur_ff", 1.0);
   set_hal_pin("conf0.cur_p", 0.5);
   set_hal_pin("conf0.cur_i", 0.0);
   set_hal_pin("conf0.max_sat", 0.5);
   set_hal_pin("stp0.max_vel", 40.0);
   set_hal_pin("stp0.max_acc", 2500.0);
   set_fb_enc();
}

//P50B08100DXS
void set_sanyo(){ // TODO
   set_hal_pin("conf0.polecount", 2.0);
   set_hal_pin("conf0.max_vel", RPM(4500));
   set_hal_pin("conf0.max_acc", RPM(4500) / 0.01);
   set_hal_pin("conf0.max_force", 11.76);
   set_hal_pin("conf0.max_ac_cur", 25.7);

   set_hal_pin("conf0.fb_type", ENC1);
   set_hal_pin("conf0.fb_rev", 0.0);
   set_hal_pin("conf0.fb_res", 16384.0);
   set_hal_pin("conf0.r", 0.4);
   set_hal_pin("conf0.l", 0.01);
   set_hal_pin("conf0.j", KGCM2(2.65));
   set_hal_pin("conf0.psi", 0.783 / 2.0 / M_PI);

   set_hal_pin("conf0.pos_p", 60.0);
   set_hal_pin("conf0.acc_p", 0.4);
   set_hal_pin("conf0.acc_pi", 70.0);
   set_hal_pin("conf0.cur_lp", 0.5);
   set_hal_pin("conf0.max_sat", 5.0);
   set_hal_pin("cauto0.cur", 3.0);
   set_hal_pin("cauto0.time", 1.0);
   set_fb_enc();
}

//Mitsubishi HA-FF38-UE-S1
void set_mitsubishi(){ // TODO
   set_hal_pin("conf0.polecount", 2.0);
   set_hal_pin("conf0.max_vel", RPM(8000));
   set_hal_pin("conf0.max_acc", RPM(8000) / 0.01);
   set_hal_pin("conf0.max_force", 2.0);
   set_hal_pin("conf0.max_ac_cur", 6.0);

   //	set_hal_pin("conf0.fb_type", MITSU1);
   set_hal_pin("conf0.fb_rev", 0.0);
   set_hal_pin("conf0.out_rev", 0.0);
   set_hal_pin("conf0.fb_res", 16384.0);
   set_hal_pin("conf0.r", 1.2); // 0.75, 1.2
   set_hal_pin("conf0.l", 0.0025);
   set_hal_pin("conf0.j", KGCM2(0.5));
   set_hal_pin("conf0.psi", 0.3 / 2.0 / M_PI); //0.3

   set_hal_pin("conf0.pos_p", 50.0);
   set_hal_pin("conf0.acc_p", 0.1);
   set_hal_pin("conf0.acc_pi", 50.0);
   set_hal_pin("conf0.cur_p", 0.5);
   set_hal_pin("conf0.cur_i", 0.001);

   set_hal_pin("cauto0.cur", 3.0);
   set_hal_pin("cauto0.mag_offset", 1.638);
   set_hal_pin("conf0.autophase", 0.0);
   set_fb_mitsu();
}

void set_br(){
   set_hal_pin("conf0.polecount", 3.0);
   set_hal_pin("conf0.max_vel", RPM(6000));
   set_hal_pin("conf0.max_acc", RPM(6000) / 0.01);
   set_hal_pin("conf0.max_force", 1.6);
   set_hal_pin("conf0.max_ac_cur", 2.9);

   set_hal_pin("conf0.fb_type", RES1);
   set_hal_pin("conf0.fb_rev", 1.0);
   set_hal_pin("conf0.fb_res", 16384.0);
   set_hal_pin("conf0.r", 51.0);
   set_hal_pin("conf0.l", 0.02);
   set_hal_pin("conf0.j", KGCM2(0.008));
   set_hal_pin("conf0.psi", 0.6 / 2.0 / M_PI);

   set_hal_pin("conf0.pos_p", 60.0);
   set_hal_pin("conf0.acc_p", 0.1);
   set_hal_pin("conf0.acc_pi", 50.0);
   set_hal_pin("conf0.cur_lp", 0.0);
   set_fb_res();
}
//linear 25 ohm
//e240 15
void set_e240(){ // TODO
   set_hal_pin("conf0.polecount", 1.0);
   set_hal_pin("conf0.max_vel", RPM(8000));
   set_hal_pin("conf0.max_acc", RPM(8000) / 0.01);
   set_hal_pin("conf0.max_force", 4.8);
   set_hal_pin("conf0.max_ac_cur", 13.0);

   set_hal_pin("conf0.fb_type", ENC1);
   set_hal_pin("conf0.fb_rev", 0.0);
   set_hal_pin("conf0.fb_res", 2048.0);
   //set_hal_pin("conf0.r", 5.4);
   //set_hal_pin("conf0.l", 0.0082);
   //set_hal_pin("conf0.j", KGCM2(0.268));
   //set_hal_pin("conf0.psi", 0.135);
   set_hal_pin("conf0.r", 25.0);
   set_hal_pin("conf0.l", 0.003);
   set_hal_pin("conf0.j", KGCM2(0.268));
   set_hal_pin("conf0.psi", 0.068);

   set_hal_pin("conf0.pos_p", 45.0);
   set_hal_pin("conf0.acc_p", 0.025);
   set_hal_pin("conf0.acc_pi", 10.0);
   set_hal_pin("conf0.cur_lp", 0.0);
   set_hal_pin("cur0.magpos", 0.0);
   //set_hal_pin("enc0.quad_en0", 1.0);
   set_fb_enc();
}

void set_kollmorgen(){ // b 404 s
   set_hal_pin("conf0.polecount", 2.0);
   set_hal_pin("conf0.max_vel", RPM(5000));
   set_hal_pin("conf0.max_acc", RPM(5000) / 0.01);
   set_hal_pin("conf0.max_force", 35.3);
   set_hal_pin("conf0.max_ac_cur", 55.9);

   set_hal_pin("conf0.fb_type", RES1);
   set_hal_pin("conf0.fb_rev", 0.0);
   set_hal_pin("conf0.fb_res", 4096.0);
   set_hal_pin("conf0.r", 0.34/2.0);
   set_hal_pin("conf0.l", 0.0084/2.0);
   set_hal_pin("conf0.j", 0.000656);
   set_hal_pin("conf0.psi", 0.1);

   set_hal_pin("conf0.pos_p", 150.0);
   set_hal_pin("conf0.acc_p", 0.05);
   set_hal_pin("conf0.acc_pi", 100.0);
   set_hal_pin("conf0.cur_p", 0.5);
   set_hal_pin("conf0.cur_i", 0.001);
   set_hal_pin("conf0.cur_ff", 1.0);
   set_hal_pin("conf0.cur_ind", 0.9);
   set_hal_pin("conf0.autophase", 1.0);
   set_hal_pin("conf0.fb_offset", 1.324);
   set_hal_pin("cauto0.cur", 5);
   set_hal_pin("cauto0.time", 2);
   set_fb_res();
}
