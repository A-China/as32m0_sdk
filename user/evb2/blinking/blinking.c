#include "as32m0.h"
#include "printf_config.h"
#include "as32m0_gpio.h"
#include "as32m0_misc.h"
#include "as32m0_timer.h"
#include "as32m0_systick.h"


extern void _sys_exit(void);

uint32_t irq_count = 0;
void init_timer(float second);

int main(void)
{
	systick_initialize(OSC_CLK_FREQ);
	GPIO_InitTypeDef gpio_init;
	gpio_init.GPIO_Mode = GPIO_Mode_Out;
	gpio_init.GPIO_Pin	= GPIO_Pin_0;
	GPIO_Init(GPIOA, &gpio_init);
	config_uart0(OSC_CLK_FREQ, UART_SETBAUD, UART0_PD23);
	printf("===>  Timer Example ================\n");
	printf("===>  	\n");
	printf("===>  	\n");
	printf("===>  	\n");
	init_timer(0.5); //configure 1s to trigger 
	while(1)
	{
			printf("===>  Timer Example ================\n");
			printf("===>  	\n");
			printf("===>  	\n");
		
	};
	
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
	if(irq_count%2)
		APB_GPIO->GPIO_DO.SET = GPIOA_Pin_0;
	else {
		APB_GPIO->GPIO_DO.CLR = GPIOA_Pin_0;
	}
}
