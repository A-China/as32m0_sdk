#include "stdlib.h"
#include <stdint.h>
#include "printf_config.h"
#include "as32m0_pwm.h"
#include "as32m0_gpio.h"
#include "as32m0_pinctrl.h"
#include "as32m0_misc.h"
#include "as32m0_timer.h"
#include "max6675.h"


void _sys_exit(void);
void init_timer(float second);

uint32_t irq_count;

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
	
	// prepare printf (uart0)
//	config_uart0(OSC_CLK_FREQ, UART_SETBAUD, UART0_PA45);
	config_uart0(OSC_CLK_FREQ, UART_SETBAUD, UART0_PD23);
	
	printf("==> \n");
	printf("==> \n");
	printf("==> TEST max6675 K-Thermocouple\n");
	printf("==> \n");
	printf("==> \n");
	// wait
	while(apUART_Check_BUSY(APB_UART0) == 1);
	
//	init_beep();
	GPIO_InitTypeDef gpio_init;
	gpio_init.GPIO_Mode = GPIO_Mode_Out;
	gpio_init.GPIO_Pin	= GPIO_Pin_0 | GPIOA_Pin_7;
	GPIO_Init(GPIOA, &gpio_init);
	
	
//	init_timer(0.5); //configure 1s to trigger 
	
	MAX6675_Init();
	kcount = 0;
	
	while(1) {
		k_value = max6675_readCelsius();
		printf("%d - K value=%.2f\n", kcount, k_value);
		kcount++;
		delay_ms(500);

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
