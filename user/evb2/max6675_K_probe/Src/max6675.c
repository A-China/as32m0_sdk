//******************************************************************************
//!
//! \file   max6675.c
//! \brief  MAX6675 Thermocouple driver
//! \author cedar
//! \date   2013-11-26
//! \email  xuesong5825718@gmail.com
//!
//! \note   When i read this driver, i reference mbed max6675 driver, for
//!         more information, plesase refer to
//!         http://mbed.org/cookbook/MAX6675-Thermocouple
//!         If you have any question, please feel free to write to me ^_^.
//! \license
//!
//! Copyright (c) 2013 Cedar MIT License
//!
//! Permission is hereby granted, free of charge, to any person obtaining a copy
//! of this software and associated documentation files (the "Software"), to deal
//! in the Software without restriction, including without limitation the rights to
//! use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
//! the Software, and to permit persons to whom the Software is furnished to do so,
//! subject to the following conditions:
//!
//! The above copyright notice and this permission notice shall be included in all
//! copies or substantial portions of the Software.
//!
//! THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//! IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//! FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//! AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//! LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//! OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
//! IN THE SOFTWARE.
///
//******************************************************************************

#include "max6675.h"
#include "as32m0_ssp.h"
#include "as32m0_pinctrl.h"
#include "as32m0_gpio.h"
#include "as32m0_misc.h"

#define GPIO_PortB	8
#define MAX6675_PORT 	8
#define MAX6675_CS		GPIO_Pin_0
#define MAX6675_CLK		GPIO_Pin_1
#define MAX6675_MISO	GPIO_Pin_2
#define MAX6675_MI_PIN GPIOB_Pin_2
//============================================================================
//                          Low layer functins
//                                 Begin
//
//============================================================================
// SPI by software GPIOs
void __maxspi_init(void){
	PIN_INFO pin_info;
	pin_info.pin_func = GIO_FUNC0;
	pin_info.pin_stat = GIO_PU;				// if not this line will become open drain
	pin_info.pin_ds = GIO_DS_2_4;			// output driver select
	pin_info.pin_od = OD_OFF;					// open drain select
	PinCtrl_GIOSet(PIN_CTL_GPIOB, MAX6675_CS | MAX6675_CLK | MAX6675_MISO, &pin_info);
	APB_GPIO->GPIO_OE.SET = MAX6675_CS  << MAX6675_PORT;			// output
	APB_GPIO->GPIO_OE.SET = MAX6675_CLK  << MAX6675_PORT;			// output
	APB_GPIO->GPIO_OE.CLR = GPIOB_Pin_2;		// input
	GPIO_SetBits(MAX6675_PORT, MAX6675_CS);
	GPIO_ClrtBits(MAX6675_PORT, MAX6675_CLK);
}	
	
//! Init MAX6675 module.
void  MAX6675_Init(void)
{
    __maxspi_init();
}

static void max_cs_set( int PinValue )
{
		if(PinValue == 0)
				APB_GPIO->GPIO_DO.CLR = MAX6675_CS << MAX6675_PORT;		// output 0 to pin	
		else if(PinValue == 1)
				APB_GPIO->GPIO_DO.SET = MAX6675_CS << MAX6675_PORT;		// output 1 to pin	
}

static void max_clk_set( int PinValue )
{
		if(PinValue == 0)
				APB_GPIO->GPIO_DO.CLR = MAX6675_CLK << MAX6675_PORT;		// output 0 to pin	
		else if(PinValue == 1)
				APB_GPIO->GPIO_DO.SET = MAX6675_CLK << MAX6675_PORT;		// output 1 to pin	
}

static int read_miso(uint8_t port, uint32_t pin){
	
	int temp;
	uint32_t pins = 0;
	pins = (pin << port);
	if ((APB_GPIO->GPIO_DI & pins) == pins){
		temp = 0x01;
	}
	else {
		temp = 0x00;
	}
	return temp;
}


static int max_read( void )
{
  int i;
  uint8_t d = 0;

  for (i = 7; i >= 0; i--) {

    max_clk_set(0);
    delay_us(10);
//    if (GPIO_ReadInputBit(MAX6675_MI_PIN) == 1) {
		if (read_miso(MAX6675_PORT, MAX6675_MISO)){
      d |= (1 << i);
    }
		max_clk_set(1);
    delay_us(10);    
  }

  return d;
}


static void wait(volatile unsigned long tick)
{
    while (tick--);
}




//! MAX6675 temperature
float max6675_readCelsius(void) {

  uint16_t v1, v2, v3, v4;

  max_cs_set(0);
  delay_us(10);

	v1 = max_read();
  v2 = v1 << 8;
	v3 = max_read();
  v4 = v2 | v3;

//	printf("read, V1=x%X, V2=x%X, V3=x%X, V4=x%X\n", v1, v2, v3, v4);
  max_cs_set(1);

  if (v4 & 0x4) {
    // uh oh, no thermocouple attached!
    return -1;
    // return NAN;
  }

  v4 >>= 3;

  return v4 * 0.25;
}


