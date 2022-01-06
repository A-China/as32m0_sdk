

#ifndef __TEST_APB_SSP_FLASH_H__
#define __TEST_APB_SSP_FLASH_H__

#include "as32m0.h"
#include "as32m0_gpio.h"

// NOTE : this software demo is only for APB spi 

#define  APB_SSP_FLASH_CS_Init()   { PIN_INFO PIN_INFO;GPIO_InitTypeDef GPIO_MODE;         \
									 PIN_INFO.pin_func = GIO_FUNC0;                        \
					                 PIN_INFO.pin_stat = GIO_PU;                           \
					                 PinCtrl_GIOSet(PIN_CTL_GPIOB, GPIO_Pin_0, &PIN_INFO); \
                                     GPIO_MODE.GPIO_Mode=GPIO_Mode_Out;                    \
									 GPIO_MODE.GPIO_Pin=GPIO_Pin_0; GPIO_Init(GPIOB,&GPIO_MODE);}

#define Flash_CS(n)  (n? ( GPIO_SetBits(GPIOB,GPIO_Pin_0)) :(GPIO_ClrtBits(GPIOB,GPIO_Pin_0)))

#define  APB_FLASH_Enable     Flash_CS(0)
#define  APB_FLASH_Disable    Flash_CS(1)

void test_apb_ssp_flash(void);

void test_apb_ssp_with_dma(uint8_t master);

#endif

