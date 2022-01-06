//******************************************************************************
//!
//! \file   unipolar_stepper.c
//! \brief  unipolar stepper driver
//! \author achina
//! \date   2021-12-22
//! \email  achina@gmail.com
//!
//! \note   
//! \license
//!
//! Copyright (c) 2013 A-China License
//!
//! Permission is hereby granted, free of charge, to any person obtaining a copy
//! of stepper software and associated documentation files (the "Software"), to deal
//! in the Software without restriction, including without limitation the rights to
//! use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
//! the Software, and to permit persons to whom the Software is furnished to do so,
//! subject to the following conditions:
//!
//! The above copyright notice and stepper permission notice shall be included in all
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

#include "unipolar_stepper.h"
#include "as32m0_ssp.h"
#include "as32m0_pinctrl.h"
#include "as32m0_gpio.h"
#include "as32m0_misc.h"


#define HIGH	1
#define LOW		0


//============================================================================
//                          Low layer functins
//                                 Begin
//
//============================================================================
/*
 *   constructor for four-pin version
 *   Sets which wires should control the motor.
 */
STEPPER_def * stepper_init(STEPPER_def *stepper, int number_of_steps, uint32_t port, uint32_t motor_pin_0, uint32_t motor_pin_1,
                                      uint32_t motor_pin_2, uint32_t motor_pin_3)
{
	
  stepper->number_of_steps = number_of_steps;		// total number of steps per rov for this motor
  stepper->direction = 0;      					// motor direction
  stepper->last_step_time = 0; 					// time stamp in us of the last step taken
	stepper->step_number = 0;							// which step the motor is on

  // mcu007 pins for the motor control connection:
	stepper->motor_port = port;
  stepper->motor_pin_1 = motor_pin_0;
  stepper->motor_pin_2 = motor_pin_1;
  stepper->motor_pin_3 = motor_pin_2;
  stepper->motor_pin_4 = motor_pin_3;
	stepper->pin_count = 4;								// 4 control signals

//  // setup the pins on the microcontroller:
//	PIN_INFO pin_info;
//	pin_info.pin_func = GIO_FUNC0;
//	pin_info.pin_stat = GIO_PU;				// if not stepper line will become open drain
//	pin_info.pin_ds = GIO_DS_2_4;			// output driver select
//	pin_info.pin_od = OD_OFF;					// open drain select
//	PinCtrl_GIOSet(port, motor_pin_0 | motor_pin_1 | motor_pin_2 | motor_pin_3, &pin_info);
//	APB_GPIO->GPIO_OE.SET = motor_pin_0 << port;			// output
//	APB_GPIO->GPIO_OE.SET = motor_pin_1 << port;			// output
//	APB_GPIO->GPIO_OE.SET = motor_pin_2 << port;			// output
//	APB_GPIO->GPIO_OE.SET = motor_pin_3 << port;			// output	
//	GPIO_SetBits(port, motor_pin_0);
//	GPIO_SetBits(port, motor_pin_1);
//	GPIO_SetBits(port, motor_pin_2);
//	GPIO_SetBits(port, motor_pin_3);
	
//	systick_initialize(OSC_CLK_FREQ);	//the system clk
	
	printf("motor_port %d \n", stepper->motor_port);
	printf("motor_pinA %d \n", stepper->motor_pin_1);
	printf("motor_pinA %d \n", stepper->motor_pin_2);
	printf("motor_pinA %d \n", stepper->motor_pin_3);
	printf("motor_pinA %d \n", stepper->motor_pin_4);
	printf("step per rov %d \n", stepper->number_of_steps);
	printf("step numbers %d \n", stepper->step_number);
	printf("last step time %d \n", stepper->last_step_time);
	printf("step_delay %d \n", stepper->step_delay);
	
}

/*
 * Sets the speed in revs per minute
 * input value: 0~180
 */
void setSpeed(STEPPER_def *stepper, long whatSpeed)
{
	if(whatSpeed > 180) whatSpeed = 180;
  stepper->step_delay = 60L * 1000L * 1000L / stepper->number_of_steps / (whatSpeed * 0.1);
	printf("step_delay %d \n", stepper->step_delay);
}

/*
 * Moves the motor steps_to_move steps.  If the number is negative,
 * the motor moves in the reverse direction.
 */
