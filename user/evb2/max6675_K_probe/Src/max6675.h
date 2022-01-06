//******************************************************************************
//!
//! \file   max6675.h
//! \brief  MAX6675 Thermocouple driver
//! \author Henry
//! \date   2021-11-08
//! \email  xuesong5825718@gmail.com
//!
//! \note   When i read this driver, i reference mbed max6675 driver, for
//!         more information, plesase refer to
//!         http://mbed.org/cookbook/MAX6675-Thermocouple
//!         If you have any question, please feel free to write to me ^_^.
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

#ifndef __MAX6676_H__
#define __MAX6676_H__

#ifdef __cplusplus
extern "C"
{
#endif

//! Init MAX6675 module.
extern void  MAX6675_Init(void);

//! MAX6675 temperature
extern float max6675_readCelsius(void);

#ifdef __cplusplus
}
#endif

#endif // __MAX6676_H__
