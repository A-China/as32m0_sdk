#include "printf_config.h"
#include "as32m0_pinctrl.h"
#include "as32m0_max7219.h"
#include "as32m0_misc.h"

extern void _sys_exit(void);
extern uint8_t char_buf[];

void init_SPI(uint8_t speed);

int main(void)
{
	uint8_t i;
	uint16_t in;
	int32_t loc;
	uint32_t loc1 = 0;
	uint32_t loc2 = 4;
	uint8_t *ptr;
	uint8_t db1[32];
	uint8_t db2[32];
	config_uart0(OSC_CLK_FREQ, UART_SETBAUD, UART0_PD23);
	printf("===>  MAX7219 test ================\n");
	printf("===>  	\n");
	printf("===>  	\n");
	printf("===>  	\n");

	init_SPI(6);
	MAX7219_init();
	delay_ms(500);
	
	MAX7219_clear_all();
	delay_s(2);
	loc =0;
	while(1){
			while(apUART_Check_RXFIFO_EMPTY(APB_UART0) == 1);
			in = SER_GetChar();	
			switch(in){
				case '9':
					while(1){
						i=MAX7219_print_string(loc, "A-CHINA SEMICONDUCTOR Ltd");
						delay_ms(500);	
						MAX7219_clear_all();
						if(loc == (SER_NUM-1) + (i-1))
							loc = 0;
						else
							loc = loc + 1;
						}
					break;
						
				case '1':
					i=MAX7219_print_string(loc, "~");
					printf("n = %d\n", i);
				
				break;
				
				case '2':
					loc ++;
					printf("2loc %d\n", loc);
				break;
				
				case '3':

					i=MAX7219_print_string(loc1, "~");
					i=MAX7219_print_string(loc2, "~");
					break;
				
				case '4':
						i=MAX7219_print_string(3, "1234");
					break;
				
				case '5':
						ptr = MAX7219_char_lookup(0x30);
						printf("pointer addr: %08X\n", ptr);
						for(i=0; i<8; i++){
							printf("x%02X ", *(ptr+i));
						}
						printf("\n");
					break;
						
				case '6':
					
					ptr = char_buf;
					printf("0x%08X \n", ptr);
					for(i=0; i<32; i++){
						  if(i%8 ==0) printf("\n");
							printf("x%02X ", *(ptr+i));
						}
						printf("\n");
					break;
				
				case 'c':
						MAX7219_clear_all();
					break;
				
				default:
					break;
					
				}
	}	
} 

void init_SPI(uint8_t speed){
	PIN_INFO PIN_INFO;
	PIN_INFO.pin_func = GIO_FUNC1;
	PIN_INFO.pin_stat = GIO_PU;
	PIN_INFO.pin_ds		= GIO_DS_2_4;
	PIN_INFO.pin_od   = OD_OFF;
	PIN_INFO.pin_sonof = SONOF_ON;
	APB_GPIO->GPIO_DO.SET = GPIOB_Pin_0;
	APB_GPIO->GPIO_OE.SET = GPIOB_Pin_0;
	PinCtrl_GIOSet(PIN_CTL_GPIOB, GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 , &PIN_INFO);
  ssp_config(speed);
}


