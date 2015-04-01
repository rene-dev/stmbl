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

#ifdef USBTERM
#include "stm32_ub_usb_cdc.h"
#endif

volatile uint16_t rxbuf;
GLOBAL_HAL_PIN(g_amp);
GLOBAL_HAL_PIN(g_vlt);
GLOBAL_HAL_PIN(g_tmp);
GLOBAL_HAL_PIN(rt_time);


int __errno;
volatile double systime_s = 0.0;
void Wait(unsigned int ms);

//menc
volatile int menc_pos = -1;
volatile uint16_t menc_buf[10];

#define NO 0
#define YES 1


void set_cmd_enc0(){
	set_hal_pin("conf0.cmd_type", 1.0);
}

void set_cmd_enc1(){
	set_hal_pin("conf0.cmd_type", 2.0);
}

void set_cmd_res1(){
	set_hal_pin("conf0.cmd_type", 3.0);
}

void set_cmd_mitsu0(){
	set_hal_pin("conf0.cmd_type", 4.0);
}

void set_cmd_mtisu1(){
	set_hal_pin("conf0.cmd_type", 5.0);
}

void set_cmd_sincos1(){
	set_hal_pin("conf0.cmd_type", 6.0);
}

void set_cmd_stepdir0(){
	set_hal_pin("conf0.cmd_type", 7.0);
}

void set_cmd_stepdir1(){
	set_hal_pin("conf0.cmd_type", 8.0);
}

void set_cmd_sin(){
	set_hal_pin("conf0.cmd_type", 9.0);
}

void set_cmd_vel(){
	set_hal_pin("conf0.cmd_type", 10.0);
}

void set_cmd_square(){
	set_hal_pin("conf0.cmd_type", 11.0);
}

void set_bosch4(){ // TODO
	set_hal_pin("conf0.pole_count", 4.0);
	set_hal_pin("conf0.max_vel", RPM(1000));
	set_hal_pin("conf0.max_acc", RPM(1000) / 0.002);
	set_hal_pin("conf0.max_force", 1.5);
	set_hal_pin("conf0.max_cur", 5.3);

	set_hal_pin("conf0.fb_type", 3.0);
	set_hal_pin("conf0.fb_rev", 1.0);
	set_hal_pin("conf0.fb_res", 1.0);
	set_hal_pin("conf0.r", 15.0);//typenschild
	set_hal_pin("conf0.l", 0.002);//unknown
	set_hal_pin("conf0.j", 0.000141);//typenschild
	set_hal_pin("conf0.km", 0.2727);//typenschild

	set_hal_pin("conf0.pos_p", 80.0);
	set_hal_pin("conf0.acc_p", 0.15);
	set_hal_pin("conf0.acc_pi", 50.0);
	set_hal_pin("conf0.cur_lp", 0.4);
}

void set_bosch1(){ // TODO
	set_hal_pin("conf0.pole_count", 4.0);
	set_hal_pin("conf0.max_vel", RPM(8000));
	set_hal_pin("conf0.max_acc", RPM(8000) / 0.002);
	set_hal_pin("conf0.max_force", 4.8);
	set_hal_pin("conf0.max_cur", 13.0);

	set_hal_pin("conf0.fb_type", 3.0);
	set_hal_pin("conf0.fb_rev", 1.0);
	set_hal_pin("conf0.fb_res", 1.0);
	set_hal_pin("conf0.r", 4.9);//typenschild
	set_hal_pin("conf0.l", 0.0006);//gemessen
	set_hal_pin("conf0.j", KGCM2(0.39));//typenschild
	set_hal_pin("conf0.km", 0.366);//typenschild

	set_hal_pin("conf0.pos_p", 60.0);
	set_hal_pin("conf0.acc_p", 0.01);
	set_hal_pin("conf0.acc_pi", 70.0);
	set_hal_pin("conf0.cur_lp", 1.0);
}

void set_kuka(){ // TODO
	set_hal_pin("conf0.pole_count", 1.0);
	set_hal_pin("conf0.max_vel", RPM(3000));
	set_hal_pin("conf0.max_acc", RPM(3000) / 0.002);
	set_hal_pin("conf0.max_force", 4.8);
	set_hal_pin("conf0.max_cur", 13.0);

	set_hal_pin("conf0.fb_type", 3.0);
	set_hal_pin("conf0.fb_rev", 1.0);
	set_hal_pin("conf0.fb_res", 1.0);
	set_hal_pin("conf0.r", 2.0);
	set_hal_pin("conf0.l", 0.0006);
	set_hal_pin("conf0.j", KGCM2(0.39));
	set_hal_pin("conf0.km", 0.366);

	set_hal_pin("conf0.pos_p", 60.0);
	set_hal_pin("conf0.acc_p", 0.01);
	set_hal_pin("conf0.acc_pi", 70.0);
	set_hal_pin("conf0.cur_lp", 1.0);
}

