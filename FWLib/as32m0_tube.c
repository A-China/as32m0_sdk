#include "as32m0_tube.h"

void sendChar(char bData)
{
  *TUBE_BASE = bData;	
}

void sendString(char *pbString)
{
	while(*pbString)sendChar(*pbString++);
}

int vprint_var(const char * __restrict pcFmt, ...)
{
	va_list ap;
	char pbString[256];

	va_start(ap,pcFmt);
	vsprintf(pbString,pcFmt,ap);
	sendString(pbString);
	va_end(ap);
        return 0;
}

void vprint(char *pcFmt,...)
{
	while(*pcFmt != '\0')
	{
		*TUBE_BASE = *pcFmt++;
	}
}


