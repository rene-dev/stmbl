#include <stm32f4_discovery.h>
#include <stm32f4xx_conf.h>
#include "printf.h"
#include "scanf.h"
#include "param.h"
#include "setup.h"
#include "input.h"
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

#define pwm_scale 0.9//max/min PWM duty cycle

#define NO 0
#define YES 1

#define MAX_WAVE 4

#define offseta 0.0 * 2.0 * M_PI / 3.0
#define offsetb 1.0 * 2.0 * M_PI / 3.0
#define offsetc 2.0 * 2.0 * M_PI / 3.0

volatile float mag_pos = 0;
volatile float soll_pos = 0;
volatile float soll_pos_old = 0;
volatile float ist = 0;
volatile float ist_old = 0;
volatile float voltage_scale = 0;// -1 bis 1

volatile int t1, t2;//rohdaten sin/cos
volatile int t1_last = 0, t2_last = 0;//rohdaten sin/cos letzter aufruf
volatile int t1_mid = 0,t2_mid = 0;//mittelpunkt sin/cos
volatile int amp1,amp2;//betrag
volatile int erreger = 0;//resolver erreger pin an/aus
volatile int erreger_enable = NO;//erreger aktiv
volatile int k = 0,l = 0; // adc res pos
volatile int data[10][2][2];
volatile int rescal = 0;//potis einstellen
volatile float calv = 0.5;//potis einstellen
volatile float startpos = 0.0;
volatile int count = 0;
volatile int mode = 1;
volatile float amp = 1.0;
volatile float freq = 1.0;
volatile float pole_count = 4;
volatile float ferror = 90;//schleppfehler
volatile float res_offset = DEG(36.6); //minimaler positiver resolver output bei mag_pos = 0
volatile float time_wave = 0; // time scale
volatile float cmd = 0; //dummywert für befehle
volatile float overload = 1000;// overload error time
volatile float encres = 4096;//16384;// encoder
volatile float scale = 1;//100;// encoder

input* inputs[4];

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

float get_enc_pos(){
	static float value = 0;
	value += minus((TIM_GetCounter(TIM3) * 2 * M_PI / encres),value)*0.01f;
	//value = (TIM_GetCounter(TIM3) * 2 * M_PI / encres)*0.005f+(1-0.005f)*value;
	//return value*scale;//nochmal in der setup
	return (TIM_GetCounter(TIM3) * 2 * M_PI / encres);
}

float get_res_pos(){
	return ist - res_offset;
}

void output_ac_pwm(){
	float volt = CLAMP(voltage_scale,-1.0,1.0);
	//volt = volt*-1;

	if(rescal){
		mag_pos = 0;
		volt = calv;
	}else{
		mag_pos = get_res_pos() * pole_count + DEG(90);
	}

	mag_pos = mod(mag_pos);
	PWM_U = (sinf(mag_pos + offseta) * pwm_scale * volt + 1.0) * mag_res / 2.0;
	PWM_V = (sinf(mag_pos + offsetb) * pwm_scale * volt + 1.0) * mag_res / 2.0;
	PWM_W = (sinf(mag_pos + offsetc) * pwm_scale * volt + 1.0) * mag_res / 2.0;
}

void output_dc_pwm(){
	float volt = CLAMP(voltage_scale,-1.0,1.0);

	int foo = volt * mag_res * pwm_scale / 2 + mag_res / 2;
	PWM_U = foo;//PD12 PIN1
	PWM_V = mag_res-foo;//PD13 PIN2
	PWM_W = 0;//PD15 PIN3
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
}

float get_cmd(float period){
	static float time = 0.0;
	static float pos = 0.0;

	time += period;

	switch(mode){
		case 0: // hold
			return(0.0);
		case 1: // enc
			return(get_enc_pos());
		case 2: // vel
			pos += amp * period * M_PI * 2.0;
			pos = mod(pos);
			return(pos);
		case 3: // square
			if(sinf(freq * 2 * M_PI * time) > 0){
				return(amp * M_PI * 2.0);
			}
			else{
				return(-amp * M_PI * 2.0);
			}
		case 4: // sine
			return(amp * sinf(freq * 2 * M_PI * time) * M_PI * 2.0);
	}
	return(0.0);
}

