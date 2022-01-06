#include "as32m0_pwm.h"

static int32_t apPWM_Check_CMD_Busy(PWM_TypeDef *pPWMx)
{
	uint32_t retry = 10000;
	while(retry--){
        if(!(pPWMx->PWM_CMD_ST & 0x1)){
        	return 0;
        }
	}
	return -1;
}
void apPWM_Enable(PWM_TypeDef *pPWMx)
{
	pPWMx->PWM_M_CTRL |= PWM_M_CTRL_EN;
}

void apPWM_Disable(PWM_TypeDef *pPWMx)
{
	pPWMx->PWM_M_CTRL &= ~PWM_M_CTRL_EN;
}

int32_t apPWM_Config(PWM_TypeDef *pPWMx, PWM_INFO *pwm_info)
{
		int32_t ret = -1;

		/* Check PWM CMD Status */
		ret = apPWM_Check_CMD_Busy(pPWMx);
		if(ret < 0){
			return -1;
		}

        if(pwm_info->ch_0_info.ch_p_point > pwm_info->ch_0_info.ch_n_point){
        	uint32_t point_temp = pwm_info->ch_0_info.ch_p_point;
        	pwm_info->ch_0_info.ch_p_point = pwm_info->ch_0_info.ch_n_point;
        	pwm_info->ch_0_info.ch_n_point = point_temp;
        }
        
        if(pwm_info->ch_1_info.ch_p_point > pwm_info->ch_1_info.ch_n_point){
        	uint32_t point_temp = pwm_info->ch_1_info.ch_p_point;
        	pwm_info->ch_1_info.ch_p_point = pwm_info->ch_1_info.ch_n_point;
        	pwm_info->ch_1_info.ch_n_point = point_temp;
        }

        pPWMx->PWM_CH0_CTRL = ((pwm_info->ch_0_info.oen_n << PWM_CH0_CTRL_BIT_OEN_N) | \
                               (pwm_info->ch_0_info.oen_p << PWM_CH0_CTRL_BIT_OEN_P) | \
                               (pwm_info->ch_0_info.out_n << PWM_CH0_CTRL_BIT_OUT_N) | \
                               (pwm_info->ch_0_info.out_p << PWM_CH0_CTRL_BIT_OUT_P) | \
                               (pwm_info->ch_0_info.oc_en << PWM_CH0_CTRL_BIT_OC_EN));

        pPWMx->PWM_CH0_PER = (uint32_t)((pwm_info->ch_0_info.ch_p_point << 16) | pwm_info->ch_0_info.ch_n_point);


        pPWMx->PWM_CH1_CTRL = ((pwm_info->ch_1_info.oen_n << PWM_CH1_CTRL_BIT_OEN_N) | \
                               (pwm_info->ch_1_info.oen_p << PWM_CH1_CTRL_BIT_OEN_P) | \
                               (pwm_info->ch_1_info.out_n << PWM_CH1_CTRL_BIT_OUT_N) | \
                               (pwm_info->ch_1_info.out_p << PWM_CH1_CTRL_BIT_OUT_P) | \
                               (pwm_info->ch_1_info.oc_en << PWM_CH1_CTRL_BIT_OC_EN));

        pPWMx->PWM_CH1_PER = (uint32_t)((pwm_info->ch_1_info.ch_p_point << PWM_CH1_PER_BIT_P_POINT) | \
                                        (pwm_info->ch_1_info.ch_n_point << PWM_CH1_PER_BIT_N_POINT));

        if(pwm_info->int_en){
        	apPWM_Int_Enable(pPWMx);
        }

        apPWM_Enable(pPWMx);

        pPWMx->PWM_PERIOD = (uint32_t)(((pwm_info->div & 0x3FF) << PWM_PERIOD_BIT_DIV) | \
                                                 (pwm_info->lmt << PWM_PERIOD_BIT_LMT));
				

        return 0;
}

int32_t apPWM_Set_Cycle(PWM_TypeDef *pPWMx, PWM_CYCLE_INFO *cycle_info)
{
    uint32_t pwm_period = pPWMx->PWM_PERIOD;
    uint32_t pwm_end_point = 0;
	int32_t ret = -1;

	/* Check PWM CMD Status */
	ret = apPWM_Check_CMD_Busy(pPWMx);
	if(ret < 0){
		return -1;
	}
    
    pwm_end_point = cycle_info->offset + (pwm_period & 0xFFFF)*(cycle_info->cycle)/100;

    if(pwm_end_point > (pwm_period & 0xFFFF)){
    	pwm_end_point = pwm_period & 0xFFFF;
    }
    
    pPWMx->PWM_CH1_PER = \
    pPWMx->PWM_CH0_PER = (uint32_t)((cycle_info->offset << PWM_CH1_PER_BIT_P_POINT) | \
                                    (pwm_end_point      << PWM_CH1_PER_BIT_N_POINT));
    
    
    if(cycle_info->cyc_polarity == CYCLE_HIGH_ACTIVE) {
    	pPWMx->PWM_CH1_CTRL = \
    	pPWMx->PWM_CH0_CTRL =  (uint32_t)((PWM_CH1_CTRL_OC_EN | PWM_CH1_CTRL_OEN_P | PWM_CH1_CTRL_OEN_N | PWM_CH1_CTRL_OUT_P));
    
    } else {
    	pPWMx->PWM_CH1_CTRL = \
    	pPWMx->PWM_CH0_CTRL =  (uint32_t)((PWM_CH1_CTRL_OC_EN | PWM_CH1_CTRL_OEN_P | PWM_CH1_CTRL_OEN_N | PWM_CH1_CTRL_OUT_N));
    
    }

    pPWMx->PWM_PERIOD = pwm_period;
    return 0;
}

