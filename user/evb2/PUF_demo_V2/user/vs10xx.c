#include "vs10xx.h"
#include "as32m0_gpio.h"
#include "as32m0_ssp.h"
#include "as32m0_dma.h"

	DMA_sChLLI     pChLLI_1;
	DMA_sChCtrl    pChCtrl_1;
	DMA_sChConfig  pChConfig_1;
		
	DMA_sChLLI*     pChLLI_1_ptr = &pChLLI_1;
	DMA_sChCtrl*    pChCtrl_1_ptr = &pChCtrl_1;
	DMA_sChConfig*  pChConfig_1_ptr = &pChConfig_1;	

void init_VS10xx (){
	APB_GPIO->GPIO_DO.SET = AUDIO_XCS;
	APB_GPIO->GPIO_DO.SET = AUDIO_XRESET;
	APB_GPIO->GPIO_DO.SET = AUDIO_DREQ;
	APB_GPIO->GPIO_DO.SET = AUDIO_XDCS;
	APB_GPIO->GPIO_OE.SET = AUDIO_XCS;
	APB_GPIO->GPIO_OE.SET = AUDIO_XRESET;
	APB_GPIO->GPIO_OE.CLR = AUDIO_DREQ;
	APB_GPIO->GPIO_OE.SET = AUDIO_XDCS;
 	reset();
	printf("init VS10xx Done\n");
};

void reset(){
	APB_GPIO->GPIO_DO.CLR = AUDIO_XRESET;
	audio_delay(1<<20);
	spi_send_byte(0xFF);
	APB_GPIO->GPIO_DO.SET = AUDIO_XCS;
	APB_GPIO->GPIO_DO.SET = AUDIO_XDCS;
  APB_GPIO->GPIO_DO.SET = AUDIO_XRESET;
	while((APB_GPIO->GPIO_DI & AUDIO_DREQ) == 0);
	softReset(1);
};

void audio_delay(unsigned int count){
	int i;
	for(i=0;i<count;i++);
}

void spi_send_byte(uint8_t data){
    APB_SPI->DataRegister = data;
    apSSP_DeviceEnable(APB_SPI);
    while(apSSP_DeviceBusyGet(APB_SPI));
    apSSP_DeviceDisable(APB_SPI);
    apSSP_ReadFIFO(APB_SPI);
}

void spi_send_8byte(uint8_t *data){
	uint8_t i;

	for(i=0;i<8;i++){
    APB_SPI->DataRegister = *data++;
	}
    apSSP_DeviceEnable(APB_SPI);
    while(apSSP_DeviceBusyGet(APB_SPI));
		apSSP_DeviceReceiveClear(APB_SPI);
}

void spi_send_32byte_dma(uint8_t *data){
//	init_dma_spi();
/*
	DMA_LittleEndian(AHBMaster_1);
	//printf("DMA Config\n");
	// DMA Sync
	for (i=0;i<16;i++)
           DMA_Enable_Sync(i);	
	// Channel 1 Configuration : MEM_TO_PERIPHERAL_DMA_CTRL
	pChLLI_1_ptr->LLIAddr = 0;
	pChLLI_1_ptr->LM      = AHBMaster_1;
	
	pChCtrl_1_ptr->TCIrqEn       = DMA_Bit_Clr;
	pChCtrl_1_ptr->Prot          = DMA_PROT_USER | DMA_PROT_NON_BUFFERABLE | DMA_PROT_NON_CACHEABLE;
	pChCtrl_1_ptr->DestInc       = DMA_Bit_Clr;
	pChCtrl_1_ptr->SourInc       = DMA_Bit_Set;
	pChCtrl_1_ptr->DestBusSel    = AHBMaster_1;
	pChCtrl_1_ptr->SourBusSel    = AHBMaster_1;
	pChCtrl_1_ptr->DestWidth     = DMA_WIDTH_8_BIT;
	pChCtrl_1_ptr->SourWidth     = DMA_WIDTH_8_BIT;
	pChCtrl_1_ptr->DestBurstSize = DMA_BURST_32;
	pChCtrl_1_ptr->SourBurstSize = DMA_BURST_32;
	pChCtrl_1_ptr->TransferSize  = 512;
	
	pChConfig_1_ptr->Halt           = DMA_Bit_Clr;
	//ChConfig->ActiveChannel 
	pChConfig_1_ptr->LockTransfer   = DMA_Bit_Clr;
	pChConfig_1_ptr->TCIrqMask      = DMA_Bit_Set;
	pChConfig_1_ptr->ErrorIrqMask   = DMA_Bit_Set;
	pChConfig_1_ptr->FlowCtrl       = DMA_MEM_TO_PERIPHERAL_DMA_CTRL;
	pChConfig_1_ptr->DestPeripheral = 1;  // use
	pChConfig_1_ptr->SourPeripheral = 0;
	pChConfig_1_ptr->ChannelEnable  = DMA_Bit_Set;
	*/
//	apSSP_DeviceEnable(APB_SPI);
 //   apSSP_DeviceEnable(APB_SPI);
//	apSSP_DeviceReceiveClear(APB_SPI);

	DMA_SetChannelReg(apSSP_DMAAddressGet(APB_SPI_BASE), (uint32_t)(data),pChLLI_1_ptr, pChCtrl_1_ptr, pChConfig_1_ptr, Ch_0);
	while (DMA_GetActiveChannels());

}