void TIM5_IRQHandler(void){ //1KHz
	TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
	float s = 0,c = 0;
	int freq = 1000;
	float periode = 1.0 / freq;
	for(int i = 0;i<10;i++){
		s += data[i][0][0];
		c += data[i][0][1];
	}
	for(int i = 0;i<10;i++){
		s += data[i][1][0];
		c += data[i][1][1];
	}
	ist = atan2f(s,c);

	count++;

	soll_pos = startpos + get_cmd(periode) + res_offset;//MIN(res_pos1, res_pos2) + MIN(ABS(minus(res_pos1,res_pos2)), ABS(minus(res_pos2,res_pos1))) / 2;
	soll_pos = mod(soll_pos);

	float tmpv;

	tmpv = minus(ist, ist_old) * freq * 0.3 + pid.feedbackv * 0.7;
	pid.feedbacka = (tmpv - pid.feedbackv) * freq * 0.3 + pid.feedbacka * 0.7;
	pid.feedbackv = tmpv;
	tmpv = minus(soll_pos, soll_pos_old) * freq * 0.3 + pid.commandv * 0.7;
	pid.commanda = (tmpv - pid.commandv) * freq * 0.3 + pid.commanda * 0.7;
	pid.commandv = tmpv;
	pid.error = minus(soll_pos, ist);

	soll_pos_old = soll_pos;
	ist_old = ist;

	if(!rescal){
	if(ferror != 0.0f && ABS(pid.error) > DEG(ferror)){
	  disable();
	  state = EFOLLOW;
	  pid.enable = 0;
	}
	if(amp1 < 10000 && amp2 < 10000){//TODO nur letzter wert!
		disable();
		state = EFEEDBACK;
		pid.enable = 0;
	}
	}
	//calc_pid(&pid, periode * 1000.0);

	pid2ps.ext_cmd_pos = soll_pos;
	pid2ps.feedback_pos = ist;
	pid2ps.ext_cmd_vel = pid.commandv;
	pid2ps.feedback_vel = pid.feedbackv;
	pid2ps.ext_cmd_acc = pid.commanda;
	//pid2ps.feedback_acc = pid.feedbacka;


	pid2(&pid2ps);

	//voltage_scale = pid.output;
	voltage_scale = pid2ps.cmd_pwm;

	if(!rescal){
	//if(pid.saturated_count >= overload && overload != 0){
	if(pid2ps.saturated_s >= 1.0 && overload != 0){

		disable();
		state = EOVERLOAD;
		pid.enable = 0;
	}
	}
	output_ac_pwm();
	time_wave++;
	if(time_wave >= 100){
		time_wave = 0;
	}
}

void reset(float r){
	printf_("reset\n");
	disable();
	state = RUNNING;
	startpos = minus(minus(ist,res_offset),get_cmd(0.001f));//MIN(res_pos1, res_pos2) + MIN(ABS(minus(res_pos1,res_pos2)), ABS(minus(res_pos2,res_pos1))) / 2;
	soll_pos_old = startpos + get_cmd(0.001) + res_offset;
	pid.enable = 1;
	enable();
}

