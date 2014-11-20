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

//bosch grau, gelb, grün

//mayr gelb sw, rot
//amp zu gering!
// #define pole_count 2
// #define res_offset 0.5 //minimaler positiver resolver output bei mag_pos = 0
// float p = 8.000000
// float i = 400.000000
// float d = 0.050000
// float p = 3.000000
// float i = 100.000000
// float d = 0.004999

// bosch 320V
/*
float p = 1.000000
float i = 0.050000
float d = 0.009999
float b = 0.000000
float ff1 = 0.001500
float ff2 = 0.000049
*/

#define pwm_scale 0.9//max/min _ duty cycle

#define NO 0
#define YES 1

#define MAX_WAVE 4

//#define offseta 0.0 * 2.0 * M_PI / 3.0
//#define offsetb 1.0 * 2.0 * M_PI / 3.0
//#define offsetc 2.0 * 2.0 * M_PI / 3.0

volatile float res = 0;

volatile int t1, t2;//rohdaten sin/cos
volatile int t1_last = 0, t2_last = 0;//rohdaten sin/cos letzter aufruf
volatile int t1_mid = 0,t2_mid = 0;//mittelpunkt sin/cos
volatile int amp1,amp2;//betrag
volatile int erreger = 0;//resolver erreger pin an/aus
volatile int erreger_enable = NO;//erreger aktiv
volatile int k = 0,l = 0; // adc res pos
volatile int data[10][2][2];
volatile float time_wave = 0; // time scale
volatile float cmd = 0; //dummywert für befehle
volatile float encres = 4096;//16384;// encoder
volatile float scale = 1;//100;// encoder


struct hal_pin mag_pos;
struct hal_pin mag_vel;
struct hal_pin mag_vel_p;
struct hal_pin mag_vel_min;
struct hal_pin pwm;


struct hal_pin ferror;
struct hal_pin overload_s;
struct hal_pin res_amp;
struct hal_pin pole_count;
struct hal_pin phase_count;
struct hal_pin sync;
struct hal_pin res_offset;
struct hal_pin startpos;
struct hal_pin reset;
struct hal_pin en;
struct hal_pin dc;
struct hal_pin ac_sync;
struct hal_pin ac_async;

struct hal_pin wave0;
struct hal_pin wave1;
struct hal_pin wave2;
struct hal_pin wave3;
struct hal_pin gain0;
struct hal_pin gain1;
struct hal_pin gain2;
struct hal_pin gain3;
struct hal_pin offset0;
struct hal_pin offset1;
struct hal_pin offset2;
struct hal_pin offset3;

