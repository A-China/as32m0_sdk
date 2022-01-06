#include "test_apb_ssp_flash.h"
#include "as32m0_ssp.h"
#include "as32m0_dma.h"
#include "as32m0_gpio.h"
#include "as32m0_pinctrl.h"
#include "as32m0_uart.h"

static void spi_flash_send_byte(uint8_t data)
{
	apSSP_WriteFIFO(APB_SPI,data);
	apSSP_DeviceEnable(APB_SPI);
	while(apSSP_DeviceBusyGet(APB_SPI));
	apSSP_DeviceDisable(APB_SPI);
	apSSP_ReadFIFO(APB_SPI);
}
static uint16_t spi_flash_rece_byte(void)
{
	apSSP_WriteFIFO(APB_SPI,0x00);
	apSSP_DeviceEnable(APB_SPI);
	while(apSSP_DeviceBusyGet(APB_SPI));
	apSSP_DeviceDisable(APB_SPI);
	return apSSP_ReadFIFO(APB_SPI);
}
static void FLASH_BUSY(void)
{
	APB_FLASH_Enable;
	spi_flash_send_byte(0x05);
	while((spi_flash_rece_byte()&0x01)==0x01);
	APB_FLASH_Disable;
}
/*
static void ssp_config(void)
{
	apSSP_sDeviceControlBlock pParam; 
	apSSP_sDeviceControlBlock* pParam_ptr = &pParam;  
	apSSP_Initialize(APB_SPI);
	
	/* Set Device Parameters */
