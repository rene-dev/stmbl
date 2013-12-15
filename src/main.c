#include <stm32f4_discovery.h>
#include <stm32f4xx_conf.h>
#include "../../sin.h"
#include "printf.h"
#include "scanf.h"
#include "stlinky.h"
#include "param.h"
#include "setup.h"
#include <math.h>

void Delay(volatile uint32_t nCount);

#define pi 3.14159265
#define ABS(a)	   (((a) < 0) ? -(a) : (a))
#define CLAMP(x, low, high)  (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))
#define MIN(a, b)  (((a) < (b)) ? (a) : (b))
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))
#define DEG(a) ((a)*pi/180.0)
#define RAD(a) ((a)*180.0/pi)

#define pole_count 4
float max_mag_diff;
volatile float res_offset;

#define pwm_scale 0.8
#define sin_res 1024

#define NO 0
#define YES 1

#define offseta 0.0 * 2.0 * pi / 3.0
#define offsetb 1.0 * 2.0 * pi / 3.0
#define offsetc 2.0 * 2.0 * pi / 3.0

#define read_pos  (7+8)
#define read_neg  (22+8)
#define read_w  4

volatile int res1_pos;
volatile int res2_pos;
volatile int res1_neg;
volatile int res2_neg;
volatile int res_avg;
volatile int res_avg_tmp;

volatile int followe;

// pid para
struct pid_para{
	float p;
	float i;
	float d;
	float ff0;
	float ff1;
	float i_limit;
	float error_limit;
	float output_limit;
	float periode;
} pid_pp, pid_fp;

// pid cur para
float cur_p;
float cur_min;
float cur_max;

// pid storage
struct pid_mem{
	float error_old;
	float error_sum;
	float in_old;
	float in_old_old;
} pid_pm, pid_fm;

volatile float mot_pos;
volatile float mot_vel;
volatile float vel;
volatile float mag_pos;
volatile float mag_offset;
volatile float current_scale;
volatile float res_pos;
volatile float res_pos_old;
volatile float res_pos_pos;
volatile float res_neg_pos;

volatile int do_pid;
volatile float do_pos;

volatile int dacpos;

const uint16_t sin1[] = { // Sinus
  2047, 2447, 2831, 3185, 3498, 3750, 3939, 4056,
  4095, 4056, 3939, 3750, 3495, 3185, 2831, 2447,
  2047, 1647, 1263,  909,  599,  344,  155,   38,
     0,   38,  155,  344,  599,  909, 1263, 1647
};

float sine(float x){
    while(x < -pi){
        x += 2.0 * pi;
    }
    while(x > pi){
        x -= 2.0 * pi;
    }
    return(sint[(int)(x * sin_res / 2.0 / pi) + sin_res / 2]);
}

float minus(float a, float b){
	if(ABS(a - b) < pi){
		return(a - b);
	}
	else if(a > b){
		return(a - b - 2.0 * pi);
	}
	else{
		return(a - b + 2.0 * pi);
	}
}

float mod(float a){
    while(a < -pi){
        a += 2.0 * pi;
    }
    while(a > pi){
        a -= 2.0 * pi;
    }
    return(a);
}

void output_pwm(){
    float ctr = mod((mag_pos + res_offset + mag_offset) * pole_count);
    TIM4->CCR1 = (sine(ctr + offseta) * pwm_scale * current_scale + 1.0) * mag_res / 2.0;
    TIM4->CCR2 = (sine(ctr + offsetb) * pwm_scale * current_scale + 1.0) * mag_res / 2.0;
    TIM4->CCR3 = (sine(ctr + offsetc) * pwm_scale * current_scale + 1.0) * mag_res / 2.0;
}

