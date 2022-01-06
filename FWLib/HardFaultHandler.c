/*********************************************************************
File    : HardFaultHandler.c
Purpose : Generic SEGGER HardFault handler
***************************  END-OF-HEADER  **************************
*/
#include <stdio.h>
void hardfault_handler_c(unsigned int * hardfault_args,unsigned int lr_value);
/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
static volatile unsigned int _Continue;  // Set this variable to 1 to run furthe
/*********************************************************************
*
*       HardFault_Handler()
*
*  Function description
*    Generic hardfault handler
*
*  Notes
*    (1) Ensure that HardFault_Handler is part of the exception table
*/

void hardfault_handler_c(unsigned int * hardfault_args,unsigned int lr_value)
{
	// Read saved registers from the stack, please make sure to check the SP_OFFSET.
	// SP_OFFSET may be modified depending on the compiler optimization level
	// SP_OFFSET is number of registers that are pushed on the stack on Hardfault entry.
	static volatile unsigned int r0;            // Register R0
	static volatile unsigned int r1;            // Register R1
	static volatile unsigned int r2;            // Register R2
	static volatile unsigned int r3;            // Register R3
	static volatile unsigned int r12;           // Register R12
	static volatile unsigned int lr;            // Link register
	static volatile unsigned int pc;            // Program counter
	static volatile unsigned int psr;			 // Program status word PSR
	
	printf("\n*********The saved registers from the stack****************\n");
	printf("r0 is 0x%x\n",  (unsigned long)hardfault_args[0]);
	printf("r1 is 0x%x\n",  (unsigned long)hardfault_args[1]);
	printf("r2 is 0x%x\n",  (unsigned long)hardfault_args[2]);
	printf("r3 is 0x%x\n",  (unsigned long)hardfault_args[3]);
	printf("r12 is 0x%x\n", (unsigned long)hardfault_args[4]);
	printf("lr is 0x%x\n",  (unsigned long)hardfault_args[5]);
	printf("pc is 0x%x\n",  (unsigned long)hardfault_args[6]);
	printf("psr is 0x%x\n", (unsigned long)hardfault_args[7]);
	printf("current_lr is 0x%x\n", lr_value);
	printf("*************************************************************\n");
  // Halt execution
  // When stuck here, change the variable value to != 0 in order to step out
  _Continue = 0u;
  while (_Continue == 0u);
}

__asm void HardFault_Handler(void)
{
		MOVS r0, #4
		MOV  r1, LR
		TST  r0, r1
		BEQ  stacking_used_MSP
		MRS  R0, PSP
		B    get_LR_and_branch
stacking_used_MSP
		MRS  R0, MSP
get_LR_and_branch
		MOV  R1, LR
		LDR  R2,=__cpp(hardfault_handler_c)
		BX   R2
}

/*************************** End of file ****************************/