int32_t apPWM_DZConfig(PWM_TypeDef *pPWMx, PWM_DZ_INFO_PTR pwm_dz_info_ptr)
{
	int32_t ret = -1;
    uint32_t chm_n_point = 0;
    uint32_t chn_p_point = 0, chn_n_point = 0;

	/* Check PWM CMD Status */
	ret = apPWM_Check_CMD_Busy(pPWMx);
	if(ret < 0){
		return -1;
	}

	/* Set Chm end point */
	chm_n_point = (pwm_dz_info_ptr->lmt & 0xFFFF)*(pwm_dz_info_ptr->chm_info.cycle)/100;
	if(chm_n_point > pwm_dz_info_ptr->lmt){
		chm_n_point = pwm_dz_info_ptr->lmt;
	}

	/* Set Chn start and end point base Chm point */
	chn_p_point = chm_n_point + pwm_dz_info_ptr->chn_info.dead_time;

	chn_n_point = pwm_dz_info_ptr->lmt - pwm_dz_info_ptr->chn_info.dead_time;
//	if(chn_p_point > chn_n_point){
//		uint32_t point_temp = chn_p_point;
//		chn_p_point = chn_n_point;
//		chn_n_point = point_temp;
//	}


	pPWMx->PWM_CH0_PER = (uint32_t)((0                      << PWM_CH0_PER_BIT_P_POINT) | \
			                        ((chm_n_point & 0xFFFF) << PWM_CH0_PER_BIT_N_POINT));

    pPWMx->PWM_CH1_PER = (uint32_t)(((chn_p_point & 0xFFFF) << PWM_CH1_PER_BIT_P_POINT) | \
                                    ((chn_n_point & 0xFFFF) << PWM_CH1_PER_BIT_N_POINT));

    if(pwm_dz_info_ptr->chm_info.out_en){
    	if(pwm_dz_info_ptr->chm_info.chm_polarity == CYCLE_HIGH_ACTIVE){
            pPWMx->PWM_CH0_CTRL = (PWM_CH0_CTRL_OC_EN | PWM_CH0_CTRL_OUT_P | PWM_CH0_CTRL_OEN_P | PWM_CH0_CTRL_OEN_N);
    	}else{
    		pPWMx->PWM_CH0_CTRL = (PWM_CH0_CTRL_OC_EN | PWM_CH0_CTRL_OUT_N | PWM_CH0_CTRL_OEN_P | PWM_CH0_CTRL_OEN_N);
    	}
    }else{
    	 pPWMx->PWM_CH0_CTRL &= ~(PWM_CH0_CTRL_OC_EN);
    }

    if(pwm_dz_info_ptr->chn_info.out_en){
    	if(pwm_dz_info_ptr->chn_info.chn_polarity == CYCLE_HIGH_ACTIVE){
            pPWMx->PWM_CH1_CTRL = (PWM_CH1_CTRL_OC_EN | PWM_CH1_CTRL_OUT_P | PWM_CH1_CTRL_OEN_P | PWM_CH1_CTRL_OEN_N);
    	}else{
    		pPWMx->PWM_CH1_CTRL = (PWM_CH1_CTRL_OC_EN | PWM_CH1_CTRL_OUT_N | PWM_CH1_CTRL_OEN_P | PWM_CH1_CTRL_OEN_N);
    	}
    }else{
    	 pPWMx->PWM_CH1_CTRL &= ~(PWM_CH1_CTRL_OC_EN);
    }

    if(pwm_dz_info_ptr->int_en){
    	apPWM_Int_Enable(pPWMx);
    }

    apPWM_Enable(pPWMx);

    pPWMx->PWM_PERIOD = (uint32_t)(((pwm_dz_info_ptr->div & 0x3FF)  << PWM_PERIOD_BIT_DIV) | \
                                   ((pwm_dz_info_ptr->lmt & 0xFFFF) << PWM_PERIOD_BIT_LMT));
	return 0;
}

void apPWM_Int_Clr(PWM_TypeDef *pPWMx)
{
        pPWMx->PWM_INT |= PWM_INT_ST;
}
void apPWM_Int_Enable(PWM_TypeDef *pPWMx)
{
        pPWMx->PWM_INT |= PWM_INT_EN;
}
void apPWM_Int_Disable(PWM_TypeDef *pPWMx)
{
        pPWMx->PWM_INT &= ~(PWM_INT_ST | PWM_INT_EN);  //Disable interrupt & not modify interrupt status
}