void spi_send_512byte_dma(uint8_t *data){
//	init_dma_spi();
	unsigned char i;
	apSSP_DMAModeSet(APB_SPI, apSSP_DMA_TX_ON);
	DMA_Enable();
	for(i=0;i<16;i++){
		while((APB_GPIO->GPIO_DI & AUDIO_DREQ) == 0){};
		spi_send_32byte_dma(data);
		data=data+32;
	}
	apSSP_DMAModeSet(APB_SPI, apSSP_DMA_TX_OFF);
	DMA_Disable();

}

unsigned char spi_getdata(void){
    APB_SPI->DataRegister = 0xFF;
    apSSP_DeviceEnable(APB_SPI);
    while(apSSP_DeviceBusyGet(APB_SPI));
    apSSP_DeviceDisable(APB_SPI);
    return apSSP_ReadFIFO(APB_SPI);
}

void writeRegister(unsigned char addressbyte, unsigned int value){
	APB_GPIO->GPIO_DO.SET = AUDIO_XCS;
	while((APB_GPIO->GPIO_DI & AUDIO_DREQ) == 0);
	APB_GPIO->GPIO_DO.CLR = AUDIO_XCS;
	spi_send_byte(VS_WRITE_COMMAND);
	spi_send_byte(addressbyte);
	spi_send_byte(value >> 8);
	spi_send_byte(value & 0xFF);
	APB_GPIO->GPIO_DO.SET = AUDIO_XCS;
}

unsigned int readRegister(unsigned char addressbyte){
	unsigned int temp = 0;
	APB_GPIO->GPIO_DO.SET = AUDIO_XCS;
	while((APB_GPIO->GPIO_DI & AUDIO_DREQ) == 0);
	APB_GPIO->GPIO_DO.CLR = AUDIO_XCS;
	spi_send_byte(VS_READ_COMMAND);
	spi_send_byte(addressbyte);
	temp = spi_getdata() <<8;
	temp |= spi_getdata();
	APB_GPIO->GPIO_DO.SET = AUDIO_XCS;
	return temp;
}
void writeData(unsigned char *databuf){
	APB_GPIO->GPIO_DO.SET = AUDIO_XCS;
	APB_GPIO->GPIO_DO.SET = AUDIO_XDCS;
	apSSP_DeviceEnable(APB_SPI);
	APB_GPIO->GPIO_DO.CLR = AUDIO_XDCS;
	spi_send_512byte_dma(databuf);

	

/*
	  while (i--)
    {
			spi_send_byte(*databuf++);
    }
*/		
  APB_GPIO->GPIO_DO.SET = AUDIO_XDCS;
	apSSP_DeviceDisable(APB_SPI);
};

void softReset(unsigned char test){
	writeRegister(SPI_MODE, 0x0804);
	audio_delay(1<<18);
  while((APB_GPIO->GPIO_DI & AUDIO_DREQ) == 0);
    
    /* A quick sanity check: write to two registers, then test if we
     get the same results. Note that if you use a too high SPI
     speed, the MSB is the most likely to fail when read again. */
	if(test == 1){
		writeRegister(SPI_HDAT0, 0xABAD);
		writeRegister(SPI_HDAT1, 0x1DEA);
		if (readRegister(SPI_HDAT0) != 0xABAD || readRegister(SPI_HDAT1) != 0x1DEA) {
					printf("There is something wrong with VS10xx\n");
						printf("SPI_HDAT0 : %.8x\n", readRegister(SPI_HDAT0));
						printf("SPI_HDAT1 : %.8x\n", readRegister(SPI_HDAT1));
			}
  }  
  writeRegister(SPI_CLOCKF,0XC000);   //Set the clock
  writeRegister(SPI_AUDATA,0xbb81);   //samplerate 48k,stereo
  writeRegister(SPI_BASS, 0x0055);    //set accent
  writeRegister(SPI_VOL, 0x6060);     //Set volume level
        
 while((APB_GPIO->GPIO_DI & AUDIO_DREQ) == 0);
};
void init_dma_spi(){
	unsigned char i;

	DMA_LittleEndian(AHBMaster_1);	
	// DMA Sync
	for (i=0;i<16;i++)
           DMA_Enable_Sync(i);	
	// Channel 1 Configuration : MEM_TO_PERIPHERAL_DMA_CTRL
	pChLLI_1_ptr->LLIAddr = 0;
	pChLLI_1_ptr->LM      = AHBMaster_1;
	
	pChCtrl_1_ptr->TCIrqEn       = DMA_Bit_Clr;
	pChCtrl_1_ptr->Prot          = DMA_PROT_USER | DMA_PROT_NON_BUFFERABLE | DMA_PROT_NON_CACHEABLE;
	pChCtrl_1_ptr->DestInc       = DMA_Bit_Clr;
	pChCtrl_1_ptr->SourInc       = DMA_Bit_Set;
	pChCtrl_1_ptr->DestBusSel    = AHBMaster_1;
	pChCtrl_1_ptr->SourBusSel    = AHBMaster_1;
	pChCtrl_1_ptr->DestWidth     = DMA_WIDTH_8_BIT;
	pChCtrl_1_ptr->SourWidth     = DMA_WIDTH_8_BIT;
	pChCtrl_1_ptr->DestBurstSize = DMA_BURST_8;
	pChCtrl_1_ptr->SourBurstSize = DMA_BURST_8;
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
}

