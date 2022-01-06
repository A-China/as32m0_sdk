#include "test_gpio_out.h"
#include "printf_config.h"
#include "as32m0_misc.h"

void test_gpio_out()
{
	int8_t i;
	PIN_INFO pin_info;

	printf("\n");
		while(apUART_Check_BUSY(APB_UART0) == 1);
	printf("Set GPIO OUT \n");
	printf("\n");
    
	pin_info.pin_func = GIO_FUNC0;
	pin_info.pin_stat = GIO_PU;				// if not this line will become open drain
	pin_info.pin_ds = GIO_DS_2_4;			// output driver select
	pin_info.pin_od = OD_OFF;					// open drain select
	
    	
	PinCtrl_GIOSet(PIN_CTL_GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7, &pin_info);
	PinCtrl_GIOSet(PIN_CTL_GPIOB, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7, &pin_info);
	PinCtrl_GIOSet(PIN_CTL_GPIOC, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7, &pin_info);
	PinCtrl_GIOSet(PIN_CTL_GPIOD, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3,  &pin_info);
    
	APB_GPIO->GPIO_OE.SET = 0xFFFFFFFF; //Set GPIO Output
    
	printf("// \n");
	printf("// Set GPIO ALL High \n");
	printf("// \n");
		while(apUART_Check_BUSY(APB_UART0) == 1);
		
	//Set Output High
	APB_GPIO->GPIO_DO.SET = 0xFFFFFFFF; //Set GPIO High
    
	printf("// \n");
	printf("// Set GPIO ALL GPIO Low \n");
	printf("// \n");
		while(apUART_Check_BUSY(APB_UART0) == 1);
	// Set Output Zero
	APB_GPIO->GPIO_DO.CLR = 0xFFFFFFFF; //Set GPIO Zero
    
	printf("// \n");
	printf("// Set GPIO High One By One \n");
	printf("// \n");
		while(apUART_Check_BUSY(APB_UART0) == 1);
	
	for (i=0;i<28;i++)
	{
		APB_GPIO->GPIO_DO.SET = (1 << i);
	}
    
	printf("// \n");
	printf("// Set GPIO Low One By One \n");
	printf("// \n");
		while(apUART_Check_BUSY(APB_UART0) == 1);
	
	for (i=0;i<28;i++)
	{
		APB_GPIO->GPIO_DO.CLR = (1 << i);
	}   
	
	i=0;
	
	while(1)
	{
		for (i=0;i<8;i++)
		{
			APB_GPIO->GPIO_DO.MODIFY = (1 << i);
			printf(".");
			delay_ms(100);
			APB_GPIO->GPIO_DO.CLR = 0xFFFFFFFF; //Set GPIO Zero
		} 
		for (i=16;i<24;i++)
		{
			APB_GPIO->GPIO_DO.MODIFY = (1 << i);
			printf(".");
			delay_ms(100);
			APB_GPIO->GPIO_DO.CLR = 0xFFFFFFFF; //Set GPIO Zero
		} 
		for (i=15; i>7; i--)
		{
			APB_GPIO->GPIO_DO.MODIFY = (1 << i);
			printf(".");
			delay_ms(100);
			APB_GPIO->GPIO_DO.CLR = 0xFFFFFFFF; //Set GPIO Zero
		} 
		for (i=28;i>23;i--)
		{
			APB_GPIO->GPIO_DO.MODIFY = (1 << i);
			printf(".");
			delay_ms(100);
			APB_GPIO->GPIO_DO.CLR = 0xFFFFFFFF; //Set GPIO Zero
		} 
		/***************/
		for (i=24;i<29;i++)
		{
			APB_GPIO->GPIO_DO.MODIFY = (1 << i);
			printf(".");
			delay_ms(100);
			APB_GPIO->GPIO_DO.CLR = 0xFFFFFFFF; //Set GPIO Zero
		} 
		for (i=8;i<16;i++)
		{
			APB_GPIO->GPIO_DO.MODIFY = (1 << i);
			printf(".");
			delay_ms(100);
			APB_GPIO->GPIO_DO.CLR = 0xFFFFFFFF; //Set GPIO Zero
		} 
		for (i=23;i>15;i--)
		{
			APB_GPIO->GPIO_DO.MODIFY = (1 << i);
			printf(".");
			delay_ms(100);
			APB_GPIO->GPIO_DO.CLR = 0xFFFFFFFF; //Set GPIO Zero
		} 
		for (i=7;i>=0;i--)
		{
			APB_GPIO->GPIO_DO.MODIFY = (1 << i);
			printf(".");
			delay_ms(100);
			APB_GPIO->GPIO_DO.CLR = 0xFFFFFFFF; //Set GPIO Zero
		} 
		  printf("\n");
	}
}



