#include "stdlib.h"
#include <stdint.h>
#include "printf_config.h"
#include "as32m0_pwm.h"
#include "as32m0_gpio.h"
#include "as32m0_pinctrl.h"
#include "as32m0_misc.h"
#include "as32m0_timer.h"
#include "as32m0_systick.h"
#include "unipolar_stepper.h"

#define GPIO_PortB	8
#define STEPPER_PORT 	8
#define motor_pinA	GPIO_Pin_0
#define motor_pinB	GPIO_Pin_1
#define motor_pinC	GPIO_Pin_2
#define motor_pinD  GPIO_Pin_3
#define motor2_pinA	GPIO_Pin_4
#define motor2_pinB	GPIO_Pin_5
#define motor2_pinC	GPIO_Pin_6
#define motor2_pinD GPIO_Pin_7

void _sys_exit(void);
void init_timer(float second);
void stepperio_init(void);

uint32_t irq_count;
const int stepsPerRevolution = 2038;

/**
	* @brief  initial APB_PWM3 (PWM6) for beepper (4KHz, 78:22)
  * @param  void 
  */
uint32_t init_beep(void) {
	
	PWM_INFO pwm_info;
	int32_t ret;

	pwm_info.int_en = 0;				// pwm interrupt 0=disable, 1=enable
	pwm_info.div = 2;
	pwm_info.lmt = 3000;

	pwm_info.ch_1_info.ch_n_point = 2000;						//@ SPWM ch1 only
	pwm_info.ch_1_info.ch_p_point = 0;
	pwm_info.ch_1_info.oc_en      = 1;
	pwm_info.ch_1_info.oen_n      = 1;
	pwm_info.ch_1_info.oen_p      = 1;
	pwm_info.ch_1_info.out_n      = 1;
	pwm_info.ch_1_info.out_p      = 0;

	//printf("Configing APB_PWM3 beep \n");
	ret = apPWM_Config(APB_PWM3, &pwm_info);				// APB_PWM3 is pin PWM6
	if(ret < 0){
		printf("[%d] Check CMD Status busy \n",__LINE__);
	}	
	// APB_PINC->PB6.FuncSel = 0xC0; // SWITCH TO GPIO
	APB_PINC->PB6.FuncSel = 0xF0; // SWITCH TO GPIO
	return ret;
}

int main()
{
	float k_value;
	int16_t kcount;
	uint16_t i, j;
	STEPPER_def myStepper;
	STEPPER_def myStepper2;
	
	systick_initialize(OSC_CLK_FREQ);	//the system clk
	
	// prepare printf (uart0)
//	config_uart0(OSC_CLK_FREQ, UART_SETBAUD, UART0_PA45);
	config_uart0(OSC_CLK_FREQ, UART_SETBAUD, UART0_PD23);
	
	printf("==> \n");
	printf("==> \n");
	printf("==> TEST unipolar stepping motor 28BYJ-48\n");
	printf("==> \n");
	printf("==> \n");
	// wait
	while(apUART_Check_BUSY(APB_UART0) == 1);
	
//	init_beep();
	GPIO_InitTypeDef gpio_init;
	gpio_init.GPIO_Mode = GPIO_Mode_Out;
	gpio_init.GPIO_Pin	= GPIO_Pin_0 | GPIOA_Pin_7;
	GPIO_Init(GPIOA, &gpio_init);
	
// init steppers GPIO
	stepperio_init();
	
	init_timer(0.5); //configure 1s to trigger 
	
	stepper_init(&myStepper, stepsPerRevolution, STEPPER_PORT, motor_pinA, motor_pinC, motor_pinB, motor_pinD);
	stepper_init(&myStepper2, stepsPerRevolution, STEPPER_PORT, motor2_pinA, motor2_pinC, motor2_pinB, motor2_pinD);
	kcount = 0;
	
	while(1) {
		kcount++;
		   // Rotate CW 1/2 turn slowly
    setSpeed(&myStepper, 150);   
    step(&myStepper, 2038);
//		delay_ms(500);
//		setSpeed(&myStepper, 5);  
//		step(&myStepper, -200);
//		delay_ms(500);
//		setSpeed(&myStepper, 150);  
//		step(&myStepper, 20000);
//		delay_ms(500);
//		setSpeed(&myStepper, 100);  
//		step(&myStepper, -20000);
//		delay_ms(500);
		
		setSpeed(&myStepper2, 150);   
    step(&myStepper2, 2038);
		for(i=0; i<3000; i++){
			step(&myStepper, 1);
			step(&myStepper2, 1);
		}
		for(i=0; i<3000; i++){
			step(&myStepper, -1);
			step(&myStepper2, -1);
		}	
		for(j=0; j<50; j++){
			for(i=0; i<80; i++){
				step(&myStepper, 1);
				step(&myStepper2, 1);
			}
			for(i=0; i<80; i++){
				step(&myStepper, -1);
				step(&myStepper2, -1);
			}				
		}
		for(j=0; j<50; j++){
			for(i=0; i<200; i++){
				step(&myStepper, 1);
				step(&myStepper2, -1);
			}
			for(i=0; i<200; i++){
				step(&myStepper, -1);
				step(&myStepper2, 1);
			}				
		}		
	}
	
	
	//-------  System Config ----------
	
	
//  printf("\n");
//	printf("\n");
//	printf("==> TEST W8212 RGB PASSED \n");
//	printf("\n");
//	printf("\n");
//	printf("#"); // invoke TB $finish;
//	_sys_exit();
// 	return 0x12;
}


