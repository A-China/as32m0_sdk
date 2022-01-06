#include "as32m0_adc.h"
#include "as32m0_dma.h"
/*1:ready  0:not ready*/
uint8_t  ADC_WAIT_READY(ADC_TypeDef * ADC)
{
	return (ADC->ADC_Reset>>PARA_ADC_STARTUP);
}

void  ADC_RESET(ADC_TypeDef * ADC)
{
	ADC->ADC_Reset &= ~(1<<PARA_ADC_RESET);
}

void  ADC_NOT_RESET(ADC_TypeDef * ADC)
{
	ADC->ADC_Reset |= 1<<PARA_ADC_RESET;
}

/*1:data valid  0: data not valid */
uint8_t  ADC_DATA_VALID(ADC_TypeDef * ADC)
{
	return (ADC->ADC_Reset>>PARA_ADC_DATA_VALID_STA);
}


uint16_t ADC_DATA_READ(ADC_TypeDef * ADC)
{
	uint16_t rece_data;
	rece_data = ADC->ADC_Data&0xfff;
	return rece_data;
}

void ADC_CLR_CLK_GATE(ADC_TypeDef * ADC)
{
	ADC->ADC_Clk_Gate = 0;  
}
void ADC_SET_CLK_GATE(ADC_TypeDef * ADC)
{
	ADC->ADC_Clk_Gate = 1;  
}

void ADC_CLOCK_DIV(ADC_TypeDef * ADC, uint8_t div)
{
	ADC->ADC_Clock_Div = div;
}

void ADC_MODE_CONFIG(ADC_TypeDef * ADC, MODE_ADC mode)
{
	ADC->ADC_Mode  = 0;
	ADC->ADC_Mode |= mode;
}

//high level active
void ADC_SOC_WRITE(ADC_TypeDef * ADC, uint8_t w_data)
{
	ADC->ADC_Soc_Ctrl = w_data;
}

void ADC_Config(ADC_TypeDef * ADC, uint8_t channel, uint8_t SAMCTRL)
{
	uint8_t Channel = 0;
	Channel = channel&0xf;
	ADC->ADC_Config = 0;
	ADC->ADC_Config |= (Channel<<0) | (SAMCTRL<<4);
}

void ADC_PD(ADC_TypeDef * ADC, uint8_t PD, uint8_t PDBIAS)
{
	ADC->ADC_PD = 0;
	ADC->ADC_PD |= (PD<<0) | (PDBIAS<<1);
}

void ADC_SAMPLE_NUM(ADC_TypeDef * ADC, uint16_t sample_number)
{
	ADC->ADC_Sample_Num = sample_number;
}

void ADC_ENABLE_DATA_VALID_INTR(ADC_TypeDef * ADC)
{
	ADC->ADC_Inter_Enable |= 1<<PARA_ADC_DATA_VALID_INTE;
}

void ADC_DISABLE_DATA_VALID_INTR(ADC_TypeDef * ADC)
{
	ADC->ADC_Inter_Enable &= ~(1<<PARA_ADC_DATA_VALID_INTE);
}

void ADC_ENABLE_FIFO_OVER_INTR(ADC_TypeDef * ADC)
{
	ADC->ADC_Inter_Enable |= 1<<PARA_ADC_FIFO_OVERFLOW;
}

void ADC_DISABLE_FIFO_OVER_INTR(ADC_TypeDef * ADC)
{
	ADC->ADC_Inter_Enable &= ~(1<<PARA_ADC_FIFO_OVERFLOW);
}

//1:inter happend
uint8_t ADC_DATA_VALID_INTR_STA(ADC_TypeDef * ADC)
{
	return ADC->ADC_Valid_intr;
}

//1:inter happend
uint8_t ADC_FIFO_OVER__INTR_STA(ADC_TypeDef * ADC)
{
	return ADC->ADC_Overflow_intr;
}

void ADC_DATA_VALID_INTR_CLR(ADC_TypeDef * ADC)
{
	ADC->ADC_Valid_intr = 1;
}

