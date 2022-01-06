#ifndef __AS32M0_PWM_H__
#define __AS32M0_PWM_H__
#include "as32m0.h"

/* bits in PWM_M_CTRL bit function */
#define PWM_M_CTRL_BIT_EN            0   
#define PWM_M_CTRL_EN               (1 << PWM_M_CTRL_BIT_EN)   

/* bits in PWM_CH0_CTRL bit function */
#define PWM_CH0_CTRL_BIT_OC_EN       0
#define PWM_CH0_CTRL_BIT_OUT_P       4
#define PWM_CH0_CTRL_BIT_OUT_N       5
#define PWM_CH0_CTRL_BIT_OEN_P       6
#define PWM_CH0_CTRL_BIT_OEN_N       7

#define PWM_CH0_CTRL_OC_EN          (1 << PWM_CH0_CTRL_BIT_OC_EN) 
#define PWM_CH0_CTRL_OUT_P          (1 << PWM_CH0_CTRL_BIT_OUT_P)
#define PWM_CH0_CTRL_OUT_N          (1 << PWM_CH0_CTRL_BIT_OUT_N)
#define PWM_CH0_CTRL_OEN_P          (1 << PWM_CH0_CTRL_BIT_OEN_P)
#define PWM_CH0_CTRL_OEN_N          (1 << PWM_CH0_CTRL_BIT_OEN_N)

/* bits in PWM_CH1_CTRL bit function */
#define PWM_CH1_CTRL_BIT_OC_EN       0 
#define PWM_CH1_CTRL_BIT_OUT_P       4
#define PWM_CH1_CTRL_BIT_OUT_N       5
#define PWM_CH1_CTRL_BIT_OEN_P       6
#define PWM_CH1_CTRL_BIT_OEN_N       7

#define PWM_CH1_CTRL_OC_EN          (1 << PWM_CH1_CTRL_BIT_OC_EN) 
#define PWM_CH1_CTRL_OUT_P          (1 << PWM_CH1_CTRL_BIT_OUT_P)
#define PWM_CH1_CTRL_OUT_N          (1 << PWM_CH1_CTRL_BIT_OUT_N)
#define PWM_CH1_CTRL_OEN_P          (1 << PWM_CH1_CTRL_BIT_OEN_P)
#define PWM_CH1_CTRL_OEN_N          (1 << PWM_CH1_CTRL_BIT_OEN_N)

/* bits in PWM_CH0_PER bit function */
#define PWM_CH0_PER_BIT_N_POINT     0 
#define PWM_CH0_PER_BIT_P_POINT     16

#define PWM_CH0_PER_N_POINT        (0xFFFF << PWM_CH0_PER_BIT_N_POINT) 
#define PWM_CH0_PER_P_POINT        (0xFFFF << PWM_CH0_PER_BIT_P_POINT)

/* bits in PWM_CH1_PER bit function */
#define PWM_CH1_PER_BIT_N_POINT     0 
#define PWM_CH1_PER_BIT_P_POINT     16

#define PWM_CH1_PER_N_POINT        (0xFFFF << PWM_CH1_PER_BIT_N_POINT) 
#define PWM_CH1_PER_P_POINT        (0xFFFF << PWM_CH1_PER_BIT_P_POINT)

/* bits in PWM_PERIOD bit function */
#define PWM_PERIOD_BIT_LMT          0                // [15:0]
#define PWM_PERIOD_BIT_DIV          16               // [25:16]

#define PWM_PERIOD_LMT             (0xFFFF << PWM_PERIOD_BIT_LMT) 
#define PWM_PERIOD_DIV             (0x3FF << PWM_PERIOD_BIT_DIV)

/* bits in PWM_INT bit function */
#define PWM_INT_BIT_EN              0
#define PWM_INT_BIT_ST              1

#define PWM_INT_EN                 (1 << PWM_INT_BIT_EN)
#define PWM_INT_ST                 (1 << PWM_INT_BIT_ST)

/* bits in PWM_CMD_ST bit function */
#define PWM_CMDST_BIT_ST            0

#define PWM_CMDST_ST               (1 << PWM_CMDST_BIT_ST)

typedef enum pwm_cycle_polarity{
    CYCLE_HIGH_ACTIVE = 0,
    CYCLE_LOW_ACTIVE  = 1
}PWM_CYCLE_POLARITY;

typedef struct pwm_cycle_info{
    uint32_t offset;
    uint8_t cycle;
    PWM_CYCLE_POLARITY cyc_polarity;
}PWM_CYCLE_INFO;

typedef struct ch_info {
	uint8_t oc_en;
	uint8_t out_p;
	uint8_t out_n;
	uint8_t oen_p;
	uint8_t oen_n;
	uint32_t ch_p_point;
	uint32_t ch_n_point;
}CH_INFO;

typedef struct pwm_info {
	CH_INFO ch_0_info;
	CH_INFO ch_1_info;
	uint32_t div;
	uint32_t lmt;
	uint8_t int_en;
}PWM_INFO;

typedef struct chm_info{
	uint8_t  cycle;
	uint8_t  out_en;
	PWM_CYCLE_POLARITY  chm_polarity;
}CHM_INFO;

typedef struct chn_info{
	uint32_t dead_time;
	uint8_t  out_en;
	PWM_CYCLE_POLARITY  chn_polarity;
}CHN_INFO;

typedef struct pwm_dz_info{
	uint32_t div;
	uint32_t lmt;
	uint8_t int_en;
	CHM_INFO chm_info;
	CHN_INFO chn_info;
}PWM_DZ_INFO, *PWM_DZ_INFO_PTR;


int32_t apPWM_Config(PWM_TypeDef *pPWMx, PWM_INFO *pwm_info);
int32_t apPWM_Set_Cycle(PWM_TypeDef *pPWMx, PWM_CYCLE_INFO *cycle_info); //just for test

int32_t apPWM_DZConfig(PWM_TypeDef *pPWMx, PWM_DZ_INFO_PTR pwm_dz_info_ptr);

void apPWM_Int_Clr(PWM_TypeDef *pPWMx);
void apPWM_Int_Enable(PWM_TypeDef *pPWMx);
void apPWM_Int_Disable(PWM_TypeDef *pPWMx);
void apPWM_Enable(PWM_TypeDef *pPWMx);
void apPWM_Disable(PWM_TypeDef *pPWMx);



//void apPWM_(PWM_TypeDef *pPWM)
//void apPWM_(PWM_TypeDef *pPWM)
//void apPWM_(PWM_TypeDef *pPWM)


#endif
