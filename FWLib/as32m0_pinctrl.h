
#ifndef __AS32M0_PINCTRL_H__
#define __AS32M0_PINCTRL_H__


#ifdef	__cplusplus
extern "C" {	/* allow C++ to use these headers */
#endif	/* __cplusplus */

#include "as32m0.h"
#include "as32m0_gpio.h"

#define bsPINCTRL_PULL_UP               0
#define bwPINCTRL_PULL_UP               1
#define bsPINCTRL_PULL_DOWN             1
#define bwPINCTRL_PULL_DOWN             1
#define bsPINCTRL_SCHMITT_TRIGGER       2
#define bwPINCTRL_SCHMITT_TRIGGER       1
#define bsPINCTRL_OPEN_DRAIN       			3
#define bwPINCTRL_OPEN_DRAIN            1
#define bsPINCTRL_DRIVER_STRENGTH       4
#define bwPINCTRL_DRIVER_STRENGTH       2

#define bsPINCTRL_FUNC_MUX              12
#define bwPINCTRL_FUNC_MUX              2
//---------------------------------------
//
//  PAD Address 
//
//---------------------------------------
#define PINCTRL_MAXNUM_PIN							28
#define PINCTRL_R_PA0 		 APB_PINC_BASE
#define PINCTRL_R_PA1 		(APB_PINC_BASE + 0x04)
#define PINCTRL_R_PA2 		(APB_PINC_BASE + 0x08)
#define PINCTRL_R_PA3 		(APB_PINC_BASE + 0x0c)
#define PINCTRL_R_PA4 		(APB_PINC_BASE + 0x10)
#define PINCTRL_R_PA5 		(APB_PINC_BASE + 0x14)
#define PINCTRL_R_PA6 		(APB_PINC_BASE + 0x18)
#define PINCTRL_R_PA7 		(APB_PINC_BASE + 0x1c)

#define PINCTRL_R_PB0 		(APB_PINC_BASE + 0x20)
#define PINCTRL_R_PB1 		(APB_PINC_BASE + 0x24)
#define PINCTRL_R_PB2 		(APB_PINC_BASE + 0x28)
#define PINCTRL_R_PB3 		(APB_PINC_BASE + 0x2c)
#define PINCTRL_R_PB4 		(APB_PINC_BASE + 0x30)
#define PINCTRL_R_PB5 		(APB_PINC_BASE + 0x34)
#define PINCTRL_R_PB6 		(APB_PINC_BASE + 0x38)
#define PINCTRL_R_PB7 		(APB_PINC_BASE + 0x3c)

#define PINCTRL_R_PC0 		(APB_PINC_BASE + 0x40)
#define PINCTRL_R_PC1 		(APB_PINC_BASE + 0x44)
#define PINCTRL_R_PC2 		(APB_PINC_BASE + 0x48)
#define PINCTRL_R_PC3 		(APB_PINC_BASE + 0x4c)
#define PINCTRL_R_PC4 		(APB_PINC_BASE + 0x50)
#define PINCTRL_R_PC5 		(APB_PINC_BASE + 0x54)
#define PINCTRL_R_PC6 		(APB_PINC_BASE + 0x58)
#define PINCTRL_R_PC7 		(APB_PINC_BASE + 0x5c)

#define PINCTRL_R_PD0 		(APB_PINC_BASE + 0x60)
#define PINCTRL_R_PD1 		(APB_PINC_BASE + 0x64)
#define PINCTRL_R_PD2 		(APB_PINC_BASE + 0x68)
#define PINCTRL_R_PD3 		(APB_PINC_BASE + 0x6c)

typedef enum {
  PIN_CTL_GPIOA = 0,
  PIN_CTL_GPIOB = 1,
  PIN_CTL_GPIOC = 2,
  PIN_CTL_GPIOD = 3,
}GPIO_PAD_CTRL;

typedef enum pin_func{
	GIO_FUNC0 = 0xc0,
	GIO_FUNC1 = 0xd0,
	GIO_FUNC2 = 0xe0,
	GIO_FUNC3 = 0xf0,
}PIN_FUNC;
/*------------------------------------
	GIO_PU			: PULL-UP								(Default)
	GIO_PD			: PULL-DOWN							
	GIO_SONOF 		: Schmitt Trigger
	GIO_OD			: Open drain Select
	GIO_DS_1_2 		: Drive Strength 1.2 ma	(Default)
	GIO_DS_2_4 		: Drive Strength 2.4 ma
	GIO_DS_3_6 		: Drive Strength 3.6 ma
	GIO_DS_6 		: Drive Strength 6 ma
---------------------------------------*/
typedef enum {
	GIO_PU 			= 0x41,
	GIO_PD 			= 0x42,
	GIO_POFF    = 0x40
}PIN_STAT;

typedef enum {
	SONOF_ON 		= 0x44,
	SONOF_OFF 	= 0x40,
	GIO_SONOF_ON 		= 0x44,
	GIO_SONOF_OFF 	= 0x40,	
}PIN_SONOF;

typedef enum {
	OD_ON 			= 0x48,				// open drain enabled
	OD_OFF 			= 0x40,				// open drain disabled
	GIO_OD_ON 	= 0x48,				// open drain enabled
	GIO_OD_OFF	= 0x40,				// open drain disabled	
}PIN_OD;

typedef enum {
	GIO_DS_1_2 	= 0x40,
	GIO_DS_2_4 	= 0x50,
	GIO_DS_3_6 	= 0x60,
	GIO_DS_6 	= 0x70,
}PIN_DS;

typedef struct {
	PIN_FUNC pin_func;
	PIN_DS	 pin_ds;
	PIN_OD   pin_od;
	PIN_SONOF pin_sonof;
	PIN_STAT pin_stat;
}PIN_INFO, *PIN_INFO_PTR;

/*-----------------------------------------------------------
	addr = PINCTRL_R_PXy, where X is A/B/C/D, y=1-7
	e.g PINCTRL_SetPadCtrl(PINCTRL_R_PA3, GIO_PU | GIO_DS_1_2 | SONOF_OFF);
------------------------------------------------------------*/
void PINCTRL_SetPadCtrl(uint32_t addr, uint8_t data);
void PINCTRL_SetFuncMux(uint32_t addr, uint8_t data);
uint32_t PINCTRL_GetPadConfig(uint32_t addr);

/*
	GPIO_Pin :	GPIO_Pin_x, where x is 1-7
	e.g PinCtrl_GIOSet(PIN_CTL_GPIOA, (GPIO_Pin_3 | GPIO_Pin_4), &pin_info);
*/

void PinCtrl_GIOSet(GPIO_PAD_CTRL GPIOx, uint32_t GPIO_Pin, PIN_INFO_PTR pin_info_ptr);

#ifdef __cplusplus
} /* allow C++ to use these headers */
#endif	/* __cplusplus */

#endif

