#include "as32m0_WL1601.h"

void wl1601_init_spi(uint8_t speed){
	PIN_INFO PIN_INFO;
	PIN_INFO.pin_func = GIO_FUNC1;
	PIN_INFO.pin_stat = GIO_PU;
	PIN_INFO.pin_ds		= GIO_DS_2_4;
	PIN_INFO.pin_od   = OD_OFF;
	PIN_INFO.pin_sonof = SONOF_ON;
	PinCtrl_GIOSet(PIN_CTL_GPIOB, GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 , &PIN_INFO); // SPI pin
	WL1601_CSn_SET(1);
	WL1601_CSn_OUT_OE(1);
	apSSP_sDeviceControlBlock pParam; 
	apSSP_sDeviceControlBlock* pParam_ptr = &pParam;  
	apSSP_Initialize(APB_SPI);
	
	/* Set Device Parameters */
 	pParam_ptr->ClockRate        = 0;  // sspclkout = sspclk/(ClockPrescale*(ClockRate+1))
 	pParam_ptr->ClockPrescale    = speed;  //8MHz
 	pParam_ptr->eSCLKPhase       = apSSP_SCLKPHASE_LEADINGEDGE;
 	pParam_ptr->eSCLKPolarity    = apSSP_SCLKPOLARITY_IDLELOW;
  pParam_ptr->eFrameFormat     = apSSP_FRAMEFORMAT_MOTOROLASPI;
  pParam_ptr->eDataSize        = apSSP_DATASIZE_8BITS;
  pParam_ptr->eLoopBackMode    = apSSP_LOOPBACKOFF;
 	pParam_ptr->eMasterSlaveMode = apSSP_MASTER;
  pParam_ptr->eSlaveOutput     = apSSP_SLAVEOUTPUTDISABLED;
  apSSP_DeviceParametersSet(APB_SPI, pParam_ptr); 
}

void wl1601_init_RF(void){
	wl1601_chip_enable();
	wl1601_wr_regs(30, 0x80);
	wl1601_wr_regs(94, 0x00);
	//-----------------------
	wl1601_wr_regs(4, 0xC2);
	wl1601_wr_regs(17, 0x3A);
	wl1601_wr_regs(18, 0x0C);
	wl1601_wr_regs(34, 0x08);
	wl1601_wr_regs(35, 0x08);
	wl1601_wr_regs(45, 0x00);
	wl1601_wr_regs(46, 0x09);
	wl1601_wr_regs(52, 0x19);
	wl1601_wr_regs(53, 0x40);
	wl1601_wr_regs(80, 0xF8);
	wl1601_wr_regs(65, 0x00); // Packet Type
	wl1601_wr_regs(64, 0x78);
	wl1601_wr_regs(81, 0x47);
	// sync_word
	wl1601_wr_regs(72, 0x77);
	wl1601_wr_regs(73, 0x15);
	wl1601_wr_regs(74, 0x99);
	wl1601_wr_regs(75, 0x44);
	wl1601_wr_regs(76, 0x33);
	wl1601_wr_regs(77, 0x11);
	wl1601_wr_regs(78, 0x88);
	wl1601_wr_regs(79, 0x05);
}

void wl1601_init_TX(uint8_t power_gain, uint8_t channel){
		power_gain = power_gain & 0x1F;
		power_gain = power_gain <<4;
		channel = channel & 0x7F;
		wl1601_wr_regs(15, channel);
		wl1601_wr_regs(18, power_gain);
}

void wl1601_init_RX(uint8_t mode, uint8_t channel){
		mode = mode & 0x1;
		mode = mode <<5;
		channel = channel & 0x7F;
		wl1601_wr_regs(15, channel);
		wl1601_wr_regs(70, mode);
}

void wl1601_state_to_direct(void){
		wl1601_wr_regs(1, 0xF9);
		wl1601_wr_regs(65, 0x01);
		wl1601_wr_regs(68, 0xb0);
		wl1601_wr_regs(22, 0x83);
		wl1601_wr_regs(1, 0xF9);
	 // set frequency
		wl1601_wr_regs(14, 0x04);
		wl1601_wr_regs(15, 0x4b);
}

