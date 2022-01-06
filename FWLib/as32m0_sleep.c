#include "as32m0.h"
#include "as32m0_sleep.h"


void enter_sleep_mode(SLEEP_MODE_t sleep_mode,SLEEP_INSTRUCTION_t instruction)
{
	/* Set SLEEPDEEP bit of Cortex System Comtrol Register */
	if(sleep_mode == NORMAL_SLEEP_MODE)
	{
		SCB->SCR &= (uint32_t)~((uint32_t)SCB_SCR_SLEEPDEEP_Msk);
	}
	else
	{
		SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
	}



	/* Select STOP mode entry */
	if(instruction == WFI_MODE)
	{
		/* Request Wait For Interrupt */
		__WFI();
	}
	else
	{
		/* Request Wait For Event */
		__WFE();
		__WFE();
	}

	/* Reset SLEEPDEEP bit of Cortex System Control Register */
	SCB->SCR &= (uint32_t)~((uint32_t)SCB_SCR_SLEEPDEEP_Msk);
}


