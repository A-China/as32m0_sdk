#include "test_SD_card.h"
	DMA_sChLLI     pChLLI_sdRx;
	DMA_sChCtrl    pChCtrl_sdRx;
	DMA_sChConfig  pChConfig_sdRx;
		
	DMA_sChLLI*     pChLLI_sdRx_ptr = &pChLLI_sdRx;
	DMA_sChCtrl*    pChCtrl_sdRx_ptr = &pChCtrl_sdRx;
	DMA_sChConfig*  pChConfig_sdRx_ptr = &pChConfig_sdRx;	
	
	DMA_sChLLI     pChLLI_sdTx;
	DMA_sChCtrl    pChCtrl_sdTx;
	DMA_sChConfig  pChConfig_sdTx;
		
	DMA_sChLLI*     pChLLI_sdTx_ptr = &pChLLI_sdTx;
	DMA_sChCtrl*    pChCtrl_sdTx_ptr = &pChCtrl_sdTx;
	DMA_sChConfig*  pChConfig_sdTx_ptr = &pChConfig_sdTx;	
	
	DMA_sChLLI     pChLLI_sdTx2;
	DMA_sChCtrl    pChCtrl_sdTx2;
	DMA_sChConfig  pChConfig_sdTx2;
		
	DMA_sChLLI*     pChLLI_sdTx2_ptr = &pChLLI_sdTx2;
	DMA_sChCtrl*    pChCtrl_sdTx2_ptr = &pChCtrl_sdTx2;
	DMA_sChConfig*  pChConfig_sdTx2_ptr = &pChConfig_sdTx2;	
	
void delay(uint32_t count){
	int i;
	for(i=0;i<count;i++);
}

void init_SD_card_PowerUp(){
	uint8_t temp, i;
	APB_GPIO->GPIO_OE.SET = SD_CS;
	APB_GPIO->GPIO_DO.SET = SD_CS;
	delay(1<<18);
	for(i=0;i<40;i++){
		SD_spi_send_byte(0xFF);
	}
  	SD_spi_command(0, 0x00000000, 0);
	SD_spi_command(8, 0x000001aa, 7);
	SD_spi_command(58, 0x000001aa, 3);
	i=0;	
	do{
		SD_spi_command(55, 0x00000000, 0);//if the response is 0x05 = old card =>CMD1
		temp=SD_spi_command(41, 0x40000000, 0);//	
		i++;
	}while(temp!=0x00);
	SD_spi_command(58, 0x000001aa, 3);

}