void ADC_FIFO_OVER_INTR_CLR(ADC_TypeDef * ADC)
{
	ADC->ADC_Overflow_intr = 1;
}

//1:BUSY
uint8_t ADC_BUSY_STA(ADC_TypeDef * ADC)
{
	return ADC->ADC_Busy;
}


uint32_t adc_shot_test(uint8_t channel, uint8_t SAMCTRL, uint8_t clk_div ,uint32_t read_num)
{
	uint32_t i = 0, delay_index;
	uint32_t temp = 0;
	uint32_t data[100]={0};
//	printf("\n");
//	printf("channel is %d, SAMCTRL is %d, clk_div is %d \n",channel,SAMCTRL,clk_div);
//	printf("\n");
	//clear adc clock 
	ADC_CLR_CLK_GATE(APB_ADC);
	//clear adc reset
	ADC_NOT_RESET(APB_ADC);	
	APB_ADC->ADC_PD = 0x0;//power on mode
	//adc satrt ready
	for(delay_index = 0;delay_index<1000; delay_index++);
	//config one_shot mode
	ADC_MODE_CONFIG(APB_ADC,ADC_ONE_SHOT);
	//connfig clk_div
	ADC_CLOCK_DIV(APB_ADC, clk_div);
	//config insel,config samctrl
	ADC_Config(APB_ADC,channel,SAMCTRL);
	for(i=0;i<read_num;i++)
	{
		//config soc reg
		ADC_SOC_WRITE(APB_ADC, 0);
		while(!(ADC_DATA_VALID(APB_ADC)));
		data[i]=ADC_DATA_READ(APB_ADC);
	}
	ADC_MODE_CONFIG(APB_ADC,ADC_NONE);					// disable ADC
/*	
	for(i=0;i<read_num;i++)
	{
		printf("ADC DATA is 0x%x\n",data[i]);
		while(apUART_Check_BUSY(APB_UART0) == 1);
	}
	*/
	APB_ADC->ADC_PD = 0x1;//power off mode
	ADC_RESET(APB_ADC);
	ADC_SET_CLK_GATE(APB_ADC);
	temp = 0;
	for(i=0;i<read_num;i++)
	{
		temp = temp + data[i];
	}
	return (temp/read_num);
}

void ADC_INIT_MODE(uint8_t channel, uint8_t SAMCTRL, uint8_t clk_div , MODE_ADC mode){
	uint32_t delay_index;
//clear adc clock 
	ADC_CLR_CLK_GATE(APB_ADC);
	//clear adc reset
	ADC_NOT_RESET(APB_ADC);	
	APB_ADC->ADC_PD = 0x0;//power on mode
	//adc satrt ready
	for(delay_index = 0;delay_index<1000; delay_index++);
	//config one_shot mode
	ADC_MODE_CONFIG(APB_ADC,mode);
	//connfig clk_div
	ADC_CLOCK_DIV(APB_ADC, clk_div);
	//config insel,config samctrl
	ADC_Config(APB_ADC,channel,SAMCTRL);
}
uint32_t ADC_SHOT_MODE_GET_RESULT(uint8_t channel, uint8_t SAMCTRL, uint32_t read_num){
	uint32_t i = 0;
	uint32_t temp = 0;
	uint32_t data[100]={0};
	ADC_Config(APB_ADC,channel,SAMCTRL);
	for(i=0;i<read_num;i++)
	{
		//config soc reg
		ADC_SOC_WRITE(APB_ADC, 0);
		while(!(ADC_DATA_VALID(APB_ADC)));
		data[i]=ADC_DATA_READ(APB_ADC);
	}
	temp = 0;
	for(i=0;i<read_num;i++)
	{
		temp = temp + data[i];
	}
	return (temp/read_num);

}

