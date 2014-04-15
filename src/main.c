#include <stm32f4_discovery.h>
#include <stm32f4xx_conf.h>
#include "stm32_ub_usb_cdc.h"
#include "../../sin.h"
#include "printf.h"
#include "scanf.h"
#include "stlinky.h"
#include "param.h"
#include "setup.h"
#include <math.h>
#include "ang.h"

char buf[APP_TX_BUF_SIZE]; // puffer fuer Datenempfang

int __errno;
void Delay(volatile uint32_t nCount);
void Wait(unsigned int ms);

#define ABS(a)	   (((a) < 0) ? -(a) : (a))
#define CLAMP(x, low, high)  (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))
#define MIN(a, b)  (((a) < (b)) ? (a) : (b))
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))

#define pole_count 4
float max_mag_diff;
volatile struct ang res_offset;

	#define pwm_scale 0.8
	#define sin_res 1024

	#define NO 0
	#define YES 1

	struct ang offseta;
	struct ang offsetb;
	struct ang offsetc;

	#define read_pos  (7+8)
	#define read_neg  (22+8)
	#define read_w  4

	volatile int res1_pos;
volatile int res2_pos;
volatile int res1_neg;
volatile int res2_neg;
volatile int res_avg;
volatile int t1, t2;
//volatile int res_avg_tmp;

volatile int followe;

// pid para
volatile struct pid_para{
	volatile float p;
	volatile float i;
	volatile float d;
	volatile float ff0;
	volatile float ff1;
	volatile float i_limit;
	volatile float error_limit;
	volatile float output_limit;
	volatile float periode;
} pid_p;

// pid cur para
float cur_p;
float cur_min;
float cur_max;

// pid storage
volatile struct pid_mem{
	volatile float error_old;
	volatile float error_sum;
	volatile float in_old;
	volatile float in_old_old;
} pid_m;

// use kalman
volatile float kal;


// soll
volatile struct ang mot_pos;
	volatile struct ang mot_vel;

	// ctr
	volatile struct ang mag_pos;
	volatile struct ang mag_offset;
	volatile float current_scale;

// ist
volatile struct ang vel;
	volatile struct ang res_pos;
	volatile struct ang res_pos_pos;
	volatile struct ang res_neg_pos;


	// state
	volatile int do_pid;
volatile int do_cal;
volatile int do_rt_cal;


volatile int dacpos;

int strcmp(char* x,char* y){
	int i = 0;
	while(x[i] == y[i] && x[i] != 0)
		i++;
	if(x[i] == y[i] && x[i] == 0)
		return 1;
	else
		return 0;
}

void output_pwm(){
	struct ang ctr = mul(plus(plus(mag_pos, res_offset), mag_offset), pole_count);
	//struct ang ctr = plus(mag_pos, mag_offset);
	float ctra = ang_sin(plus(ctr, offseta));
	float ctrb = ang_sin(plus(ctr, offsetb));
	float ctrc = ang_sin(plus(ctr, offsetc));
	TIM4->CCR1 = (ctra * pwm_scale * current_scale + 1.0) * mag_res / 2.0;
	TIM4->CCR2 = (ctrb * pwm_scale * current_scale + 1.0) * mag_res / 2.0;
	TIM4->CCR3 = (ctrc * pwm_scale * current_scale + 1.0) * mag_res / 2.0;
}

volatile struct kal1D{
	volatile float state;
	volatile float state_var;
	volatile float sens;
	volatile float sens_var;
	volatile float move;
	volatile float move_var;
} k_pos;

void kalman1D(struct kal1D *k){
	// update
	k->state = (k->sens_var * k->state + k->state_var * k->sens) / (k->state_var + k->sens_var);
	k->state_var = 1.0/(1.0/k->state_var + 1.0/k->sens_var);

	// predict
	k->state += k->move;
	k->state_var += k->move_var;
}

/*float acc(float vel, float dist, float max_acc, float min_acc, float max_vel, float time_step){
	float max_break_time = sqrtf(2.0 * ABS(dist) / min_acc); // s = a/2 * t^2, t = sqrt(2s/a)
	float max_break_vel = min_acc * max_break_time; // v = a * t

	float v = min(max_break_vel, max_vel);
	if(dist < 0){
		v *= -1.0;
	}
	if(vel <= v){
		return(min(max_acc, (v - vel) / time_step)); // v = a * t, a = v / t
	}
	else{
		return(max(min_acc, (v - vel) / time_step)); // v = a * t, a = v / t
	}
}*/