/*
 	pParam_ptr->ClockRate        = 0;  // sspclkout = sspclk/(ClockPrescale*(ClockRate+1))
 	pParam_ptr->ClockPrescale    = 2;  //8MHz
 	pParam_ptr->eSCLKPhase       = apSSP_SCLKPHASE_TRAILINGEDGE;
 	pParam_ptr->eSCLKPolarity    = apSSP_SCLKPOLARITY_IDLELOW;
  	pParam_ptr->eFrameFormat     = apSSP_FRAMEFORMAT_MOTOROLASPI;
  	pParam_ptr->eDataSize        = apSSP_DATASIZE_8BITS;
  	pParam_ptr->eLoopBackMode    = apSSP_LOOPBACKOFF;
 	pParam_ptr->eMasterSlaveMode = apSSP_MASTER;
  	pParam_ptr->eSlaveOutput     = apSSP_SLAVEOUTPUTDISABLED;
  	apSSP_DeviceParametersSet(APB_SPI, pParam_ptr); 
} 
*/
/*
void test_apb_ssp_flash()
{
	uint32_t index = 0;
	uint8_t temp_val  = 0;

	printf("\n");
	printf("\n");
	printf("// TEST APB SPI ACCESS FLASH \n");
	printf("\n");
	printf("\n");

	ssp_config();
	APB_SSP_FLASH_CS_Init();
	//*******************************
	//
	// read spi flash
	//    spi flash spec : Read JEDEC (9Fh)
	//*******************************
	//WRITE ENABLE
	APB_FLASH_Enable;
	spi_flash_send_byte(0x06);
	APB_FLASH_Disable;
	//WRITE DATA
	APB_FLASH_Enable;
	spi_flash_send_byte(0x02);
	spi_flash_send_byte(0x00);
	spi_flash_send_byte(0x00);
	spi_flash_send_byte(0x00);
    for(index=0;index<32;index++)
	{
		spi_flash_send_byte(index+7);
	}
	APB_FLASH_Disable;
	FLASH_BUSY();

	APB_FLASH_Enable;
	spi_flash_send_byte(0x3);
	spi_flash_send_byte(0x00);
	spi_flash_send_byte(0x00);
	spi_flash_send_byte(0x00);
	for(index=0;index<32;index++)
	{
		temp_val = spi_flash_rece_byte();
        if(temp_val != index+7)
        {
        	printf("error:temp_val is 0x%x != 0x%x\n ",temp_val,index+7);
        	break;
        }
	}
	APB_FLASH_Disable;
	if(index == 32)
	{
		printf("Anto test spi_flash success\n");
	}
	else
	{
		printf("Anto test spi_flash failed\n");
	}
}
*/
/*
void test_apb_ssp_with_dma(uint8_t master)
{

	uint32_t i ;
	uint32_t tmp;
	uint8_t test = 0x00;
	uint8_t rece_mem[32]={0};

	DMA_sChLLI     pChLLI_0;
	DMA_sChLLI     pChLLI_1;
	DMA_sChCtrl    pChCtrl_0;
	DMA_sChCtrl    pChCtrl_1;
	DMA_sChConfig  pChConfig_0;
	DMA_sChConfig  pChConfig_1;

	DMA_sChLLI*     pChLLI_0_ptr = &pChLLI_0;
	DMA_sChLLI*     pChLLI_1_ptr = &pChLLI_1;
	DMA_sChCtrl*    pChCtrl_0_ptr = &pChCtrl_0;
	DMA_sChCtrl*    pChCtrl_1_ptr = &pChCtrl_1;
	DMA_sChConfig*  pChConfig_0_ptr = &pChConfig_0;
	DMA_sChConfig*  pChConfig_1_ptr = &pChConfig_1;

	printf("\n");
	printf("\n");
	printf("// TEST APB SPI ACCESS FLASH with DMA MASTER %d \n", 1+master);
	printf("\n");
	printf("\n");
	// wait
	while(apUART_Check_BUSY(APB_UART0) == 1);
	
	// ssp config
	ssp_config();
	// DMA configuration
	DMA_LittleEndian(master);	
	// DMA Sync
	for (i=0;i<16;i++)
           DMA_Enable_Sync(i);	
	// Channel 1 Configuration : MEM_TO_PERIPHERAL_DMA_CTRL
	pChLLI_1_ptr->LLIAddr = 0;
	pChLLI_1_ptr->LM      = master;
	
	pChCtrl_1_ptr->TCIrqEn       = DMA_Bit_Clr;
	pChCtrl_1_ptr->Prot          = DMA_PROT_USER | DMA_PROT_NON_BUFFERABLE | DMA_PROT_NON_CACHEABLE;
	pChCtrl_1_ptr->DestInc       = DMA_Bit_Clr;
	pChCtrl_1_ptr->SourInc       = DMA_Bit_Clr;
	pChCtrl_1_ptr->DestBusSel    = master;
	pChCtrl_1_ptr->SourBusSel    = master;
	pChCtrl_1_ptr->DestWidth     = DMA_WIDTH_8_BIT;
	pChCtrl_1_ptr->SourWidth     = DMA_WIDTH_8_BIT;
	pChCtrl_1_ptr->DestBurstSize = DMA_BURST_4;
	pChCtrl_1_ptr->SourBurstSize = DMA_BURST_4;
	pChCtrl_1_ptr->TransferSize  = 32;
	
	pChConfig_1_ptr->Halt           = DMA_Bit_Clr;
	//ChConfig->ActiveChannel 
	pChConfig_1_ptr->LockTransfer   = DMA_Bit_Clr;
	pChConfig_1_ptr->TCIrqMask      = DMA_Bit_Set;
	pChConfig_1_ptr->ErrorIrqMask   = DMA_Bit_Set;
	pChConfig_1_ptr->FlowCtrl       = DMA_MEM_TO_PERIPHERAL_DMA_CTRL;
	pChConfig_1_ptr->DestPeripheral = 1;  // use
	pChConfig_1_ptr->SourPeripheral = 0;
	pChConfig_1_ptr->ChannelEnable  = DMA_Bit_Set;
	
	
	// Channel 6 Configuration : PERIPHERAL_TO_MEM_DMA_CTRL
	pChLLI_0_ptr->LLIAddr = 0;
	pChLLI_0_ptr->LM      = master;
	
	pChCtrl_0_ptr->TCIrqEn       = DMA_Bit_Clr;
	pChCtrl_0_ptr->Prot          = DMA_PROT_USER | DMA_PROT_NON_BUFFERABLE | DMA_PROT_NON_CACHEABLE;
	pChCtrl_0_ptr->DestInc       = DMA_Bit_Set;
	pChCtrl_0_ptr->SourInc       = DMA_Bit_Clr;
	pChCtrl_0_ptr->DestBusSel    = master;
	pChCtrl_0_ptr->SourBusSel    = master;
	pChCtrl_0_ptr->DestWidth     = DMA_WIDTH_8_BIT;
	pChCtrl_0_ptr->SourWidth     = DMA_WIDTH_8_BIT;
	pChCtrl_0_ptr->DestBurstSize = DMA_BURST_4;
	pChCtrl_0_ptr->SourBurstSize = DMA_BURST_4;
	pChCtrl_0_ptr->TransferSize  = pChCtrl_1_ptr->TransferSize;
	
	pChConfig_0_ptr->Halt           = DMA_Bit_Clr;
	//ChConfig.ActiveChannel 
	pChConfig_0_ptr->LockTransfer   = DMA_Bit_Clr;
	pChConfig_0_ptr->TCIrqMask      = DMA_Bit_Set;
	pChConfig_0_ptr->ErrorIrqMask   = DMA_Bit_Set;
	pChConfig_0_ptr->FlowCtrl       = DMA_PERIPHERAL_TO_MEM_DMA_CTRL;
	pChConfig_0_ptr->DestPeripheral = 0;
	pChConfig_0_ptr->SourPeripheral = 0;  // use
	pChConfig_0_ptr->ChannelEnable  = DMA_Bit_Set;
	
	
	// spi flash instruction phase 
 	 apSSP_WriteFIFO(APB_SPI, 0x3);  
  	apSSP_WriteFIFO(APB_SPI, 0x0);   
  	apSSP_WriteFIFO(APB_SPI, 0x0);   
  	apSSP_WriteFIFO(APB_SPI, 0x0);   
  
  	APB_FLASH_Enable; 
  	apSSP_DeviceEnable(APB_SPI); 
 	 while(apSSP_DeviceBusyGet(APB_SPI)); 
  	apSSP_DeviceReceiveClear(APB_SPI); // clear high Z data 
   
  	// NOTE : SSP dma must open at data phase.
  	apSSP_DMAModeSet(APB_SPI, apSSP_DMA_TX_ON);
	apSSP_DMAModeSet(APB_SPI, apSSP_DMA_RX_ON);
  
 	DMA_Enable();
	DMA_SetChannelReg((uint32_t)(&rece_mem), apSSP_DMAAddressGet(APB_SPI_BASE), pChLLI_0_ptr, pChCtrl_0_ptr, pChConfig_0_ptr, Ch_0);
 	DMA_SetChannelReg(apSSP_DMAAddressGet(APB_SPI_BASE), (uint32_t)(&test),pChLLI_1_ptr, pChCtrl_1_ptr, pChConfig_1_ptr, Ch_1);
		
  	// wait dma op completion
 	 while (DMA_GetActiveChannels());
  
  	// close ssp and spi flash
  	apSSP_DeviceDisable(APB_SPI);
 	APB_FLASH_Disable; 
  
  	// prepare for next spi flash access
  	apSSP_DMAModeSet(APB_SPI, apSSP_DMA_TX_OFF);
	apSSP_DMAModeSet(APB_SPI, apSSP_DMA_RX_OFF); 
	
	// read date form SRMA_BASE+0x1000, and result compare
  	for (i=0;i<pChCtrl_1_ptr->TransferSize;i++)
 	{
  	    tmp = rece_mem[i];
 	    if (tmp != i+7)                
  		printf("Error : rece_mem[%d] = 0x%x \n", i, tmp);
  	}

	printf("// Auto Compare Passed \n");
}
*/


















