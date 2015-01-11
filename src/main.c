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

int __errno;
volatile float systime_s = 0.0;
void Wait(unsigned int ms);

#define NO 0
#define YES 1

void link_pid(){
	// pos
	link_hal_pins("net0.cmd", "pos_minus0.in0");
	link_hal_pins("net0.fb", "pos_minus0.in1");
	link_hal_pins("pos_minus0.out", "pid0.pos_error");
	link_hal_pins("net0.fb", "auto0.fb_in");
	link_hal_pins("net0.fb_d", "auto0.fb_d_in");


	// vel
	link_hal_pins("net0.cmd", "pderiv0.in");
	link_hal_pins("pderiv0.out", "net0.cmd_d");
	link_hal_pins("net0.cmd_d", "pid0.vel_ext_cmd");
	set_hal_pin("pderiv0.in_lp", 1);
	set_hal_pin("pderiv0.out_lp", 1);
	set_hal_pin("pderiv0.vel_max", 1000.0 / 60.0 * 2.0 * M_PI);
	set_hal_pin("pderiv0.acc_max", 1000.0 / 60.0 * 2.0 * M_PI / 0.005);

	link_hal_pins("net0.fb", "pderiv1.in");
	link_hal_pins("pderiv1.out", "net0.fb_d");
	link_hal_pins("net0.fb_d", "pid0.vel_fb");
	set_hal_pin("pderiv1.in_lp", 1);
	set_hal_pin("pderiv1.out_lp", 1);
	set_hal_pin("pderiv1.vel_max", 1000.0 / 60.0 * 2.0 * M_PI);
	set_hal_pin("pderiv1.acc_max", 1000.0 / 60.0 * 2.0 * M_PI / 0.005);

	// timer pwm
	link_hal_pins("pid0.pwm_cmd", "auto0.pwm_in");
	link_hal_pins("auto0.pwm_out", "p2uvw0.pwm");
	// link_hal_pins("p2uvw0.u", "pwmout0.u");
	// link_hal_pins("p2uvw0.v", "pwmout0.v");
	// link_hal_pins("p2uvw0.w", "pwmout0.w");
	//pwm over uart
	link_hal_pins("p2uvw0.u", "pwm2uart0.u");
	link_hal_pins("p2uvw0.v", "pwm2uart0.v");
	link_hal_pins("p2uvw0.w", "pwm2uart0.w");


	// magpos
	link_hal_pins("auto0.mag_pos_out", "p2uvw0.magpos");

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
	set_hal_pin("pwm2uart0.volt", 130.0);
	set_hal_pin("pwm2uart0.pwm_max", 0.9);

	set_hal_pin("p2uvw0.volt", 130.0);
	set_hal_pin("p2uvw0.pwm_max", 0.9);
	set_hal_pin("pid0.volt", 60.0);
	set_hal_pin("p2uvw0.poles", 1.0);
	set_hal_pin("pid0.enable", 1.0);
}

void link_ac_sync_res(){//bosch
	// cmd
	link_hal_pins("enc0.pos", "net0.cmd");
	set_hal_pin("pderiv0.in_lp", 1);
	set_hal_pin("pderiv0.out_lp", 1);
	set_hal_pin("pderiv0.vel_max", 1000.0 / 60.0 * 2.0 * M_PI);
	set_hal_pin("pderiv0.acc_max", 1000.0 / 60.0 * 2.0 * M_PI / 0.005);

	// fb
	link_hal_pins("res0.pos", "net0.fb");
	set_hal_pin("res0.enable", 1.0);
	set_hal_pin("pderiv1.in_lp", 1);
	set_hal_pin("pderiv1.out_lp", 1);
	set_hal_pin("pderiv1.vel_max", 1000.0 / 60.0 * 2.0 * M_PI);
	set_hal_pin("pderiv1.acc_max", 1000.0 / 60.0 * 2.0 * M_PI / 0.005);

	// res_offset
	//set_hal_pin("ap0.fb_offset_in", -0.64);

	// pole count
	set_hal_pin("auto0.pole_count", 4.0);

	// pid
	set_hal_pin("pid0.pos_p", 100.0);
	set_hal_pin("pid0.pos_lp", 1.0);
	set_hal_pin("pid0.vel_lp", 0.5);
	set_hal_pin("pid0.cur_lp", 0.5);
	set_hal_pin("pid0.vel_max", 1000.0 / 60.0 * 2.0 * M_PI);
	set_hal_pin("pid0.acc_max", 1000.0 / 60.0 * 2.0 * M_PI / 0.005);
}

