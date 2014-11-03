#include <stm32f4_discovery.h>
#include <stm32f4xx_conf.h>
#include "printf.h"
#include "scanf.h"
#include "param.h"
#include "setup.h"
#include <math.h>

#ifdef USBTERM
#include "stm32_ub_usb_cdc.h"
#endif

int __errno;
void Wait(unsigned int ms);

//bosch grau, gelb, grün
#define pole_count 4
#define res_offset DEG(36.6) //minimaler positiver resolver output bei mag_pos = 0

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

#define pwm_scale 0.9//max/min PWM duty cycle

#define NO 0
#define YES 1

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
volatile float w = -1;
volatile int k = 0,l = 0;
volatile int data[10][2][2];
volatile int rescal = 0;//potis einstellen
volatile float calv = 0.5;//potis einstellen
volatile float res_s_var = 0.0;
volatile float res_c_var = 0.0;
volatile float startpos = 0.0;
volatile int count = 0;
volatile int mode = 0;
volatile float amp = 0.0;
volatile float freq = 0.0;

enum{
	STBY,
	RUNNING,
	EFOLLOW,
	EFEEDBACK
} state;

void enable(){
	PWM_E = mag_res*0.97;
}

void disable(){
	PWM_E = 0;
}

float get_enc_pos(){
	return (TIM_GetCounter(TIM3) * 2 * M_PI / 2048);//nochmal in der setup
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
			if(w >= 0){
				data[k][0][0] = t1 - t1_mid;
				data[k][0][1] = t2 - t2_mid;
				k++;
			}
		}else{//andere halbwelle
			GPIO_ResetBits(GPIOC,GPIO_Pin_2);//erreger
			amp2 = (t1_mid-t1)*(t1_mid-t1)+(t2_mid-t2)*(t2_mid-t2);
			if(w >= 0){
				data[l][1][0] = t1_mid - t1;
				data[l][1][1] = t2_mid - t2;
				l++;
			}
		}
		if(l == 10 && k == 10){
			w = -1;
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

float get_cmd(float periode){
	static float time = 0.0;
	static float pos = 0.0;

	time += periode;

	switch(mode){
		case 0: // hold
			return(0.0);
		case 1: // enc
			return(get_enc_pos());
		case 2: // vel
			pos += amp * periode;
			pos = mod(pos);
			return(pos);
		case 3: // square
			if(sinf(freq * 2 * M_PI * time) > 0){
				return(amp);
			}
			else{
				return(-amp);
			}
		case 4: // sine
			return(amp * sinf(freq * 2 * M_PI * time));
	}
	return(0.0);
}

void TIM5_IRQHandler(void){ //1KHz
	TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
	float s = 0,c = 0;
	int freq = 1000;
	float periode = 1.0 / freq;
	kal.periode = periode;
	for(int i = 0;i<10;i++){
		s += data[i][0][0] * 0.05;
		c += data[i][0][1] * 0.05;
	}
	for(int i = 0;i<10;i++){
		s += data[i][1][0] * 0.05;
		c += data[i][1][1] * 0.05;
	}
	//revs = (int)(ist/(2*M_PI));
	//ist = revs*2*M_PI+atan2f(s,c);
	ist = atan2f(s,c);

	if(count > 1000){
		if(count < 2000){
			res_s_var += s * s / 1000;
			res_c_var += c * c / 1000;
		}
	}

	count++;

	soll_pos = startpos + get_cmd(periode) + res_offset;//MIN(res_pos1, res_pos2) + MIN(ABS(minus(res_pos1,res_pos2)), ABS(minus(res_pos2,res_pos1))) / 2;
	soll_pos = mod(soll_pos);

	pid.feedbacka = (minus(ist, ist_old) * freq - pid.feedbackv) * freq * 0.5 + pid.feedbacka * 0.5;
	pid.feedbackv = minus(ist, ist_old) * freq * 0.5 + pid.feedbackv * 0.5;
	pid.commanda = (minus(soll_pos, soll_pos_old) * freq - pid.commandv) * freq * 0.5 + pid.commanda * 0.5;
	pid.commandv = minus(soll_pos, soll_pos_old) * freq * 0.5 + pid.commandv * 0.5;
	pid.error = minus(soll_pos, ist);

	soll_pos_old = soll_pos;
	ist_old = ist;

	/*if(ABS(pid.error) > DEG(90)){
	  disable();
	  state = EFOLLOW;
	  pid.enable = 0;
	  }*/

	kal.res = ist;
	update(&kal);

	calc_pid(&pid, periode * 1000.0);
	kal.volt = pid.output;
	predict(&kal);
	voltage_scale = pid.output;

	if(amp1 < 1000000 || amp2 < 1000000){
		//voltage_scale = 0.0;
		state = EFEEDBACK;
	}
	output_ac_pwm();
	w=0;//request data
}

int main(void)
{
#define MAX_WAVE 4
	unsigned char buf[MAX_WAVE * 2 + 2];
	int wave[MAX_WAVE];
	int bufpos = 0;
	char name[] = "wave ";
	int e = 0;

	setup();
	param_init();

	for(bufpos = 0; bufpos < MAX_WAVE; bufpos++){
		name[4] = bufpos + '1';
		register_int(name,&wave[bufpos]);
		wave[bufpos] = 0;
	}

	register_int("e",&pid.enable);
	register_float("p",&pid.pgain);
	register_float("i",&pid.igain);
	register_float("d",&pid.dgain);
	register_float("b",&pid.bgain);
	// register_float("ff0",&pid.ff0gain);
	register_float("ff1",&pid.ff1gain);
	register_float("ff2",&pid.ff2gain);
	register_float("w",&w);
	register_int("rescal",&rescal);
	register_float("ist",&ist);
	register_float("calv",&calv);
	register_float("s_var",&res_s_var);
	register_float("c_var",&res_c_var);
	register_float("amp",&amp);
	register_float("freq",&freq);
	register_int("mode",&mode);
	register_float("startpos",&startpos);

	state = STBY;

	GPIO_ResetBits(GPIOC,GPIO_Pin_2);//reset erreger
	Wait(10);
	TIM_Cmd(TIM2, ENABLE);//int
	Wait(50);
	erreger_enable = YES;
	Wait(50);
	TIM_Cmd(TIM4, ENABLE);//PWM
	TIM_Cmd(TIM5, ENABLE);//PID
	Wait(50);
	startpos = get_res_pos();
	pid.enable = 1;
	enable();



	while(1)  // Do not exit
	{
		//printf_("%f %f diff: %f\r",RAD(res_pos1),RAD(res_pos2),RAD(res_pos1-res_pos2));
		//printf_("%i %i",t1_mid,t2_mid);
		//printf_("%i %i diff: %i\r",amp1,amp2,amp1-amp2);
		for(bufpos = 0; bufpos < MAX_WAVE; bufpos++){
			switch(wave[bufpos]){
				case 1:
					e = (int)RAD(pid.error);
					break;
				case 2:
					e = (int)RAD(pid.error_d);
					break;
				case 3:
					e = (int)RAD(pid.error_dd);
					break;
				case 4:
					e = (int)RAD(soll_pos);
					break;
				case 5:
					e = (int)RAD(pid.cmd_d);
					break;
				case 6:
					e = (int)RAD(pid.cmd_dd);
					break;
				case 7:
					e = (int)RAD(ist);
					break;
				case 8:
					e = (int)RAD(pid.feedbackv);
					break;
				case 9:
					e = (int)(voltage_scale * 10);
					break;
				case 10:
					e = (int)(pid.saturated_count / 10);
					break;
				case 11:
					e = (int)RAD(mag_pos);
					break;
				case 12:
					e = (int)RAD(startpos);
					break;
				case 13:
					e = (int)RAD(res_offset);
					break;
				default:
					e = 0;
			}
			e = CLAMP(e + 128,1,255);

			buf[bufpos * 2] = bufpos + 128;
			buf[bufpos * 2 + 1] = e;
		}
		buf[MAX_WAVE * 2] = 255;
		buf[MAX_WAVE * 2 + 1] = 0;


		//printf_("e: %f\n", pid.error);
		//printf_("soll: %f\n", soll_pos);

#ifdef USBTERM
		if(UB_USB_CDC_GetStatus()==USB_CDC_CONNECTED){
			UB_USB_CDC_SendString(buf, NONE);//schleppfehler senden
			/*
			   if(w == -1){
			   w = -2;
			   printf_("pos:\n");
			   for(int i = 0;i<10;i++){
			   printf_("%i %i\n",data[i][0][0], data[i][0][1]);
			   }
			   printf_("neg:\n");
			   for(int i = 0;i<10;i++){
			   printf_("%i %i\n",data[i][1][0], data[i][1][1]);
			   }
			   }
			 */
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
					//if(name[0] == '?')
					list_param();
					//else
					//	printf_("unknown command\n");
					//break;
			}
			//if(UB_USB_CDC_ReceiveString(rx_buf)==RX_READY) {
			//  UB_USB_CDC_SendString(rx_buf,LF);
			//
			//}
		}
#endif

		Wait(20);
	}
}

void Wait(unsigned int ms){
	volatile unsigned int t = time + ms;
	while(t >= time){
	}
}