void init_SD_card_spi_dma(void){
	uint8_t i;
	DMA_LittleEndian(AHBMaster_1);	
	// DMA Sync
	for (i=0;i<16;i++)
           DMA_Enable_Sync(i);
	// Channel Rx Configuration : PERIPHERAL_TO_MEM_DMA_CTRL
	pChLLI_sdRx_ptr->LLIAddr = 0;
	pChLLI_sdRx_ptr->LM      = AHBMaster_1;
	
	pChCtrl_sdRx_ptr->TCIrqEn       = DMA_Bit_Clr;
	pChCtrl_sdRx_ptr->Prot          = DMA_PROT_USER | DMA_PROT_NON_BUFFERABLE | DMA_PROT_NON_CACHEABLE;
	pChCtrl_sdRx_ptr->DestInc       = DMA_Bit_Set;
	pChCtrl_sdRx_ptr->SourInc       = DMA_Bit_Clr;
	pChCtrl_sdRx_ptr->DestBusSel    = AHBMaster_1;
	pChCtrl_sdRx_ptr->SourBusSel    = AHBMaster_1;
	pChCtrl_sdRx_ptr->DestWidth     = DMA_WIDTH_8_BIT;
	pChCtrl_sdRx_ptr->SourWidth     = DMA_WIDTH_8_BIT;
	pChCtrl_sdRx_ptr->DestBurstSize = DMA_BURST_4;
	pChCtrl_sdRx_ptr->SourBurstSize = DMA_BURST_4;
	pChCtrl_sdRx_ptr->TransferSize  = 512;
	
	pChConfig_sdRx_ptr->Halt           = DMA_Bit_Clr;
	//ChConfig.ActiveChannel 
	pChConfig_sdRx_ptr->LockTransfer   = DMA_Bit_Clr;
	pChConfig_sdRx_ptr->TCIrqMask      = DMA_Bit_Set;
	pChConfig_sdRx_ptr->ErrorIrqMask   = DMA_Bit_Set;
	pChConfig_sdRx_ptr->FlowCtrl       = DMA_PERIPHERAL_TO_MEM_DMA_CTRL;
	pChConfig_sdRx_ptr->DestPeripheral = 0;
	pChConfig_sdRx_ptr->SourPeripheral = 0;  // use
	pChConfig_sdRx_ptr->ChannelEnable  = DMA_Bit_Set;
	
	// Channel Tx Configuration : MEM_TO_PERIPHERAL_DMA_CTRL
	pChLLI_sdTx_ptr->LLIAddr = 0;
	pChLLI_sdTx_ptr->LM      = AHBMaster_1;
	
	pChCtrl_sdTx_ptr->TCIrqEn       = DMA_Bit_Clr;
	pChCtrl_sdTx_ptr->Prot          = DMA_PROT_USER | DMA_PROT_NON_BUFFERABLE | DMA_PROT_NON_CACHEABLE;
	pChCtrl_sdTx_ptr->DestInc       = DMA_Bit_Clr;
	pChCtrl_sdTx_ptr->SourInc       = DMA_Bit_Clr;
	pChCtrl_sdTx_ptr->DestBusSel    = AHBMaster_1;
	pChCtrl_sdTx_ptr->SourBusSel    = AHBMaster_1;
	pChCtrl_sdTx_ptr->DestWidth     = DMA_WIDTH_8_BIT;
	pChCtrl_sdTx_ptr->SourWidth     = DMA_WIDTH_8_BIT;
	pChCtrl_sdTx_ptr->DestBurstSize = DMA_BURST_4;
	pChCtrl_sdTx_ptr->SourBurstSize = DMA_BURST_4;
	pChCtrl_sdTx_ptr->TransferSize  = 512;
	
	pChConfig_sdTx_ptr->Halt           = DMA_Bit_Clr;
	//ChConfig->ActiveChannel 
	pChConfig_sdTx_ptr->LockTransfer   = DMA_Bit_Clr;
	pChConfig_sdTx_ptr->TCIrqMask      = DMA_Bit_Set;
	pChConfig_sdTx_ptr->ErrorIrqMask   = DMA_Bit_Set;
	pChConfig_sdTx_ptr->FlowCtrl       = DMA_MEM_TO_PERIPHERAL_DMA_CTRL;
	pChConfig_sdTx_ptr->DestPeripheral = 1;  // use
	pChConfig_sdTx_ptr->SourPeripheral = 0;
	pChConfig_sdTx_ptr->ChannelEnable  = DMA_Bit_Set;
	
	pChLLI_sdTx2_ptr->LLIAddr = 0;
	pChLLI_sdTx2_ptr->LM      = AHBMaster_1;
	
	pChCtrl_sdTx2_ptr->TCIrqEn       = DMA_Bit_Clr;
	pChCtrl_sdTx2_ptr->Prot          = DMA_PROT_USER | DMA_PROT_NON_BUFFERABLE | DMA_PROT_NON_CACHEABLE;
	pChCtrl_sdTx2_ptr->DestInc       = DMA_Bit_Clr;
	pChCtrl_sdTx2_ptr->SourInc       = DMA_Bit_Set;
	pChCtrl_sdTx2_ptr->DestBusSel    = AHBMaster_1;
	pChCtrl_sdTx2_ptr->SourBusSel    = AHBMaster_1;
	pChCtrl_sdTx2_ptr->DestWidth     = DMA_WIDTH_8_BIT;
	pChCtrl_sdTx2_ptr->SourWidth     = DMA_WIDTH_8_BIT;
	pChCtrl_sdTx2_ptr->DestBurstSize = DMA_BURST_4;
	pChCtrl_sdTx2_ptr->SourBurstSize = DMA_BURST_4;
	pChCtrl_sdTx2_ptr->TransferSize  = 512;
	
	pChConfig_sdTx2_ptr->Halt           = DMA_Bit_Clr;
	//ChConfig->ActiveChannel 
	pChConfig_sdTx2_ptr->LockTransfer   = DMA_Bit_Clr;
	pChConfig_sdTx2_ptr->TCIrqMask      = DMA_Bit_Set;
	pChConfig_sdTx2_ptr->ErrorIrqMask   = DMA_Bit_Set;
	pChConfig_sdTx2_ptr->FlowCtrl       = DMA_MEM_TO_PERIPHERAL_DMA_CTRL;
	pChConfig_sdTx2_ptr->DestPeripheral = 1;  // use
	pChConfig_sdTx2_ptr->SourPeripheral = 0;
	pChConfig_sdTx2_ptr->ChannelEnable  = DMA_Bit_Set;
	
};

