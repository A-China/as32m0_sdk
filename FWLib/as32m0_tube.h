#ifndef __AS32M0_TUBE_H__
#define __AS32M0_TUBE_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "as32m0.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#define  END_EXEC()	 *TUBE_BASE = 0x04

void sendChar(char bData);
void sendString(char *pbString);
int vprint_var(const char * __restrict fmt, ...);
void vprint(char *fmt,...);

#ifdef __cplusplus
  }
#endif

#endif // 