/*
void init_hal_pins(){
	set_comp_type("in");
	init_hal_pin("enc_pos", &enc_pos, 0.0);
	init_hal_pin("res_pos", &res_pos, 0.0);
	init_hal_pin("sin_pos", &sin_pos, 0.0);
	init_hal_pin("square_pos", &square_pos, 0.0);
	init_hal_pin("vel_pos", &vel_pos, 0.0);
	set_comp_type("plus");
	init_hal_pin("p0_in0", &p0_in0, 0.0);
	init_hal_pin("p0_in1", &p0_in1, 0.0);
	init_hal_pin("p0_out", &p0_out, 0.0);
	set_comp_type("div");
	init_hal_pin("d0_in0", &d0_in0, DEG(90.0));
	init_hal_pin("d0_in1", &d0_in1, 1.0);
	init_hal_pin("d0_out", &d0_out, 0.0);
	set_comp_type("in_sim");
	init_hal_pin("amp", &amp, 0.1);
	init_hal_pin("freq", &freq, 1.0);
	set_comp_type("out");
	init_hal_pin("mag_pos", &mag_pos, 0.0);
	init_hal_pin("mag_vel", &mag_vel, 0.0);
	init_hal_pin("mag_vel_p", &mag_vel_p, 1.02);
	init_hal_pin("mag_vel_min", &mag_vel_min, 1.0);
	init_hal_pin("pwm", &pwm, 0.0);

	set_comp_type("global");
	init_hal_pin("ferror", &ferror, DEG(90.0));
	init_hal_pin("overload_s", &overload_s, 1.0);
	init_hal_pin("res_amp", &res_amp, 10000.0);
	init_hal_pin("pole_count", &pole_count, 4.0);
	init_hal_pin("phase_count", &phase_count, 3.0);
	init_hal_pin("sync", &sync, 1.0);
	init_hal_pin("res_offset", &res_offset, DEG(36.6));
	init_hal_pin("startpos", &startpos, 0.0);
	init_hal_pin("reset", &reset, 0.0);
	init_hal_pin("en", &en, 1.0);
	init_hal_pin("dc", &dc, 0.0);
	init_hal_pin("ac_sync", &ac_sync, 1.0);
	init_hal_pin("ac_async", &ac_async, 0.0);
	set_comp_type("term");
	init_hal_pin("wave0", &wave0, 0.0);
	init_hal_pin("wave1", &wave1, 0.0);
	init_hal_pin("wave2", &wave2, 0.0);
	init_hal_pin("wave3", &wave3, 0.0);
	init_hal_pin("gain0", &gain0, 10.0);
	init_hal_pin("gain1", &gain1, 10.0);
	init_hal_pin("gain2", &gain2, 10.0);
	init_hal_pin("gain3", &gain3, 10.0);
	init_hal_pin("offset0", &offset0, 0.0);
	init_hal_pin("offset1", &offset1, 0.0);
	init_hal_pin("offset2", &offset2, 0.0);
	init_hal_pin("offset3", &offset3, 0.0);

}

void link_hal(){
	link_hal_pins("enc_pos", "pid_ext_pos");
	link_hal_pins("res_pos", "pid_fb_pos");
	link_hal_pins("pid_cmd_pwm", "pwm");
	link_hal_pins("p0_out", "mag_pos");
	link_hal_pins("d0_out", "p0_in1");
	link_hal_pins("pole_count", "d0_in1");
	link_hal_pins("pid_cmd_vel", "mag_vel");
}

void link_dc(){
	link_hal_pins("p0_in0", "p0_in0");
	set_hal_pin("p0_in0", 0.0);
	set_hal_pin("sync", 1.0);
	set_hal_pin("phase_count", 2.0);
	set_hal_pin("pole_count", 1.0);
}

void link_ac_sync(){
	link_hal_pins("res_pos", "p0_in0");
	set_hal_pin("sync", 1.0);
	set_hal_pin("phase_count", 3.0);
	set_hal_pin("pole_count", 4.0);
}

void link_ac_async(){
	link_hal_pins("res_pos", "p0_in0");
	set_hal_pin("sync", 0.0);
	set_hal_pin("phase_count", 3.0);
	set_hal_pin("pole_count", 1.0);
}
*/
enum{
	STBY = 0,
	RUNNING,
	EFOLLOW,
	EFEEDBACK,
	EOVERLOAD,
	EOVERSPEED,
} state;

void enable(){
	PWM_E = mag_res*0.97;
}

void disable(){
	PWM_E = 0;
}

void read_res_pos(){
	//write_hal_pin(&res_pos, res - read_hal_pin(&res_offset));
}

