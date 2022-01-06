#include "stdlib.h"
#include "printf_config.h"
#include "test_gpio_out.h"
#include "as32m0_pwm.h"
#include "as32m0_misc.h"

void _sys_exit(void);



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

	pwm_info.ch_1_info.ch_n_point = 2000;			//@ SPWM ch1 only
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
	
	// prepare printf
	config_uart0(OSC_CLK_FREQ, UART_SETBAUD, UART0_PD23);
	
	printf("==> \n");
	printf("==> \n");
	printf("==> TEST GPIO OUT \n");
	printf("==> \n");
	printf("==>1 \n");
	
	//@ wait
	while(apUART_Check_BUSY(APB_UART0) == 1);
	
	//---------
	// Clear GPIO PinCtrl Clock Gate and Reset
	//

	while(1){
		test_gpio_out();
	
		
	}
	_sys_exit();
	 return 0x18;
}




//	void _sys_exit(void)
//	{
//		while(1);
//	}

void test_gpio()
{
	uint8_t i;
	PIN_INFO pin_info;

	printf("\n");
	printf("Set GPIO OUT \n");
	printf("\n");

	pin_info.pin_func = GIO_FUNC0;
	//pin_info.pin_stat = GIO_PU;
	// PinCtrl_GIOSet(PIN_CTL_GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7, &pin_info);
	PinCtrl_GIOSet(PIN_CTL_GPIOB, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7, &pin_info);
	PinCtrl_GIOSet(PIN_CTL_GPIOC, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7, &pin_info);
	PinCtrl_GIOSet(PIN_CTL_GPIOD, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3, &pin_info);

	APB_GPIO->GPIO_OE.SET = 0xFFFFFFFF; //Set GPIO Output

	printf("\n");
	printf("Set GPIO ALL High \n");
	printf("\n");

	//Set Output High
	APB_GPIO->GPIO_DO.SET = 0xFFFFFFFF; //Set GPIO High

	printf("\n");
	printf("Set GPIO ALL GPIO Low \n");
	printf("\n");

	// Set Output Zero
	APB_GPIO->GPIO_DO.CLR = 0xFFFFFFFF; //Set GPIO Zero


	printf("\n");
	printf("Set GPIO High One By One \n");
	printf("\n");

	for (i=0;i<28;i++)
	{
		APB_GPIO->GPIO_DO.SET = (1 << i);
	}

	printf("\n");
	printf("Set GPIO Low One By One \n");
	printf("\n");

	for (i=0;i<28;i++)
	{
		APB_GPIO->GPIO_DO.CLR = (1 << i);

	}

} 

