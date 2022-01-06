#include "as32m0_lvd.h"

static void LVD_Enable(void)
{
	APB_LVD->CR |= (bwLVD_CR_EN << bsLVD_CR_EN);
}

static void LVD_Filter_Enable(void)
{
	APB_LVD->FLT |= (bwLVD_FLT_EN << bsLVD_FLT_EN);
}

void LVD_Disable(void)
{
	APB_LVD->CR &= ~(bwLVD_CR_EN << bsLVD_CR_EN);
}

void LVD_Init(LVD_INFO_PTR lvd_info_ptr)
{
	volatile uint32_t reg_val = 0;

	if(!lvd_info_ptr->byp_vdt_en){
		APB_LVD->SET = ((lvd_info_ptr->LVD_VDT & bwLVD_SET_SET) << bsLVD_SET_SET);
	}else{
		APB_LVD->SET |= (bwLVD_SET_BYP << bsLVD_SET_BYP);
	}

	if(lvd_info_ptr->filter_en){
		APB_LVD->FLT = ((lvd_info_ptr->filter_lmt & bwLVD_FLT_LMT) << bsLVD_FLT_LMT);

		LVD_Filter_Enable();
	}

	switch(lvd_info_ptr->LVD_TRIGGER_MODE){
	case HIGH_TRIGGER :
		reg_val = ((bwLVD_CR_LV << bsLVD_CR_LV) | (bwLVD_CR_PE << bsLVD_CR_PE));
		break;
	case LOW_TRIGGER :
		reg_val = ((bwLVD_CR_LV << bsLVD_CR_LV) | (bwLVD_CR_NE << bsLVD_CR_NE));
		break;
	case POSEDGE_TRIGGER :
		reg_val = (bwLVD_CR_PE << bsLVD_CR_PE);
		break;
	case NEGEDGE_TRIGGER :
		reg_val = (bwLVD_CR_NE << bsLVD_CR_NE);
		break;
	}

	if(lvd_info_ptr->LVD_MODE == RESET_MODE){
		reg_val |= (bwLVD_CR_MODE << bsLVD_CR_MODE);
	}
	APB_LVD->CR = reg_val;

	LVD_Enable();
}

void LVD_Int_Clr(void)
{
	APB_LVD->ST |= (bwLVD_ST_STATUS << bsLVD_ST_STATUS);
}