void init_pid(){
	max_mag_diff = 2*pi/pole_count/4;

	// pid para
	// pid_p
	pid_pp.periode = 1/2000.0;
	pid_pp.p = 0.5;
	pid_pp.i = 0.0;
	pid_pp.d = 0.0;
	pid_pp.ff0 = 0.0;
	pid_pp.ff1 = 0.0;
	pid_pp.i_limit = 11500 / 60.0 * 2.0 * pi;
	pid_pp.output_limit = 11500 / 60.0 * 2.0 * pi;

	// pid_f
	pid_fp.periode = 1/2000.0;
	pid_fp.p = 0.0;
	pid_fp.i = 0.0;
	pid_fp.d = 0.0;
	pid_fp.ff0 = 0.0;
	pid_fp.ff1 = 0.0;
	pid_fp.i_limit = max_mag_diff;
	pid_fp.output_limit = max_mag_diff;

// pid cur para
	cur_p = 1 / (max_mag_diff);
	cur_min = 0.1;
	cur_max = 1;

// pid storage
	pid_pm.error_old = 0;
	pid_pm.error_sum = 0;
	pid_pm.in_old = 0;
	pid_pm.in_old = 0;

	pid_fm.error_old = 0;
	pid_fm.error_sum = 0;
	pid_fm.in_old = 0;
	pid_fm.in_old = 0;
}

void rotate_mag(){ // rotate mag_pos
	// in mag_pos, v
	// out mag_pos
	mag_pos += vel/10000;
	mag_pos = mod(mag_pos);
}

void pid_f(){ // calc force / mag_offset
	// in v, res_vel
	// out mag_offset
	float res_vel = minus(res_pos, res_pos_old) / pid_fp.periode;
	float error = minus(vel, res_vel);
	//float error = minus(mot_pos, res_pos);
	float error_d = error - pid_fm.error_old;
	float in_d = minus(vel, pid_fm.in_old);
	//float in_d = minus(res_pos, pid_fm.in_old);
	float in_dd = minus(in_d, minus(pid_fm.in_old, pid_fm.in_old_old));

	pid_fm.error_old = error;
	pid_fm.error_sum = CLAMP((pid_fm.error_sum + error), -pid_fp.i_limit, pid_fp.i_limit);
	pid_fm.in_old_old = pid_fm.in_old;
	//pid_fm.in_old = res_pos;
	pid_fm.in_old = vel;

	mag_offset = CLAMP(mod(
		pid_fp.p * error +
		pid_fp.i * pid_fp.periode * pid_fm.error_sum +
		pid_fp.d / pid_fp.periode * error_d +
		pid_fp.ff0 * in_d +
		pid_fp.ff1 * in_dd
		), -pid_fp.output_limit, pid_fp.output_limit);

	//current_scale = CLAMP(ABS((cur_p * cur_p * cur_p * cur_p * mag_offset * mag_offset * mag_offset * mag_offset)) * (cur_max - cur_min) + cur_min, 0, 1);
	current_scale = CLAMP(ABS((cur_p * mag_offset)) * (cur_max - cur_min) + cur_min, 0, 1);
}

void pid_p(){ // calc v
	// in res_pos, mot_pos
	// out v

	float error = minus(mot_pos, res_pos);
	float error_d = error - pid_pm.error_old;
	float in_d = minus(mot_pos, pid_pm.in_old);
	float in_dd = minus(in_d, minus(pid_pm.in_old, pid_pm.in_old_old));

	pid_pm.error_old = error;
	pid_pm.error_sum = CLAMP((pid_pm.error_sum + error), -pid_pp.i_limit, pid_pp.i_limit);
	pid_pm.in_old_old = pid_pm.in_old;
	pid_pm.in_old = mot_pos;


	if(do_pos){
		vel =
			pid_pp.p * error +
			pid_pp.i * pid_pp.periode * pid_pm.error_sum +
			pid_pp.d / pid_pp.periode * error_d +
			pid_pp.ff0 * in_d +
			pid_pp.ff1 * in_dd
			;
	}
	else{
		vel = mot_vel;
	}
}

void TIM2_IRQHandler(void){//PWM int handler, 10KHz
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    rotate_mag();
    output_pwm();
    //
}