uint8_t wl1601_check_CRC_error(void){
			uint8_t status = 0;
			wl1601_rd_regs(96, &status);
			return status>>7;
}

uint8_t wl1601_TX_send_FIFO_small_packet(uint8_t *data, uint8_t len){
	uint8_t rd_tmp, times, i, j;
	WL1600_SetFlag_Mask(WL1600_Clean_Tx_Fifo);
	wl1601_wr_regs(37, 3);
	if(len >= 64){
			wl1601_wr_regs(33, 0);
			i = 0;
			wl1601_wr_regs(33, 3);
			WL1601_CSn_SET(0);
			APB_SPI->DataRegister = 100;
			apSSP_DeviceEnable(APB_SPI);
			delay_us(1);
			apSSP_ReadFIFO(APB_SPI);
			APB_SPI->DataRegister = len;
			delay_us(1);
			apSSP_ReadFIFO(APB_SPI);		
			for(rd_tmp=0;rd_tmp<63;rd_tmp++){
				while(apSSP_TxFifoNotFull(APB_SPI) == 0);
				//printf("*data[%d]: %x\n",rd_tmp, *data);
				APB_SPI->DataRegister = *data;
				data++;
				apSSP_ReadFIFO(APB_SPI);
			}
			while(apSSP_DeviceBusyGet(APB_SPI));
			WL1601_CSn_SET(1);
			apSSP_DeviceDisable(APB_SPI);
			while(apSSP_RxFifoNotEmpty(APB_SPI) == 1){
				apSSP_ReadFIFO(APB_SPI);
			}
			//while(1);
			WL1600_SetFlag_Mask(WL1600_Tx_Mode);
			j=63;
			delay_us(10);
			while( (WL1600_GetFlagStatus(WL1600_Get_Pkg_Flag) != 1) && j<len){
				if(WL1600_GetFlagStatus(WL1600_Get_Fifo_Flag) == 1){
					APB_GPIO->GPIO_DO.SET = GPIOC_Pin_0;
					wl1601_wr_regs(100, *data);	
					data++;
					j = j +1;
				}
				
			}	
			APB_GPIO->GPIO_DO.CLR = GPIOC_Pin_0;
			printf("TX large packet Done j:%d!!\n", j);
			return 1;
	}
	else{
			wl1601_wr_FIFO(len, data, len);
			WL1600_SetFlag_Mask(WL1600_Tx_Mode);
			delay_us(1);
			while( wl1601_rd_reg_bit(97, 6) == 0);
			WL1600_SetFlag_Mask(WL1600_Clean_Pkg_Flag);
			WL1600_SetFlag_Mask(WL1600_Clean_Tx_Fifo);
			printf("TX small packet Done !!\n");
			return 1;
	}
}

uint8_t wl1601_RX_recv_FIFO_small_packet(uint8_t *data){
	uint8_t len;
	WL1600_SetFlag_Mask(WL1600_Clean_Rx_Fifo);
	WL1600_SetFlag_Mask(WL1600_Rx_Mode);
	while( WL1600_GetFlagStatus(WL1600_Get_Pkg_Flag) != 1);
	WL1600_SetFlag_Mask(WL1600_Clean_Pkg_Flag);
	if(wl1601_check_CRC_error()){
		printf("CRC error\n");
		return 0;
	}	
	else{
		wl1601_rd_FIFO(63, data, 0);
		return 1;
	}
}

uint8_t wl1601_RX_recv_FIFO_large_packet(uint8_t *data){
	uint8_t rd_tmp, len;
	wl1601_wr_regs(33, 0x03);
	WL1600_SetFlag_Mask(WL1600_Clean_Rx_Fifo);
	WL1600_SetFlag_Mask(WL1600_Rx_Mode);
	len = 0;
	while( WL1600_GetFlagStatus(WL1600_Get_Fifo_Flag) != 1);//{
		//printf("Flag : %x\n", wl1601_check_PKT_FIFO_flag());
	//}
	while( WL1600_GetFlagStatus(WL1600_Get_Pkg_Flag) != 1){
	//	if((wl1601_check_PKT_FIFO_flag() & WL1601_FIFO_FLAG) != WL1601_FIFO_FLAG){
			//wl1601_rd_regs(100, data);
			//data++;
			//len++;
			wl1601_rd_FIFO(63, data, 0);
	//	}
	}

	printf("Len %d\n", len);
	return len;
}

