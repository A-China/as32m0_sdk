
#ifndef __PRINTF_CONFIG_H__
#define __PRINTF_CONFIG_H__

#include "as32m0.h"
#include "as32m0_uart.h"
/*
 *	UART Config Location
 */
#define UART0_PA45                      0
#define UART0_PB67											1
#define UART0_PB01											2
#define UART0_PD23											3
#define UART1_PA67											0
#define UART1_PC01											1
//------------------------------
//
// printf function support (Use APB_UART0)
//
//------------------------------
int SER_PutChar (int c);
int SER_GetChar (void);
int SER_PutChar1 (int c); 

void sendchar(UART_TypeDef* UARTx, uint8_t ch);

void config_uart(uint32_t freq, uint32_t baud);

void config_uart0(uint32_t freq, uint32_t baud, uint8_t loc);

void config_uart1(uint32_t freq, uint32_t baud, uint8_t loc);

void uart1_send(uint8_t ch[], uint32_t num);

#endif
