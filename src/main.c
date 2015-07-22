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
#include "eeprom.h"
#include "link.h"
#include <math.h>

#include "stm32_ub_usb_cdc.h"

volatile uint16_t rxbuf;
GLOBAL_HAL_PIN(g_amp);
GLOBAL_HAL_PIN(g_vlt);
GLOBAL_HAL_PIN(g_tmp);
GLOBAL_HAL_PIN(g_a);
GLOBAL_HAL_PIN(g_b);
GLOBAL_HAL_PIN(g_c);
GLOBAL_HAL_PIN(rt_time);

int __errno;
volatile double systime_s = 0.0;
void Wait(unsigned int ms);

//Mitsubishi absolute encoder position and buffer
volatile int menc_pos = -1;
volatile uint16_t menc_buf[10];

//5 kHz interrupt for hal. at this point all ADCs have been sampled,
//see setup_res() in setup.c if you are interested in the magic behind this.
void DMA2_Stream0_IRQHandler(void){
    DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0);
    GPIO_SetBits(GPIOB,GPIO_Pin_8);
    int freq = 5000;
    float period = 1.0 / freq;
    //GPIO_ResetBits(GPIOB,GPIO_Pin_3);//messpin
		systime_s += period;

		unsigned int start = SysTick->VAL;

    for(int i = 0; i < hal.rt_func_count; i++){//run all realtime hal functions
        hal.rt[i](period);
    }
		unsigned int end = SysTick->VAL;
		if(start > end){
			PIN(rt_time) = ((float)(start - end)) / RCC_Clocks.HCLK_Frequency;
		}
    GPIO_ResetBits(GPIOB,GPIO_Pin_8);
}

//feedback port UART RX not empty. used for Mitsubishi absolute encoders.
void USART3_IRQHandler(){
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);//reset tx enable
	uint16_t buf;
	if(USART_GetITStatus(USART3, USART_IT_RXNE)){
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
		USART_ClearFlag(USART3, USART_FLAG_RXNE);
		buf = USART3->DR;
		if(menc_pos >= 0 && menc_pos <= 8){
			menc_buf[menc_pos++] = buf;//append data to buffer
		}
	}
	if(USART_GetITStatus(USART3, USART_IT_TC)){
		USART_ClearITPendingBit(USART3, USART_IT_TC);
		USART_ClearFlag(USART3, USART_FLAG_TC);
		buf = USART3->DR;
	}
}

//UART RX not empty interrupt for f1. sets all the hal pins measured on the f1.
void UART_DRV_IRQ(){
	static int32_t datapos = -1;
	static from_hv_t from_hv;
	USART_ClearITPendingBit(UART_DRV, USART_IT_RXNE);
	USART_ClearFlag(UART_DRV, USART_FLAG_RXNE);
	rxbuf = UART_DRV->DR;

	if(rxbuf == 0x154){//start condition
		datapos = 0;
	}else if(datapos >= 0 && datapos < sizeof(from_hv_t)){
		((uint8_t*)&from_hv)[datapos++] = (uint8_t)rxbuf;//append data to buffer
	}
	if(datapos == sizeof(from_hv_t)){//all data received
		datapos = -1;
		PIN(g_amp) = TOFLOAT(from_hv.amp);
		PIN(g_vlt) = TOFLOAT(from_hv.volt);
		PIN(g_tmp) = TOFLOAT(from_hv.temp);
		PIN(g_a) = from_hv.a;
		PIN(g_b) = from_hv.b;
		PIN(g_c) = from_hv.c;
	}
}

