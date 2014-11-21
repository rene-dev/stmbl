#include <stm32f4_discovery.h>
#include <stm32f4xx_conf.h>
#include "printf.h"
#include "scanf.h"
#include "param.h"
#include "hal.h"
#include "setup.h"
#include <math.h>

#ifdef USBTERM
#include "stm32_ub_usb_cdc.h"
#endif

int __errno;
void Wait(unsigned int ms);

#define NO 0
#define YES 1

void link_ac_sync_res(){
	link_hal_pins("res0.pos", "pos_minus0.in1");
	link_hal_pins("enc0.pos", "pos_minus0.in0");
	link_hal_pins("pos_minus0.out", "pid0.pos_error");
	link_hal_pins("pid0.pwm_cmd", "p2uvw0.pwm");
	link_hal_pins("res0.pos", "pderiv0.in");
	link_hal_pins("pderiv0.out", "pid0.vel_fb");
	set_hal_pin("pderiv0.lp", 0.8);
	link_hal_pins("enc0.pos", "pderiv1.in");
	link_hal_pins("pderiv1.out", "pid0.vel_ext_c");
	set_hal_pin("pderiv1.lp", 0.8);
	link_hal_pins("res0.pos", "plus1.in0");
	set_hal_pin("plus1.in1", -0.64);
	link_hal_pins("plus1.out", "mul0.in0");
	set_hal_pin("mul0.in1", 4.0);
	link_hal_pins("mul0.out", "mod0.in");
	link_hal_pins("mod0.out", "plus0.in0");
	set_hal_pin("plus0.in1", M_PI / 2.0);
	link_hal_pins("plus0.out", "p2uvw0.magpos");
	link_hal_pins("p2uvw0.u", "pwmout0.u");
	link_hal_pins("p2uvw0.v", "pwmout0.v");
	link_hal_pins("p2uvw0.w", "pwmout0.w");
	link_hal_pins("enc0.pos", "term0.wave0");
	link_hal_pins("res0.pos", "term0.wave1");
	set_hal_pin("res0.enable", 1.0);
	set_hal_pin("pid0.enable", 1.0);
	set_hal_pin("pwmout0.enable", 0.9);
	set_hal_pin("pwmout0.volt", 130.0);
	set_hal_pin("pwmout0.pwm_max", 0.9);
	set_hal_pin("p2uvw0.volt", 130.0);
	set_hal_pin("p2uvw0.pwm_max", 0.9);
	set_hal_pin("pid0.volt", 130.0);
	set_hal_pin("p2uvw0.poles", 1.0);
}

void link_ac_sync_enc(){
	link_hal_pins("enc0.pos", "pos_minus0.in1");
	link_hal_pins("res0.pos", "pos_minus0.in0");
	link_hal_pins("pos_minus0.out", "pid0.pos_error");
	link_hal_pins("pid0.pwm_cmd", "p2uvw0.pwm");
	link_hal_pins("enc0.pos", "pderiv0.in");
	link_hal_pins("pderiv0.out", "pid0.vel_fb");
	set_hal_pin("pderiv0.lp", 0.8);
	link_hal_pins("res0.pos", "pderiv1.in");
	link_hal_pins("pderiv1.out", "pid0.vel_ext_c");
	set_hal_pin("pderiv1.lp", 0.8);
	link_hal_pins("enc0.pos", "plus1.in0");
	set_hal_pin("plus1.in1", -0.64);
	link_hal_pins("plus1.out", "mul0.in0");
	set_hal_pin("mul0.in1", 3.0);
	link_hal_pins("mul0.out", "mod0.in");
	link_hal_pins("mod0.out", "plus0.in0");
	set_hal_pin("plus0.in1", M_PI / 2.0);
	link_hal_pins("plus0.out", "p2uvw0.magpos");
	link_hal_pins("p2uvw0.u", "pwmout0.u");
	link_hal_pins("p2uvw0.v", "pwmout0.v");
	link_hal_pins("p2uvw0.w", "pwmout0.w");
	link_hal_pins("res0.pos", "term0.wave0");
	link_hal_pins("enc0.pos", "term0.wave1");
	set_hal_pin("res0.enable", 1.0);
	set_hal_pin("pid0.enable", 1.0);
	set_hal_pin("pwmout0.enable", 0.9);
	set_hal_pin("pwmout0.volt", 130.0);
	set_hal_pin("pwmout0.pwm_max", 0.9);
	set_hal_pin("p2uvw0.volt", 130.0);
	set_hal_pin("p2uvw0.pwm_max", 0.9);
	set_hal_pin("pid0.volt", 130.0);
	set_hal_pin("pid0.pos_p", 80.0);
	set_hal_pin("pid0.pos_lp", 1.0);
	set_hal_pin("pid0.vel_lp", 0.6);
	set_hal_pin("pid0.vel_max", 200.0);
	set_hal_pin("pid0.acc_max", 3000.0);
	set_hal_pin("p2uvw0.poles", 1.0);
	set_hal_pin("enc0.res", 4096.0);
}

