/*----------------------------------------------------------------------------
 * Name:    Retarget.c
 * Purpose: 'Retarget' layer for target-dependent low level functions
 * Note(s):
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2011 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include "printf_config.h"


#if   defined ( __CC_ARM )
//#pragma import(__use_no_semihosting)

struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;
FILE __stdin;



int fputc(int c, FILE *f) {
	if(c=='\n')
		SER_PutChar('\r');
  return (SER_PutChar(c));
}


int fgetc(FILE *f) {
	uint8_t in;
	while(apUART_Check_RXFIFO_EMPTY(APB_UART0) == 1);
	in = SER_GetChar();
	SER_PutChar(in);
  return in;
}


int ferror(FILE *f) {
  /* Your implementation of ferror */
  return EOF;
}


void _ttywrch(int c) {
  SER_PutChar(c);
}


void _sys_exit(int return_code) {
label:  goto label;  /* endless loop */
}

#elif defined ( __GNUC__ )

int _write_r(void *reent, int fd, char *ptr, size_t len)
{

    size_t i;

    for (i=0; i<len; i++)

    {

       SER_PutChar(ptr[i]);

    }

    return len;

}
#endif