void wl1601_chip_enable(void){
	WL1601_CE_OUT_OE(1);
	WL1601_CE_SET(1);
}
void wl1601_chip_disable(void){
	WL1601_CE_OUT_OE(0);
	WL1601_CE_SET(1);
}

void wl1601_wake_up(void){
	WL1601_CSn_OUT_OE(0);
	WL1601_CSn_SET(0);
	delay_ms(5);
	WL1601_CSn_SET(1);
}
void wl1601_set250K(uint8_t tx){
	wl1601_wr_regs(64, 0x68);
	wl1601_wr_regs(106, 0x68);
	wl1601_wr_regs(107, 0xC3);
	wl1601_wr_regs(108, 0x16);
	if(tx ==1){
		wl1601_wr_regs(109, 0x7C);
		wl1601_wr_regs(110, 0x6E);
		wl1601_wr_regs(11, 0xA1);
	}
	else{
		wl1601_wr_regs(109, 0x89);
		wl1601_wr_regs(110, 0xC1);
		wl1601_wr_regs(11, 0x97);	
	}
}

uint8_t wl1601_rd_FIFO_full_thld(void){
	uint8_t t1, t2;
	wl1601_rd_regs(80, &t1);
	wl1601_rd_regs(81, &t2);
	t1 &= 0x07;
  t2 &= 0xC0;
	t2 = t2 >>6;
	t1 = t1 <<2;
	return t1 | t2;
}

uint8_t WL1600_GetFlagStatus(WL1600_GetFlag_Type flag_type){
	uint8_t status = 0xff;
	switch(flag_type)
	{
		case WL1600_Get_Auto_Flag:
			status = wl1601_rd_reg_bit(37,2);
			break;
		case WL1600_Get_Pkg_Flag:
			status = wl1601_rd_reg_bit(97, 6);
			break;
		case WL1600_Fec_Error_Flag:
			status = wl1601_rd_reg_bit(96, 6);
			break;
		case WL1600_Crc_Error_Flag:
			status = wl1601_rd_reg_bit(96, 7);
			break;
		case WL1600_Syncword_Recv:
			status = wl1601_rd_reg_bit(97, 7); //reg97.7位 只读 返回1：表示收到SYNCWORD,只有接收时有效，跳出接收状态时为0
			break;
		case WL1600_Get_Fifo_Flag:
			status = wl1601_rd_reg_bit(97, 5);
			break;
		case WL1600_Ch_No3_No0:
			status = wl1601_rd_reg_nbit(97, 1, 4);
			break;
		case WL1600_Fifo_Wr_Ptr:
			status = wl1601_rd_reg_nbit(104, 0, 5);
			break;
		case WL1600_Fifo_Rr_Ptr:
			status = wl1601_rd_reg_nbit(105, 0, 5);
			break;
		default:
			status = 0xff;
			break; 
	}
	return status;
}

