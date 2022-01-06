#include "as32m0.h"
#include "printf_config.h"
#include "as32m0_timer.h"
#define TEST_ASIC_RTC	32000

#define TEST_WDG_TIMER_WRAPPING_MODE	1
#define TEST_WDG_TIMER_FREERUN_MODE		1
#define TEST_WDG_TIMER_ONESTOP_MODE		1
#define TEST_WDG_RESET_MODE				1

__IO uint32_t wdg_tmr_irq_times = 0;

__IO uint32_t wdg_tmr_irq_happened_times = 0;

uint8_t wdg_teset = 0;

void _sys_exit(void);
void test_wdg(void);

int main()
{
	// prepare printf
	config_uart0(OSC_CLK_FREQ, UART_SETBAUD, UART0_PD23);
	
	printf("==> \n");
	printf("==> \n");
	printf("==> TEST WDG RESET Begin \n");
	printf("==> \n");
	printf("==> \n");
	// wait
	while(apUART_Check_BUSY(APB_UART0) == 1);
	
	//-------  System Config ----------
	test_wdg();

	printf("\n");
	printf("\n");
	printf("==> TEST WDG RESET End \n");
	printf("\n");
	printf("\n");
	
	printf("#"); // invoke TB $finish;
	while(apUART_Check_BUSY(APB_UART0) == 1);
	//AHB_SYSCTRL->SYSCTRL_SysClkSwitch = 0x0; 
	
	_sys_exit();
	return 0x12;
}

void test_wdg()
{
	uint32_t i=0;

	if(WatchDog_Reset_Happened()){
		printf("***********************************\n");
		printf(">>> MCU start from WDG reset <<<\n");
		printf("***********************************\n");
	}

	printf("Make CPU Enable Irq \n");
	__enable_irq();

	printf("Clear Timer NVIC Pending Irq : n01_WDG_IRQn \n");
	NVIC_ClearPendingIRQ(n00_WDG_IRQn  );

	printf("Enable Timer NVIC Irq : n01_WDG_IRQn \n");
	NVIC_EnableIRQ(n00_WDG_IRQn );

	printf("Test watchdog function in watchdog timer\n");

#if TEST_WDG_TIMER_WRAPPING_MODE

	printf("// \n");
	printf("// Test WDG Timer Continuous Wrapping Mode \n");

	TMR_Clr_CNT(APB_WDG_TMR);
	TMR_Set_CMP(APB_WDG_TMR, TEST_ASIC_RTC*2); // NOTE : Make sure use osc clock
	TMR_Set_Op_Mode(APB_WDG_TMR, TMR_CTL_OP_MODE_WRAPPING);
	WDG_Reset_Disable();
	WDG_Int_Enable();
	WDG_Enable();
	wdg_tmr_irq_happened_times = 0;
	wdg_tmr_irq_times = 5;

	while(wdg_tmr_irq_happened_times != 5);  // wait
	printf("// \n");
	printf("// Test Continuous Wrapping Mode Pass \n");

#endif

#if TEST_WDG_TIMER_FREERUN_MODE

	printf("// \n");
	printf("// Test WDG Timer Continuous Free-run Mode \n");

	TMR_Clr_CNT(APB_WDG_TMR);
	TMR_Set_CMP(APB_WDG_TMR, TEST_ASIC_RTC*3); // NOTE : Make sure use osc clock
	TMR_Set_Op_Mode(APB_WDG_TMR, TMR_CTL_OP_MODE_FREERUN);
	WDG_Reset_Disable();
	WDG_Int_Enable();
	WDG_Enable();
	wdg_tmr_irq_happened_times = 0;
	wdg_tmr_irq_times = 1;

	while(wdg_tmr_irq_happened_times != 1);  // wait

	printf("// \n");
	printf("// Test Continuous Free-run Mode Pass \n");

#endif

#if TEST_WDG_TIMER_ONESTOP_MODE

	printf("// \n");
	printf("// Test WDG Timer One Shot Mode At Last \n");

	TMR_Clr_CNT(APB_WDG_TMR);
	TMR_Set_CMP(APB_WDG_TMR, TEST_ASIC_RTC*4); // NOTE : Make sure use osc clock
	TMR_Set_Op_Mode(APB_WDG_TMR, TMR_CTL_OP_MODE_ONESHOT);
	WDG_Reset_Disable();
	WDG_Int_Enable();
	WDG_Enable();
	wdg_tmr_irq_happened_times = 0;
	// Make sure bigger than 1 and IRQHandler donot Disable Interrupt to verify one shot mode.
	wdg_tmr_irq_times = 5;

	while(wdg_tmr_irq_happened_times == 0);  // wait, NOTE, there is equal zero.
	printf("// \n");
	printf("// Test One Shot Mode Pass \n");
	for(i=0; i<100; i++);  //delay to make sure timer is stoped

#endif

#if TEST_WDG_RESET_MODE

	printf("// \n");
	printf("// Test WDG Timer Reset in Continuous Wrapping Mode\n");
	printf("// \n");

	wdg_teset =1;
	TMR_Clr_CNT(APB_WDG_TMR);
	TMR_Set_CMP(APB_WDG_TMR, TEST_ASIC_RTC);
	TMR_Set_Op_Mode(APB_WDG_TMR, TMR_CTL_OP_MODE_WRAPPING);
	WDG_Reset_Enable();
	WDG_Int_Enable();
	WDG_Enable();
	wdg_tmr_irq_times = 1;

	printf("// Wait for reset \n");
	while(wdg_teset == 1);
#endif
}


void n00_WDG_IRQHandler()
{
	printf(">>> n00_WDG_IRQHandler Happend ! <<< \n");

	if(wdg_teset != 1){
		WatchDog_Int_Clr();
	}else{
		printf("WatchDog Int not clr in reset test\n");
	}

	wdg_tmr_irq_times --;
	wdg_tmr_irq_happened_times ++;

	if(wdg_tmr_irq_times == 0){
		WDG_Int_Disable();
	}
}