void DMA2_Stream2_IRQHandler(void){
	// welches flag?
	//DMA_ClearFlag(DMA2_FLAG_IT);
	DMA_ClearITPendingBit(DMA2_Stream2, DMA_IT_TCIF2);
	printf_("DMA\n");
}

void TIM2_IRQHandler(void){ //20KHz
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	GPIO_SetBits(GPIOC,GPIO_Pin_4);//messpin
}

void ADC_IRQHandler(void) // 20khz
{
	int freq = 20000;
	float period = 1.0 / freq;
	while(!ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC));
	ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
	GPIO_ResetBits(GPIOC,GPIO_Pin_4);//messpin

	for(int i = 0; i < hal.fast_rt_func_count; i++){
		hal.fast_rt[i](period);
	}
}

void TIM5_IRQHandler(void){ //5KHz
	TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
	int freq = 5000;
	float period = 1.0 / freq;
/*
	if(read_hal_pin(&ferror) > 0.0 && ABS(pid2ps.pos_error) > read_hal_pin(&ferror)){
		disable();
		write_hal_pin(&en, 0.0);
		state = EFOLLOW;
		pid2ps.enable = 0;
	}
	if(pid2ps.saturated_s >= read_hal_pin(&overload_s) && read_hal_pin(&overload_s) > 0.0){
		disable();
		state = EOVERLOAD;
		write_hal_pin(&en, 0.0);
		pid2ps.enable = 0;
	}
*/
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
}

int main(void)
{
	float period = 0.0;
	float lasttime = 0.0;
	setup();

	#include "plus.comp"
	#include "plus.comp"
	#include "mod.comp"
	#include "mul.comp"
	#include "pos_minus.comp"
	#include "pwm2uvw.comp"
	#include "pwmout.comp"
	#include "enc.comp"
	#include "res.comp"
	#include "pid.comp"
	#include "term.comp"
	#include "sim.comp"
	#include "pderiv.comp"
	#include "pderiv.comp"

	set_comp_type("net");
	HAL_PIN(enable) = 0.0;

	for(int i = 0; i < hal.init_func_count; i++){
		hal.init[i]();
	}

	TIM_Cmd(TIM2, ENABLE);//int
	TIM_Cmd(TIM4, ENABLE);//PWM
	TIM_Cmd(TIM5, ENABLE);//PID

	link_ac_sync_res();

	while(1)  // Do not exit
	{
		Wait(1);
		period = time/1000.0 + (1.0 - SysTick->VAL/RCC_Clocks.HCLK_Frequency)/1000.0 - lasttime;
		lasttime = time/1000.0 + (1.0 - SysTick->VAL/RCC_Clocks.HCLK_Frequency)/1000.0;
		for(int i = 0; i < hal.nrt_func_count; i++){
			hal.nrt[i](period);
		}
		
	}
}

void Wait(unsigned int ms){
	volatile unsigned int t = time + ms;
	while(t >= time){
	}
}