void SD_printf_datamap(uint32_t len, uint32_t sd_addr, uint8_t *data){
	uint8_t i, j;
	printf("\n");
	sd_addr = sd_addr<<9;
	for(i=0;i<len/16;i++){
		printf("%.8xh ", sd_addr);
			for(j=0;j<16;j++, data++)
				printf("%.2x ", *data);
		printf("\n");
		sd_addr = sd_addr + 0x10;
	}
	printf("\n");
}

void SD_spi_read_CSD(){
		uint32_t i;
		uint8_t  temp, temp2[20];
		SD_spi_read_command(9, 0, 0);
	for(i=0;i<16;i++){
		if(i==0){
			do{
			  temp=SD_spi_getdata();
			}while(temp == 0xFF);
			temp2[0] = temp;
		}
		else
			temp2[i]=SD_spi_getdata();
	}	
	printf("Data = ");
	for(i=0;i<20;i++){
		printf("0x%.2x ", temp2[i]);
	}	
	printf("\n");
}
void SD_spi_set_block_len(uint32_t len){
    SD_spi_command(16, len, 0);
}

uint8_t SD_spi_read_single_blk(uint32_t len, uint32_t sd_addr, uint8_t *data){// SDHC Card only 512 block length
	uint8_t  temp;
	uint16_t temp16;
	APB_GPIO->GPIO_DO.CLR = SD_CS;
	SD_spi_read_command(17, sd_addr, 0);
	do{
		temp=SD_spi_getdata();
	}while(temp == 0xFF);
	if(temp == 0xFE){
	#ifdef SIM	
		printf("Data Token 0xFE\n");
	#endif
	}	
 else{
	 #ifdef SIM	
		printf("Data Token NG\n");
	 #endif
	  return 0;
 }
 
 /*
	for(i=0;i<len;i++){
			*data=SD_spi_getdata();
			data++;
	}
 */

  	apSSP_DeviceEnable(APB_SPI); 
  	apSSP_DeviceReceiveClear(APB_SPI); // clear high Z data 
  	// NOTE : SSP dma must open at data phase.
		apSSP_DMAModeSet(APB_SPI, apSSP_DMA_TX_ON);
		apSSP_DMAModeSet(APB_SPI, apSSP_DMA_RX_ON);
		DMA_Enable();
		DMA_SetChannelReg((uint32_t)(data), apSSP_DMAAddressGet(APB_SPI_BASE), pChLLI_sdRx_ptr, pChCtrl_sdRx_ptr, pChConfig_sdRx_ptr, Ch_0);
		DMA_SetChannelReg(apSSP_DMAAddressGet(APB_SPI_BASE), (uint32_t)(temp),pChLLI_sdTx_ptr, pChCtrl_sdTx_ptr, pChConfig_sdTx_ptr, Ch_1);
		
  	// wait dma op completion
 	 while (DMA_GetActiveChannels());
  	// close ssp and spi flash
  	apSSP_DeviceDisable(APB_SPI);
		apSSP_DMAModeSet(APB_SPI, apSSP_DMA_TX_OFF);
		apSSP_DMAModeSet(APB_SPI, apSSP_DMA_RX_OFF);
		DMA_Disable();
		
	temp16 = (SD_spi_getdata())<<8;
	temp16 |= SD_spi_getdata();
	#ifdef SIM	
		printf("CRC = 0x%.4x\n", temp16);
		while(apUART_Check_BUSY(APB_UART0) == 1);
	#endif
	APB_GPIO->GPIO_DO.SET = SD_CS;
	SD_spi_getdata();
	SD_spi_getdata();
	//data = tmp_ptr;
	//crc16 = crc16_mmc(data, 512);
	while(apUART_Check_BUSY(APB_UART0) == 1);
/*
	if(crc16 == temp16){
	 #ifdef SIM		
		printf("CRC Read Correct\n");
	 #endif
		return 2;
	}		
	else{
	 #ifdef SIM			 
		printf("CRC Read Wrong\n");
	 #endif
		return 1;
	}	
*/
 return 2;
}
uint8_t SD_spi_write_single_blk(uint32_t len, uint32_t sd_addr, uint8_t *data){
	uint8_t  temp, temp2;
//	uint16_t crc16;
	APB_GPIO->GPIO_DO.CLR = SD_CS;
//	crc16 = crc16_mmc(data, 512);
//	crc16 = 0x0000;
//	printf("Write CRC = 0x%.4x \n", crc16);
	while(apUART_Check_BUSY(APB_UART0) == 1);
	SD_spi_read_command(24, sd_addr, 0);
//	printf("Write =  ");
	SD_spi_send_byte(0xFE);
/*	
	for(i=0;i<len;i++){
		SD_spi_send_byte(*data++);
	}
*/
  apSSP_DeviceEnable(APB_SPI); 
  apSSP_DeviceReceiveClear(APB_SPI); // clear high Z data 
  // NOTE : SSP dma must open at data phase.
	apSSP_DMAModeSet(APB_SPI, apSSP_DMA_TX_ON);
	DMA_Enable();
	DMA_SetChannelReg(apSSP_DMAAddressGet(APB_SPI_BASE), (uint32_t)(data),pChLLI_sdTx2_ptr, pChCtrl_sdTx2_ptr, pChConfig_sdTx2_ptr, Ch_1);
  	// wait dma op completion
 	while (DMA_GetActiveChannels());
  	// close ssp and spi flash
  apSSP_DeviceDisable(APB_SPI);
	apSSP_DMAModeSet(APB_SPI, apSSP_DMA_TX_OFF);
	DMA_Disable();
/*
	SD_spi_send_byte((uint8_t)((crc16 & 0xFF00)>>8));
	SD_spi_send_byte((uint8_t)(crc16 & 0x00FF));	
*/
	SD_spi_send_byte(0x00);
	SD_spi_send_byte(0x00);
	SD_spi_send_byte(0xFF);
//	printf("\n");
	do{
	  temp2 = SD_spi_getdata();
	}while(temp2==0xFF);
	temp2 = temp2 & 0x1F;
	while(apUART_Check_BUSY(APB_UART0) == 1);
//	printf("CRC = 0x%.4x \n", crc16_mmc(temp2, 8));
	do{
	  temp = SD_spi_getdata();
	}while(temp!=0xFF);
  APB_GPIO->GPIO_DO.SET = SD_CS;
	SD_spi_getdata();
	SD_spi_getdata();	
	return temp2;
}

