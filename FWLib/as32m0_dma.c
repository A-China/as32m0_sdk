
#include "as32m0_dma.h"

// DMACCxLLI.LLI =0, If the LLI is 0, then the current LLI is the last in the chain, 
// and the DMA channel is disabled after all DMA transfers associated with it are completed.


uint32_t DMA_GetIntStatus()
{
	return (AHB_DMA->IntStatus & 0xff);	
}

uint32_t DMA_GetIntTCStatus()
{
	return (AHB_DMA->IntTCStatus & 0xff);	
}

void DMA_SetIntTCClear(uint8_t ch)
{
	switch(ch)
	{
		case Ch_0 : AHB_DMA->IntTCClear |= 1<<Ch_0; break;
		case Ch_1 : AHB_DMA->IntTCClear |= 1<<Ch_1; break;
		case Ch_2 : AHB_DMA->IntTCClear |= 1<<Ch_2; break;
		case Ch_3 : AHB_DMA->IntTCClear |= 1<<Ch_3; break;
		case Ch_4 : AHB_DMA->IntTCClear |= 1<<Ch_4; break;
		case Ch_5 : AHB_DMA->IntTCClear |= 1<<Ch_5; break;
		case Ch_6 : AHB_DMA->IntTCClear |= 1<<Ch_6; break;
		case Ch_7 : AHB_DMA->IntTCClear |= 1<<Ch_7; break;
		default   : break;
	}
}

uint32_t DMA_GetIntErrorStatus()
{
	return (AHB_DMA->IntErrorStatus & 0xff);	
}

void DMA_SetIntErrClr(uint8_t ch)
{
	switch(ch)
	{
		case Ch_0 : AHB_DMA->IntErrorClear |= 1<<Ch_0; break;
		case Ch_1 : AHB_DMA->IntErrorClear |= 1<<Ch_1; break;
		case Ch_2 : AHB_DMA->IntErrorClear |= 1<<Ch_2; break;
		case Ch_3 : AHB_DMA->IntErrorClear |= 1<<Ch_3; break;
		case Ch_4 : AHB_DMA->IntErrorClear |= 1<<Ch_4; break;
		case Ch_5 : AHB_DMA->IntErrorClear |= 1<<Ch_5; break;
		case Ch_6 : AHB_DMA->IntErrorClear |= 1<<Ch_6; break;
		case Ch_7 : AHB_DMA->IntErrorClear |= 1<<Ch_7; break;
		default   : break;
	}	
}


uint32_t DMA_GetRawIntTCStatus()
{
	return (AHB_DMA->RawIntTCStatus & 0xff);	
}

uint32_t DMA_GetRawIntErrorStatus()
{
	return (AHB_DMA->RawIntErrorStatus & 0xff);	
}

/**********************  DMA ActiveChannels (RO) ******************/
uint32_t DMA_GetActiveChannels()  //  channel is active
{
	return (AHB_DMA->ActiveChannels & 0xff);
}

//************************* DMA Software Request  ***********************//
//
void DMA_SetSoftBReq(uint32_t id)  // 0~15
{
	AHB_DMA->SoftBReq |= 1 << id;
}

uint32_t DMA_GetSoftBReq()
{
	return (AHB_DMA->SoftBReq & 0xffff);		
}

void DMA_SetSoftSReq(uint32_t id)
{
	AHB_DMA->SoftSReq |= 1 << id;
}

uint32_t DMA_GetSoftSReq()
{
	return (AHB_DMA->SoftSReq & 0xffff);
}

void DMA_SetSoftLBReq(uint32_t id)
{
	AHB_DMA->SoftLBReq |= 1 << id;
}

uint32_t DMA_GetSoftLBReq()
{
	return (AHB_DMA->SoftLBReq & 0xffff);
}

void DMA_SetSoftLSReq(uint32_t id)
{
	AHB_DMA->SoftLBReq |= 1 << id;
}

uint32_t DMA_GetSoftLSReq()
{
	return (AHB_DMA->SoftLBReq & 0xffff);
}

//******************** DMA configuration *********************//
//
void DMA_BigEndian(uint8_t master)
{
	if (master == AHBMaster_1)
		AHB_DMA->Configuration |= 1 << bsDMA_AHB1_ENDIAN;
	else if (master == AHBMaster_2)
		AHB_DMA->Configuration |= 1 << bsDMA_AHB2_ENDIAN;
}

