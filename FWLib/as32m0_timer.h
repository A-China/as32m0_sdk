
#ifndef __AS32M0_TIMER_H__
#define __AS32M0_TIMER_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "as32m0.h"
#include <stdint.h>

// TMR_CTL
#define bsTMR_CTL_TMR_EN            0            // timer enable
#define bwTMR_CTL_TMR_EN                 1
#define bsTMR_CTL_RELOAD            1            // when set, timer counter clear zero.
#define bwTMR_CTL_RELOAD                 1
#define bsTMR_CTL_OP_MODE           2
#define bwTMR_CTL_OP_MODE                2

#define TMR_CTL_OP_MODE_WRAPPING    0            // 0 - continuous wrapping mode
#define TMR_CTL_OP_MODE_ONESHOT     1            // 1 - one-stop mode
#define TMR_CTL_OP_MODE_FREERUN     2            // 2 - continuous free-run mode

#define bsTMR_CTL_INT_EN            4            // int enable
#define bwTMR_CTL_INT_EN                 1
#define bsTMR_CTL_WatchDog_EN       5            // watchdog int enable, timer 0 only
#define bwTMR_CTL_WatchDog_EN            1
#define bsTMR_CTL_INT_CLR           6            // read as int status, write as int clear
#define bwTMR_CTL_INT_CLR                1
#define bsTMR_CTL_WatchDog_INT_CLR  7            // timer 0 only, read as watchdog int status, write as watchdog int clear
#define bwTMR_CTL_WatchDog_INT_CLR       1
#define bsTMR_CTL_TETCH_EN          8            // Fetch enable
#define bwTMR_CTL_TETCH_EN               1
#define bsTMR_CTL_TETCH_MODE        9            // Fetch Mode
#define bwTMR_CTL_TETCH_MODE             1


//#define bsTMR_CTL_HALTENA         15           // hardware do not support for all three timer 0, 1, 2.
//#define bwTMR_CTL_HALTENA              1


// TMR_LOCK                           
#define TMR_UNLOCK                 0xDEADFACE   // timer 0 only, write any value exclude 0xDEADFACE to LOCK bit to lock timer,
                                                //               write TMR_UNLOCK                   to LOCK bit to unlock timer.

typedef enum{
	FETCH_EDGE_MODE = 0,
	FETCH_LEVEL_MODE = 1,
}TIMER_FETCH_MODE;

//-----------
// TMR_CNT
//
void TMR_Set_CNT(TMR_TypeDef *pTMR, uint32_t value);
uint32_t TMR_Get_CNT(TMR_TypeDef *pTMR);

//-----------
// TMR_CMP
//
void TMR_Set_CMP(TMR_TypeDef *pTMR, uint32_t value);
uint32_t TMR_Get_CMP(TMR_TypeDef *pTMR);

//-----------
// TMR_CTL
//
void TMR_Set_CTL(TMR_TypeDef *pTMR, uint32_t value);
uint32_t TMR_Get_CTL(TMR_TypeDef *pTMR);
//
void TMR_Enable(TMR_TypeDef *pTMR);
void TMR_Disable(TMR_TypeDef *pTMR);
//
void TMR_Clr_CNT(TMR_TypeDef *pTMR);
//
void TMR_Set_Op_Mode(TMR_TypeDef *pTMR, uint8_t mode);
//
void TMR_Int_Enable(TMR_TypeDef *pTMR);
void TMR_Int_Disable(TMR_TypeDef *pTMR);

//
void TMR_Fetch_Enable(TMR_TypeDef *pTMR, TIMER_FETCH_MODE fetch_mode);
void TMR_Fetch_Disable(TMR_TypeDef *pTMR);

//
uint8_t TMR_Int_Happened(TMR_TypeDef *pTMR);
void TMR_Int_Clr(TMR_TypeDef *pTMR);


//-----------
// TMR_LOCK
//
void TMR_0_LOCK(void);
void TMR_0_UNLOCK(void);

//----WDG FUNCTIONS-----
void WDG_Enable(void);
void WDG_Disable(void);

//
void WDG_Int_Enable(void);
void WDG_Int_Disable(void);

void WDG_Reset_Enable(void);
void WDG_Reset_Disable(void);

uint8_t WatchDog_Reset_Happened(void);
void WatchDog_Int_Clr(void);


#ifdef __cplusplus
  }
#endif

#endif // 
