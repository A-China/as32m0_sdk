/******************************************************************************
 * @file     systick.h
 * @brief    Header File for systick Driver
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-10-30     			   first version
 ******************************************************************************/

#ifndef __AS32M0_SYSTICK_H__
#define __AS32M0_SYSTICK_H__

#ifdef	__cplusplus
extern "C" {	/* allow C++ to use these headers */
#endif	/* __cplusplus */

#include "as32m0.h"

typedef struct {
	__IO uint32_t SYST_CSR;                          /*< Offset: 0x00  SysTick Control and Status Register */
	__IO uint32_t SYST_RVR;                          /*< Offset: 0x04  SysTick Reload Value Register       */
	__IO uint32_t SYST_CVR;                          /*< Offset: 0x08  SysTick Current Value Register      */
	__I  uint32_t SYST_CALIB;                        /*< Offset: 0x0C  SysTick Calibration Register        */
} systick_type;

#define systick_base                       0xE000E010
#define systick                           ((systick_type *) systick_base )   /*< SysTick configuration struct */

#define SYST_CSR_BIT_ENABLE               (0)
#define SYST_CSR_BIT_TICKINT              (1)
#define SYST_CSR_BIT_CLKSOURCE            (2)
#define SYST_CSR_BIT_COUNTFLAG            (16)

#define SYST_CSR_ENABLE                   (1 <<SYST_CSR_BIT_ENABLE   )
#define SYST_CSR_TICKINT                  (1 <<SYST_CSR_BIT_TICKINT  )
#define SYST_CSR_CLKSOURCE                (1 <<SYST_CSR_BIT_CLKSOURCE)
#define SYST_CSR_COUNTFLAG                (1<<SYST_CSR_BIT_COUNTFLAG )


#define SYST_RVR_FULL                     (0XFFFFFF)
#define SYST_CVR_FULL                     (0XFFFFFF)

#define SYST_CALIB_BIT_TENMS              (0)  /*23:0*/
#define SYST_CALIB_BIT_SKEW               (30)
#define SYST_CALIB_BIT_NOREF              (31)

#define SYST_CALIB_TENMS                  (0xFFFFFF <<SYST_CALIB_BIT_TENMS)  /*23:0*/
#define SYST_CALIB_SKEW                   (1<<SYST_CALIB_BIT_SKEW         )
#define SYST_CALIB_NOREF                  (1<<SYST_CALIB_BIT_NOREF        )

int32_t systick_initialize_without_int(uint32_t sysclk);
int32_t systick_initialize(uint32_t sysclk);
int32_t systick_uninitialize(void);

/* run time(s) since run (seconds)*/
uint32_t systick_get_s(void);

/* run time(ms) since run (milliseconds)*/
uint32_t systick_get_ms(void);

/* run time(us) since run (microseconds)*/
uint64_t systick_get_us(void);

/* delay seconds */
uint32_t systick_delay_s(uint32_t seconds);

/* delay milliseconds */
uint32_t systick_delay_ms(uint32_t milliseconds);

/* delay microseconds */
uint32_t systick_delay_us(uint32_t microseconds);


#ifdef __cplusplus
} /* allow C++ to use these headers */
#endif	/* __cplusplus */

#endif