void DMA_LittleEndian(uint8_t master)
{
	if (master == AHBMaster_1)
		AHB_DMA->Configuration &= ~(1 << bsDMA_AHB1_ENDIAN);
	else if (master == AHBMaster_2)
		AHB_DMA->Configuration &= ~(1 << bsDMA_AHB2_ENDIAN);
}

void DMA_Enable()
{
	AHB_DMA->Configuration |= 1 << bsDMA_CTRLR_ENABLED;
}

void DMA_Disable()
{
	AHB_DMA->Configuration &= ~(1 << bsDMA_CTRLR_ENABLED);
}

uint32_t DMA_GetConfiguration()
{
	return (AHB_DMA->Configuration & 0x3);
}

/************** DMA Sync *****************/
void DMA_Enable_Sync(uint32_t id)  // 0~15
{
	AHB_DMA->Sync |= 1 << id;
}

void DMA_Disable_Sync(uint32_t id)
{
	AHB_DMA->Sync &= ~(1 << id);
}

uint32_t DMA_GetSync()
{
	return (AHB_DMA->Sync & 0xffff);
}

//****************** Channel Configuration *****************//
//
uint32_t DMA_GetSourAddr(uint8_t ch)
{
	return AHB_DMA->ChReg[ch].ChSrcAddr;
}

uint32_t DMA_GetDestAddr(uint8_t ch)
{
	return AHB_DMA->ChReg[ch].ChDestAddr;
}

uint32_t DMA_SetChLLI(DMA_sChLLI* pChLLI)
{
	uint32_t data;
	data = (pChLLI->LLIAddr << bsDMA_NEXT_LLI) | (pChLLI->LM << bsDMA_NEXT_LLI_BUS);
	
	return data;
}

void DMA_GetChLLI(DMA_sChLLI* pChLLI, uint8_t ch)
{
	pChLLI->LLIAddr = (AHB_DMA->ChReg[ch].ChLLI >> bsDMA_NEXT_LLI)     & bwDMA_NEXT_LLI;
	pChLLI->LM      = (AHB_DMA->ChReg[ch].ChLLI >> bsDMA_NEXT_LLI_BUS) & bwDMA_NEXT_LLI_BUS;
}


uint32_t DMA_SetChControl(DMA_sChCtrl* pChCtrl)
{
	uint32_t data;
	data = (pChCtrl->TCIrqEn       << bsDMA_TC_INTERRUPT_ENABLE) |
	       (pChCtrl->Prot          << bsDMA_PROTECTION         ) |
	       (pChCtrl->DestInc       << bsDMA_DEST_INCREMENT     ) |
	       (pChCtrl->SourInc       << bsDMA_SRC_INCREMENT      ) |
	       (pChCtrl->DestBusSel    << bsDMA_DEST_BUS           ) |
	       (pChCtrl->SourBusSel    << bsDMA_SRC_BUS            ) |
	       (pChCtrl->DestWidth     << bsDMA_DEST_WIDTH         ) |
	       (pChCtrl->SourWidth     << bsDMA_SRC_WIDTH          ) |
	       (pChCtrl->DestBurstSize << bsDMA_DEST_BURST_SIZE    ) |
	       (pChCtrl->SourBurstSize << bsDMA_SRC_BURST_SIZE     ) |
	       (pChCtrl->TransferSize  << bsDMA_TRANSFER_SIZE      );
	       
	return data;       
}

void DMA_GetChControl(DMA_sChCtrl* pChCtrl, uint8_t ch)
{
	pChCtrl->TCIrqEn        = (AHB_DMA->ChReg[ch].ChControl >> bsDMA_TC_INTERRUPT_ENABLE) & bwDMA_TC_INTERRUPT_ENABLE;
	pChCtrl->Prot           = (AHB_DMA->ChReg[ch].ChControl >> bsDMA_PROTECTION)          & bwDMA_PROTECTION;
	pChCtrl->DestInc        = (AHB_DMA->ChReg[ch].ChControl >> bsDMA_DEST_INCREMENT)      & bwDMA_DEST_INCREMENT;
	pChCtrl->SourInc        = (AHB_DMA->ChReg[ch].ChControl >> bsDMA_SRC_INCREMENT)       & bwDMA_SRC_INCREMENT;
	pChCtrl->DestBusSel     = (AHB_DMA->ChReg[ch].ChControl >> bsDMA_DEST_BUS)            & bwDMA_DEST_BUS;
	pChCtrl->SourBusSel     = (AHB_DMA->ChReg[ch].ChControl >> bsDMA_SRC_BUS)             & bwDMA_SRC_BUS;
	pChCtrl->DestWidth      = (AHB_DMA->ChReg[ch].ChControl >> bsDMA_DEST_WIDTH)          & bwDMA_DEST_WIDTH;
	pChCtrl->SourWidth      = (AHB_DMA->ChReg[ch].ChControl >> bsDMA_SRC_WIDTH)           & bwDMA_SRC_WIDTH;
	pChCtrl->DestBurstSize  = (AHB_DMA->ChReg[ch].ChControl >> bsDMA_DEST_BURST_SIZE)     & bwDMA_DEST_BURST_SIZE;
	pChCtrl->SourBurstSize  = (AHB_DMA->ChReg[ch].ChControl >> bsDMA_SRC_BURST_SIZE)      & bwDMA_SRC_BURST_SIZE;
	pChCtrl->TransferSize   = (AHB_DMA->ChReg[ch].ChControl >> bsDMA_TRANSFER_SIZE)       & bwDMA_TRANSFER_SIZE;
}