void output_pwm(float period){
	/*float volt = CLAMP(read_hal_pin(&pwm),-1.0,1.0);
	static float pos = 0.0;

	if(read_hal_pin(&sync) > 0.0){
		pos = read_hal_pin(&mag_pos) * read_hal_pin(&pole_count);
	}
	else{
		float vel = read_hal_pin(&mag_vel) * read_hal_pin(&mag_vel_p);
		if(vel > 0){
			vel = MAX(vel, read_hal_pin(&mag_vel_min));
		}
		else{
			vel = MIN(vel, -read_hal_pin(&mag_vel_min));
		}
		pos += vel * read_hal_pin(&pole_count) * period;
	}

	pos = mod(pos);

	switch((int)read_hal_pin(&phase_count)){
		case 0:
			PWM_U = 0;
			PWM_V = 0;
			PWM_W = 0;
			break;
		case 1:
			PWM_U = (sinf(pos) * pwm_scale * volt + 1.0) * mag_res / 2.0;//PD12 PIN1
			PWM_V = mag_res / 2.0;//PD13 PIN2
			PWM_W = 0;//PD15 PIN3
			break;
		case 2:
			PWM_U = (sinf(pos) * pwm_scale * volt + 1.0) * mag_res / 2.0;//PD12 PIN1
			PWM_V = (sinf(pos + M_PI) * pwm_scale * volt + 1.0) * mag_res / 2.0;//PD13 PIN2
			PWM_W = 0;//PD15 PIN3
			break;
		case 3:
			PWM_U = (sinf(pos) * pwm_scale * volt + 1.0) * mag_res / 2.0;
			PWM_V = (sinf(pos + 2.0 * M_PI / 3.0) * pwm_scale * volt + 1.0) * mag_res / 2.0;
			PWM_W = (sinf(pos + 2.0 * M_PI / 3.0 * 2.0) * pwm_scale * volt + 1.0) * mag_res / 2.0;
			break;
		default:
			PWM_U = 0;
			PWM_V = 0;
			PWM_W = 0;
	}*/
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

	t1 = ADC_GetConversionValue(ADC1);
	t2 = ADC_GetConversionValue(ADC2);
	t1_mid = t1_mid * 0.95 + (t1+t1_last)/2 * 0.05;
	t2_mid = t2_mid * 0.95 + (t2+t2_last)/2 * 0.05;
	if(erreger_enable){//erreger signal aktiv
		if(erreger){//eine halbwelle
			GPIO_SetBits(GPIOC,GPIO_Pin_2);//erreger
			amp1 = (t1-t1_mid)*(t1-t1_mid)+(t2-t2_mid)*(t2-t2_mid);
			data[k][0][0] = t1 - t1_mid;
			data[k][0][1] = t2 - t2_mid;
			k++;
		}else{//andere halbwelle
			GPIO_ResetBits(GPIOC,GPIO_Pin_2);//erreger
			amp2 = (t1_mid-t1)*(t1_mid-t1)+(t2_mid-t2)*(t2_mid-t2);
			data[l][1][0] = t1_mid - t1;
			data[l][1][1] = t2_mid - t2;
			l++;
		}
		if(l == 10 && k == 10){
			l = 0;
			k = 0;
		}
	}else{//mittelpunkt messen
		if(t1_mid == 0 && t2_mid == 0){//erster durchlauf
			t1_mid = t1;
			t2_mid = t2;
		}else{//restliche durchläufe
			t1_mid = t1_mid * 0.95 + t1 * 0.05;
			t2_mid = t2_mid * 0.95 + t2 * 0.05;
		}

	}
	t1_last = t1;
	t2_last = t2;
	erreger = !erreger; // 10khz

	for(int i = 0; i < hal.fast_rt_func_count; i++){
		hal.fast_rt[i](period);
	}
}

void get_pos(float period){
	static float time = 0.0;
	static float pos = 0.0;

	time += period;
/*	read_res_pos();
	read_enc_pos();

	pos += read_hal_pin(&amp) * period * M_PI * 2.0;
	pos = mod(pos);
	write_hal_pin(&vel_pos, pos);

	if(sinf(read_hal_pin(&freq) * 2 * M_PI * time) > 0.0){
		write_hal_pin(&square_pos, read_hal_pin(&amp) * M_PI * 2.0);
	}
	else{
		write_hal_pin(&square_pos, -read_hal_pin(&amp) * M_PI * 2.0);
	}

	write_hal_pin(&sin_pos, read_hal_pin(&amp) * sinf(read_hal_pin(&freq) * 2 * M_PI * time) * M_PI * 2.0);*/
}