void WL1600_SetFlag_Mask(WL1600_SetMask_Type flag_type){
	switch(flag_type)
	{
		case WL1600_Rx_Mode:
			wl1601_wr_reg_bit(14,0,REG_BIT_CLEAN);
			wl1601_wr_reg_bit(15,7,REG_BIT_SET);
			break;
		case WL1600_Tx_Mode:
			wl1601_wr_reg_bit(15,7,REG_BIT_CLEAN);
			wl1601_wr_reg_bit(14,0,REG_BIT_SET);
			break;
		case WL1600_Idle_Mode://idle
			wl1601_wr_reg_bit(14,0,REG_BIT_CLEAN);
			wl1601_wr_reg_bit(15,7,REG_BIT_CLEAN);
			break;
		case WL1600_Sleep_Mode:
			wl1601_wr_reg_bit(70, 6, REG_BIT_SET);
			break;
		case WL1600_Auto_Ack_Mask_Disable:
			wl1601_wr_reg_bit(33,2,0);
			break;
		case WL1600_Auto_Ack_Mask_Enable:
			wl1601_wr_reg_bit(33,2,1);
			break;
		case WL1600_Clean_Auto_Flag:
			wl1601_wr_reg_bit(37, 2, REG_BIT_SET);
			break;			
		case WL1600_Auto_Ack_Disable:
			wl1601_wr_reg_bit(82, 3, 0);
			break;
		case WL1600_Auto_Ack_Enable:
			wl1601_wr_reg_bit(82, 3, 1);
			break;
		case WL1600_Pkg_Flag_Mask_Disable:
			wl1601_wr_reg_bit(33, 1, 0);
			break;
		case WL1600_Pkg_Flag_Mask_Enable:
			wl1601_wr_reg_bit(33, 1, 1);
			break;
		case WL1600_Clean_Pkg_Flag:
			wl1601_wr_reg_bit(37, 1, REG_BIT_SET);
			break;
		case WL1600_Set_FEC_Type_No:
			wl1601_wr_reg_bit(65, 5, REG_BIT_CLEAN);
			wl1601_wr_reg_bit(65, 4, REG_BIT_CLEAN);
			break;
		case WL1600_Set_FEC_Type_FEC13:
			wl1601_wr_reg_bit(65, 5, REG_BIT_CLEAN);
			wl1601_wr_reg_bit(65, 4, REG_BIT_SET);
			break;
		case WL1600_Set_FEC_Type_FEC23:
			wl1601_wr_reg_bit(65, 5, REG_BIT_SET);
			wl1601_wr_reg_bit(65, 4, REG_BIT_CLEAN);
			break;
		case WL1600_CRC_ON_Disable:
			wl1601_wr_reg_bit(82, 7, 0);
			break;
		case WL1600_CRC_ON_Enable:
			wl1601_wr_reg_bit(82, 7, 1);
			break;
		case WL1600_Scramble_ON_Disable:
			wl1601_wr_reg_bit(82, 6, 0);
			break;
		case WL1600_Scramble_ON_Enable:
			wl1601_wr_reg_bit(82, 6, 1);
			break;
		case WL1600_Pack_Length_Disable:
			wl1601_wr_reg_bit(82, 5, 0);
			break;
		case WL1600_Pack_Length_Enable:
			wl1601_wr_reg_bit(82, 5, 1);
			break;
		case WL1600_Pkt_Hint_Pority_High:
			wl1601_wr_reg_bit(82, 2, 0);
			break;
		case WL1600_Pkt_Hint_Pority_Low:
			wl1601_wr_reg_bit(82, 2, 1);
			break;
		case WL1600_Clean_Rx_Fifo:
			wl1601_wr_reg_bit(105, 7, REG_BIT_SET);
			break;
		case WL1600_Clean_Tx_Fifo:
			wl1601_wr_reg_bit(104, 7, REG_BIT_SET);
			break;
		default:
			break; 
	}
}

uint8_t wl1601_wr_regs(uint8_t addr, uint8_t data){
	uint8_t out=0;
	addr = addr & 0x7F;
	WL1601_CSn_SET(0);
  APB_SPI->DataRegister = addr;
	apSSP_DeviceEnable(APB_SPI);
	delay_us(1);
	APB_SPI->DataRegister = data;
  while(apSSP_DeviceBusyGet(APB_SPI));
  apSSP_DeviceDisable(APB_SPI);
  out = apSSP_ReadFIFO(APB_SPI);
  apSSP_ReadFIFO(APB_SPI);
	WL1601_CSn_SET(1);
	return out; // Return Status
}

uint8_t wl1601_rd_regs(uint8_t addr, uint8_t *data){
	uint8_t out=0;
	addr = addr | 0x80;
	WL1601_CSn_SET(0);
  APB_SPI->DataRegister = addr;
	apSSP_DeviceEnable(APB_SPI);
	delay_us(1);
	APB_SPI->DataRegister = 0x00;
  while(apSSP_DeviceBusyGet(APB_SPI));
  apSSP_DeviceDisable(APB_SPI);
  out = apSSP_ReadFIFO(APB_SPI);
  *data = apSSP_ReadFIFO(APB_SPI);
	WL1601_CSn_SET(1);
	return out; // Return Status
}

