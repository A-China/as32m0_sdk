
#ifndef __AS32M0_ADC_H__
#define __AS32M0_ADC_H__


#ifdef	__cplusplus
extern "C" {	/* allow C++ to use these headers */
#endif	/* __cplusplus */

#include "as32m0.h"

#define PARA_ADC_STARTUP    0x1
#define PARA_ADC_RESET      0x0

#define PARA_ADC_DATA_VALID_STA      2
#define PARA_ADC_DATA_VALID_INTE     0
#define PARA_ADC_FIFO_OVERFLOW       1
#define PARA_ADC_BUSY                0

typedef enum ADC_xmode
{
	ADC_ONE_SHOT = 1,
	ADC_CONTINUOUS = 2,
	ADC_DMA = 4,
	ADC_NONE =0
} MODE_ADC;

uint8_t  ADC_WAIT_READY(ADC_TypeDef * ADC);
void  ADC_RESET(ADC_TypeDef * ADC);
void  ADC_NOT_RESET(ADC_TypeDef * ADC);
uint8_t  ADC_DATA_VALID(ADC_TypeDef * ADC);
uint16_t ADC_DATA_READ(ADC_TypeDef * ADC);
void ADC_CLOCK_DIV(ADC_TypeDef * ADC, uint8_t div);
void ADC_MODE_CONFIG(ADC_TypeDef * ADC, MODE_ADC mode);
void ADC_SOC_WRITE(ADC_TypeDef * ADC, uint8_t w_data);
void ADC_Config(ADC_TypeDef * ADC, uint8_t channel, uint8_t SAMCTRL);
void ADC_PD(ADC_TypeDef * ADC, uint8_t PD, uint8_t PDBIAS);
void ADC_SAMPLE_NUM(ADC_TypeDef * ADC, uint16_t sample_number);
void ADC_ENABLE_DATA_VALID_INTR(ADC_TypeDef * ADC);
void ADC_DISABLE_DATA_VALID_INTR(ADC_TypeDef * ADC);
void ADC_ENABLE_FIFO_OVER_INTR(ADC_TypeDef * ADC);
void ADC_DISABLE_FIFO_OVER_INTR(ADC_TypeDef * ADC);
uint8_t ADC_DATA_VALID_INTR_STA(ADC_TypeDef * ADC);
uint8_t ADC_FIFO_OVER__INTR_STA(ADC_TypeDef * ADC);
void ADC_FIFO_OVER_INTR_CLR(ADC_TypeDef * ADC);
void ADC_DATA_VALID_INTR_CLR(ADC_TypeDef * ADC);
uint8_t ADC_BUSY_STA(ADC_TypeDef * ADC);
void ADC_CLR_CLK_GATE(ADC_TypeDef * ADC);
void ADC_SET_CLK_GATE(ADC_TypeDef * ADC);
uint32_t adc_shot_test(uint8_t channel, uint8_t SAMCTRL, uint8_t clk_div ,uint32_t read_num);
void ADC_INIT_MODE(uint8_t channel, uint8_t SAMCTRL, uint8_t clk_div , MODE_ADC mode);
uint32_t ADC_SHOT_MODE_GET_RESULT(uint8_t channel, uint8_t SAMCTRL, uint32_t read_num);
uint32_t ADC_CONTINUOUS_MODE_GET_RESULT(uint8_t channel, uint8_t SAMCTRL, uint32_t read_num);
uint32_t ADC_DMA_MODE_GET_RESULT(uint8_t channel, uint8_t SAMCTRL, uint32_t read_num);
#ifdef __cplusplus
} /* allow C++ to use these headers */
#endif	/* __cplusplus */

#endif