void TIM7_IRQHandler(void){//DAC int handler
    TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
    dacpos++;//DMA fragen?
    if(dacpos >= 32){
        dacpos = 0;
    }

    if((dacpos >= read_pos - read_w && dacpos <= read_pos + read_w) || (dacpos >= read_neg - read_w || dacpos <= (read_neg + read_w) % 32)){// phase shift 2
        ADC_SoftwareStartConv(ADC1);
        ADC_SoftwareStartConv(ADC2);
        //ADC_SoftwareStartConv(ADC3);
    }
    if(dacpos == read_pos - read_w - 1){
        res_avg = (res_avg_tmp/(read_w*4+2)/2)*0.2f+res_avg*0.8f;
        res_avg_tmp = 0;
    }
    if(dacpos == read_pos + read_w + 1){
        res_pos_pos = atan2f(res1_pos, res2_pos);
        res1_pos = 0;
        res2_pos = 0;

    }
    if(dacpos == (read_neg + read_w + 1) % 32){
		res_neg_pos = atan2f(res1_neg, res2_neg);

        res1_neg = 0;
        res2_neg = 0;

				res_pos_old = res_pos;
				res_pos = res_pos_pos + minus(res_neg_pos, res_pos_pos) / 2.0;

				pid_p();
				pid_f();
    }
}


void ADC_IRQHandler(void)
{
    int t1, t2;
    while(!ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC));
    ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
    GPIO_SetBits(GPIOD,GPIO_Pin_11);

    t1 = ADC_GetConversionValue(ADC1);
    t2 = ADC_GetConversionValue(ADC2);
    res_avg_tmp += t1+t2;
    t1 -= res_avg;
    t2 -= res_avg;

    float max = 930;
        if(dacpos >= read_pos - read_w && dacpos <= read_pos + read_w){
					res1_pos += t1;
					res2_pos += t2;
        }
        else if(dacpos >= read_neg - read_w || dacpos <= (read_neg + read_w) % 32){
        	res1_neg -= t1;
					res2_neg -= t2;
        }


    GPIO_ResetBits(GPIOD,GPIO_Pin_11);
}

void findoff(void){
    float oldpos;
    float initpos;
    printf_("finding\n");
    mag_pos = 0;
    oldpos = 10;
    while(ABS(res_pos-oldpos) > DEG(1)){
        oldpos = res_pos;
    }
    printf_("off: %f\n",res_pos);
    while(1){}
}