void stepperio_init(void) {
	
  // setup the pins on the microcontroller:
	PIN_INFO pin_info;
	pin_info.pin_func = GIO_FUNC0;
	pin_info.pin_stat = GIO_PU;				// if not stepper line will become open drain
	pin_info.pin_ds = GIO_DS_2_4;			// output driver select
	pin_info.pin_od = OD_OFF;					// open drain select
	PinCtrl_GIOSet(STEPPER_PORT, motor_pinA | motor_pinB | motor_pinC | motor_pinD | motor2_pinA | motor2_pinB | motor2_pinC | motor2_pinD, &pin_info);
	APB_GPIO->GPIO_OE.SET = motor_pinA << STEPPER_PORT;			// output
	APB_GPIO->GPIO_OE.SET = motor_pinB << STEPPER_PORT;			// output
	APB_GPIO->GPIO_OE.SET = motor_pinC << STEPPER_PORT;			// output
	APB_GPIO->GPIO_OE.SET = motor_pinD << STEPPER_PORT;			// output	
	APB_GPIO->GPIO_OE.SET = motor2_pinA << STEPPER_PORT;			// output
	APB_GPIO->GPIO_OE.SET = motor2_pinB << STEPPER_PORT;			// output
	APB_GPIO->GPIO_OE.SET = motor2_pinC << STEPPER_PORT;			// output
	APB_GPIO->GPIO_OE.SET = motor2_pinD << STEPPER_PORT;			// output	
	GPIO_SetBits(STEPPER_PORT, motor_pinA);
	GPIO_SetBits(STEPPER_PORT, motor_pinB);
	GPIO_SetBits(STEPPER_PORT, motor_pinC);
	GPIO_SetBits(STEPPER_PORT, motor_pinD);	
	GPIO_SetBits(STEPPER_PORT, motor2_pinA);
	GPIO_SetBits(STEPPER_PORT, motor2_pinB);
	GPIO_SetBits(STEPPER_PORT, motor2_pinC);
	GPIO_SetBits(STEPPER_PORT, motor2_pinD);	
	
}

void init_timer(float second){
	NVIC_ClearPendingIRQ(n10_TMR0_IRQn  );
	NVIC_EnableIRQ(n10_TMR0_IRQn );
	__enable_irq();
	TMR_Clr_CNT(APB_TMR0);
	TMR_Set_CMP(APB_TMR0, OSC_CLK_FREQ*second); 		// NOTE : Make sure use osc clock
	TMR_Set_Op_Mode(APB_TMR0, TMR_CTL_OP_MODE_WRAPPING);
	TMR_Int_Enable(APB_TMR0);
	TMR_Enable(APB_TMR0);
}

void n10_TMR0_IRQHandler()
{
	TMR_Int_Clr(APB_TMR0);
	irq_count++;
	if(irq_count%2) {
		APB_GPIO->GPIO_DO.SET = GPIOA_Pin_0;
	}
	else {
		APB_GPIO->GPIO_DO.CLR = GPIOA_Pin_0;
	}
}