void step(STEPPER_def *stepper, long steps_to_move)
{
  int steps_left = abs(steps_to_move);  // how many steps to take

  // determine direction based on whether steps_to_mode is + or -:
  if (steps_to_move > 0) { stepper->direction = 1; }
  if (steps_to_move < 0) { stepper->direction = 0; }

  // decrement the number of steps, moving one step each time:
  while (steps_left > 0)
  {
		// get current time since mcu reset in microsecond
    unsigned long now = systick_get_us();
    // move only if the appropriate delay has passed:
    if (now - stepper->last_step_time >= stepper->step_delay)
    {
      // get the timeStamp of when you stepped:
      stepper->last_step_time = now;
      // increment or decrement the step number,
      // depending on direction:
      if (stepper->direction == 1)
      {
        stepper->step_number++;
        if (stepper->step_number == stepper->number_of_steps) {
          stepper->step_number = 0;
        }
      }
      else
      {
        if (stepper->step_number == 0) {
          stepper->step_number = stepper->number_of_steps;
        }
        stepper->step_number--;
      }
      // decrement the steps left:
      steps_left--;
      // step the motor to step number 0, 1, ..., {3 or 10}
      if (stepper->pin_count == 5)
        stepMotor(stepper, stepper->step_number % 10);
      else
        stepMotor(stepper, stepper->step_number % 4);
    }
  }
	printf("step numbers %d \n", stepper->step_number);
}

void gpioWrite(GPIO_PAD port, uint32_t pin, uint8_t logic) {
	if(logic == 0) {
		APB_GPIO->GPIO_DO.CLR = pin << port;
	}
	else if(logic == 1) {
		APB_GPIO->GPIO_DO.SET = pin << port;
	}
}

/*
 * Moves the motor forward or backwards.
 */
static void stepMotor(STEPPER_def *stepper, int thisStep)
{
  if (stepper->pin_count == 2) {
    switch (thisStep) {
      case 0:  // 01
				gpioWrite(stepper->motor_port,stepper->motor_pin_1, LOW);
				gpioWrite(stepper->motor_port,stepper->motor_pin_2, HIGH);
      break;
      case 1:  // 11
				gpioWrite(stepper->motor_port,stepper->motor_pin_1, HIGH);
				gpioWrite(stepper->motor_port,stepper->motor_pin_2, HIGH);
      break;
      case 2:  // 10
				gpioWrite(stepper->motor_port,stepper->motor_pin_1, HIGH);
				gpioWrite(stepper->motor_port,stepper->motor_pin_2, LOW);
      break;
      case 3:  // 00
				gpioWrite(stepper->motor_port,stepper->motor_pin_1, LOW);
				gpioWrite(stepper->motor_port,stepper->motor_pin_2, LOW);
      break;
    }
  }
  if (stepper->pin_count == 4) {
    switch (thisStep) {
      case 0:  // 1010
				gpioWrite(stepper->motor_port,stepper->motor_pin_1, HIGH);
				gpioWrite(stepper->motor_port,stepper->motor_pin_2, LOW);
				gpioWrite(stepper->motor_port,stepper->motor_pin_3, HIGH);
				gpioWrite(stepper->motor_port,stepper->motor_pin_4, LOW);
      break;
      case 1:  // 0110
				gpioWrite(stepper->motor_port,stepper->motor_pin_1, LOW);
				gpioWrite(stepper->motor_port,stepper->motor_pin_2, HIGH);
				gpioWrite(stepper->motor_port,stepper->motor_pin_3, HIGH);
				gpioWrite(stepper->motor_port,stepper->motor_pin_4, LOW);
      break;
      case 2:  //0101
				gpioWrite(stepper->motor_port,stepper->motor_pin_1, LOW);
				gpioWrite(stepper->motor_port,stepper->motor_pin_2, HIGH);
				gpioWrite(stepper->motor_port,stepper->motor_pin_3, LOW);
				gpioWrite(stepper->motor_port,stepper->motor_pin_4, HIGH);
      break;
      case 3:  //1001
				gpioWrite(stepper->motor_port,stepper->motor_pin_1, HIGH);
				gpioWrite(stepper->motor_port,stepper->motor_pin_2, LOW);
				gpioWrite(stepper->motor_port,stepper->motor_pin_3, LOW);
				gpioWrite(stepper->motor_port,stepper->motor_pin_4, HIGH);
      break;
    }
  }

}