void set_festo(){ // TODO
	set_hal_pin("conf0.pole_count", 4.0);
	set_hal_pin("conf0.max_vel", RPM(8090));
	set_hal_pin("conf0.max_acc", RPM(8090) / 0.002);
	set_hal_pin("conf0.max_force", 2.8);
	set_hal_pin("conf0.max_cur", 6.4);

	set_hal_pin("conf0.fb_type", 3.0);
	set_hal_pin("conf0.fb_rev", 1.0);
	set_hal_pin("conf0.fb_res", 1.0);
	set_hal_pin("conf0.r", 9.6);//typenschild
	set_hal_pin("conf0.l", 0.0133);//gemessen
	set_hal_pin("conf0.j", KGCM2(0.26));//typenschild
	set_hal_pin("conf0.km", 0.457);//typenschild

	set_hal_pin("conf0.pos_p", 60.0);
	set_hal_pin("conf0.acc_p", 0.02);
	set_hal_pin("conf0.acc_pi", 70.0);
	set_hal_pin("conf0.cur_lp", 0.0);

	set_hal_pin("brake0.brake", 1.0);
}

void set_manutec(){ // TODO
	set_hal_pin("conf0.pole_count", 3.0);
	set_hal_pin("conf0.max_vel", RPM(8000));
	set_hal_pin("conf0.max_acc", RPM(8000) / 0.002);
	set_hal_pin("conf0.max_force", 4.8);
	set_hal_pin("conf0.max_cur", 13.0);

	set_hal_pin("conf0.fb_type", 2.0);
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
}

void set_bergerlahr(){ // TODO
	set_hal_pin("conf0.pole_count", 3.0);
	set_hal_pin("conf0.max_vel", RPM(12000));
	set_hal_pin("conf0.max_acc", RPM(12000) / 0.003);
	set_hal_pin("conf0.max_force", 3.0);
	set_hal_pin("conf0.max_cur", 6.0);

	set_hal_pin("conf0.fb_type", 2.0); // 6
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
}

//mc101ns
//100w 3000rpm 0.8A
void set_sankyo(){ // TODO
	set_hal_pin("conf0.pole_count", 4.0);
	set_hal_pin("conf0.max_vel", RPM(3000));
	set_hal_pin("conf0.max_acc", RPM(3000) / 0.002);
	set_hal_pin("conf0.max_force", 3.0);
	set_hal_pin("conf0.max_cur", 6.0);

	set_hal_pin("conf0.fb_type", 2.0);
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
}

//P50B08100DXS
void set_sanyo(){ // TODO
	set_hal_pin("conf0.pole_count", 2.0);
	set_hal_pin("conf0.max_vel", RPM(4500));
	set_hal_pin("conf0.max_acc", RPM(4500) / 0.002);
	set_hal_pin("conf0.max_force", 11.76);
	set_hal_pin("conf0.max_cur", 25.7);

	set_hal_pin("conf0.fb_type", 2.0);
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
}

//Mitsubishi HA-FF38-UE-S1
void set_mitsubishi(){ // TODO
	set_hal_pin("conf0.pole_count", 2.0);
	set_hal_pin("conf0.max_vel", RPM(8000));
	set_hal_pin("conf0.max_acc", RPM(8000) / 0.002);
	set_hal_pin("conf0.max_force", 2.0);
	set_hal_pin("conf0.max_cur", 6.0);

	set_hal_pin("conf0.fb_type", 5.0);
	set_hal_pin("conf0.fb_rev", 1.0);
	set_hal_pin("conf0.fb_res", 16384.0);
	set_hal_pin("conf0.r", 1.5);
	set_hal_pin("conf0.l", 0.0025);
	set_hal_pin("conf0.j", KGCM2(0.5));
	set_hal_pin("conf0.km", 0.22);

	set_hal_pin("conf0.pos_p", 60.0);
	set_hal_pin("conf0.acc_p", 0.15);
	set_hal_pin("conf0.acc_pi", 50.0);
	set_hal_pin("conf0.cur_lp", 0.5);
}

void set_br(){
	set_hal_pin("conf0.pole_count", 3.0);
	set_hal_pin("conf0.max_vel", RPM(6000));
	set_hal_pin("conf0.max_acc", RPM(6000) / 0.002);
	set_hal_pin("conf0.max_force", 1.6);
	set_hal_pin("conf0.max_cur", 2.9);

	set_hal_pin("conf0.fb_type", 3.0);
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
}

//precise 2 phase induction motor
void set_precise(){

}

void DMA2_Stream0_IRQHandler(void){ //5kHz
    DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0);
    GPIO_SetBits(GPIOB,GPIO_Pin_8);
    int freq = 5000;
    float period = 1.0 / freq;
    //GPIO_ResetBits(GPIOB,GPIO_Pin_3);//messpin
		systime_s += period;

		unsigned int start = SysTick->VAL;

    for(int i = 0; i < hal.rt_func_count; i++){
        hal.rt[i](period);
    }

		unsigned int end = SysTick->VAL;

		float t = 0.0;
		if(start > end){
			PIN(rt_time) = ((float)(start - end)) / RCC_Clocks.HCLK_Frequency;
		}

    GPIO_ResetBits(GPIOB,GPIO_Pin_8);
}