int main(void)
{
	unsigned char buf[MAX_WAVE + 2];
	int wave[MAX_WAVE];
	float offset[MAX_WAVE];
	float gain[MAX_WAVE];
	int bufpos = 0;
	char w_name[] = "wave ";
	char o_name[] = "offset ";
	char g_name[] = "gain ";

	float e = 0;

	setup();
	param_init();
	TIM_SetAutoreload(TIM3, encres - 1);

	for(bufpos = 0; bufpos < MAX_WAVE; bufpos++){
		w_name[4] = bufpos + '1';
		o_name[6] = bufpos + '1';
		g_name[4] = bufpos + '1';
		register_int(w_name,&wave[bufpos]);
		register_float(o_name,&offset[bufpos]);
		register_float(g_name,&gain[bufpos]);
		wave[bufpos] = 0;
		offset[bufpos] = 0;
		gain[bufpos] = 1;
	}

/*
	register_int("e",&pid.enable);
	register_float("p",&pid.pgain);
	register_float("i",&pid.igain);
	register_float("d",&pid.dgain);
	register_float("b",&pid.bgain);
	// register_float("ff0",&pid.ff0gain);
	register_float("ff1",&pid.ff1gain);
	register_float("ff2",&pid.ff2gain);
	register_int("rescal",&rescal);
	register_float("ist",&ist);
	register_float("calv",&calv);
	register_float("amp",&amp);
	register_float("freq",&freq);
	register_int("mode",&mode);
	register_float("spos",&startpos);
	register_float("mpol",&pole_count);
	register_float("resoff",&res_offset);
	register_float("ferror",&ferror);
	register_float("reset",&cmd);
	register_read_callback("reset", reset);
	register_int("amp1",&amp1);
	register_int("amp2",&amp2);
	register_int("state",&state);
	register_float("scale",&scale);
	*/

register_float("reset",&cmd);
register_read_callback("reset", reset);
register_int("rescal",&rescal);
register_float("ist",&ist);
register_float("calv",&calv);
register_float("amp",&amp);
register_float("freq",&freq);
register_int("mode",&mode);
register_float("spos",&startpos);
register_float("mpol",&pole_count);
register_float("resoff",&res_offset);
register_float("ferror",&ferror);

register_float("pos_p",&pid2ps.pos_p);
register_float("ff1",&pid2ps.ff1);

register_float("vel_p",&pid2ps.vel_p);
register_float("vel_i",&pid2ps.vel_i);
register_float("ff2",&pid2ps.ff2);

register_float("acc_p",&pid2ps.acc_p);

register_float("force_p",&pid2ps.force_p);

register_float("cur_p",&pid2ps.cur_p);
register_float("cur_d",&pid2ps.cur_d);
register_float("ind_p",&pid2ps.ind_p);

register_float("volt",&pid2ps.volt);

register_float("max_vel",&pid2ps.max_vel);
register_float("max_sum",&pid2ps.max_vel_error_sum);
register_float("max_acc",&pid2ps.max_acc);
register_float("max_for",&pid2ps.max_force);
register_float("max_cur",&pid2ps.max_cur);
register_float("max_vol",&pid2ps.max_volt);
register_float("max_pwm",&pid2ps.max_pwm);




	state = STBY;

	GPIO_ResetBits(GPIOC,GPIO_Pin_2);//reset erreger
	Wait(10);
	TIM_Cmd(TIM2, ENABLE);//int
	Wait(50);
	erreger_enable = YES;
	Wait(50);
	TIM_Cmd(TIM4, ENABLE);//PWM
	TIM_Cmd(TIM5, ENABLE);//PID
	Wait(10);
	startpos = get_res_pos();
	soll_pos_old = startpos + res_offset;
	pid.enable = 1;
	enable();



	while(1)  // Do not exit
	{
		for(bufpos = 0; bufpos < MAX_WAVE; bufpos++){
			switch(wave[bufpos]){
				/*
				case 1:
					e = (int)RAD((pid.error + offset[bufpos]) * gain[bufpos]);
					break;
				case 2:
					e = (int)RAD((pid.error_d + offset[bufpos]) * gain[bufpos]);
					break;
				case 3:
					e = (int)RAD((pid.error_dd + offset[bufpos]) * gain[bufpos]);
					break;
				case 4:
					e = (int)RAD((soll_pos + offset[bufpos]) * gain[bufpos]);
					break;
				case 5:
					e = (int)RAD((pid.cmd_d + offset[bufpos]) * gain[bufpos]);
					break;
				case 6:
					e = (int)RAD((pid.cmd_dd + offset[bufpos]) * gain[bufpos]);
					break;
				case 7:
					e = (int)RAD((ist + offset[bufpos]) * gain[bufpos]);
					break;
				case 8:
					e = (int)RAD((pid.feedbackv + offset[bufpos]) * gain[bufpos]);
					break;
				case 9:
					e = (int)((voltage_scale*127 + offset[bufpos]) * gain[bufpos]);
					break;
				case 10:
					e = (int)((pid.saturated_count + offset[bufpos]) * gain[bufpos]);
					break;
				case 11:
					e = (int)RAD((mag_pos + offset[bufpos]) * gain[bufpos]);
					break;
				case 12:
					e = (int)RAD((startpos + offset[bufpos]) * gain[bufpos]);
					break;
				case 13:
					e = (int)RAD((res_offset + offset[bufpos]) * gain[bufpos]);
					break;
				case 14:
					e = (int)((time_wave + offset[bufpos]) * gain[bufpos]);
					break;
				default:
					e = 0;
					*/
				case 1:
					e = RAD(pid2ps.pos_error);
					break;
				case 2:
					e = RAD(pid2ps.vel_error);
					break;
				case 3:
					//e = RAD(pid2ps.acc_error);
					break;
				case 4:
					e = RAD(pid2ps.ext_cmd_pos);
					break;
				case 5:
					e = RAD(pid2ps.ext_cmd_vel);
					break;
				case 6:
					e = RAD(pid2ps.ext_cmd_acc);
					break;
				case 7:
					e = RAD(pid2ps.feedback_pos);
					break;
				case 8:
					e = RAD(pid2ps.feedback_vel);
					break;
				case 9:
					//e = RAD(pid2ps.feedback_acc);
					break;
				case 10:
					e = RAD(pid2ps.cmd_vel);
					break;
				case 11:
					e = RAD(pid2ps.cmd_acc);
					break;
				case 12:
					e = RAD(pid2ps.cmd_force);
					break;
				case 13:
					e = RAD(pid2ps.cmd_cur);
					break;
				case 14:
					e = RAD(pid2ps.cmd_volt);
					break;
				case 15:
					e = RAD(pid2ps.cmd_pwm);
					break;
				case 16:
					e = RAD(pid2ps.saturated_s);
					break;
				case 17:
					e = (int)((time_wave + offset[bufpos]) * gain[bufpos]);
					break;
				default:
					e = 0;
			}
			e = (e + offset[bufpos]) * gain[bufpos];
			e = CLAMP(e + 128,1,255);

			buf[bufpos + 1] = (int)e;
		}
		buf[0] = 255;
		buf[MAX_WAVE + 1] = 0;


#ifdef USBTERM
		if(UB_USB_CDC_GetStatus()==USB_CDC_CONNECTED){
			UB_USB_CDC_SendString((char*)buf, NONE);//schleppfehler senden

			char name[APP_TX_BUF_SIZE];
			float value = 0;
			int i = scanf_("%s = %f",name,&value);
			switch(i){
				case 2:
					if(is_param(name))
						printf_("%s = %f\n",name,get_param(name));
					else{
						printf_("not found\n");
					}
					break;
				case 5:
					if(is_param(name)){
						if(set_param(name,value))
							printf_("OK, %s = %f\n",name,get_param(name));
						else
							printf_("error!\n");
					}
					break;
				case -1:
					break;
				default:
					list_param();
			}
		}
#endif

		Wait(1);
	}
}

void Wait(unsigned int ms){
	volatile unsigned int t = time + ms;
	while(t >= time){
	}
}