uint32_t ADC_CONTINUOUS_MODE_GET_RESULT(uint8_t channel, uint8_t SAMCTRL, uint32_t read_num){
	uint32_t i = 0;
	uint32_t temp = 0;
	uint32_t data[100]={0};
	ADC_Config(APB_ADC,channel,SAMCTRL);
	ADC_SOC_WRITE(APB_ADC, 1);
	for(i=0;i<read_num;i++)
	{
		while(!(ADC_DATA_VALID(APB_ADC)));
		data[i]=APB_ADC->ADC_Data;
		//printf("ADC DATA is 0x%x\n",data);
	}
	temp = 0;
	for(i=0;i<read_num;i++)
	{
		temp = temp + data[i];
	}
	ADC_SOC_WRITE(APB_ADC, 0);
	return (temp/read_num);

}

uint32_t ADC_DMA_MODE_GET_RESULT(uint8_t channel, uint8_t SAMCTRL, uint32_t read_num){
	uint32_t i = 0;
	uint32_t temp = 0;
	uint32_t data[100]={0};
	DMA_sChLLI     pChLLI_0;
	DMA_sChCtrl    pChCtrl_0;
	DMA_sChConfig  pChConfig_0;

	DMA_sChLLI*     pChLLI_0_ptr = &pChLLI_0;
	DMA_sChCtrl*    pChCtrl_0_ptr = &pChCtrl_0;
	DMA_sChConfig*  pChConfig_0_ptr = &pChConfig_0;

	// DMA configuration
	DMA_LittleEndian(AHBMaster_1);
	// DMA Sync
	for (i=0;i<16;i++)
		   DMA_Enable_Sync(i);

	// Channel 6 Configuration : PERIPHERAL_TO_MEM_DMA_CTRL
	pChLLI_0_ptr->LLIAddr = 0;
	pChLLI_0_ptr->LM      = AHBMaster_1;

	pChCtrl_0_ptr->TCIrqEn       = DMA_Bit_Clr;
	pChCtrl_0_ptr->Prot          = DMA_PROT_USER | DMA_PROT_NON_BUFFERABLE | DMA_PROT_NON_CACHEABLE;
	pChCtrl_0_ptr->DestInc       = DMA_Bit_Set;
	pChCtrl_0_ptr->SourInc       = DMA_Bit_Clr;
	pChCtrl_0_ptr->DestBusSel    = AHBMaster_1;
	pChCtrl_0_ptr->SourBusSel    = AHBMaster_1;
	pChCtrl_0_ptr->DestWidth     = DMA_WIDTH_32_BIT;
	pChCtrl_0_ptr->SourWidth     = DMA_WIDTH_32_BIT;
	pChCtrl_0_ptr->DestBurstSize = DMA_BURST_1;
	pChCtrl_0_ptr->SourBurstSize = DMA_BURST_1;
	pChCtrl_0_ptr->TransferSize  = read_num;

	pChConfig_0_ptr->Halt           = DMA_Bit_Clr;
	//ChConfig.ActiveChannel
	pChConfig_0_ptr->LockTransfer   = DMA_Bit_Clr;
	pChConfig_0_ptr->TCIrqMask      = DMA_Bit_Set;
	pChConfig_0_ptr->ErrorIrqMask   = DMA_Bit_Set;
	pChConfig_0_ptr->FlowCtrl       = DMA_PERIPHERAL_TO_MEM_DMA_CTRL;
	pChConfig_0_ptr->DestPeripheral = 0;
	pChConfig_0_ptr->SourPeripheral = 8;  // use
	pChConfig_0_ptr->ChannelEnable  = DMA_Bit_Set;	
	
	ADC_Config(APB_ADC,channel,SAMCTRL);
	ADC_SAMPLE_NUM(APB_ADC,read_num );
	DMA_Enable();
  DMA_SetChannelReg((uint32_t)(&data), (uint32_t)(APB_ADC_BASE+0x4), pChLLI_0_ptr, pChCtrl_0_ptr, pChConfig_0_ptr, Ch_0);
    //config soc reg
	ADC_SOC_WRITE(APB_ADC, 0);
	while (DMA_GetActiveChannels());
	//ADC_MODE_CONFIG(APB_ADC,ADC_NONE);
	temp = 0;
	for(i=0;i<read_num;i++)
	{
		temp = temp + data[i];
	}
	ADC_SOC_WRITE(APB_ADC, 0);
	DMA_Disable();
	return (temp/read_num);

}
