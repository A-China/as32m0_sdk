#include "printf_config.h"
#include "as32m0_gpio.h"
#include "as32m0_sysctrl.h"
#include "as32m0_pinctrl.h"

void sendchar(UART_TypeDef* pBase, uint8_t ch)
{
  while(apUART_Check_TXFIFO_FULL(pBase) == 1); 
  UART_SendData(pBase, ch);
}

int SER_PutChar (int c) 
{
  sendchar(APB_UART0, (uint8_t)c);
  return (c);
}

int SER_GetChar (void) 
{
//	while(apUART_Check_RXFIFO_EMPRY(APB_UART0) == 1);
  return UART_ReceData(APB_UART0);
}

int SER_PutChar1 (int c) 
{
  sendchar(APB_UART1, (uint8_t)c);
  return (c);
}


void config_uart(uint32_t freq, uint32_t baud)
{
  UART_sStateStruct uart_base;
  PIN_INFO pin_info;

  SYSCTRL_ClrReset_APB_SCI0();
  SYSCTRL_ClrReset_APB_SCI1();
  SYSCTRL_ClrReset_APB_PinCtrl();

  pin_info.pin_func = GIO_FUNC1;
  pin_info.pin_stat = GIO_PU;
  PinCtrl_GIOSet(PIN_CTL_GPIOA, GPIO_Pin_4 | GPIO_Pin_5, &pin_info);
  PinCtrl_GIOSet(PIN_CTL_GPIOC, GPIO_Pin_0 | GPIO_Pin_1, &pin_info);

  uart_base.word_length       = UART_WLEN_8_BITS;
  uart_base.parity            = UART_PARITY_NOT_CHECK;
  uart_base.fifo_enable       = 1;
  uart_base.two_stop_bits     = 0;
  uart_base.receive_en        = 1;
  uart_base.transmit_en       = 1;
  uart_base.UART_en           = 1;
  uart_base.rxfifo_waterlevel = UART_FIFO_ONE_SECOND;
  uart_base.txfifo_watchlevel = UART_FIFO_ONE_SECOND;
  uart_base.ClockFrequency    = freq;
  uart_base.BaudRate          = baud;
  
  apUART_Initialize(APB_UART0, &uart_base);
  apUART_Initialize(APB_UART1, &uart_base);
}

void config_uart0(uint32_t freq, uint32_t baud, uint8_t loc)
{
  UART_sStateStruct uart_base;
  PIN_INFO pin_info;

  SYSCTRL_ClrReset_APB_SCI0();
  SYSCTRL_ClrReset_APB_PinCtrl();
	switch(loc){
		case UART0_PA45:
			pin_info.pin_func = GIO_FUNC1;
			pin_info.pin_stat = GIO_PU;
			PinCtrl_GIOSet(PIN_CTL_GPIOA, GPIO_Pin_4 | GPIO_Pin_5, &pin_info);		
			break;
		case UART0_PB67:
			pin_info.pin_func = GIO_FUNC1;
			pin_info.pin_stat = GIO_PU;
			PinCtrl_GIOSet(PIN_CTL_GPIOB, GPIO_Pin_6 | GPIO_Pin_7, &pin_info);			
		break;
		case UART0_PB01:
			pin_info.pin_func = GIO_FUNC2;
			pin_info.pin_stat = GIO_PU;
			PinCtrl_GIOSet(PIN_CTL_GPIOB, GPIO_Pin_0 | GPIO_Pin_1, &pin_info);			
		break;
		case UART0_PD23:
			pin_info.pin_func = GIO_FUNC2;
			pin_info.pin_stat = GIO_PU;
			PinCtrl_GIOSet(PIN_CTL_GPIOD, GPIO_Pin_2 | GPIO_Pin_3, &pin_info);		
			break;
		default:
			pin_info.pin_func = GIO_FUNC1;
			pin_info.pin_stat = GIO_PU;
			PinCtrl_GIOSet(PIN_CTL_GPIOA, GPIO_Pin_4 | GPIO_Pin_5, &pin_info);		
			break;			
	}

  uart_base.word_length       = UART_WLEN_8_BITS;
  uart_base.parity            = UART_PARITY_NOT_CHECK;
  uart_base.fifo_enable       = 1;
  uart_base.two_stop_bits     = 0;
  uart_base.receive_en        = 1;
  uart_base.transmit_en       = 1;
  uart_base.UART_en           = 1;
  //uart_base.rxfifo_waterlevel = UART_FIFO_ONE_SECOND;
  uart_base.rxfifo_waterlevel = UART_FIFO_ONE_EIGHTH;
  uart_base.txfifo_watchlevel = UART_FIFO_ONE_SECOND;
  uart_base.ClockFrequency    = freq;
  uart_base.BaudRate          = baud;

  apUART_Initialize(APB_UART0, &uart_base);
}

void config_uart1(uint32_t freq, uint32_t baud, uint8_t loc)
{
  UART_sStateStruct uart_base;
  PIN_INFO pin_info;

  SYSCTRL_ClrReset_APB_SCI1();
  SYSCTRL_ClrReset_APB_PinCtrl();
	switch(loc){
		case UART1_PA67:
			pin_info.pin_func = GIO_FUNC1;
			pin_info.pin_stat = GIO_PU;
			PinCtrl_GIOSet(PIN_CTL_GPIOA, GPIO_Pin_6 | GPIO_Pin_7, &pin_info);
			break;
		case UART1_PC01:
			pin_info.pin_func = GIO_FUNC1;
			pin_info.pin_stat = GIO_PU;	
			PinCtrl_GIOSet(PIN_CTL_GPIOC, GPIO_Pin_0 | GPIO_Pin_1, &pin_info);
			break;
		default:
			pin_info.pin_func = GIO_FUNC1;
			pin_info.pin_stat = GIO_PU;
			PinCtrl_GIOSet(PIN_CTL_GPIOA, GPIO_Pin_6 | GPIO_Pin_7, &pin_info);
			break;			
	}
	
  uart_base.word_length       = UART_WLEN_8_BITS;
  uart_base.parity            = UART_PARITY_NOT_CHECK;
  uart_base.fifo_enable       = 1;
  uart_base.two_stop_bits     = 0;
  uart_base.receive_en        = 1;
  uart_base.transmit_en       = 1;
  uart_base.UART_en           = 1;
  //uart_base.rxfifo_waterlevel = UART_FIFO_ONE_SECOND;
  uart_base.rxfifo_waterlevel = UART_FIFO_ONE_EIGHTH;
  uart_base.txfifo_watchlevel = UART_FIFO_ONE_SECOND;
  uart_base.ClockFrequency    = freq;
  uart_base.BaudRate          = baud;

  apUART_Initialize(APB_UART1, &uart_base);
}