uint64_t SD_spi_command(uint8_t cmd, uint32_t arg, uint8_t resp_type){
	uint8_t tmp[6], crc7, i;
	uint8_t temp[5] = {0x00};
	uint64_t temp64 = 0;
	APB_GPIO->GPIO_DO.CLR = SD_CS;
	delay(10);
	tmp[0] = (cmd & 0x3F) | 0x40;
	tmp[1] =  (uint8_t) ((arg>>24) & 0xFF);
	tmp[2] =  (uint8_t) ((arg>>16) & 0xFF);
	tmp[3] =  (uint8_t) ((arg>>8) & 0xFF);
	tmp[4] =  (uint8_t) (arg & 0xFF);
	crc7 = crc7_mmc(tmp, 5);
	tmp[5] =  (uint8_t) ((crc7<<1) | 0x01);
	#ifdef SIM
		printf("\nCMD%.2d Argument=0x%.8x crc7=0x%.2x\n", cmd, arg, crc7);
		printf("Send: 0x%.2x 0x%.2x 0x%.2x 0x%.2x 0x%.2x 0x%.2x\n", tmp[0], tmp[1], tmp[2], tmp[3], tmp[4], tmp[5]);
		while(apUART_Check_BUSY(APB_UART0) == 1);
	#endif
	for(i=0; i<6;i++){
		APB_SPI->DataRegister = tmp[i];
	}
  apSSP_DeviceEnable(APB_SPI);
  while(apSSP_DeviceBusyGet(APB_SPI));
  apSSP_DeviceDisable(APB_SPI);
	apSSP_DeviceReceiveClear(APB_SPI);
	i = 0;
	do{
		temp[0] = SD_spi_getdata();
		i++;
		tmp[0] = temp[0];
	}while((temp[0]&0x80) == 0x80);
	switch(resp_type){
		case 0:
			SD_spi_getdata();
			#ifdef SIM
				printf("R1 Response : 0x%.10x \n", temp[0]);
			#endif
			temp64 = temp[0];
			break;
		case 1:
			break;
		case 2:
			temp[1] = SD_spi_getdata();
		  #ifdef SIM
				printf("R2 Response : 0x%.8x%.2x \n", temp[0], temp[1]);
			#endif
		  temp64 = temp[0]<<8 | temp[1];
			break;
		case 3:
			for(i=1;i<5;i++)
			 temp[i] = SD_spi_getdata();
			#ifdef SIM
				printf("R3 Response : 0x%.2x%.2x%.2x%.2x%.2x \n", temp[0], temp[1], temp[2], temp[3], temp[4]);
			#endif
			for(i=0; i<5;i++)
				temp64 |= (temp[i]<<(8*i)); 
			break;
		case 7:
			for(i=1;i<5;i++)
			 temp[i] = SD_spi_getdata();
		 #ifdef SIM
			printf("R7 Response : 0x%.2x%.2x%.2x%.2x%.2x \n", temp[0], temp[1], temp[2], temp[3], temp[4]);
		 #endif
			for(i=0; i<5;i++)
				temp64 |= (temp[i]<<(8*i)); 
			break;
		default:
			break;
	}
	APB_GPIO->GPIO_DO.SET = SD_CS;
	while(apUART_Check_BUSY(APB_UART0) == 1);
		SD_spi_getdata();
	  SD_spi_getdata();
   return temp64;	
};

