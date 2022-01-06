
#include "as32m0_timer.h"

//-----------
// TMR_CNT
//
void TMR_Set_CNT(TMR_TypeDef *pTMR, uint32_t value)
{
	pTMR->CNT = value;
}

uint32_t TMR_Get_CNT(TMR_TypeDef *pTMR)
{
	return pTMR->CNT;
}

//-----------
// TMR_CMP
//
void TMR_Set_CMP(TMR_TypeDef *pTMR, uint32_t value)
{
	pTMR->CMP = value;
}

uint32_t TMR_Get_CMP(TMR_TypeDef *pTMR)
{
	return pTMR->CMP;
}

//-----------
// TMR_CTL
//
void TMR_Set_CTL(TMR_TypeDef *pTMR, uint32_t value)
{
	pTMR->CTL = value;
}

uint32_t TMR_Get_CTL(TMR_TypeDef *pTMR)
{
	return pTMR->CTL;
}


//
void TMR_Enable(TMR_TypeDef *pTMR)
{
	pTMR->CTL |= 1 << bsTMR_CTL_TMR_EN;
}

void TMR_Disable(TMR_TypeDef *pTMR)
{
	pTMR->CTL &= ~(1 << bsTMR_CTL_TMR_EN);
}

//
void TMR_Clr_CNT(TMR_TypeDef *pTMR)
{
	pTMR->CTL |= 1 << bsTMR_CTL_RELOAD;
}

//
void TMR_Set_Op_Mode(TMR_TypeDef *pTMR, uint8_t mode)
{
	uint32_t reg_val =  pTMR->CTL;
	reg_val &= ~(3 << bsTMR_CTL_OP_MODE);
	reg_val |= mode << bsTMR_CTL_OP_MODE;
	pTMR->CTL = reg_val;

//	pTMR->CTL &= ~(3 << bsTMR_CTL_OP_MODE);
//	pTMR->CTL |= mode << bsTMR_CTL_OP_MODE;
}

//
void TMR_Int_Enable(TMR_TypeDef *pTMR)
{
	pTMR->CTL |= 1 << bsTMR_CTL_INT_EN;
}

void TMR_Int_Disable(TMR_TypeDef *pTMR)
{
//	pTMR->CTL &= ~((1 << bsTMR_CTL_INT_EN) | (1 << bsTMR_CTL_INT_CLR));
	pTMR->CTL &= ~((1 << bsTMR_CTL_INT_EN) );
}

//
void TMR_Fetch_Enable(TMR_TypeDef *pTMR, TIMER_FETCH_MODE fetch_mode)
{
	if(fetch_mode == FETCH_LEVEL_MODE){
		pTMR->CTL |= 1 << bsTMR_CTL_TETCH_MODE;
	}else{
		pTMR->CTL &= ~(1 << bsTMR_CTL_TETCH_MODE);
	}
	pTMR->CTL |= 1 << bsTMR_CTL_TETCH_EN;
}
void TMR_Fetch_Disable(TMR_TypeDef *pTMR)
{
	pTMR->CTL &= ~((1 << bsTMR_CTL_TETCH_EN) | (1 << bsTMR_CTL_INT_CLR));
}


//
uint8_t TMR_Int_Happened(TMR_TypeDef *pTMR)
{
	return ( (pTMR->CTL >> bsTMR_CTL_INT_CLR) & BW2M(bwTMR_CTL_INT_CLR) );
}

void TMR_Int_Clr(TMR_TypeDef *pTMR)
{
	pTMR->CTL |= 1 << bsTMR_CTL_INT_CLR;
}

//-----------
// TMR_LOCK
//
void TMR_0_LOCK() // timer 0 only
{
	APB_TMR0->LOCK = 1;
}

void TMR_0_UNLOCK() // timer 0 only
{
	APB_TMR0->LOCK = TMR_UNLOCK;
}

//---------- watchdog functions -----

void WDG_Enable(void)
{
	APB_WDG_TMR->CTL |= (1 << bsTMR_CTL_TMR_EN | 1 << 16);  //Changing EN must also set bit 16 to 1
}

void WDG_Disable(void)
{
	APB_WDG_TMR->CTL |= ~(1 << bsTMR_CTL_TMR_EN);
}

//
void WDG_Int_Enable(void)
{
	APB_WDG_TMR->CTL |= 1 << bsTMR_CTL_INT_EN;
}

void WDG_Int_Disable(void)
{
	APB_WDG_TMR->CTL &= ~((1 << bsTMR_CTL_INT_EN) | (1 << bsTMR_CTL_INT_CLR)); //Disable Int not clear int status bit
}

void WDG_Reset_Enable(void)
{
	APB_WDG_TMR->CTL |= 1 << bsTMR_CTL_WatchDog_EN;
}
void WDG_Reset_Disable(void)
{
	APB_WDG_TMR->CTL &= ~(1 << bsTMR_CTL_WatchDog_EN);
}


uint8_t WatchDog_Reset_Happened()
{
	return ( (APB_WDG_TMR->CTL >> bsTMR_CTL_WatchDog_INT_CLR) & BW2M(bwTMR_CTL_WatchDog_INT_CLR) );
}

void WatchDog_Int_Clr()
{
	APB_WDG_TMR->CTL |= 1 << bsTMR_CTL_INT_CLR;
}