uint32_t DMA_SetChConfig(DMA_sChConfig* pChConfig)
{
	uint32_t data;
	data = (pChConfig->Halt              << bsDMA_HALT                ) |
	       //(pChConfig->ActiveChannel   << bsDMA_ACTIVE              ) |  // read only
	       (pChConfig->LockTransfer      << bsDMA_BUS_LOCK            ) |
	       (pChConfig->TCIrqMask         << bsDMA_TC_INTERRUPT_MASK   ) |
	       (pChConfig->ErrorIrqMask      << bsDMA_ERROR_INTERRUPT_MASK) |
	       (pChConfig->FlowCtrl          << bsDMA_FLOW_CONTROL        ) |
	       (pChConfig->DestPeripheral    << bsDMA_DEST_PERIPHERAL     ) |
	       (pChConfig->SourPeripheral    << bsDMA_SRC_PERIPHERAL      ) |
	       (pChConfig->ChannelEnable     << bsDMA_CHANNEL_ENABLED     );
	       
	return data;       
}

void DMA_GetChConfig(DMA_sChConfig* pChConfig, uint8_t ch)
{
	pChConfig->Halt            = (AHB_DMA->ChReg[ch].ChConfiguration >> bsDMA_HALT)                  & bwDMA_HALT;
	pChConfig->ActiveChannel   = (AHB_DMA->ChReg[ch].ChConfiguration >> bsDMA_ACTIVE)                & bwDMA_ACTIVE;
	pChConfig->LockTransfer    = (AHB_DMA->ChReg[ch].ChConfiguration >> bsDMA_BUS_LOCK)              & bwDMA_BUS_LOCK;
	pChConfig->TCIrqMask       = (AHB_DMA->ChReg[ch].ChConfiguration >> bsDMA_TC_INTERRUPT_MASK)     & bwDMA_TC_INTERRUPT_MASK;
	pChConfig->ErrorIrqMask    = (AHB_DMA->ChReg[ch].ChConfiguration >> bsDMA_ERROR_INTERRUPT_MASK)  & bwDMA_ERROR_INTERRUPT_MASK;
	pChConfig->FlowCtrl        = (AHB_DMA->ChReg[ch].ChConfiguration >> bsDMA_FLOW_CONTROL)          & bwDMA_FLOW_CONTROL;
	pChConfig->DestPeripheral  = (AHB_DMA->ChReg[ch].ChConfiguration >> bsDMA_DEST_PERIPHERAL)       & bwDMA_DEST_PERIPHERAL;
	pChConfig->SourPeripheral  = (AHB_DMA->ChReg[ch].ChConfiguration >> bsDMA_SRC_PERIPHERAL)        & bwDMA_SRC_PERIPHERAL;
	pChConfig->ChannelEnable   = (AHB_DMA->ChReg[ch].ChConfiguration >> bsDMA_CHANNEL_ENABLED)       & bwDMA_CHANNEL_ENABLED;
}


// most important
void DMA_SetChannelReg(uint32_t DestAddr, 
                       uint32_t SourADDR,
                       DMA_sChLLI* pChLLI,
                       DMA_sChCtrl* pChCtrl,
                       DMA_sChConfig* pChConfig,
                       uint8_t ch)
{
	AHB_DMA->ChReg[ch].ChSrcAddr       = SourADDR;
	AHB_DMA->ChReg[ch].ChDestAddr      = DestAddr;
	AHB_DMA->ChReg[ch].ChLLI           = DMA_SetChLLI(pChLLI);
	AHB_DMA->ChReg[ch].ChControl       = DMA_SetChControl(pChCtrl);
	AHB_DMA->ChReg[ch].ChConfiguration = DMA_SetChConfig(pChConfig);
}