uint32_t SD_spi_read_command(uint8_t cmd, uint32_t arg, uint8_t resp_type){
	uint8_t tmp[6], crc7, i;
	uint8_t temp[5] = {0x00};
	uint64_t temp64 = 0;
	APB_GPIO->GPIO_DO.CLR = SD_CS;
	delay(10);
	tmp[0] = (cmd & 0x3F) | 0x40;
	tmp[1] =  (uint8_t) ((arg>>24) & 0xFF);
	tmp[2] =  (uint8_t) ((arg>>16) & 0xFF);
	tmp[3] =  (uint8_t) ((arg>>8) & 0xFF);
	tmp[4] =  (uint8_t) (arg & 0xFF);
	crc7 = crc7_mmc(tmp, 5);
	tmp[5] =  (uint8_t) ((crc7<<1) | 0x01);
	#ifdef SIM
		printf("\nCMD%.2d Argument=0x%.8x crc7=0x%.2x\n", cmd, arg, crc7);
		printf("Send: 0x%.2x 0x%.2x 0x%.2x 0x%.2x 0x%.2x 0x%.2x\n", tmp[0], tmp[1], tmp[2], tmp[3], tmp[4], tmp[5]);
	#endif
	while(apUART_Check_BUSY(APB_UART0) == 1);
//	for(i=0; i<6;i++)
//		SD_spi_send_byte(tmp[i]);
		for(i=0; i<6;i++){
			APB_SPI->DataRegister = tmp[i];
		}
    apSSP_DeviceEnable(APB_SPI);
    while(apSSP_DeviceBusyGet(APB_SPI));
    apSSP_DeviceDisable(APB_SPI);
		apSSP_DeviceReceiveClear(APB_SPI);
	
	i = 0;
	do{
		temp[0] = SD_spi_getdata();
		i++;
		tmp[0] = temp[0];
	}while((temp[0]&0x80) == 0x80);
	switch(resp_type){
		case 0:
			SD_spi_getdata();
			#ifdef SIM
				printf("R1 Response : 0x%.10x \n", temp[0]);
			#endif
			temp64 = temp[0];
			break;
		case 1:
			break;
		case 2:
			temp[1] = SD_spi_getdata();
		  #ifdef SIM
				printf("R2 Response : 0x%.8x%.2x \n", temp[0], temp[1]);
			#endif
		  temp64 = temp[0]<<8 | temp[1];
			break;
		case 3:
			for(i=1;i<5;i++)
			 temp[i] = SD_spi_getdata();
			#ifdef SIM
				printf("R3 Response : 0x%.2x%.2x%.2x%.2x%.2x \n", temp[0], temp[1], temp[2], temp[3], temp[4]);
			#endif
			for(i=0; i<5;i++)
				temp64 |= (temp[i]<<(8*i)); 
			break;
		case 7:
			for(i=1;i<5;i++)
			 temp[i] = SD_spi_getdata();
		 #ifdef SIM
			printf("R7 Response : 0x%.2x%.2x%.2x%.2x%.2x \n", temp[0], temp[1], temp[2], temp[3], temp[4]);
		 #endif
			for(i=0; i<5;i++)
				temp64 |= (temp[i]<<(8*i)); 
			break;
		default:
			break;
	}
//	APB_GPIO->GPIO_DO.SET = SD_CS;
	while(apUART_Check_BUSY(APB_UART0) == 1);
  return temp64;	
};

