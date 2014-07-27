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

#define pwm_scale 0.8

#define NO 0
#define YES 1

#define offseta 0.0 * 2.0 * pi / 3.0
#define offsetb 1.0 * 2.0 * pi / 3.0
#define offsetc 2.0 * 2.0 * pi / 3.0

volatile float mag_pos;
volatile float current_scale = 1.0;

volatile int t1, t2;//rohdaten sin/cos
volatile int t1_mid = 0,t2_mid = 0;//mittelpunkt sin/cos
volatile float res_pos1;//winkel vom resolver, -pi bsi +pi
volatile float res_pos2;//winkel vom resolver, -pi bsi +pi
volatile int erreger = 0;
volatile int erreger_enable = NO;

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
    float ctr = mod(mag_pos);
    TIM4->CCR1 = (sinf(ctr + offseta) * pwm_scale * current_scale + 1.0) * mag_res / 2.0;
    TIM4->CCR2 = (sinf(ctr + offsetb) * pwm_scale * current_scale + 1.0) * mag_res / 2.0;
    TIM4->CCR4 = (sinf(ctr + offsetc) * pwm_scale * current_scale + 1.0) * mag_res / 2.0;
}

void TIM2_IRQHandler(void){//20KHz
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    GPIO_SetBits(GPIOC,GPIO_Pin_4);//messpin
    ADC_SoftwareStartConv(ADC1);
    ADC_SoftwareStartConv(ADC2);
}

void ADC_IRQHandler(void)
{
    while(!ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC));
    ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
    GPIO_ResetBits(GPIOC,GPIO_Pin_4);//messpin

    //GPIO_ToggleBits(GPIOC,GPIO_Pin_2);//toggle erreger
    t1 = ADC_GetConversionValue(ADC1);
    t2 = ADC_GetConversionValue(ADC2);

    if(erreger_enable){//erreger signal aktiv
        if(erreger){//eine halbwelle
            GPIO_SetBits(GPIOC,GPIO_Pin_2);//erreger
            res_pos1 = atan2f(t1-t1_mid, t2-t2_mid);
        }else{//andere halbwelle
            GPIO_ResetBits(GPIOC,GPIO_Pin_2);//toggle erreger
            res_pos2 = atan2f(t1_mid-t1, t2_mid-t2);   
        }
    }else{//mittelpunkt messen
        if(t1_mid == 0 && t2_mid == 0){//erster durchlauf
            t1_mid = t1;
            t2_mid = t2;
        }else{//restliche durchläufe
            t1_mid = t1_mid * 0.99 + (t1 - t1_mid) * 0.01;
            t2_mid = t2_mid * 0.99 + (t2 - t2_mid) * 0.01;
        }
            
    }
    erreger = !erreger;
}

int main(void)
{
    setup();
    GPIO_ResetBits(GPIOC,GPIO_Pin_2);//reset erreger
    TIM_Cmd(TIM4, ENABLE);//PWM
    TIM_Cmd(TIM2, ENABLE);//int
    Wait(10);
    erreger_enable = NO;
    Wait(50);
    erreger_enable = YES;
    
    GPIO_SetBits(GPIOD,GPIO_Pin_14);//enable
    
    while(1)  // Do not exit
    {
        printf_("%f %f diff: %f",RAD(res_pos1),RAD(res_pos2),RAD(res_pos1-res_pos2));
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