void init_pid(){
	// kalman
	k_pos.state = 1.0;
	k_pos.state_var = 1.0;
	//k_pos.sens = 0.0;
	//k_pos.sens_var = 1.0;
	k_pos.move = 0.0;
	k_pos.move_var = 0.0001;
	kal = 1;

	max_mag_diff = 2*pi/pole_count/4;

	// pid para
	// pid_p
	pid_p.periode = 1/2000.0;
	pid_p.p = 2.0;
	pid_p.i = 0.0;
	pid_p.d = 0.0;
	pid_p.ff0 = 0.0;
	pid_p.ff1 = 0.0;
	pid_p.i_limit = max_mag_diff;
	pid_p.output_limit = max_mag_diff;

	// pid cur para
	cur_p = 1 / (max_mag_diff);
	cur_min = 0.1;
	cur_max = 1;

	// pid storage
	pid_m.error_old = 0;
	pid_m.error_sum = 0;
	pid_m.in_old = 0;
	pid_m.in_old_old = 0;

}

void pid(){ // calc force / mag_offset
	// in v, res_vel
	// out mag_offset
	if(pid_p.i != 0){
		pid_p.i_limit = max_mag_diff / (pid_p.i * pid_p.periode);
	}

	float error = rad(minus(mot_pos, res_pos));
	float error_d = error - pid_m.error_old;

	pid_m.error_old = error;
	pid_m.error_sum = CLAMP((pid_m.error_sum + error), -pid_p.i_limit, pid_p.i_limit);

	float ctr = CLAMP(
		pid_p.p * error// +
		//pid_p.i * pid_p.periode * pid_m.error_sum// +
		//pid_p.d / pid_p.periode * error_d
		, -pid_p.output_limit, pid_p.output_limit);

	mag_offset = new_ang(ctr);

	//current_scale = CLAMP(ABS((cur_p * cur_p * cur_p * cur_p * mag_offset * mag_offset * mag_offset * mag_offset)) * (cur_max - cur_min) + cur_min, 0, 1);
	current_scale = CLAMP(ABS((cur_p * ctr) * (cur_p * ctr)) * (cur_max - cur_min) + cur_min, 0, 1);
}


void TIM2_IRQHandler(void){//PWM int handler, 10KHz
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	//rotate_mag();
	//output_pwm();
}

void TIM7_IRQHandler(void){//DAC int handler
	TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
    GPIO_SetBits(GPIOD,GPIO_Pin_11);
	ADC_SoftwareStartConv(ADC1);
	ADC_SoftwareStartConv(ADC2);
}


void ADC_IRQHandler(void)
{
	while(!ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC));
	ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);


	t1 = ADC_GetConversionValue(ADC1);
	t2 = ADC_GetConversionValue(ADC2);
	res_avg = res_avg * 0.99 + (t1 + t2)/2 * 0.01;
	t1 -= res_avg;
	t2 -= res_avg;

	if(dacpos % 4 == 0 || dacpos % 4 == 1){
        GPIO_SetBits(GPIOA,GPIO_Pin_7);
	}else{
        GPIO_ResetBits(GPIOA,GPIO_Pin_7);
    }
    
    if(dacpos >= 19){
        dacpos = 0;
        GPIO_ResetBits(GPIOD,GPIO_Pin_11);
        //pid();
        //output_pwm();
    }else{
        dacpos++;
    }
    
	float max = 930;
	if(dacpos >= read_pos - read_w && dacpos <= read_pos + read_w){
		res1_pos += t1;
		res2_pos += t2;
	}
	else if(dacpos >= read_neg - read_w || dacpos <= (read_neg + read_w) % 32){
		res1_neg -= t1;
		res2_neg -= t2;
	}
}

/*void findoff(void){
    float oldpos;
    float initpos;
    printf_("finding\n");
    mag_pos = new_ang(DEG(0));
    oldpos = 10;
    while(ABS(res_pos-oldpos) > DEG(0.1)){
        oldpos = res_pos;
    }
    printf_("off: %f\n",res_pos);
    while(1){}
}*/



void rt_cal(){
}

/*void mot_cal(){
	do_pid = NO;
	do_cal = YES;
	do_rt_cal = NO;

	current_scale = 1;
	mag_pos = 0;
	mag_offset = 0;
	res_offset = 0;
	vel = 0;
	Wait(500);

	// cal res noise
	unsigned int N = 5000;
	float res_mu = 0;
	float res_var = 0;


	for(int i = 0; i < N; i++){
		res_mu += res_pos / N;
		Delay(1000);
	}

	for(int i = 0; i < N; i++){
		res_var += (res_pos - res_mu) * (res_pos - res_mu);
		Delay(1000);
	}
	res_var /= N;


	// cal mot_dir
	float start_pos = res_pos;
	float mot_dir = 1;

	vel = DEG(10);
	while(ABS(minus(start_pos, res_pos)) < DEG(10));
	vel = 0;

	if(minus(start_pos, res_pos) < 0){
		mot_dir = -1;
	}

	// cal pole_count
	float mot_pole_count = 0;

	mag_pos =  0;
	Wait(500);

	start_pos = res_pos;
	while(ABS(minus(start_pos, res_pos)) < DEG(90)){
		mag_pos += DEG(360/100);
		Wait(50);
		mot_pole_count++;
	}
	while(ABS(minus(start_pos, res_pos)) > DEG(5)){
		mag_pos += DEG(360/100);
		Wait(50);
		mot_pole_count++;
	}

	mot_pole_count /= 100;
	mot_pole_count += 0.5;
	mot_pole_count = (int)mot_pole_count;


	// cal res_offset
	res_offset = 0;
	vel = DEG(20);
	Wait(1000);
	vel = 0;
	mag_pos = 0;
	Wait(500);
	for(int i = 0; i < 50; i++){
		res_offset += res_pos / 100;
	}

	vel = DEG(-20);
	Wait(1000);
	vel = 0;
	mag_pos = 0;
	Wait(500);
	for(int i = 0; i < 50; i++){
		res_offset += res_pos / 100;
	}


	// cal Vmax, Amax



	current_scale = 0.3;
}*/

