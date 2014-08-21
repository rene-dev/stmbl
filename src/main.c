#include <stm32f4_discovery.h>
#include <stm32f4xx_conf.h>
//#include "../../sin.h"
#include "printf.h"
//#include "scanf.h"
//include "stlinky.h"
//#include "param.h"
#include "setup.h"
#include <math.h>

#ifdef USBTERM
#include "stm32_ub_usb_cdc.h"
#endif

int __errno;
void Delay(volatile uint32_t nCount);
void Wait(unsigned int ms);

#define pi 3.14159265
#define ABS(a)	   (((a) < 0) ? -(a) : (a))
#define CLAMP(x, low, high)  (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))
#define MIN(a, b)  (((a) < (b)) ? (a) : (b))
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))
#define DEG(a) ((a)*pi/180.0)
#define RAD(a) ((a)*180.0/pi)

#define pole_count 4
#define res_offset DEG(52) //minimaler positiver resolver output bei mag_pos = 0

#define pwm_scale 0.9//max/min PWM duty cycle

#define NO 0
#define YES 1

#define offseta 0.0 * 2.0 * pi / 3.0
#define offsetb 1.0 * 2.0 * pi / 3.0
#define offsetc 2.0 * 2.0 * pi / 3.0

volatile float mag_pos = 0;
volatile float soll_pos = 0;
volatile float voltage_scale = 0;// -1 bis 1

volatile int t1, t2;//rohdaten sin/cos
volatile int t1_last = 0, t2_last = 0;//rohdaten sin/cos letzter aufruf
volatile int t1_mid = 0,t2_mid = 0;//mittelpunkt sin/cos
volatile float res_pos1;//winkel vom resolver, -pi bsi +pi
volatile float res_pos2;//winkel vom resolver, -pi bsi +pi
volatile int amp1,amp2;//betrag
volatile int erreger = 0;//resolver erreger pin an/aus
volatile int erreger_enable = NO;//erreger aktiv
volatile float schleppfehler = 0;

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


float get_pos(){
	return((res_pos2 + res_pos1) / 2 - res_offset);
}

void output_pwm(){
		mag_pos = get_pos() * pole_count + DEG(90);
    float ctr = mod(mag_pos);
    float volt = CLAMP(voltage_scale,-1.0,1.0);
    TIM4->CCR1 = (sinf(ctr + offseta) * pwm_scale * volt + 1.0) * mag_res / 2.0;
    TIM4->CCR2 = (sinf(ctr + offsetb) * pwm_scale * volt + 1.0) * mag_res / 2.0;
    TIM4->CCR4 = (sinf(ctr + offsetc) * pwm_scale * volt + 1.0) * mag_res / 2.0;
}

void TIM2_IRQHandler(void){ //20KHz
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    GPIO_SetBits(GPIOC,GPIO_Pin_4);//messpin
    ADC_SoftwareStartConv(ADC1);
    ADC_SoftwareStartConv(ADC2);
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
            res_pos1 = atan2f(t1-t1_mid, t2-t2_mid);
            amp1 = (t1-t1_mid)*(t1-t1_mid)+(t2-t2_mid)*(t2-t2_mid);
        }else{//andere halbwelle
            GPIO_ResetBits(GPIOC,GPIO_Pin_2);//erreger
            res_pos2 = atan2f(t1_mid-t1, t2_mid-t2);
            amp2 = (t1_mid-t1)*(t1_mid-t1)+(t2_mid-t2)*(t2_mid-t2);
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

struct pid_context{
	float old_ist;
	float i_sum;
	float p;
	float i;
	float d;
	float periode;
	float i_max;
	float v;
} c;

float pid(float ist, float soll, struct pid_context* context){
	float ctr = 0;
	float err = minus(soll, ist);
  schleppfehler = err;//TODO: in kontext oder so
	float p = context->p / context->v * 10;
	float i = context->i / context->v * 10;
	float d = context->d / context->v * 10;

	context->i_sum = CLAMP(context->i_sum + err * context->periode, -context->i_max / i, context->i_max / i);

	ctr = p * err + i * context->i_sum;
	return(CLAMP(ctr, -1, 1));
}

void TIM5_IRQHandler(void){ //1KHz
	TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
	c.v = 30;
	c.p = 5;
	c.periode = 0.001;
	c.i_max = 1;
	c.i = 30;
	c.d = 0;
	float ist = get_pos();

	voltage_scale = pid(ist, soll_pos, &c);

	if(amp1 < 1000000 || amp2 < 1000000){
		voltage_scale = 0.0;
	}

	output_pwm();
}

int main(void)
{
    setup();
    GPIO_ResetBits(GPIOC,GPIO_Pin_2);//reset erreger
    Wait(10);
    TIM_Cmd(TIM2, ENABLE);//int
    Wait(50);
    erreger_enable = YES;
    Wait(50);
		soll_pos = get_pos();
		TIM_Cmd(TIM4, ENABLE);//PWM
		TIM_Cmd(TIM5, ENABLE);//PID

    GPIO_SetBits(GPIOD,GPIO_Pin_14);//enable

    while(1)  // Do not exit
    {
        //printf_("%f %f diff: %f\r",RAD(res_pos1),RAD(res_pos2),RAD(res_pos1-res_pos2));
        //printf_("%i %i",t1_mid,t2_mid);
        //printf_("%i %i diff: %i\r",amp1,amp2,amp1-amp2);
        int e = (int)((RAD(schleppfehler)+180)/360*127)+127;
        char buf[2];
        buf[0] = e;
        buf[1] = 0;
        printf_("%i\r",e);
        
        #ifdef USBTERM
        if(UB_USB_CDC_GetStatus()==USB_CDC_CONNECTED){
	        UB_USB_CDC_SendString(buf, NONE);
        }
        #endif
        
        Wait(50);
    }
}

//grob 12us
void Delay(volatile uint32_t nCount) {
    float one;
    while(nCount--)
    {
        one = (float) nCount;
    }
}

void Wait(unsigned int ms){
	volatile unsigned int t = time + ms;
	while(t >= time){
	}
}