//disabled in setup.c
void TIM8_UP_TIM13_IRQHandler(void){ //20KHz
	TIM_ClearITPendingBit(TIM8, TIM_IT_Update);
	//GPIO_SetBits(GPIOB,GPIO_Pin_3);//messpin
}

//feedback UART
void USART3_IRQHandler(){
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);//reset tx enable
	//GPIO_SetBits(GPIOB,GPIO_Pin_9);//testpin
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
	 //GPIO_ResetBits(GPIOB,GPIO_Pin_9);//testpin
}

//on dir pin change reverse position timer
void EXTI9_5_IRQHandler(){
    if (EXTI_GetITStatus(EXTI_Line8) != RESET) {
	    if(GPIO_ReadInputDataBit(ENC0_A_PORT, ENC0_A_PIN))
			TIM1->CR1 |= TIM_CR1_DIR;
	    else
			TIM1->CR1 &= ~TIM_CR1_DIR;
        EXTI_ClearITPendingBit(EXTI_Line8);
    }
}

//DRV UART
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
		data.byte[datapos++] = (uint8_t)rxbuf;//append data to buffer
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

	#include "comps/adc.comp"
	#include "comps/enc.comp"
	#include "comps/res.comp"
	#include "comps/encm.comp"
	#include "comps/sim.comp"

	#include "comps/mux.comp"
	#include "comps/mux.comp"

	#include "comps/rev.comp"
	#include "comps/rev.comp"

	#include "comps/cauto.comp"

	#include "comps/pderiv.comp"
	#include "comps/pderiv.comp"

	#include "comps/pid.comp"

	#include "comps/dmux.comp"

	#include "comps/cur.comp"

	#include "comps/pwm2uart.comp"



	#include "comps/term.comp"
	#include "comps/led.comp"
	#include "comps/fan.comp"
	#include "comps/brake.comp"
	#include "comps/tune.comp"
	#include "comps/dmux.comp"
	#include "comps/dmux.comp"


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
	HAL_PIN(rt_calc_time) = 0.0;

	set_comp_type("conf");
	HAL_PIN(r) = 0.0;
	HAL_PIN(l) = 0.0;
	HAL_PIN(j) = 0.0;
	HAL_PIN(km) = 0.0;
	HAL_PIN(pole_count) = 0.0;
	HAL_PIN(fb_pole_count) = 0.0;
	HAL_PIN(fb_offset) = 0.0;
	HAL_PIN(pos_p) = 0.0;
	HAL_PIN(acc_p) = 0.0;
	HAL_PIN(acc_pi) = 0.0;
	HAL_PIN(cur_lp) = 0.0;
	HAL_PIN(max_vel) = 0.0;
	HAL_PIN(max_acc) = 0.0;
	HAL_PIN(max_force) = 0.0;
	HAL_PIN(max_cur) = 0.0;
	HAL_PIN(fb_type) = 0.0;
	HAL_PIN(cmd_type) = 0.0;
	HAL_PIN(fb_rev) = 0.0;
	HAL_PIN(cmd_rev) = 0.0;
	HAL_PIN(fb_res) = 1.0;
	HAL_PIN(cmd_res) = 2000.0;
	HAL_PIN(sin_offset) = 0.0;
	HAL_PIN(cos_offset) = 0.0;
	HAL_PIN(sin_gain) = 1.0;
	HAL_PIN(cos_gain) = 1.0;




	g_amp = map_hal_pin("net0.amp");
	g_vlt = map_hal_pin("net0.vlt");
	g_tmp = map_hal_pin("net0.tmp");
	rt_time = map_hal_pin("net0.rt_calc_time");

	for(int i = 0; i < hal.init_func_count; i++){
		hal.init[i]();
	}

	link_pid();

	//set_bergerlahr();//pid2: ok
	//set_mitsubishi();//pid2: ok
	//set_festo();
	//set_manutec();
	//set_precise();
	set_bosch4();//pid2: ok
	//set_hal_pin("res0.reverse", 0.0);
	//set_bosch1();//pid2: ok
	//set_sanyo();//pid2: ok
	//set_br();

	link_hal_pins("cauto0.ready", "led0.g");
	link_hal_pins("cauto0.start", "led0.r");
	//link_hal_pins("led0.g", "test0.test2");

	link_hal_pins("cauto0.ready", "pid0.enable");
	//link_hal_pins("net0.cmd", "auto0.offset");

	set_hal_pin("cauto0.start", 1.0);

	set_hal_pin("led0.y", 1.0);
	TIM_Cmd(TIM8, ENABLE);//int

	Wait(2000);
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
