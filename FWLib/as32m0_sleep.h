#ifndef __AS32M0_SLEEP_H__
#define __AS32M0_SLEEP_H__

typedef enum SLEEP_INSTRUCTION_e {
	WFE_MODE = 0,
	WFI_MODE = 1
} SLEEP_INSTRUCTION_t;

typedef enum SLEEP_MODE_e {
	DEEP_SLEEP_MODE = 0,
	NORMAL_SLEEP_MODE = 1
} SLEEP_MODE_t;


void enter_sleep_mode(SLEEP_MODE_t sleep_mode,SLEEP_INSTRUCTION_t instruction);


#endif