void TIM5_IRQHandler(void){ //1KHz
	TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
	float s = 0,c = 0;
	int freq = 1000;
	float period = 1.0 / freq;

	for(int i = 0;i<10;i++){
		s += data[i][0][0];
		c += data[i][0][1];
	}
	for(int i = 0;i<10;i++){
		s += data[i][1][0];
		c += data[i][1][1];
	}
	res = atan2f(s,c);
/*
	get_pos(period);

	float soll_pos;
	float ist;
	static float soll_pos_old = 0.0;
	static float ist_old = 0.0;
	float tmpv;

	pid2ps.period = period;

	soll_pos = read_hal_pin(&pid_ext_pos);
	soll_pos = mod(soll_pos);

	ist = read_hal_pin(&pid_fb_pos);
	ist = mod(ist);

	tmpv = minus(ist, ist_old) * freq * 0.3 + pid2ps.feedback_vel * 0.7;
	//pid2ps.feedback_acc = (tmpv - pid2ps.feedback_vel) * freq * 0.3 + pid2ps.feedback_acc * 0.7;
	pid2ps.feedback_vel = tmpv;

	tmpv = minus(soll_pos, soll_pos_old) * freq * 0.3 + pid2ps.ext_cmd_vel * 0.7;
	pid2ps.ext_cmd_acc = (tmpv - pid2ps.ext_cmd_vel) * freq * 0.3 + pid2ps.ext_cmd_acc * 0.7;
	pid2ps.ext_cmd_vel = tmpv;

	pid2ps.ext_cmd_pos = soll_pos;
	pid2ps.feedback_pos = ist;

	soll_pos_old = soll_pos;
	ist_old = ist;

	pid2ps.pos_p = read_hal_pin(&pid_pos_p);
	pid2ps.ff1 = read_hal_pin(&pid_ff1);
	pid2ps.pos_lpass = read_hal_pin(&pid_pos_lp);
	pid2ps.vel_p = read_hal_pin(&pid_vel_p);
	pid2ps.vel_i = read_hal_pin(&pid_vel_i);
	pid2ps.ff2 = read_hal_pin(&pid_ff2);
	pid2ps.vel_lpass = read_hal_pin(&pid_vel_lp);
	pid2ps.acc_p = read_hal_pin(&pid_acc_p);
	pid2ps.force_p = read_hal_pin(&pid_force_p);
	pid2ps.cur_p = read_hal_pin(&pid_cur_p);
	pid2ps.cur_d = read_hal_pin(&pid_cur_d);
	pid2ps.ind_p = read_hal_pin(&pid_ind_p);
	pid2ps.cur_lpass = read_hal_pin(&pid_cur_lp);

	pid2ps.enable = read_hal_pin(&pid_enable);
	pid2ps.max_vel = read_hal_pin(&pid_max_vel);
	pid2ps.max_vel_error_sum = read_hal_pin(&pid_max_sum);
	pid2ps.max_acc = read_hal_pin(&pid_max_acc);
	pid2ps.max_force = read_hal_pin(&pid_max_force);
	pid2ps.max_cur = read_hal_pin(&pid_max_cur);
	pid2ps.max_volt = read_hal_pin(&pid_max_volt);
	pid2ps.max_pwm = read_hal_pin(&pid_max_pwm);

	if(amp1 < read_hal_pin(&res_amp) && amp2 < read_hal_pin(&res_amp) && read_hal_pin(&res_amp) > 0.0){//TODO nur letzter wert!
		disable();
		write_hal_pin(&en, 0.0);
		state = EFEEDBACK;
		pid2ps.enable = 0;
	}

	pid2(&pid2ps);


	if(read_hal_pin(&ferror) > 0.0 && ABS(pid2ps.pos_error) > read_hal_pin(&ferror)){
		disable();
		write_hal_pin(&en, 0.0);
		state = EFOLLOW;
		pid2ps.enable = 0;
	}

	write_hal_pin(&pid_cmd_vel, pid2ps.cmd_vel);
	write_hal_pin(&pid_ext_vel, pid2ps.ext_cmd_vel);
	write_hal_pin(&pid_fb_vel, pid2ps.cmd_vel);
	write_hal_pin(&pid_cmd_acc, pid2ps.cmd_acc);
	write_hal_pin(&pid_ext_acc, pid2ps.ext_cmd_acc);
	write_hal_pin(&pid_cmd_force, pid2ps.cmd_force);
	write_hal_pin(&pid_cmd_cur, pid2ps.cmd_cur);
	write_hal_pin(&pid_fb_cur, pid2ps.feedback_cur);
	write_hal_pin(&pid_cmd_volt, pid2ps.cmd_volt);
	write_hal_pin(&pid_cmd_pwm, pid2ps.cmd_pwm);
	write_hal_pin(&pid_saturated_s, pid2ps.saturated_s);

	write_hal_pin(&pid_error_pos, pid2ps.pos_error);
	write_hal_pin(&pid_error_vel, pid2ps.vel_error);
	write_hal_pin(&pid_error_cur, pid2ps.cur_error);


	if(pid2ps.saturated_s >= read_hal_pin(&overload_s) && read_hal_pin(&overload_s) > 0.0){
		disable();
		state = EOVERLOAD;
		write_hal_pin(&en, 0.0);
		pid2ps.enable = 0;
	}

	write_hal_pin(&pid_enable, pid2ps.enable);

	div0();
	plus0();

	output_pwm(period);

	time_wave++;
	if(time_wave >= 100){
		time_wave = 0;
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

void reset_(){
	printf_("reset\n");
	disable();
	state = RUNNING;
	//startpos = minus(minus(ist,res_offset),get_cmd(0.001f));//MIN(res_pos1, res_pos2) + MIN(ABS(minus(res_pos1,res_pos2)), ABS(minus(res_pos2,res_pos1))) / 2;
	//soll_pos_old = startpos + get_cmd(0.001) + res_offset;
	//write_hal_pin(&pid_enable, 1.0);
	//write_hal_pin(&en, 1.0);
	enable();
}

//#define HAL_FLOAT(name) struct hal_pin COMP##name = {COMP_##name, 0.0, 0}; init_hal_pin(COMP_##name, &COMP_name, 0.0);
//#define PIN(name) (name.source->value)

int main(void)
{
	int last_time = 0;
	float period = 0.0;
//	init_hal();
	setup();


	#include "enc.comp"
	#include "pid.comp"
	#include "term.comp"
	#include "sim.comp"
	#include "deriv.comp"

	for(int i = 0; i < hal.init_func_count; i++){
		hal.init[i]();
	}

//	param_init();


	//TIM_SetAutoreload(TIM3, encres - 1);

/*
	state = STBY;

	GPIO_ResetBits(GPIOC,GPIO_Pin_2);//reset erreger
	Wait(10);

	Wait(50);
	erreger_enable = YES;
	Wait(50);
	*/
	TIM_Cmd(TIM2, ENABLE);//int
	TIM_Cmd(TIM4, ENABLE);//PWM
	TIM_Cmd(TIM5, ENABLE);//PID
	/*
	Wait(10);
	//startpos = get_res_pos();
	//soll_pos_old = startpos + res_offset;
	//write_hal_pin("pid0.enable", 1.0);
	enable();



*/
unsigned int* mem;
mem = (unsigned int*) SCB->VTOR + TIM5_IRQn;
	while(1)  // Do not exit
	{

		Wait(1);
		printf_("tim5: %h vec: %h\n", (unsigned int)TIM5_IRQHandler, (unsigned int)*(mem));

		period = 0.001;
		//printf_("rt_in_count %i\n", hal.rt_in_func_count);
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