int main(void)
{
	float period = 0.0;
	float lasttime = 0.0;

	setup();

	#include "comps/adc.comp"
  #include "comps/fault.comp"
	#include "comps/enc_cmd.comp"
  #include "comps/enc_fb.comp"

  #include "comps/en.comp"
	//#include "comps/res.comp"
	//#include "comps/encm.comp"
	#include "comps/sim.comp"
  #include "comps/stp.comp"

	#include "comps/rev.comp"
	#include "comps/rev.comp"

	#include "comps/cauto.comp"

	#include "comps/pderiv.comp"
	#include "comps/pderiv.comp"

	#include "comps/pid.comp"

	#include "comps/rev.comp"

	#include "comps/curpid.comp"
  #include "comps/pmsm.comp"
  #include "comps/mot.comp"
  #include "comps/idq.comp"

	#include "comps/pwm2uart.comp"

	#include "comps/absavg.comp"

	#include "comps/term.comp"
	#include "comps/led.comp"
	#include "comps/fan.comp"
	#include "comps/brake.comp"


	set_comp_type("net");
	HAL_PIN(enable) = 0.0;
	HAL_PIN(cmd) = 0.0;
	HAL_PIN(fb) = 0.0;
  HAL_PIN(fb_error) = 0.0;
	HAL_PIN(cmd_d) = 0.0;
	HAL_PIN(fb_d) = 0.0;
	HAL_PIN(amp) = 0.0;
	HAL_PIN(vlt) = 0.0;
	HAL_PIN(tmp) = 0.0;
	HAL_PIN(a) = 0.0;
	HAL_PIN(b) = 0.0;
	HAL_PIN(c) = 0.0;
	HAL_PIN(rt_calc_time) = 0.0;

	set_comp_type("conf");
	HAL_PIN(r) = 0.0;
	HAL_PIN(l) = 0.0;
	HAL_PIN(j) = 0.0;
	HAL_PIN(psi) = 0.0;
	HAL_PIN(pole_count) = 0.0;
	HAL_PIN(fb_pole_count) = 0.0;
	HAL_PIN(fb_offset) = 0.0;
	HAL_PIN(pos_p) = 0.0;
	HAL_PIN(acc_p) = 0.0;
	HAL_PIN(acc_pi) = 0.0;
	HAL_PIN(cur_p) = 0.0;
  HAL_PIN(cur_i) = 0.0;
  HAL_PIN(cur_ff) = 1.0;
	HAL_PIN(max_vel) = 0.0;
	HAL_PIN(max_acc) = 0.0;
	HAL_PIN(max_force) = 0.0;
	HAL_PIN(max_cur) = 0.0;
	HAL_PIN(fb_type) = 0.0;
	HAL_PIN(cmd_type) = 0.0;
	HAL_PIN(fb_rev) = 0.0;
	HAL_PIN(cmd_rev) = 0.0;
	HAL_PIN(out_rev) = 0.0;
	HAL_PIN(fb_res) = 1.0;
	HAL_PIN(cmd_res) = 2000.0;
	HAL_PIN(sin_offset) = -17600.0;
	HAL_PIN(cos_offset) = -17661.0;
	HAL_PIN(sin_gain) = 0.0001515;
	HAL_PIN(cos_gain) = 0.00015;

  HAL_PIN(max_volt) = 370.0;
  HAL_PIN(max_temp) = 100.0;
  HAL_PIN(max_pos_error) = M_PI / 2.0;
  HAL_PIN(high_volt) = 350.0;
  HAL_PIN(low_volt) = 12.0;
  HAL_PIN(high_temp) = 80.0;
  HAL_PIN(fan_temp) = 40.0;
  HAL_PIN(autophase) = 1.0;
  HAL_PIN(max_sat) = 0.2;


	g_amp_hal_pin = map_hal_pin("net0.amp");
	g_vlt_hal_pin = map_hal_pin("net0.vlt");
	g_tmp_hal_pin = map_hal_pin("net0.tmp");
	g_a_hal_pin = map_hal_pin("net0.a");
	g_b_hal_pin = map_hal_pin("net0.b");
	g_c_hal_pin = map_hal_pin("net0.c");
	rt_time_hal_pin = map_hal_pin("net0.rt_calc_time");

	for(int i = 0; i < hal.init_func_count; i++){
		hal.init[i]();
	}

	link_pid();

	//set_e240();
	//set_bergerlahr();//pid2: ok
	//set_mitsubishi();//pid2: ok
	//set_festo();
	//set_manutec();
	set_rexroth();//pid2: ok
  //link_hal_pins("enc10.ipos", "rev1.in");
  //set_sankyo();
	//set_hal_pin("res0.reverse", 0.0);
	//set_bosch1();//pid2: ok
	//set_bosch4();//pid2: ok
	//set_sanyo();//pid2: ok
	//set_br();


  //set_cmd_stp();
  //set_cmd_lcnc();


  link_hal_pins("conf0.max_cur", "fault0.max_cur");
  link_hal_pins("conf0.max_volt", "fault0.max_volt");
  link_hal_pins("conf0.max_temp", "fault0.max_temp");
  link_hal_pins("conf0.max_pos_error", "fault0.max_pos_error");
  link_hal_pins("conf0.high_volt", "fault0.high_volt");
  link_hal_pins("conf0.high_temp", "fault0.high_temp");
  link_hal_pins("conf0.low_volt", "fault0.low_volt");
  link_hal_pins("conf0.fan_temp", "fault0.fan_temp");
  link_hal_pins("conf0.autophase", "fault0.phase_on_start");
  link_hal_pins("conf0.max_sat", "fault0.max_sat");

  set_hal_pin("fault0.reset", 0.0);

  link_hal_pins("fault0.phase_start", "cauto0.start");
  link_hal_pins("cauto0.ready", "fault0.phase_ready");

  link_hal_pins("pid0.pos_error", "fault0.pos_error");
  link_hal_pins("pid0.saturated", "fault0.sat");
  link_hal_pins("net0.vlt", "fault0.volt");
  link_hal_pins("net0.tmp", "fault0.temp");
  link_hal_pins("net0.amp", "fault0.amp");
  link_hal_pins("net0.fb_error", "fault0.fb_error");
  link_hal_pins("net0.cmd", "fault0.cmd");
  link_hal_pins("rev1.out", "fault0.fb");
  link_hal_pins("fault0.start_offset", "cauto0.start_offset");

  link_hal_pins("fault0.cur", "pid0.max_cur");
  link_hal_pins("fault0.cur", "cur0.cur_max");

  link_hal_pins("fault0.brake", "brake0.brake");
  link_hal_pins("fault0.fan", "fan0.fan");

  link_hal_pins("fault0.enable_out", "pwm2uart0.enable");
  link_hal_pins("fault0.enable_pid", "pid0.enable");

  link_hal_pins("net0.enable", "fault0.enable");

  link_hal_pins("fault0.led_green", "led0.g");
  link_hal_pins("fault0.led_red", "led0.r");

  link_hal_pins("cauto0.i_d", "curpid0.id_cmd");
  link_hal_pins("pid0.cur_cmd", "curpid0.iq_cmd");
  link_hal_pins("net0.vlt", "curpid0.volt");
  link_hal_pins("conf0.r", "curpid0.rd");
  link_hal_pins("conf0.r", "curpid0.rq");
  link_hal_pins("conf0.l", "curpid0.ld");
  link_hal_pins("conf0.l", "curpid0.lq");
  link_hal_pins("conf0.cur_ff", "curpid0.ff");
  link_hal_pins("conf0.cur_p", "curpid0.kp");
  link_hal_pins("conf0.cur_i", "curpid0.ki");

  link_hal_pins("curpid0.ud", "pmsm0.ud");
  link_hal_pins("curpid0.uq", "pmsm0.uq");
  link_hal_pins("net0.fb_d", "pmsm0.vel");
  link_hal_pins("conf0.r", "pmsm0.r");
  link_hal_pins("conf0.l", "pmsm0.ld");
  link_hal_pins("conf0.l", "pmsm0.lq");
  link_hal_pins("conf0.psi", "pmsm0.psi");
  link_hal_pins("conf0.polecount", "pmsm0.polecount");
  link_hal_pins("pmsm0.id", "curpid0.id_fb");
  link_hal_pins("pmsm0.iq", "curpid0.iq_fb");
  link_hal_pins("pmsm0.torque", "mot0.torque");
  link_hal_pins("conf0.j", "mot0.j");
  link_hal_pins("net0.fb", "mot0.real_pos");
  set_hal_pin("mot0.kp", 0.01);
  link_hal_pins("curpid0.ud", "idq0.ud");
  link_hal_pins("curpid0.uq", "idq0.uq");
  link_hal_pins("cauto0.magpos", "idq0.pos");
  link_hal_pins("conf0.pole_count", "idq0.polecount");
  link_hal_pins("net0.vlt", "idq0.volt");
  link_hal_pins("idq0.a", "pwm2uart0.a");
  link_hal_pins("idq0.b", "pwm2uart0.b");
  link_hal_pins("conf0.pole_count", "pmsm0.polecount");
  set_hal_pin("cauto0.pole_count", 1.0);




	link_hal_pins("pid0.pos_error", "avg0.in");
	set_hal_pin("avg0.ac", 0.0001);


  //set_cur_cmd();





	TIM_Cmd(TIM8, ENABLE);//int

	UB_USB_CDC_Init();

	while(1)//run non realtime stuff
	{
		Wait(2);
		period = systime/1000.0 + (1.0 - SysTick->VAL/RCC_Clocks.HCLK_Frequency)/1000.0 - lasttime;
		lasttime = systime/1000.0 + (1.0 - SysTick->VAL/RCC_Clocks.HCLK_Frequency)/1000.0;
		for(int i = 0; i < hal.nrt_func_count; i++){//run all non realtime hal functions
			hal.nrt[i](period);
		}
	}
}

void Wait(unsigned int ms){
	volatile unsigned int t = systime + ms;
	while(t >= systime){
	}
}
