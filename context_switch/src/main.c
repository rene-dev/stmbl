#include <stm32f4_discovery.h>
#include <stm32f4xx_conf.h>
#include "setup.h"
#include <math.h>

int __errno;

void SysTick_Handler(void);

volatile uint32_t *stack[3];
volatile uint32_t id = 0;

volatile uint32_t stack_space0[1024];
volatile uint32_t stack_space1[1024];
volatile uint32_t stack_space2[1024];

volatile uint32_t *tstack;
volatile uint32_t exc_return = 0xFFFFFFFD;
volatile uint32_t basepri = 0;

/* Stack
 *		->							FPSCR
 *		->							S15..S0
 *		->	xPSR					xPSR
 *		->	PC						PC
 *		->	LR(R14)					LR(R14)
 *		->	R12						R12
 *		->	R3..R0					R3..R0		Block stacked by an exception
 *
 *		->	R11..R4					R11..R4
 */
 
volatile uint32_t * init(volatile uint32_t *stackstart, void (*pc)(void)){
	volatile uint32_t *stackFrame = stackstart;
	*--stackFrame = 0x01000000;										// xPSR
	*--stackFrame = (uint32_t)pc;									// PC
	*--stackFrame = 0xFFFFFFF9;										// LR
	*--stackFrame = 0x12121212;										// r12
	*--stackFrame = 0x03030303;										// r3
	*--stackFrame = 0x02020202;										// r2
	*--stackFrame = 0x01010101;										// r1
	*--stackFrame = 0x00000000;										// r0
	*--stackFrame = 0x11111111;										// r11
	*--stackFrame = 0x10101010;										// r10
	*--stackFrame = 0x09090909;										// r9
	*--stackFrame = 0x08080808;										// r8
	*--stackFrame = 0x07070707;										// r7
	*--stackFrame = 0x06060606;										// r6
	*--stackFrame = 0x05050505;										// r5
	*--stackFrame = 0x04040404;										// r4
	return(stackFrame);
}

void thread0(){
	while(1){
		GPIO_SetBits(GPIOD,GPIO_Pin_12);
	}
}
	
void thread1(){
	while(1){
		GPIO_ResetBits(GPIOD,GPIO_Pin_12);
	}
}

int main(void)
{
	setup();
	GPIO_SetBits(GPIOD,GPIO_Pin_13);//enable

	stack[0] = init(stack_space0 + 1020, thread0);
	stack[1] = init(stack_space1 + 1020, thread1);
	stack[2] = init(stack_space2 + 1020, thread1);
	
	id = 2;
	tstack = stack[id];
	
	__asm__ volatile ( // undo save
		"ldr		r0,%0				\n"		// r0 = tstack
		"msr		psp,r0				\n"		// psp = r0
		:
		: "m" (tstack)
		:
	);
	
	while(1)  // Do not exit
	{
		
	}
}

/* Stack
 *		->							FPSCR
// *		->							S15..S0
 *		->	xPSR					xPSR
 *		->	PC						PC
 *		->	LR(R14)					LR(R14)
 *		->	R12						R12
 *		->	R3..R0					R3..R0		Block stacked by an exception
 *
 *		->	R11..R4					R11..R4
 */
 
 
void __attribute__((naked)) SysTick_Handler(void)
{
	__asm__ volatile (
		  	// done by cpu
		  	//"push		{xPSR}				\n"		// push xPSR
		  	//"push		{PC}				\n"		// push PC
		  	//"push		{LR}				\n"		// push LR
		  	//"push		{R12}				\n"		// push R12
		  	//"push		{R3}				\n"		// push R3
		  	//"push		{R2}				\n"		// push R2
		  	//"push		{R1}				\n"		// push R1
		  	//"push		{R0}				\n"		// push R0
		  	
			"cpsid		i					\n"		// disable int
		  	"mrs		r0,psp				\n"		// r0 = psp
			"stmdb 		r0!,{r4-r11}		\n"		// push r11-r4	
			"str		r0,%0				\n"		// tstack = r0
			:
			: "m" (tstack)
			:
	);

	if(id == 2){ // initial load
		__asm__ volatile ( // undo save
			"ldr		r0,%0				\n"		// r0 = tstack
			"ldmia 		r0!,{r4-r11}		\n"		//pop r4 - r11
			"msr		psp,r0				\n"		// psp = r0
			"str		r0,%0				\n"		// tstack = r0
			:
			: "m" (tstack)
			:
		);
		id = 0;
		tstack = stack[id];
	}

	stack[id] = tstack; // change stack
	id = 1 - id;
	tstack = stack[id];
	
	__asm__ volatile (
		//"ldr		r0,%0				\n"		// r0 = basepri
		"msr		basepri,%0			\n"		// 
		:
		: "r" (basepri)
		:
	);
	
	__asm__ volatile (
		"mov		lr,%0				\n"		// lr = exc_return
		:
		: "r" (exc_return)
		:
	);
	
	__asm__ volatile ( // load new thread
		"ldr		r0,%0				\n"		// r0 = tstack
		"ldmia 		r0!,{r4-r11}		\n"		//pop r4 - r11
		"msr		psp,r0				\n"		// psp = r0
		// done by cpu
//		"pop		{R0}				\n"		// pop R0
//		"pop		{R1}				\n"		// pop R1
//		"pop		{R2}				\n"		// pop R2
//		"pop		{R3}				\n"		// pop R3
//		"pop		{R12}				\n"		// pop R12
//		"pop		{LR}				\n"		// pop LR
//		"pop		{PC}				\n"		// pop PC
//		"pop		{xPSR}				\n"		// pop xPSR
		:
		: "m" (tstack)
		:
	);
	
	__asm__ volatile (
		"cpsie		i					\n"		// enable int
		"bx			lr					\n"		// return
		"nop							\n"		//
	);
}