int main(void)
{
    res_avg = 2051;
    res_avg_tmp = 0;
    do_pid = YES;
		do_pos = YES;
    dacpos = 31;
	init_pid();
	param_init();
    setup();
    mag_pos = 0;
    mot_pos = 0;
    followe = NO;
    current_scale = 0.3;
    mot_vel = 0;
    /* TIM4 enable counter */
    res_pos = -10;
    while (res_pos == -10){
    }
    res_offset = 2.4885;
    mot_pos = res_pos;
    TIM_Cmd(TIM4, ENABLE);//PWM
    TIM_Cmd(TIM2, ENABLE);//int
    GPIO_SetBits(GPIOD,GPIO_Pin_15);//enable


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
	int scanf_ret = 0;
	ansistate = init;
	register_float('p', &pid_pp.p);
	register_float('i', &pid_pp.i);
	register_float('d', &pid_pp.d);
	register_float('a', &pid_fp.p);
	register_float('b', &pid_fp.i);
	register_float('c', &pid_fp.d);
	register_float('m', &mot_pos);
	register_float('p', &do_pos);
	register_float('v', &mot_vel);

	float res_vel = 0;
	float verror = 0;

    if(!do_pid)
        findoff();

    while(1)  // Do not exit
    {
        //printf_("p1 = %f, p2 = %f, n1 = %f, n2 = %f\n", max_res1, max_res2, min_res1, min_res2);
        //printf_("p = %f, n = %f\rn", res_pos_pos, res_neg_pos);
        //printf_("error = %f\tmot = %f\tcur = %f\n", minus(mot_pos, res_pos), mot_pos, current_scale);
        Delay(1000000);
				res_vel = minus(res_pos, res_pos_old) / pid_fp.periode;
				verror = minus(vel, res_vel);
        printf_("pe = %f\tmo = %f\tc = %f\tv = %f\trv = %f\tve = %f\t     \r", minus(mot_pos, res_pos), mag_offset, current_scale, vel, res_vel, verror);

		if(stlinky_todo(&g_stlinky_term) == 0 && obuf_pos > 0){
			stlinky_tx(&g_stlinky_term, outbuf, obuf_pos);
			obuf_pos = 0;
		}
		buffer_pos = stlinky_avail(&g_stlinky_term);
		if(buffer_pos > 0){
			buffer_pos = stlinky_rx(&g_stlinky_term, buf, STLINKY_BSIZE);
			for(i = 0;i<buffer_pos;i++){
				if(buf[i] == 127){//backspace
					stlinky_tx(&g_stlinky_term, backspace, 3);
					line_pos = CLAMP(line_pos-1, 0, SCANF_BSIZE);
				}else if(buf[i] == 27){//ANSI control
					ansistate = bracket;
				}else if(buf[i] == '[' && ansistate == bracket){
					ansistate = letter;
				}else if(buf[i] == 'A' && ansistate == letter){//up
					stlinky_tx(&g_stlinky_term, ansierase, 4);
					if(histpos == 0)
						histpos = 9;
					else
						histpos = histpos-1;
					printf_("\r%s",history[histpos]);
					ansistate = init;
				}else if(buf[i] == 'B' && ansistate == letter){//down
					stlinky_tx(&g_stlinky_term, ansierase, 4);
					histpos = (histpos+1)%10;
					printf_("\r%s",history[histpos]);
					ansistate = init;
				}else if(buf[i] == 'C' && ansistate == letter){//right
					stlinky_tx(&g_stlinky_term, ansiright, 3);
					line_pos = CLAMP(line_pos+1, 0, SCANF_BSIZE);
					ansistate = init;
				}else if(buf[i] == 'D' && ansistate == letter){//left
					stlinky_tx(&g_stlinky_term, ansileft, 3);
					line_pos = CLAMP(line_pos-1, 0, SCANF_BSIZE);
					ansistate = init;
				}else if(ansistate == letter){
					ansistate = init;
				}else if(buf[i] == '\t'){
				}else if(buf[i] == '\n'){
					outbuf[obuf_pos] = buf[i];
					obuf_pos++;
					scanf_buffer[line_pos] = '\n';
					scanf_buffer[line_pos+1] = 0;
					history[histpos][line_pos] = 0;
					//printf_("saved %s at %i",history[histpos],histpos);
					histpos = (histpos+1)%10;
					//printf_("hier, parsen und so: %s",scanf_buffer);
					scanf_ret = scanf_("%c = %f", &c, &f);
					if(scanf_ret == 7){ // write
						if(set_float(c,f)){
							printf_("OK\n");
						}
						else{
							printf_("%c not found\n", c);
						}
					}
					else{ // read
						printf_("\n");
						if(c == '?'){
							for(int j = 0; j < PARAMS.param_count; j++){
								printf_("%c = %f\n", PARAMS.names[j], *(PARAMS.data[j]));
							}
						}
						else{
							printf_("%c = %f\n", c, get_float(c));
						}
					}
					line_pos = 0;
				}else{
					outbuf[obuf_pos] = buf[i];
					obuf_pos++;
					scanf_buffer[line_pos] = buf[i];
					history[histpos][line_pos] = scanf_buffer[line_pos];
					line_pos = CLAMP(line_pos+1, 0, SCANF_BSIZE);
				}
			}
		}

        //if(stlinky_todo(&g_stlinky_term) == 0){
		//if(stlinky_avail(&g_stlinky_term) != 0)
        //buffer_pos = stlinky_tx(&g_stlinky_term, buf, buffer_pos);
		//if(stlinky_todo(&g_stlinky_term) == 0)
		//buffer_pos = stlinky_rx(&g_stlinky_term, buf, buffer_pos);
			//}else{
			//}


			//buffer_pos = 0;

        /*if(followe){
            GPIO_ResetBits(GPIOD,GPIO_Pin_15);//disable
            TIM4->CCR1 = 0;
            TIM4->CCR2 = 0;
            TIM4->CCR3 = 0;
            TIM_Cmd(TIM4, DISABLE);//PWM
            TIM_Cmd(TIM2, DISABLE);//int
        }*/
    }
}

void Delay(volatile uint32_t nCount) {
    //float one;
    while(nCount--)
    {
        //one = (float) nCount;
    }
}