void init(){
}

int main(void)
{
	offseta = new_ang(DEG(0));
	offsetb = new_ang(DEG(120));
	offsetc = new_ang(DEG(240));

	res_avg = 2051;
	dacpos = 0;
	init_pid();
	param_init();
	setup();
	mag_pos = new_ang(DEG(0));
	mot_pos = new_ang(DEG(0));
	followe = NO;
	current_scale = 1;
	mot_vel = new_ang(DEG(0));
	/* TIM4 enable counter */
	res_pos.x = 0;
	res_pos.y = 0;
  
  while(1)
  {
    // Test ob USB-Verbindung zum PC besteht
    if(UB_USB_CDC_GetStatus()==USB_CDC_CONNECTED) {
      // Ceck ob Daten per USB empfangen wurden
      //if(UB_USB_CDC_ReceiveString(buf)==RX_READY) {
        // wenn Daten empfangen wurden
        // als Echo wieder zurücksenden
        // (mit LineFeed+CarriageReturn)
        //UB_USB_CDC_SendString(buf,NONE);
          printf_("avg:%i t1:%i t2:%i\r\n",res_avg,t1,t2);
          //}
    }
  }

	while (len(res_pos) != 1){
	}
	//	findoff();
	//    res_offset = 0.821051;
	res_offset = new_ang(DEG(37));
	Wait(10);
	mot_pos = res_pos;
	TIM_Cmd(TIM4, ENABLE);//PWM
	TIM_Cmd(TIM2, ENABLE);//int
	GPIO_SetBits(GPIOD,GPIO_Pin_15);//enable
	//mot_pos = new_ang(0);

	int buffer_pos = 0;
	int obuf_pos = 0;
	int line_pos = 0;
	int i = 0;
	int scan = 0;
	int histpos = 0;
	char buf[STLINKY_BSIZE];
	char outbuf[STLINKY_BSIZE];
	char history[10][STLINKY_BSIZE];
	char backspace[] = {0x8,0x20,0x8};
	char ansiright[] = {0x1b,'[','C'};
	char ansileft[] = {0x1b,'[','D'};
	char ansierase[] = {0x1b,'[','2','K'};
	//int ansistate = 0;
	enum{init,ansi,bracket,letter}ansistate;
	//char line[STLINKY_BSIZE];
	char c;
	float f;
	float p1 = 0;
	float p2 = 0;
	float m = 0;
	int scanf_ret = 0;
	ansistate = init;
	register_float('p', &pid_p.p);
	register_float('i', &pid_p.i);
	register_float('d', &pid_p.d);
	register_float('m', &m);
	//register_float('k', &kal);
	//register_float('v', &mot_vel);

	float res_vel = 0;
	float verror = 0;

	float res_mu = 0;
	float res_var = 0;
	float N = 100.0;

	Wait(1);

	for(int i = 0; i < N; i++){
		res_mu += rad(res_pos);
		Wait(1);
	}
	res_mu /= N;

	float t = 0;
	float tt = 0;
	for(int i = 0; i < N; i++){
		tt = rad(res_pos);
		t = (tt - res_mu) * (tt - res_mu);
		res_var += t;
		Wait(1);
	}
	res_var /= N;

	//k_pos.state = res_mu;
	k_pos.sens_var = sqrtf(res_var);

	while(1)  // Do not exit
	{
        if(UB_USB_CDC_GetStatus()==USB_CDC_CONNECTED) {
          // Ceck ob Daten per USB empfangen wurden
            if(UB_USB_CDC_ReceiveString(buf)==RX_READY) {
            // wenn Daten empfangen wurden
            // als Echo wieder zurücksenden
            // (mit LineFeed+CarriageReturn)
            UB_USB_CDC_SendString(buf,NONE);
            //printf_("fdsa");
          }
      }
	}

}

void Delay(volatile uint32_t nCount) {
	//float one;
	while(nCount--)
	{
		//one = (float) nCount;
	}
}

void Wait(unsigned int ms){
	volatile unsigned int t = time + ms;
	while(t >= time){
	}
}
