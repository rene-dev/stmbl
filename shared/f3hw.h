//stmbl
#define AREF 3.338// analog reference voltage

#define VDIVUP 249000.0 * 2.0//HV div pullup R1,R12
#define VDIVDOWN 3900.0//HV div pulldown R2,R9

#define LED_Pin GPIO_PIN_0
#define LED_GPIO_Port GPIOA

#define PWM_U TIM8->CCR3
#define PWM_V TIM8->CCR2
#define PWM_W TIM8->CCR1

#define PWM_DEADTIME 0
//io board
//#define USB_CONNECT_PIN GPIO_PIN_15
//#define USB_CONNECT_PORT GPIOB

/*
//otter
#define AREF 3.3// analog reference voltage

#define VDIVUP 56000.0//HV div pullup R1,R12
#define VDIVDOWN 2000.0//HV div pulldown R2,R9

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
