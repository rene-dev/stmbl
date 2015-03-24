/*
 * This file is part of the stmbl project.
 *
 * Copyright (C) 2013-2015 Rene Hopf <renehopf@mac.com>
 * Copyright (C) 2013-2015 Nico Stute <crinq@crinq.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "stm32f4xx_conf.h"
#include "printf.h"
#include "scanf.h"
#include "hal.h"
#include "setup.h"
#include <math.h>

#ifdef USBTERM
#include "stm32_ub_usb_cdc.h"
#endif

volatile uint16_t rxbuf;
GLOBAL_HAL_PIN(g_amp);
GLOBAL_HAL_PIN(g_vlt);
GLOBAL_HAL_PIN(g_tmp);

int __errno;
volatile float systime_s = 0.0;
void Wait(unsigned int ms);

//menc
volatile int menc_pos = -1;
volatile uint16_t menc_buf[10];

#define NO 0
#define YES 1

void link_pid(){
	link_hal_pins("cauto0.fb_out", "net0.fb");

	link_hal_pins("net0.cmd","pid0.pos_ext_cmd");
	link_hal_pins("net0.fb","pid0.pos_fb");

	// vel
	link_hal_pins("net0.cmd", "pderiv0.in");
	link_hal_pins("pderiv0.out", "net0.cmd_d");
	link_hal_pins("net0.cmd_d", "pid0.vel_ext_cmd");
	set_hal_pin("pderiv0.in_lp", 1.0);
	set_hal_pin("pderiv0.out_lp", 1.0);
	set_hal_pin("pderiv0.vel_max", 1000.0 / 60.0 * 2.0 * M_PI);
	set_hal_pin("pderiv0.acc_max", 1000.0 / 60.0 * 2.0 * M_PI / 0.005);

	link_hal_pins("net0.fb", "pderiv1.in");
	link_hal_pins("pderiv1.out", "net0.fb_d");
	link_hal_pins("net0.fb_d", "pid0.vel_fb");
	link_hal_pins("net0.vlt", "pid0.volt");
	set_hal_pin("pderiv1.in_lp", 1.0);
	set_hal_pin("pderiv1.out_lp", 1.0);
	set_hal_pin("pderiv1.vel_max", 1000.0 / 60.0 * 2.0 * M_PI);
	set_hal_pin("pderiv1.acc_max", 1000.0 / 60.0 * 2.0 * M_PI / 0.005);

	//pwm over uart
	link_hal_pins("net0.vlt", "pwm2uart0.volt");
	link_hal_pins("net0.vlt", "cur0.volt");
	link_hal_pins("pid0.induction", "cur0.induction");

	link_hal_pins("cur0.u", "pwm2uart0.u");
	link_hal_pins("cur0.v", "pwm2uart0.v");
	link_hal_pins("cur0.w", "pwm2uart0.w");

	// term
	link_hal_pins("net0.cmd", "term0.wave0");
	link_hal_pins("net0.fb", "term0.wave1");
	link_hal_pins("net0.cmd_d", "term0.wave2");
	link_hal_pins("net0.fb_d", "term0.wave3");
	set_hal_pin("term0.gain0", 10.0);
	set_hal_pin("term0.gain1", 10.0);
	set_hal_pin("term0.gain2", 1.0);
	set_hal_pin("term0.gain3", 1.0);


	// enable
	link_hal_pins("pid0.enable", "net0.enable");
	set_hal_pin("net0.enable", 1.0);

	// misc
	// set_hal_pin("pwmout0.enable", 0.9);
	// set_hal_pin("pwmout0.volt", 130.0);
	// set_hal_pin("pwmout0.pwm_max", 0.9);

	set_hal_pin("pwm2uart0.enable", 0.9);
	//set_hal_pin("pwm2uart0.volt", 130.0);
	set_hal_pin("pwm2uart0.pwm_max", 0.9);

	set_hal_pin("cur0.pwm_max", 0.9);
	set_hal_pin("pid0.enable", 1.0);


	link_hal_pins("pid0.cur_cmd", "cauto0.i_in");
	link_hal_pins("cauto0.i_out", "cur0.i");

	// magpos
	link_hal_pins("cauto0.magpos", "cur0.magpos");
	link_hal_pins("cauto0.fb_out", "net0.fb");
}

void link_ac_sync_res(){
	link_hal_pins("enc0.pos0", "net0.cmd");
	link_hal_pins("res0.pos", "cauto0.fb_in");
	link_pid();
}

void link_ac_sync_enc(){
	link_hal_pins("enc0.pos0", "net0.cmd");
	link_hal_pins("enc0.pos1", "cauto0.fb_in");
	link_pid();
}

void set_bosch4(){ // achse 4
	link_ac_sync_res();
	// pole count
	set_hal_pin("cauto0.pole_count", 4.0);

	// auto time
	set_hal_pin("cauto0.time", 0.5);

	// auto scale
	set_hal_pin("cauto0.cur", 1.0);

	set_hal_pin("pderiv0.in_lp", 1.0);
	set_hal_pin("pderiv0.out_lp", 1.0);
	set_hal_pin("pderiv0.vel_max", RPM(1000));
	set_hal_pin("pderiv0.acc_max", RPM(1000) / 0.002);


	set_hal_pin("res0.enable", 1.0);
	set_hal_pin("pderiv1.in_lp", 1.0);
	set_hal_pin("pderiv1.out_lp", 1.0);
	set_hal_pin("pderiv1.vel_max", RPM(1000));
	set_hal_pin("pderiv1.acc_max", RPM(1000) / 0.002);

	//pid
	set_hal_pin("pid0.mot_r", 15.0);//typenschild
	set_hal_pin("pid0.mot_l", 0.002);//unknown
	set_hal_pin("pid0.mot_j", 0.000141);//typenschild
	set_hal_pin("pid0.mot_km", 0.2727);//typenschild


	set_hal_pin("pid0.pos_p", 60.0);
	set_hal_pin("pid0.pos_lp", 4000.0);
	set_hal_pin("pid0.vel_p", 1.0);
	set_hal_pin("pid0.vel_lp", 4000.0);
	set_hal_pin("pid0.vel_fb_lp", 4000.0);
	set_hal_pin("pid0.acc_p", 0.015);
	set_hal_pin("pid0.acc_lp", 4000.0);
	set_hal_pin("pid0.acc_pi", 70.0);
	set_hal_pin("pid0.force_p", 1.0);
	set_hal_pin("pid0.force_lp", 4000.0);
	set_hal_pin("pid0.cur_p", 1.0);
	set_hal_pin("pid0.cur_lp", 10000.0);

	set_hal_pin("pid0.max_vel", RPM(1000));
	set_hal_pin("pid0.max_acc", RPM(1000) / 0.002);
	set_hal_pin("pid0.max_force", 1.5);
	set_hal_pin("pid0.max_cur", 5.3);

	set_hal_pin("pid0.vel_limit", RPM(1000));
	set_hal_pin("pid0.acc_limit", RPM(1000) / 0.002);
	set_hal_pin("pid0.force_limit", 1.5);
	set_hal_pin("pid0.cur_limit", 5.3);


	set_hal_pin("cur0.cur_max", 5.3);
	set_hal_pin("cur0.l", 0.002);
	set_hal_pin("cur0.r", 15.0);
	set_hal_pin("cur0.lp", 0.3);
	set_hal_pin("cauto0.start_offset", 1.311);

}

void set_bosch1(){ // achse 1
	link_ac_sync_res();
	
	set_hal_pin("enc0.res0", 2000.0);
	link_hal_pins("enc0.pos0", "lowpass0.in");
	link_hal_pins("lowpass0.out", "net0.cmd");
	set_hal_pin("lowpass0.gain", 0.002);
	set_hal_pin("lowpass0.scale", 100.0);
	
	// pole count
	set_hal_pin("cauto0.pole_count", 4.0);

	// auto time
	set_hal_pin("cauto0.time", 0.5);

	// auto scale
	set_hal_pin("cauto0.cur", 1.0);

	set_hal_pin("pderiv0.in_lp", 1.0);
	set_hal_pin("pderiv0.out_lp", 1.0);
	set_hal_pin("pderiv0.vel_max", RPM(8000));
	set_hal_pin("pderiv0.acc_max", RPM(8000) / 0.002);


	set_hal_pin("res0.enable", 1.0);
	set_hal_pin("pderiv1.in_lp", 1.0);
	set_hal_pin("pderiv1.out_lp", 1.0);
	set_hal_pin("pderiv1.vel_max", RPM(8000));
	set_hal_pin("pderiv1.acc_max", RPM(8000) / 0.002);

	//pid
	set_hal_pin("pid0.mot_r", 4.9);//typenschild
	set_hal_pin("pid0.mot_l", 0.0002);//unknown
	set_hal_pin("pid0.mot_j", KGCM2(0.39));//typenschild 0.39
	set_hal_pin("pid0.mot_km", 0.366);//typenschild


	set_hal_pin("pid0.pos_p", 60.0);
	set_hal_pin("pid0.pos_lp", 4000.0);
	set_hal_pin("pid0.vel_p", 1.0);
	set_hal_pin("pid0.vel_lp", 4000.0);
	set_hal_pin("pid0.vel_fb_lp", 4000.0);
	set_hal_pin("pid0.acc_p", 0.01);
	set_hal_pin("pid0.acc_lp", 4000.0);
	set_hal_pin("pid0.acc_pi", 70.0);
	set_hal_pin("pid0.force_p", 1.0);
	set_hal_pin("pid0.force_lp", 4000.0);
	set_hal_pin("pid0.cur_p", 1.0);
	set_hal_pin("pid0.cur_lp", 10000.0);

	set_hal_pin("pid0.max_vel", RPM(8000));
	set_hal_pin("pid0.max_acc", RPM(8000) / 0.002);
	set_hal_pin("pid0.max_force", 4.8);
	set_hal_pin("pid0.max_cur", 13.0);

	set_hal_pin("pid0.vel_limit", RPM(8000));
	set_hal_pin("pid0.acc_limit", RPM(8000) / 0.002);
	set_hal_pin("pid0.force_limit", 4.8);
	set_hal_pin("pid0.cur_limit", 13.0);


	set_hal_pin("cur0.cur_max", 13.0);
	set_hal_pin("cur0.l", 0.0006);
	set_hal_pin("cur0.r", 4.9);
	set_hal_pin("cur0.lp", 0.0);
	set_hal_pin("cauto0.start_offset", 1.311);
}

void set_kuka(){
	link_ac_sync_res();
	// pole count
	set_hal_pin("cauto0.pole_count", 1.0);

	// auto time
	set_hal_pin("cauto0.time", 0.5);

	// auto scale
	set_hal_pin("cauto0.cur", 1.0);

	set_hal_pin("pderiv0.in_lp", 1.0);
	set_hal_pin("pderiv0.out_lp", 1.0);
	set_hal_pin("pderiv0.vel_max", 1000.0 / 60.0 * 2.0 * M_PI);
	set_hal_pin("pderiv0.acc_max", 1000.0 / 60.0 * 2.0 * M_PI / 0.005);


	set_hal_pin("res0.enable", 1.0);
	set_hal_pin("pderiv1.in_lp", 1.0);
	set_hal_pin("pderiv1.out_lp", 1.0);
	set_hal_pin("pderiv1.vel_max", 1000.0 / 60.0 * 2.0 * M_PI);
	set_hal_pin("pderiv1.acc_max", 1000.0 / 60.0 * 2.0 * M_PI / 0.005);

	// res_offset
	//set_hal_pin("ap0.fb_offset_in", -0.64);
	// pid
	set_hal_pin("pid0.pos_p", 100.0);
	set_hal_pin("pid0.pos_lp", 1.0);
	set_hal_pin("pid0.vel_lp", 1.0);
	set_hal_pin("pid0.cur_lp", 1.0);
	set_hal_pin("pid0.cur_ff", 2.0);//dc wicklungswiederstand
	set_hal_pin("pid0.vel_max", 1000.0 / 60.0 * 2.0 * M_PI);
	set_hal_pin("pid0.acc_max", 1000.0 / 60.0 * 2.0 * M_PI / 0.005);
}

void set_festo(){
	link_ac_sync_res();

	set_hal_pin("enc0.res0", 2000.0);
	set_hal_pin("res0.enable", 1.0);
	set_hal_pin("pderiv0.in_lp", 1);
	set_hal_pin("pderiv0.out_lp", 1);
	set_hal_pin("pderiv0.vel_max", 13000.0 / 60.0 * 2.0 * M_PI);
	set_hal_pin("pderiv0.acc_max", 13000.0 / 60.0 * 2.0 * M_PI / 0.005);

	// fb
	set_hal_pin("res0.enable", 1.0);
	set_hal_pin("pderiv1.in_lp", 1);
	set_hal_pin("pderiv1.out_lp", 1);
	set_hal_pin("pderiv1.vel_max", 13000.0 / 60.0 * 2.0 * M_PI);
	set_hal_pin("pderiv1.acc_max", 13000.0 / 60.0 * 2.0 * M_PI / 0.005);

	set_hal_pin("pderiv0.out_lp", 0.5);
	// pole count
	set_hal_pin("cauto0.pole_count", 4.0);

	// auto time
	set_hal_pin("cauto0.time", 0.5);

	// auto scale
	set_hal_pin("cauto0.cur", 1.0);

	// pid
	set_hal_pin("pid0.pos_p", 35.0);
	set_hal_pin("pid0.vel_p", 1.0);
	set_hal_pin("pid0.vel_i", 50.0);
	set_hal_pin("pid0.pos_lp", 1.0);
	set_hal_pin("pid0.force_p", 1.0);
	set_hal_pin("pid0.cur_ind", 2.34);
	set_hal_pin("pid0.pos_lp", 1.0);
	set_hal_pin("pid0.vel_lp", 1.0);
	set_hal_pin("pid0.cur_ff", 28.0);//dc wicklungswiederstand
	set_hal_pin("pid0.vel_d", 0.0);

	set_hal_pin("pid0.vel_max", 13000.0 / 60.0 * 2.0 * M_PI);
	set_hal_pin("pid0.acc_max", 13000.0 / 60.0 * 2.0 * M_PI / 0.005);
}

void set_manutec(){
	link_ac_sync_enc();

	set_hal_pin("enc0.res0", 2000.0);
	set_hal_pin("res0.enable", 1.0);
	set_hal_pin("pderiv0.in_lp", 1.0);
	set_hal_pin("pderiv0.out_lp", 1.0);
	set_hal_pin("pderiv0.vel_max", 13000.0 / 60.0 * 2.0 * M_PI);
	set_hal_pin("pderiv0.acc_max", 13000.0 / 60.0 * 2.0 * M_PI / 0.005);

	// fb
	set_hal_pin("enc0.res1", 2400.0);
	set_hal_pin("res0.enable", 1.0);
	set_hal_pin("pderiv1.in_lp", 1.0);
	set_hal_pin("pderiv1.out_lp", 1.0);
	set_hal_pin("pderiv1.vel_max", 13000.0 / 60.0 * 2.0 * M_PI);
	set_hal_pin("pderiv1.acc_max", 13000.0 / 60.0 * 2.0 * M_PI / 0.005);

	set_hal_pin("pderiv0.out_lp", 1.0);
	// pole count
	set_hal_pin("cauto0.pole_count", 3.0);

	// auto time
	set_hal_pin("cauto0.time", 0.5);

	// auto scale
	set_hal_pin("cauto0.cur", 1.0);

	// pid
	set_hal_pin("pid0.pos_p", 26.0);
	set_hal_pin("pid0.vel_p", 1.0);
	set_hal_pin("pid0.vel_i", 80.0);
	set_hal_pin("pid0.pos_lp", 1.0);
	set_hal_pin("pid0.vel_lp", 1.0);
	set_hal_pin("pid0.cur_ff", 10.0);
	set_hal_pin("pid0.vel_d", 0.0);

	set_hal_pin("pid0.vel_max", 13000.0 / 60.0 * 2.0 * M_PI);
	set_hal_pin("pid0.acc_max", 13000.0 / 60.0 * 2.0 * M_PI / 0.005);
}

void set_bergerlahr(){
	link_ac_sync_enc();

	set_hal_pin("enc0.res0", 4096.0);
	set_hal_pin("res0.enable", 1.0);
	set_hal_pin("pderiv0.in_lp", 1.0);
	set_hal_pin("pderiv0.out_lp", 1.0);
	set_hal_pin("pderiv0.vel_max", 13000.0 / 60.0 * 2.0 * M_PI);
	set_hal_pin("pderiv0.acc_max", 13000.0 / 60.0 * 2.0 * M_PI / 0.005);

	// fb
	set_hal_pin("enc0.res1", 4096.0);
	set_hal_pin("res0.enable", 1.0);
	set_hal_pin("pderiv1.in_lp", 1.0);
	set_hal_pin("pderiv1.out_lp", 1.0);
	set_hal_pin("pderiv1.vel_max", 13000.0 / 60.0 * 2.0 * M_PI);
	set_hal_pin("pderiv1.acc_max", 13000.0 / 60.0 * 2.0 * M_PI / 0.005);

	set_hal_pin("pderiv0.out_lp", 1.0);
	// pole count
	set_hal_pin("cauto0.pole_count", 3.0);

	// auto time
	set_hal_pin("cauto0.time", 0.5);

	// auto scale
	set_hal_pin("cauto0.cur", 1.0);

	// pid
	set_hal_pin("pid0.mot_r", 23.7);
	set_hal_pin("pid0.mot_l", 0.01);
	set_hal_pin("pid0.mot_j", 0.000026);
	set_hal_pin("pid0.mot_km", 0.3724);

	set_hal_pin("pid0.pos_p", 300.0);
	set_hal_pin("pid0.acc_pi", 20.0);

	set_hal_pin("pid0.max_vel", 1250.0);
	set_hal_pin("pid0.max_acc", 300000.0);
	set_hal_pin("pid0.max_force", 3.0);
	set_hal_pin("pid0.max_cur", 6.0);

	set_hal_pin("pid0.vel_limit", 1250.0);
	set_hal_pin("pid0.acc_limit", 300000.0);
	set_hal_pin("pid0.force_limit", 3.0);
	set_hal_pin("pid0.cur_limit", 6.0);

	set_hal_pin("cur0.cur_max", 10.0);
	set_hal_pin("cur0.l", 0.053);
	set_hal_pin("cur0.r", 23.7);
	set_hal_pin("cauto0.cur", 0.5);

	set_hal_pin("cur0.lp", 0.5);
	set_hal_pin("pid0.cur_p", 1.0);
	set_hal_pin("pid0.force_p", 0.3);
	set_hal_pin("pid0.vel_p", 1.0);
	set_hal_pin("pid0.acc_p", 0.1);
}

//mc101ns
//100w 3000rpm 0.8A
void set_sankyo(){
	link_ac_sync_enc();

	set_hal_pin("enc0.res0", 4096.0);
	set_hal_pin("res0.enable", 1.0);
	set_hal_pin("pderiv0.in_lp", 1.0);
	set_hal_pin("pderiv0.out_lp", 1.0);
	set_hal_pin("pderiv0.vel_max", 13000.0 / 60.0 * 2.0 * M_PI);
	set_hal_pin("pderiv0.acc_max", 13000.0 / 60.0 * 2.0 * M_PI / 0.005);

	// fb
	set_hal_pin("enc0.res1", 8192.0);
	set_hal_pin("res0.enable", 1.0);
	set_hal_pin("pderiv1.in_lp", 1.0);
	set_hal_pin("pderiv1.out_lp", 1.0);
	set_hal_pin("pderiv1.vel_max", 13000.0 / 60.0 * 2.0 * M_PI);
	set_hal_pin("pderiv1.acc_max", 13000.0 / 60.0 * 2.0 * M_PI / 0.005);

	set_hal_pin("pderiv0.out_lp", 1.0);
	// pole count
	set_hal_pin("cauto0.pole_count", 4.0);

	// auto time
	set_hal_pin("cauto0.time", 0.5);

	// auto scale
	set_hal_pin("cauto0.cur", 1.0);

	// pid
	set_hal_pin("pid0.mot_r", 14.0);
	set_hal_pin("pid0.mot_l", 0.01);
	set_hal_pin("pid0.mot_j", 0.000026);
	set_hal_pin("pid0.mot_km", 0.12);

	set_hal_pin("pid0.pos_p", 300.0);
	set_hal_pin("pid0.acc_pi", 20.0);

	set_hal_pin("pid0.max_vel", 1250.0);
	set_hal_pin("pid0.max_acc", 300000.0);
	set_hal_pin("pid0.max_force", 3.0);
	set_hal_pin("pid0.max_cur", 6.0);

	set_hal_pin("pid0.vel_limit", 1250.0);
	set_hal_pin("pid0.acc_limit", 300000.0);
	set_hal_pin("pid0.force_limit", 3.0);
	set_hal_pin("pid0.cur_limit", 6.0);

	set_hal_pin("cur0.cur_max", 10.0);
	set_hal_pin("cur0.l", 0.053);
	set_hal_pin("cur0.r", 14.0);
	set_hal_pin("cauto0.cur", 0.5);

	set_hal_pin("cur0.lp", 0.5);
	set_hal_pin("pid0.cur_p", 1.0);
	set_hal_pin("pid0.force_p", 0.3);
	set_hal_pin("pid0.vel_p", 1.0);
	set_hal_pin("pid0.acc_p", 0.1);
}

//P50B08100DXS
void set_sanyo(){
	link_ac_sync_enc();

	set_hal_pin("enc0.res0", 4096.0);
	set_hal_pin("res0.enable", 1.0);
	set_hal_pin("pderiv0.in_lp", 1.0);
	set_hal_pin("pderiv0.out_lp", 1.0);
	set_hal_pin("pderiv0.vel_max", 13000.0 / 60.0 * 2.0 * M_PI);
	set_hal_pin("pderiv0.acc_max", 13000.0 / 60.0 * 2.0 * M_PI / 0.005);

	// fb
	set_hal_pin("enc0.res1", 16384.0);
	set_hal_pin("enc0.reverse1", 1.0);
	set_hal_pin("res0.enable", 1.0);
	set_hal_pin("pderiv1.in_lp", 1.0);
	set_hal_pin("pderiv1.out_lp", 1.0);
	set_hal_pin("pderiv1.vel_max", 13000.0 / 60.0 * 2.0 * M_PI);
	set_hal_pin("pderiv1.acc_max", 13000.0 / 60.0 * 2.0 * M_PI / 0.005);

	set_hal_pin("pderiv0.out_lp", 1.0);
	// pole count
	set_hal_pin("cauto0.pole_count", 2.0);

	// auto time
	set_hal_pin("cauto0.time", 0.5);

	// auto scale
	set_hal_pin("cauto0.scale", 0.6);

	//pid
	set_hal_pin("pid0.mot_r", 0.8);//0.4 coil => 0.8 total //datasheet
	set_hal_pin("pid0.mot_l", 1.0);//unknown, not used
	set_hal_pin("pid0.mot_j", KGCM2(2.65));//datasheet
	set_hal_pin("pid0.mot_km", 0.553);//datasheet
	set_hal_pin("pid0.max_vel", RPM(4500));//datasheet
	set_hal_pin("pid0.max_acc", RPM(4500)/0.005);
	set_hal_pin("pid0.max_force", 11.76);//datasheet
	set_hal_pin("pid0.max_cur", 25.7);//datasheet

	set_hal_pin("pid0.vel_limit", RPM(4500));
	set_hal_pin("pid0.acc_limit", RPM(4500)/0.005);
	set_hal_pin("pid0.force_limit", 11.76);
	set_hal_pin("pid0.cur_limit", 10.0);


	set_hal_pin("pid0.cur_p", 1.0);
	set_hal_pin("pid0.acc_p", 1.0);
	set_hal_pin("pid0.vel_p", 1.0);
	set_hal_pin("pid0.force_p", 1.0);
	set_hal_pin("pid0.acc_pi", 0.0);
}

void set_mitsubishi(){
	link_pid();
	link_hal_pins("enc0.pos0", "net0.cmd");
	link_hal_pins("encm0.pos", "cauto0.fb_in");
	set_hal_pin("encm0.reverse", 1.0);

	set_hal_pin("enc0.res0", 4096.0);
	set_hal_pin("res0.enable", 1.0);
	set_hal_pin("pderiv0.in_lp", 1.0);
	set_hal_pin("pderiv0.out_lp", 1.0);
	set_hal_pin("pderiv0.vel_max", RPM(8000));
	set_hal_pin("pderiv0.acc_max", RPM(8000) / 0.002);

	// fb
	set_hal_pin("pderiv1.in_lp", 1.0);
	set_hal_pin("pderiv1.out_lp", 1.0);
	set_hal_pin("pderiv1.vel_max", RPM(8000));
	set_hal_pin("pderiv1.acc_max", RPM(8000) / 0.002);

	// pole count
	set_hal_pin("cauto0.pole_count", 2.0);

	// auto time
	set_hal_pin("cauto0.time", 0.5);

	// auto cur
	set_hal_pin("cauto0.cur", 1.0);

	// pid
	set_hal_pin("pid0.mot_r", 1.5);
	set_hal_pin("pid0.mot_l", 0.0025);
	set_hal_pin("pid0.mot_j", 0.00005);
	set_hal_pin("pid0.mot_km", 0.22);

	set_hal_pin("pid0.pos_p", 70.0);
	set_hal_pin("pid0.pos_lp", 4000.0);
	set_hal_pin("pid0.vel_p", 1.0);
	set_hal_pin("pid0.vel_lp", 4000.0);
	set_hal_pin("pid0.vel_fb_lp", 4000.0);
	set_hal_pin("pid0.acc_p", 0.01);
	set_hal_pin("pid0.acc_lp", 4000.0);
	set_hal_pin("pid0.acc_pi", 50.0);
	set_hal_pin("pid0.force_p", 1.0);
	set_hal_pin("pid0.force_lp", 4000.0);
	set_hal_pin("pid0.cur_p", 1.0);
	set_hal_pin("pid0.cur_lp", 10000.0);

	set_hal_pin("pid0.max_vel", RPM(8000));
	set_hal_pin("pid0.max_acc", RPM(8000) / 0.002);
	set_hal_pin("pid0.max_force", 2.0);
	set_hal_pin("pid0.max_cur", 6.0);

	set_hal_pin("pid0.vel_limit", RPM(8000));
	set_hal_pin("pid0.acc_limit", RPM(8000) / 0.002);
	set_hal_pin("pid0.force_limit", 2.0);
	set_hal_pin("pid0.cur_limit", 6.0);


	set_hal_pin("cur0.cur_max", 15.0);
	set_hal_pin("cur0.l", 0.0025);
	set_hal_pin("cur0.r", 1.5);
	set_hal_pin("cur0.lp", 1.0);

	set_hal_pin("cauto0.cur", 0.5);
	set_hal_pin("cauto0.start_offset", 2.633170);

	// pid0.cur_p 0.5
	// pid0.force_p 0.3
	// pid0.acc_p 0.2
	// pid0.vel_p 0.7
	// pid0.pos_p 250
	// pid0.mot_l 0.0025
	// pid0.acc_pi 50


}

void DMA2_Stream0_IRQHandler(void){ //5kHz
    DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0);
    GPIO_SetBits(GPIOB,GPIO_Pin_8);
    int freq = 5000;
    float period = 1.0 / freq;
    //GPIO_ResetBits(GPIOB,GPIO_Pin_3);//messpin
		systime_s += period;

    for(int i = 0; i < hal.fast_rt_func_count; i++){
        hal.fast_rt[i](period);
    }

    for(int i = 0; i < hal.rt_in_func_count; i++){
        hal.rt_in[i](period);
    }

    for(int i = 0; i < hal.rt_filter_func_count; i++){
        hal.rt_filter[i](period);
    }

    for(int i = 0; i < hal.rt_pid_func_count; i++){
        hal.rt_pid[i](period);
    }

    for(int i = 0; i < hal.rt_calc_func_count; i++){
        hal.rt_calc[i](period);
    }

    for(int i = 0; i < hal.rt_out_func_count; i++){
        hal.rt_out[i](period);
    }
    GPIO_ResetBits(GPIOB,GPIO_Pin_8);
}

//disabled in setup.c
void TIM8_UP_TIM13_IRQHandler(void){ //20KHz
	TIM_ClearITPendingBit(TIM8, TIM_IT_Update);
	//GPIO_SetBits(GPIOB,GPIO_Pin_3);//messpin
}

void USART3_IRQHandler(){
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	GPIO_SetBits(GPIOB,GPIO_Pin_9);
	uint16_t buf;
	if(USART_GetITStatus(USART3, USART_IT_RXNE)){
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
		USART_ClearFlag(USART3, USART_FLAG_RXNE);
		buf = USART3->DR;
		if(menc_pos >= 0 && menc_pos <= 8){
			menc_buf[menc_pos++] = buf;
		}
	}
	if(USART_GetITStatus(USART3, USART_IT_TC)){
		USART_ClearITPendingBit(USART3, USART_IT_TC);
		USART_ClearFlag(USART3, USART_FLAG_TC);
		buf = USART3->DR;
	}
	 GPIO_ResetBits(GPIOB,GPIO_Pin_9);
}

void USART2_IRQHandler(){
	static int32_t datapos = -1;
	static data_t data;
	USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	USART_ClearFlag(USART2, USART_FLAG_RXNE);
	rxbuf = USART2->DR;
	//rxbuf = USART_ReceiveData(USART2);

	if(rxbuf == 0x154){//start condition
		datapos = 0;
		//GPIOC->BSRR = (GPIOC->ODR ^ GPIO_Pin_2) | (GPIO_Pin_2 << 16);//grün
	}else if(datapos >= 0 && datapos < DATALENGTH*2){
		data.byte[datapos++] = (uint8_t)rxbuf;
	}
	if(datapos == DATALENGTH*2){//all data received
		datapos = -1;
		PIN(g_amp) = (data.data[0] * AREF / ARES - AREF / (R10 + R11) * R11) / (RCUR * R10) * (R10 + R11);
		PIN(g_vlt) = data.data[1] / ARES * AREF / VDIVDOWN * (VDIVUP + VDIVDOWN);
		if(data.data[2] < ARES && data.data[2] > 0.0)
			PIN(g_tmp) = log10f(data.data[2] * AREF / ARES * TPULLUP / (AREF - data.data[2] * AREF / ARES)) * (-53) + 290;
	}


}

int main(void)
{
	float period = 0.0;
	float lasttime = 0.0;


	setup();
	//ADC_SoftwareStartConv(ADC1);

	//#include "comps/frt.comp"
	//#include "comps/rt.comp"
	//#include "comps/nrt.comp"

	//#include "comps/pos_minus.comp"
	//#include "comps/pwm2uvw.comp"
	//#include "comps/pwmout.comp"
	#include "comps/pwm2uart.comp"

	#include "comps/enc.comp"
	#include "comps/res.comp"
	#include "comps/pid2.comp"
	#include "comps/term.comp"
	#include "comps/sim.comp"
	#include "comps/pderiv.comp"
	#include "comps/pderiv.comp"
	//#include "comps/autophase.comp"
	#include "comps/lowpass.comp"
	#include "comps/cauto.comp"
	#include "comps/encm.comp"
	#include "comps/cur.comp"
	#include "comps/led.comp"

	//#include "comps/vel_observer.comp"

	set_comp_type("net");
	HAL_PIN(enable) = 0.0;
	HAL_PIN(cmd) = 0.0;
	HAL_PIN(fb) = 0.0;
	HAL_PIN(cmd_d) = 0.0;
	HAL_PIN(fb_d) = 0.0;
	HAL_PIN(amp) = 0.0;
	HAL_PIN(vlt) = 0.0;
	HAL_PIN(tmp) = 0.0;

	g_amp = map_hal_pin("net0.amp");
	g_vlt = map_hal_pin("net0.vlt");
	g_tmp = map_hal_pin("net0.tmp");

	for(int i = 0; i < hal.init_func_count; i++){
		hal.init[i]();
	}

	//set_bergerlahr();//pid2: ok
	//set_mitsubishi();//pid2: ok
	//set_festo();
	//set_manutec();
	//set_bosch4();//pid2: ok
	set_bosch1();//pid2: ok
	//set_sanyo();//pid2: ok

	link_hal_pins("cauto0.ready", "led0.g");
	link_hal_pins("cauto0.start", "led0.r");
	//link_hal_pins("led0.g", "test0.test2");

	link_hal_pins("cauto0.ready", "pid0.enable");
	//link_hal_pins("net0.cmd", "auto0.offset");

	set_hal_pin("cauto0.start", 1.0);

	set_hal_pin("led0.y", 1.0);
	TIM_Cmd(TIM8, ENABLE);//int

	Wait(1000);
	#ifdef USBTERM
	UB_USB_CDC_Init();
	#endif

	while(1)  // Do not exit
	{
		Wait(2);
		period = systime/1000.0 + (1.0 - SysTick->VAL/RCC_Clocks.HCLK_Frequency)/1000.0 - lasttime;
		lasttime = systime/1000.0 + (1.0 - SysTick->VAL/RCC_Clocks.HCLK_Frequency)/1000.0;
		for(int i = 0; i < hal.nrt_func_count; i++){
			hal.nrt[i](period);
		}
	}
}

void Wait(unsigned int ms){
	volatile unsigned int t = systime + ms;
	while(t >= systime){
	}
}