uint8_t wl1601_wr_reg_bit(uint8_t addr, uint8_t bit, uint8_t value){
	uint8_t reg_val;
	uint8_t bit_num;
	bit_num = 1<<bit;
	wl1601_rd_regs(addr, &reg_val);
	if(value == REG_BIT_SET)
		reg_val |= bit_num;
	else
		reg_val &= ~bit_num;
	return wl1601_wr_regs(addr,reg_val);
}

uint8_t wl1601_rd_reg_bit(uint8_t addr, uint8_t bit){
	uint8_t reg_val;
	uint8_t bit_num;
	bit_num = 1<<bit;
	wl1601_rd_regs(addr, &reg_val);
	if((reg_val&bit_num) == 0x0)
		return REG_BIT_CLEAN;
	else
		return REG_BIT_SET;
}

uint8_t wl1601_rd_reg_nbit(uint8_t addr, uint8_t begin_bit, uint8_t end_bit){
	uint8_t reg_val;
	uint8_t bit_num = 0;
	uint8_t i = 0;
	for(i=begin_bit;i<=end_bit;i++)
	{
		bit_num = bit_num + (0x1<<i);
	}
	wl1601_rd_regs(addr, &reg_val);
	
	return (bit_num&reg_val)>>begin_bit;	
}

void wl1601_wr_FIFO(uint8_t len, uint8_t *data, uint8_t data_size){
			uint8_t i;
			WL1601_CSn_SET(0);
			APB_SPI->DataRegister = 100;
			apSSP_DeviceEnable(APB_SPI);
			delay_us(1);
			apSSP_ReadFIFO(APB_SPI);
			APB_SPI->DataRegister = len;
			delay_us(1);
			apSSP_ReadFIFO(APB_SPI);
			for(i=0;i<data_size;i++){
				while(apSSP_TxFifoNotFull(APB_SPI) == 0);
				//printf("*data[%d]: %x\n",rd_tmp, *data);
				APB_SPI->DataRegister = *data;
				data++;
				apSSP_ReadFIFO(APB_SPI);
			}
			while(apSSP_DeviceBusyGet(APB_SPI));
			WL1601_CSn_SET(1);
			apSSP_DeviceDisable(APB_SPI);
			while(apSSP_RxFifoNotEmpty(APB_SPI) == 1){
				apSSP_ReadFIFO(APB_SPI);
			}
}

void wl1601_rd_FIFO(uint8_t len, uint8_t *data, uint8_t enable_len){
		uint8_t r_len;
		uint8_t i;
		WL1601_CSn_SET(0);
		APB_SPI->DataRegister = 100 | 0x80; //Read
		apSSP_DeviceEnable(APB_SPI);
		while(apSSP_DeviceBusyGet(APB_SPI));
		apSSP_ReadFIFO(APB_SPI);
		if(enable_len == 1){
			r_len = len;
		
		}
		else{
			APB_SPI->DataRegister = 0x00;
			while(apSSP_DeviceBusyGet(APB_SPI));
			*data = apSSP_ReadFIFO(APB_SPI);
			r_len  = *data;
			data++;
		}
		for(i=0;i<r_len;i++){
			while(apSSP_TxFifoNotFull(APB_SPI) == 0);
			APB_SPI->DataRegister = 0x0;	
			while(apSSP_DeviceBusyGet(APB_SPI));
			*data = apSSP_ReadFIFO(APB_SPI);
			//printf("*data : %x\n", *data);
			data++;
		}
		while(apSSP_DeviceBusyGet(APB_SPI));
		while(apSSP_RxFifoNotEmpty(APB_SPI) == 1){
			*data = apSSP_ReadFIFO(APB_SPI);
			data++;
		}
		apSSP_DeviceDisable(APB_SPI);
		WL1601_CSn_SET(1);

}

