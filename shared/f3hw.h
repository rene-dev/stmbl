//stmbl
#define AREF 3.3  // analog reference voltage

#define VDIVUP 20000.0 * 2.0  //HV div pullup R1,R12
#define VDIVDOWN 1000.0  //HV div pulldown R2,R9
#define SHUNT 0.013  //shunt
#define SHUNT_PULLUP 3900.0
#define SHUNT_SERIE 120.0

#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC

#define PWM_U TIM8->CCR3
#define PWM_V TIM8->CCR2
#define PWM_W TIM8->CCR1

#define PWM_DEADTIME 0
#define PWM_FREQ 100000.0
#define PWM_RES (72000000.0 * 2.0 / PWM_FREQ / 2.0)
#define RT_FREQ 5000.0
#define ADC_OVER (PWM_FREQ * 2 / RT_FREQ)
#define ADC_COUNT (ADC_OVER * 4)

//io board
#define USB_CONNECT_PIN GPIO_PIN_2
#define USB_CONNECT_PORT GPIOB

/*
//otter
//TODO: swap v,w cur feedback
#define PWM_INVERT
#define AREF 3.3// analog reference voltage

#define VDIVUP 56000.0//HV div pullup R1,R12
#define VDIVDOWN 2000.0//HV div pulldown R2,R9
#define SHUNT 0.003//shunt
#define SHUNT_PULLUP 5100.0
#define SHUNT_SERIE 100.0

#define LED_Pin GPIO_PIN_0
#define LED_GPIO_Port GPIOA

#define PWM_U TIM8->CCR1
#define PWM_V TIM8->CCR2
#define PWM_W TIM8->CCR3
 
//ottercontrol
#define USB_DISCONNECT_PIN GPIO_PIN_13
#define USB_DISCONNECT_PORT GPIOC

#define PWM_DEADTIME 50
*/
