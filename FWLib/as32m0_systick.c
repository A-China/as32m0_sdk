/******************************************************************************
 * @file     systick.c
 * @brief    Source File for systick Driver
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-10-30     		       first version
 ******************************************************************************/

#include "as32m0_systick.h"

static uint32_t fac_ms = 0;
static uint32_t fac_us = 0;
static uint32_t fac_s  = 0;
static volatile uint32_t systick_reload_num = 0;

int32_t systick_initialize(uint32_t sysclk)
{
    fac_s  = sysclk;
		fac_ms = fac_s/1000;
		fac_us = fac_ms/1000;
	
    systick_reload_num = 0;

    systick->SYST_RVR  = SYST_RVR_FULL;
    systick->SYST_CVR  = 0x0;                                                       /*reset SYST_CVR register and clean SYST_CSR_COUNTFLAG*/
    systick->SYST_CSR |= (SYST_CSR_ENABLE|SYST_CSR_TICKINT|SYST_CSR_CLKSOURCE); 		/*enable systick and interrupt ,processor clock*/

    return 0;
}
int32_t systick_uninitialize()
{
    systick->SYST_CVR  =   0x0;                               /*reset SYST_CVR register and clean SYST_CSR_COUNTFLAG*/
    systick->SYST_CSR &= ~(SYST_CSR_ENABLE|SYST_CSR_TICKINT); /*enable systick and interrupt */
    systick_reload_num = 0;

    return 0;
}

/* run time(s) since run (seconds)*/
uint32_t systick_get_s()
{
    uint32_t st_total_count = (SYST_CVR_FULL-systick->SYST_CVR)+(SYST_RVR_FULL*systick_reload_num);

    return st_total_count/fac_s;
}

/* run time(ms) since run (milliseconds)*/
uint32_t systick_get_ms()
{
    uint32_t st_total_count = (SYST_CVR_FULL-systick->SYST_CVR)+(SYST_RVR_FULL*systick_reload_num);

    return st_total_count/fac_ms;
}

/* run time(us) since run (microseconds)*/
uint64_t systick_get_us()
{
    uint32_t st_total_count = (SYST_CVR_FULL-systick->SYST_CVR)+(SYST_RVR_FULL*systick_reload_num);

    return st_total_count/fac_us;
}

/* delay seconds */
uint32_t systick_delay_s(uint32_t seconds)
{
    uint32_t st_current_val = systick->SYST_CVR;
    uint32_t st_delay_s_num = seconds*fac_s;
    if(st_delay_s_num<=st_current_val) {
        while(systick->SYST_CVR  > (st_current_val-st_delay_s_num));
    } else {
        uint64_t st_reload_num = systick_reload_num;
        uint32_t st_reload_count = ((st_delay_s_num-st_current_val)/SYST_RVR_FULL)+1;
        uint32_t st_reload_single =SYST_RVR_FULL- (st_delay_s_num-st_current_val)%SYST_RVR_FULL;
		
        while((uint32_t)(systick_reload_num-st_reload_num) < st_reload_count | systick->SYST_CVR > st_reload_single );
    }
	
    return 0;
}

/* delay milliseconds */
uint32_t systick_delay_ms(uint32_t milliseconds)
{

    uint32_t st_current_val = systick->SYST_CVR;
    uint32_t st_delay_ms_num = milliseconds*fac_ms;
    if(st_delay_ms_num <= st_current_val) {
        while(systick->SYST_CVR > (st_current_val-st_delay_ms_num));
    } else {
        uint64_t st_reload_num = systick_reload_num;
        uint32_t st_reload_count = ((st_delay_ms_num-st_current_val)/SYST_RVR_FULL)+1;
        uint32_t st_reload_single =SYST_RVR_FULL- (st_delay_ms_num-st_current_val)%SYST_RVR_FULL;
		
        while((uint32_t)(systick_reload_num-st_reload_num) < st_reload_count | systick->SYST_CVR > st_reload_single );

    }
	
    return 0;
}

/* delay microseconds  */
uint32_t systick_delay_us(uint32_t microseconds)
{

    uint32_t st_current_val = systick->SYST_CVR, st_delay_us_num = microseconds*fac_us;
    if(st_delay_us_num<=st_current_val) {
        while(systick->SYST_CVR > (st_current_val-st_delay_us_num));
    } else {
        uint32_t st_reload_single = (SYST_RVR_FULL-st_delay_us_num+ st_current_val);
        uint64_t st_reload_num = systick_reload_num;
		
        while((uint32_t)(systick_reload_num-st_reload_num) < 1 | systick->SYST_CVR > st_reload_single);

    }
	
    return 0;
}


void SysTick_Handler()
{
    systick_reload_num++;
}