void link_ac_sync_enc(){//berger lahr
	// cmd
	link_hal_pins("res0.pos", "net0.cmd");
	set_hal_pin("res0.enable", 1.0);
	set_hal_pin("pderiv0.in_lp", 1);
	set_hal_pin("pderiv0.out_lp", 1);
	set_hal_pin("pderiv0.vel_max", 13000.0 / 60.0 * 2.0 * M_PI);
	set_hal_pin("pderiv0.acc_max", 13000.0 / 60.0 * 2.0 * M_PI / 0.005);

	// fb
	link_hal_pins("enc0.pos", "net0.fb");
	set_hal_pin("enc0.res", 4096.0);
	set_hal_pin("res0.enable", 1.0);
	set_hal_pin("pderiv1.in_lp", 1);
	set_hal_pin("pderiv1.out_lp", 1);
	set_hal_pin("pderiv1.vel_max", 13000.0 / 60.0 * 2.0 * M_PI);
	set_hal_pin("pderiv1.acc_max", 13000.0 / 60.0 * 2.0 * M_PI / 0.005);

	// res_offset
	set_hal_pin("ap0.fb_offset_in", 0.0);

	// pole count
	set_hal_pin("ap0.pole_count", 3.0);

	// pid
	set_hal_pin("pid0.pos_p", 80.0);
	set_hal_pin("pid0.pos_lp", 1.0);
	set_hal_pin("pid0.vel_lp", 0.6);
	set_hal_pin("pid0.vel_max", 13000.0 / 60.0 * 2.0 * M_PI);
	set_hal_pin("pid0.acc_max", 13000.0 / 60.0 * 2.0 * M_PI / 0.005);
}

void DMA2_Stream0_IRQHandler(void){ //5kHz
    DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0);
    GPIO_SetBits(GPIOB,GPIO_Pin_4);
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
    GPIO_ResetBits(GPIOB,GPIO_Pin_4);
}

//disabled in setup.c
void TIM8_UP_TIM13_IRQHandler(void){ //20KHz
	TIM_ClearITPendingBit(TIM8, TIM_IT_Update);
	//GPIO_SetBits(GPIOB,GPIO_Pin_3);//messpin
}

int main(void)
{
	float period = 0.0;
	float lasttime = 0.0;


	setup();
	//ADC_SoftwareStartConv(ADC1);

	#include "comps/frt.comp"
	#include "comps/rt.comp"
	#include "comps/nrt.comp"

	#include "comps/pos_minus.comp"
	#include "comps/pwm2uvw.comp"
	//#include "comps/pwmout.comp"
	#include "comps/pwm2uart.comp"

	#include "comps/enc.comp"
	#include "comps/res.comp"
	#include "comps/pid.comp"
	#include "comps/term.comp"
	//#include "comps/sim.comp"
	#include "comps/pderiv.comp"
	#include "comps/pderiv.comp"
	//#include "comps/autophase.comp"
	#include "comps/auto.comp"
	#include "comps/test.comp"

	#include "comps/led.comp"

	//#include "comps/vel_observer.comp"

	set_comp_type("net");
	HAL_PIN(enable) = 0.0;
	HAL_PIN(cmd) = 0.0;
	HAL_PIN(fb) = 0.0;
	HAL_PIN(cmd_d) = 0.0;
	HAL_PIN(fb_d) = 0.0;

	for(int i = 0; i < hal.init_func_count; i++){
		hal.init[i]();
	}

	TIM_Cmd(TIM8, ENABLE);//int
	TIM_Cmd(TIM4, ENABLE);//PWM

	link_pid();
	link_ac_sync_res();
	link_hal_pins("led0.r", "test0.test0");
	link_hal_pins("led0.y", "test0.test1");
	link_hal_pins("led0.g", "test0.test2");

	link_hal_pins("auto0.mag_pos_out", "term0.wave0");
	link_hal_pins("auto0.pwm_out", "term0.wave1");
	link_hal_pins("net0.fb_d", "term0.wave2");
	link_hal_pins("auto0.state", "term0.wave3");
	set_hal_pin("term0.gain2", 10.0);
	set_hal_pin("term0.gain3", 50.0);
	link_hal_pins("auto0.ready", "pid0.enable");


	//set_hal_pin("ap0.start", 1.0);

	// link_hal_pins("sim0.sin", "net0.cmd");
	// link_hal_pins("net0.cmd", "vel_ob0.pos_in");
	// link_hal_pins("net0.cmd", "term0.wave0");
	// link_hal_pins("net0.cmd_d", "term0.wave1");
	// link_hal_pins("vel_ob0.pos_out", "term0.wave2");
	// link_hal_pins("vel_ob0.vel_out", "term0.wave3");
	// link_hal_pins("vel_ob0.trg", "rt0.trg0");
	// set_hal_pin("term0.gain0", 10.0);
	// set_hal_pin("term0.gain1", 10.0);
	// set_hal_pin("term0.gain2", 10.0);
	// set_hal_pin("term0.gain3", 10.0);
	// set_hal_pin("sim0.amp", 1.0);
	// set_hal_pin("sim0.freq", 0.5);
	// set_hal_pin("vel_ob0.alpha", 1.0);
	// set_hal_pin("vel_ob0.beta", 0.1);
	
	//->otg->GCCFG &= ~GCCFG_VBUSBSEN
	Wait(1000);
	#ifdef USBTERM
	UB_USB_CDC_Init();
	#endif


	while(1)  // Do not exit
	{
		Wait(10);
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
