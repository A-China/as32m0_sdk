#include "as32m0_pwm.h"
#include "printf_config.h"

void _sys_exit(void);
uint32_t irq_count=0;
int main()
{
	PWM_INFO pwm_info;
	int32_t ret = -1;
	
	// prepare printf
	config_uart0(OSC_CLK_FREQ, UART_SETBAUD, UART0_PD23);

	printf("==> \n");
	printf("==> \n");
	printf("==> TEST PWM Begin \n");
	printf("==> \n");
	printf("==> \n");
	// wait
	while(apUART_Check_BUSY(APB_UART0) == 1);
	printf("// Make CPU Enable Irq \n");
	__enable_irq();

	printf("// Clear PWM0-3 NVIC Pending Irq \n");
	NVIC_ClearPendingIRQ(n04_PWM0_IRQn  );
	printf("// Enable PWM0-3 NVIC Irq \n");
	NVIC_EnableIRQ(n04_PWM0_IRQn );
	APB_PINC->PC2.FuncSel = 0xf0;	//PWM0
	APB_PINC->PC3.FuncSel = 0xf0;	//PWM1
	
	pwm_info.int_en = 1;
	pwm_info.div = 16;
	pwm_info.lmt = 5000;


	pwm_info.ch_0_info.ch_n_point = 3000;
	pwm_info.ch_0_info.ch_p_point = 1000;
	pwm_info.ch_0_info.oc_en      = 1;
	pwm_info.ch_0_info.oen_n      = 1;
	pwm_info.ch_0_info.oen_p      = 1;
	pwm_info.ch_0_info.out_n      = 0;
	pwm_info.ch_0_info.out_p      = 1;

	pwm_info.ch_1_info.ch_n_point = 2200;
	pwm_info.ch_1_info.ch_p_point = 1100;
	pwm_info.ch_1_info.oc_en      = 1;
	pwm_info.ch_1_info.oen_n      = 1;
	pwm_info.ch_1_info.oen_p      = 1;
	pwm_info.ch_1_info.out_n      = 0;
	pwm_info.ch_1_info.out_p      = 1;


	printf("Configing APB_PWM0 \n");
	ret = apPWM_Config(APB_PWM0, &pwm_info);
	if(ret < 0){
		printf("[%d] Cheak CMD Status busy \n",__LINE__);
	}	
	while(irq_count < 4);
	NVIC_DisableIRQ(n04_PWM0_IRQn );
	printf("==> \n");
	printf("==> \n");
	printf("==> TEST PWM on PC2 and PC3 \n");
	printf("==> \n");
	printf("==> \n");

	printf("#"); // invoke TB $finish;

	_sys_exit();
	return 0x12;
}

void n04_PWM0_IRQHandler()
{
	printf(">>> n04_PWM0_IRQHandler Happend %d times <<< \n", ++irq_count);
	apPWM_Int_Clr(APB_PWM0);
}

