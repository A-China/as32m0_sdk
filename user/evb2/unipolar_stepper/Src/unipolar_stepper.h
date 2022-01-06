//******************************************************************************
//!
//! \file   max6675.h
//! \brief  MAX6675 Thermocouple driver
//! \author Henry
//! \date   2021-11-08
//! \email  
//!
//! \note   
//!
//! \license
//!
//! Copyright (c) 2013 Henry MIT License
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

#ifndef __unipolar_stepper_H__
#define __unipolar_stepper_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
  long step_number;    				// which step the motor is on
  int direction;      				// motor direction
  uint32_t last_step_time; 		// time stamp in us of the last step taken
  int number_of_steps; 				// total number of steps for this motor
	int pin_count;							// number of control signal (pin) to stepper
	uint32_t step_delay;
	
	// mcu007 pins for the motor control connection:
	uint32_t motor_port;
	uint32_t motor_pin_1;
	uint32_t motor_pin_2;
	uint32_t motor_pin_3;
	uint32_t motor_pin_4;
	
} STEPPER_def;


//! Init unipolar stepper module.
STEPPER_def * stepper_init(STEPPER_def *stepper, int number_of_steps, uint32_t port, uint32_t motor_pin_0, uint32_t motor_pin_1,
                                      uint32_t motor_pin_2, uint32_t motor_pin_3);

//! 
void setSpeed(STEPPER_def *stepper, long whatSpeed);
void step(STEPPER_def *stepper, long steps_to_move);
static void stepMotor(STEPPER_def *stepper, int thisStep);

#ifdef __cplusplus
}
#endif

#endif // __unipolar_stepper_H__
