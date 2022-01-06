#include "as32m0_misc.h"
/*
	Function		 delay
	Description: Use loop to delay
*/
void delay(uint32_t num){
	while(num>0){num--;}
}
/*
	Function		 delay_s
	Description: delay an approximate value for the number of second(s)
*/
void delay_s(uint32_t num){
	delay((OSC_CLK_FREQ/6)*num);
}
/*
	Function		 delay_ms
	Description: delay an approximate value for the number of millisecond(s)
*/
void delay_ms(uint32_t num){
	delay((OSC_CLK_FREQ/6000)*num);
}
/*
	Function		 delay_us
	Description: delay an approximate value for the number of microsecond(s)
*/
void delay_us(uint32_t num){
	delay((OSC_CLK_FREQ/6000000)*num);
}
void breakpoint_key(void){
 printf("Press key \n");
 while(apUART_Check_RXFIFO_EMPTY(APB_UART0) == 1);
 UART_ReceData(APB_UART0);
}
