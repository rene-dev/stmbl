#include "link.h"
#include "hal.h"



void link_pid(){
  // cmd
  link_hal_pins("conf0.cmd_type", "mux0.select");
  link_hal_pins("conf0.cmd_rev", "rev0.rev");

  set_hal_pin("mux0.in0", 0.0);
  link_hal_pins("enc0.pos0", "mux0.in1");
  link_hal_pins("enc0.pos1", "mux0.in2");
  link_hal_pins("res0.pos", "mux0.in3");
  set_hal_pin("mux0.in4", 0.0);
  link_hal_pins("encm0.pos", "mux0.in5");
  link_hal_pins("enc0.ipos1", "mux0.in6");
  link_hal_pins("enc0.pos0", "mux0.in7");
  link_hal_pins("enc0.pos1", "mux0.in8");
  link_hal_pins("sim0.sin", "mux0.in9");
  link_hal_pins("sim0.vel", "mux0.in10");
  link_hal_pins("sim0.square", "mux0.in11");

  link_hal_pins("mux0.out", "rev0.in");
  link_hal_pins("rev0.out", "net0.cmd");
  link_hal_pins("net0.cmd", "pderiv0.in");
  link_hal_pins("pderiv0.out", "net0.cmd_d");

  link_hal_pins("conf0.max_vel", "pderiv0.vel_max");
  link_hal_pins("conf0.max_acc", "pderiv0.acc_max");

  link_hal_pins("net0.cmd","pid0.pos_ext_cmd");
  link_hal_pins("net0.cmd_d", "pid0.vel_ext_cmd");

  link_hal_pins("conf0.cmd_res", "pderiv0.in_res");


  // fb
  link_hal_pins("conf0.fb_type", "mux1.select");
  link_hal_pins("conf0.fb_rev", "rev1.rev");

  set_hal_pin("mux1.in0", 0.0);
  link_hal_pins("enc0.pos0", "mux1.in1");
  link_hal_pins("enc0.pos1", "mux1.in2");
  link_hal_pins("res0.pos", "mux1.in3");
  set_hal_pin("mux1.in4", 0.0);
  link_hal_pins("encm0.pos", "mux1.in5");
  link_hal_pins("enc0.ipos1", "mux1.in6");
  link_hal_pins("enc0.pos0", "mux1.in7");
  link_hal_pins("enc0.pos1", "mux1.in8");
  link_hal_pins("sim0.sin", "mux1.in9");
  link_hal_pins("sim0.vel", "mux1.in10");
  link_hal_pins("sim0.square", "mux1.in11");

  link_hal_pins("mux1.out", "rev1.in");
  link_hal_pins("rev1.out", "cauto0.fb_in");
  link_hal_pins("cauto0.fb_out", "net0.fb");
  link_hal_pins("net0.fb", "pderiv1.in");
  link_hal_pins("pderiv1.out", "net0.fb_d");

  link_hal_pins("conf0.max_vel", "pderiv1.vel_max");
  link_hal_pins("conf0.max_acc", "pderiv1.acc_max");

  link_hal_pins("net0.fb","pid0.pos_fb");
  link_hal_pins("net0.fb_d", "pid0.vel_fb");

  link_hal_pins("conf0.fb_res", "pderiv1.in_res");


  // cauto
  link_hal_pins("conf0.pole_count", "cauto0.pole_count");
  link_hal_pins("cauto0.ready", "pid0.force_en");
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
  link_hal_pins("conf0.max_cur", "cur0.cur_max");
  link_hal_pins("conf0.cur_lp", "cur0.lp");
  link_hal_pins("pid0.induction", "cur0.induction");
  link_hal_pins("cauto0.magpos", "cur0.magpos");
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
}
