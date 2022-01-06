#include "printf_config.h"
#include "as32m0_uart.h"
#include "as32m0_sleep.h"
#include "as32m0_pinctrl.h"
#include "as32m0_sysctrl.h"
#include "as32m0_timer.h"
#include "as32m0_misc.h"
void _sys_exit(void);
void enter_sleep_timer(void);
#define SLOW_CLK 30000
int main()
{
	uint32_t i = 0;
	PIN_INFO pin_info;
	pin_info.pin_func = GIO_FUNC0;
	pin_info.pin_stat = GIO_PU;
	pin_info.pin_od   = OD_OFF;
	PinCtrl_GIOSet(PIN_CTL_GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_6 | GPIO_Pin_7, &pin_info);
	PinCtrl_GIOSet(PIN_CTL_GPIOB, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7, &pin_info);
	PinCtrl_GIOSet(PIN_CTL_GPIOC, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7, &pin_info);
	PinCtrl_GIOSet(PIN_CTL_GPIOD, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7, &pin_info);
	// prepare printf (uart0)
	config_uart0(OSC_CLK_FREQ, UART_SETBAUD, UART0_PD23);
	//-------------------------------------------------
	__enable_irq();
	NVIC_ClearPendingIRQ(n10_TMR0_IRQn  );
	NVIC_EnableIRQ(n10_TMR0_IRQn );
	TMR_Set_CMP(APB_TMR0, SLOW_CLK*5); // NOTE : Make sure use osc clock
	TMR_Set_Op_Mode(APB_TMR0, TMR_CTL_OP_MODE_WRAPPING);	
	printf("==> \n");
	printf("==> \n");
	printf("==> TEST SLEEP (__WFI();) \n");
	printf("==> \n");
	printf("==> \n");
	// wait
	while(apUART_Check_BUSY(APB_UART0) == 1);
	//-------  System Config ----------
//	test_normal_sleep(WFI_MODE);
//	//SYSCTRL_OSCClkSel_SlowClk();
//	SYSCTRL_OSC_ClkDIv_Set(1);
	//SYSCTRL_SysClkSel_OSCDiv();	
//	delay(1000);	
	while(1){
		printf("\n message times %d", i++);
		while(apUART_Check_BUSY(APB_UART0) == 1);
		enter_sleep_timer();
	};
  printf("\n");
	printf("\n");
	printf("==> TEST SLEEP PASSED \n");
	printf("\n");
	printf("\n");
	printf("#"); // invoke TB $finish;
	_sys_exit();
 	return 0x12;
}

void enter_sleep_timer(void){

	printf("==> Timer Wake Up test \n");
	while(apUART_Check_BUSY(APB_UART0) == 1);

	//Set Clock_Gate
	SYSCTRL_SetAhbClkGate(0xFFFFFFFF);
	SYSCTRL_SetApbClkGate(0xFFFFF7FF);
	SYSCTRL_SetWorkClkGate(0xFFFFFFFF);
	
	//SYSCTRL_SetReset_AHB(0xFFFFFFFF);
	//SYSCTRL_SetReset_APB(0xFFFF7FFF);
	//SYSCTRL_SetReset_WorkClk(0xFFFFFFFF);		
	SYSCTRL_OSCClkSel_SlowClk();
	//delay(1000);
	//SYSCTRL_OSC_ClkDIv_Set(8);
	//SYSCTRL_SysClkSel_OSCDiv();
	//delay(1000);
	TMR_Clr_CNT(APB_TMR0);
	TMR_Int_Enable(APB_TMR0);
	TMR_Enable(APB_TMR0);	
	SYSCTRL_ROSC_16M_PowerDown();
	enter_sleep_mode(DEEP_SLEEP_MODE,WFI_MODE);
}


void n10_TMR0_IRQHandler()
{
	TMR_Int_Disable(APB_TMR0);
	TMR_Disable(APB_TMR0);		
	//Clear Clock_Gate
	SYSCTRL_ROSC_16M_Clr_PowerDown();
	SYSCTRL_ClrAhbClkGate(0xFFFFFFFF);
	SYSCTRL_ClrApbClkGate(0xFFFFFFFF);
	SYSCTRL_ClrWorkClkGate(0xFFFFFFFF);
	//Clear Reset
	//SYSCTRL_ClrReset_AHB(0xFFFFFFFF);
	//SYSCTRL_ClrReset_APB(0xFFFFFFFF);
	//SYSCTRL_ClrReset_WorkClk(0xFFFFFFFF);	
	//SYSCTRL_SysClkSel_OSC();
	SYSCTRL_OSCClkSel_FastClk();
	//delay(50000);
	printf("n10_TMR0_IRQHandler Happened!\n");
	while(apUART_Check_BUSY(APB_UART0) == 1);	
	TMR_Int_Clr(APB_TMR0);
}