void DMA_Data_Transfer(uint32_t DestAddr,
                       uint32_t SourADDR,
                       uint32_t TransLength,
                       uint32_t ChLLI_Addr, // LLI address
                       uint32_t Load_master, // 0- AHBMaster_1; 1- AHBMaster_2.
                       DMA_sChCtrl* pChCtrl,
                       DMA_sChConfig* pChConfig,
                       uint8_t ch)
{
	uint32_t i;
	
	uint32_t DMA_Trans_Length;
	uint32_t DMA_Trans_Times;
	uint32_t DMA_The_Last_Trans_Length;
	
	uint32_t Total_Trans_Length;
	
	uint32_t DMA_Ch_Ctrl;
	
	DMA_Trans_Length = pChCtrl->TransferSize;
	Total_Trans_Length = TransLength;
	DMA_Trans_Times = 0;
	DMA_The_Last_Trans_Length = 0;
	DMA_Ch_Ctrl = DMA_SetChControl(pChCtrl);
	
	while( (uint32_t)(Total_Trans_Length/DMA_Trans_Length) > 0 )
	{
		Total_Trans_Length -= DMA_Trans_Length;
		DMA_Trans_Times ++;
	}
	
	// Total_Trans_Length is not the integral multiple of DMA_Trans_Length
	if (Total_Trans_Length > 0)  
	{
		DMA_The_Last_Trans_Length = Total_Trans_Length;
		DMA_Trans_Times ++;
	}
	
	//---------------------------------------------
	// Set LLI
	//
	for (i=0;i<DMA_Trans_Times-1;i++)
	{
	  io_write(ChLLI_Addr+i*16   , SourADDR + DMA_Trans_Length*i);         // DMACCxSrcAddr
	  io_write(ChLLI_Addr+i*16+ 4, DestAddr + DMA_Trans_Length*i);         // DMACCxDestAddr
	  io_write(ChLLI_Addr+i*16+ 8, (ChLLI_Addr +(i+1)*16) | Load_master);  // DMACCxLLI
	  io_write(ChLLI_Addr+i*16+12, DMA_Ch_Ctrl);                           // DMACCxControl
	}
	
	// the last one
  io_write(ChLLI_Addr+(DMA_Trans_Times-1)*16   , SourADDR + DMA_Trans_Length*(DMA_Trans_Times-1)); // DMACCxSrcAddr
	io_write(ChLLI_Addr+(DMA_Trans_Times-1)*16+ 4, DestAddr + DMA_Trans_Length*(DMA_Trans_Times-1)); // DMACCxDestAddr
	io_write(ChLLI_Addr+(DMA_Trans_Times-1)*16+ 8, 0);                                               // DMACCxLLI (the last LLI, set zero.)
	
	if (DMA_The_Last_Trans_Length == 0)
	{
	  io_write(ChLLI_Addr+(DMA_Trans_Times-1)*16+12, DMA_Ch_Ctrl); // DMACCxControl
	}  
	// Total_Trans_Length is not the integral multiple of DMA_Trans_Length  
	else
	{
	  // clear DMA_TRANSFER_SIZE, then set new DMA_TRANSFER_SIZE                                                                     
	  io_write(ChLLI_Addr+(DMA_Trans_Times-1)*16+12, (DMA_Ch_Ctrl & (~bwDMA_TRANSFER_SIZE)) | DMA_The_Last_Trans_Length); // DMACCxControl
	}
	
	//------------------------
	//
	//
	AHB_DMA->ChReg[ch].ChSrcAddr       = *(__IO uint32_t*)(ChLLI_Addr    );
	AHB_DMA->ChReg[ch].ChDestAddr      = *(__IO uint32_t*)(ChLLI_Addr + 4);
	AHB_DMA->ChReg[ch].ChLLI           = *(__IO uint32_t*)(ChLLI_Addr + 8);
	AHB_DMA->ChReg[ch].ChControl       = *(__IO uint32_t*)(ChLLI_Addr +12);
	AHB_DMA->ChReg[ch].ChConfiguration = DMA_SetChConfig(pChConfig);                                   
	
}




