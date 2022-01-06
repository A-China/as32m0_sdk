#ifndef __AS32M0_MISC_H__
#define __AS32M0_MISC_H__

#ifdef	__cplusplus
extern "C" {	/* allow C++ to use these headers */
#endif	/* __cplusplus */

#include "as32m0.h"
#include "as32m0_uart.h"
#define BIT(n)  1<<n

void delay(uint32_t num);
void delay_s(uint32_t num);
void delay_ms(uint32_t num);
void delay_us(uint32_t num);
void breakpoint_key(void);

#endif
