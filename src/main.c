#include <stm32f4_discovery.h>
#include <stm32f4xx_conf.h>
//#include "../../sin.h"
//#include "printf.h"
//#include "scanf.h"
//include "stlinky.h"
//#include "param.h"
#include "setup.h"
#include <math.h>

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
volatile float current_scale = 0.2;

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
    TIM4->CCR3 = (sinf(ctr + offsetc) * pwm_scale * current_scale + 1.0) * mag_res / 2.0;
}

void TIM2_IRQHandler(void){//1KHz
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    GPIO_SetBits(GPIOD,GPIO_Pin_11);
    mag_pos+=pi/100;
    output_pwm();
    GPIO_ResetBits(GPIOD,GPIO_Pin_11);
}

void TIM7_IRQHandler(void){//DAC int handler
    TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
}


void ADC_IRQHandler(void)
{
    //int t1, t2;
    while(!ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC));
    ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
    //GPIO_SetBits(GPIOD,GPIO_Pin_11);

    //t1 = ADC_GetConversionValue(ADC1);
    //t2 = ADC_GetConversionValue(ADC2);
}

int main(void)
{
    setup();
    TIM_Cmd(TIM4, ENABLE);//PWM
    TIM_Cmd(TIM2, ENABLE);//int
    GPIO_SetBits(GPIOD,GPIO_Pin_15);//enable

    while(1)  // Do not exit
    {
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

void Wait(unsigned int ms){
	volatile unsigned int t = time + ms;
	while(t >= time){
	}
}
