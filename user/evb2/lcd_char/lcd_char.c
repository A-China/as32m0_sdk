#include "stdlib.h"
#include "printf_config.h"
#include "as32m0_lcd_character_i2c.h"

void _sys_exit(void);
int main()
{
	char buff[16];
	uint8_t len;
	// prepare printf (uart0)
	config_uart0(OSC_CLK_FREQ, UART_SETBAUD, UART0_PD23);
	
	printf("==> \n");
	printf("==> \n");
	printf("==> TEST LCD char \n");
	printf("==> \n");
	printf("==> \n");
	// wait
	while(apUART_Check_BUSY(APB_UART0) == 1);
	
	//-------  System Config ----------
	I2C_init(LCD_char_ADDR, 1, I2C_PB45);
	LCD_char_init(LCD_2LINE, LCD_5x8DOTS);
	LCD_char_backlight_on();
	LCD_char_setCursor(0,0);
	len=sprintf(buff, "Hello World!");
	LCD_char_printstr(buff,len);
	LCD_char_setCursor(1,4);
	len=sprintf(buff, "Line 2");
	LCD_char_printstr(buff,len);
  printf("\n");
	printf("\n");
	printf("==> TEST LCD char END \n");
	printf("\n");
	printf("\n");
	printf("#"); // invoke TB $finish;
	_sys_exit();
 	return 0x12;
}