void SD_spi_send_byte(uint8_t data){
    APB_SPI->DataRegister = data;
    apSSP_DeviceEnable(APB_SPI);
    while(apSSP_DeviceBusyGet(APB_SPI));
    apSSP_DeviceDisable(APB_SPI);
    apSSP_ReadFIFO(APB_SPI);
}

uint8_t SD_spi_getdata(void){
    APB_SPI->DataRegister = 0xFF;
    apSSP_DeviceEnable(APB_SPI);
    while(apSSP_DeviceBusyGet(APB_SPI));
    apSSP_DeviceDisable(APB_SPI);
    return apSSP_ReadFIFO(APB_SPI);
}

uint8_t crc7_mmc(uint8_t *data, uint8_t byte_length)
{
    uint8_t i;
    uint8_t crc = 0;        // Initial value
    while(byte_length--)
    {
        crc ^= *data++;        // crc ^= *data; data++;
        for ( i = 0; i < 8; i++ )
        {
            if ( crc & 0x80 )
                crc = (crc << 1) ^ 0x12;        // 0x12 = 0x09<<(8-7)
            else
                crc <<= 1;
        }
    }
    return crc >> 1;
}

uint16_t crc16_mmc(uint8_t *data, uint16_t byte_length)
{
    uint16_t i;
    uint16_t crc = 0;        // Initial value
    while(byte_length--)
    {
        crc ^= *data++<<8;        // crc ^= *data; data++;
        for ( i = 0; i < 8; i++ )
        {
            if ( crc & 0x8000 )
                crc = (crc << 1) ^ 0x1021;        // 0x12 = 0x09<<(8-7)
            else
                crc <<= 1;
        }
    }
    return crc;
}

