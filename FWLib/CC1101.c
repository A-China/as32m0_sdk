#include "CC1101.h"

uint8_t PaTabel[8] = {0x60 ,0x60 ,0x60 ,0x60 ,0x60 ,0x60 ,0x60 ,0x60};

void CC1101_Reset(void){
	APB_GPIO->GPIO_DO.CLR = SD_CS;
	delay_us(100);
	APB_GPIO->GPIO_DO.SET = SD_CS;
	delay_us(100);
	APB_GPIO->GPIO_DO.CLR = SD_CS;
	delay_us(10);
	ssp_send_byte(CC1101_SRES);
	delay_us(500);
	APB_GPIO->GPIO_DO.SET = SD_CS;
	delay_us(500);
}

void CC1101_SpiWriteReg(uint8_t addr, uint8_t data){
	APB_GPIO->GPIO_DO.CLR = SD_CS;
	delay(10);
	APB_SPI->DataRegister = addr;
  APB_SPI->DataRegister = data;
  apSSP_DeviceEnable(APB_SPI);
  while(apSSP_DeviceBusyGet(APB_SPI));
  apSSP_DeviceDisable(APB_SPI);
	APB_GPIO->GPIO_DO.SET = SD_CS;
  apSSP_ReadFIFO(APB_SPI);	
	apSSP_ReadFIFO(APB_SPI);
}

void CC1101_SpiWriteBurstReg(uint8_t addr, uint8_t *buffer, uint8_t num){
	uint8_t i, temp;

	temp = addr | WRITE_BURST;
	i=0;
	while(apSSP_DeviceBusyGet(APB_SPI));
	do{
		APB_GPIO->GPIO_DO.CLR = SD_CS;
		delay(10);
		APB_SPI->DataRegister = temp;
		while(i < num)
		{
			if(apSSP_TxFifoNotFull(APB_SPI)){
				APB_SPI->DataRegister = buffer[i];
				i++;
			}
			else{
				break;
			}
		}
		apSSP_DeviceEnable(APB_SPI);
		while(apSSP_DeviceBusyGet(APB_SPI));
		apSSP_DeviceDisable(APB_SPI);
		APB_GPIO->GPIO_DO.SET = SD_CS;
		delay_us(5);
	}while(i!=num);
	i=0;
	while(i==num)
		{
      apSSP_ReadFIFO(APB_SPI);
			i++;
    }	
	
}

void CC1101_SpiStrobe(uint8_t strobe){
	APB_GPIO->GPIO_DO.CLR = SD_CS;
	delay(10);
	APB_SPI->DataRegister = strobe;
	apSSP_DeviceEnable(APB_SPI);
	while(apSSP_DeviceBusyGet(APB_SPI));
	apSSP_DeviceDisable(APB_SPI);
	APB_GPIO->GPIO_DO.SET = SD_CS;
	apSSP_ReadFIFO(APB_SPI);
}

void CC1101_RegConfigSettings(uint8_t freq){
    CC1101_SpiWriteReg(CC1101_FSCTRL1,  0x08);
    CC1101_SpiWriteReg(CC1101_FSCTRL0,  0x00);	
    switch(freq)
    {
      case F_868:
      	CC1101_SpiWriteReg(CC1101_FREQ2,    F2_868);
      	CC1101_SpiWriteReg(CC1101_FREQ1,    F1_868);
      	CC1101_SpiWriteReg(CC1101_FREQ0,    F0_868);
        break;
      case F_915:
        CC1101_SpiWriteReg(CC1101_FREQ2,    F2_915);
        CC1101_SpiWriteReg(CC1101_FREQ1,    F1_915);
        CC1101_SpiWriteReg(CC1101_FREQ0,    F0_915);
        break;
	  case F_433:
        CC1101_SpiWriteReg(CC1101_FREQ2,    F2_433);
        CC1101_SpiWriteReg(CC1101_FREQ1,    F1_433);
        CC1101_SpiWriteReg(CC1101_FREQ0,    F0_433);
        break;
	  default: // F must be set
	  	break;
	}	
    CC1101_SpiWriteReg(CC1101_MDMCFG4,  0x5B);
    CC1101_SpiWriteReg(CC1101_MDMCFG3,  0xF8);
    CC1101_SpiWriteReg(CC1101_MDMCFG2,  0x03);
    CC1101_SpiWriteReg(CC1101_MDMCFG1,  0x22);
    CC1101_SpiWriteReg(CC1101_MDMCFG0,  0xF8);
    CC1101_SpiWriteReg(CC1101_CHANNR,   0x00);
    CC1101_SpiWriteReg(CC1101_DEVIATN,  0x47);
    CC1101_SpiWriteReg(CC1101_FREND1,   0xB6);
    CC1101_SpiWriteReg(CC1101_FREND0,   0x10);
    CC1101_SpiWriteReg(CC1101_MCSM0 ,   0x18);
    CC1101_SpiWriteReg(CC1101_FOCCFG,   0x1D);
    CC1101_SpiWriteReg(CC1101_BSCFG,    0x1C);
    CC1101_SpiWriteReg(CC1101_AGCCTRL2, 0xC7);
		CC1101_SpiWriteReg(CC1101_AGCCTRL1, 0x00);
    CC1101_SpiWriteReg(CC1101_AGCCTRL0, 0xB2);
    CC1101_SpiWriteReg(CC1101_FSCAL3,   0xEA);
		CC1101_SpiWriteReg(CC1101_FSCAL2,   0x2A);
		CC1101_SpiWriteReg(CC1101_FSCAL1,   0x00);
    CC1101_SpiWriteReg(CC1101_FSCAL0,   0x11);
    CC1101_SpiWriteReg(CC1101_FSTEST,   0x59);
    CC1101_SpiWriteReg(CC1101_TEST2,    0x81);
    CC1101_SpiWriteReg(CC1101_TEST1,    0x35);
    CC1101_SpiWriteReg(CC1101_TEST0,    0x09);
    CC1101_SpiWriteReg(CC1101_IOCFG2,   0x0B); 	//serial clock.synchronous to the data in synchronous serial mode
    CC1101_SpiWriteReg(CC1101_IOCFG0,   0x06);  	//asserts when sync word has been sent/received, and de-asserts at the end of the packet 
    CC1101_SpiWriteReg(CC1101_PKTCTRL1, 0x04);		//two status bytes will be appended to the payload of the packet,including RSSI LQI and CRC OK
											//No address check
    CC1101_SpiWriteReg(CC1101_PKTCTRL0, 0x05);		//whitening off;CRC Enable˙variable length packets, packet length configured by the first byte after sync word
    CC1101_SpiWriteReg(CC1101_ADDR,     0x00);		//address used for packet filtration.
    CC1101_SpiWriteReg(CC1101_PKTLEN,   0x3D); 	//61 bytes max length	
}

void CC1101_Init(uint8_t f){
	CC1101_Reset();
	CC1101_RegConfigSettings(f);
	CC1101_SpiWriteBurstReg(CC1101_PATABLE,PaTabel,8);
	CC1101_ReadDummyData();
}

void CC1101_SendData(uint8_t *txBuffer, uint8_t size){
	uint8_t i;
	CC1101_SpiWriteReg(CC1101_TXFIFO,size);
	CC1101_SpiWriteBurstReg(CC1101_TXFIFO,txBuffer,size);			// write data to send
	CC1101_SpiStrobe(CC1101_STX);									// start send	
  while ((APB_GPIO->GPIO_DI & GDO0) == 0);							// Wait for GDO0 to be set -> sync transmitted  
  while ((APB_GPIO->GPIO_DI & GDO0) == GDO0);						// Wait for GDO0 to be cleared -> end of packet
	CC1101_SpiStrobe(CC1101_SFTX);									// flush TXfifo	
	printf("Tx Buffer: ");
	for(i=0; i<size; i++){
		printf("%.2x ", txBuffer[i]);
	}
	printf("\n");
}

void CC1101_SetReceive(void){
	CC1101_SpiStrobe(CC1101_SRX);
}

uint8_t CC1101_SpiReadStatus(uint8_t addr){
	uint8_t temp;
	temp = addr | READ_BURST;
	APB_GPIO->GPIO_DO.CLR = SD_CS;
	delay(10);
	APB_SPI->DataRegister = temp;
  APB_SPI->DataRegister = 0x00;
  apSSP_DeviceEnable(APB_SPI);
  while(apSSP_DeviceBusyGet(APB_SPI));
  apSSP_DeviceDisable(APB_SPI);
	APB_GPIO->GPIO_DO.SET = SD_CS;
	apSSP_ReadFIFO(APB_SPI);	
	return apSSP_ReadFIFO(APB_SPI);		
}

uint8_t CC1101_SpiReadReg(uint8_t addr){
	uint8_t temp;
	temp = addr | READ_SINGLE;
	delay(10);
	APB_GPIO->GPIO_DO.CLR = SD_CS;
	APB_SPI->DataRegister = temp;
  APB_SPI->DataRegister = 0x00;
  apSSP_DeviceEnable(APB_SPI);
  while(apSSP_DeviceBusyGet(APB_SPI));
  apSSP_DeviceDisable(APB_SPI);
	APB_GPIO->GPIO_DO.SET = SD_CS;
  apSSP_ReadFIFO(APB_SPI);	
	return apSSP_ReadFIFO(APB_SPI);			
}

void CC1101_SpiReadBurstReg(uint8_t addr, uint8_t *buffer, uint8_t num){
	uint8_t i, temp, j;

	temp = addr | READ_BURST;
 	i=0;
	j=0;
	while(apSSP_DeviceBusyGet(APB_SPI));
	do{
		APB_GPIO->GPIO_DO.CLR = SD_CS;
		delay(10);
		APB_SPI->DataRegister = temp;
		while(i < num)
		{
			if(apSSP_TxFifoNotFull(APB_SPI)){
				APB_SPI->DataRegister = 0x00;
				i++;
			}
			else{
				break;
			}
		}
		apSSP_DeviceEnable(APB_SPI);
		while(apSSP_DeviceBusyGet(APB_SPI));
		apSSP_DeviceDisable(APB_SPI);
		APB_GPIO->GPIO_DO.SET = SD_CS;
		apSSP_ReadFIFO(APB_SPI);
		while(apSSP_RxFifoNotEmpty(APB_SPI)){
			buffer[j] = apSSP_ReadFIFO(APB_SPI);	
			j++;
		}
		delay_us(5);
	}while(i!=num);
	i=0;
}

void CC1101_SpiDummyReadBurstReg(uint8_t addr, uint8_t num){
	uint8_t i, temp;

	temp = addr | READ_BURST;
 	i=0;
	while(apSSP_DeviceBusyGet(APB_SPI));
	do{
		APB_GPIO->GPIO_DO.CLR = SD_CS;
		delay(10);
		APB_SPI->DataRegister = temp;
		while(i < num)
		{
			if(apSSP_TxFifoNotFull(APB_SPI)){
				APB_SPI->DataRegister = 0x00;
				i++;
			}
			else{
				break;
			}
		}
		apSSP_DeviceEnable(APB_SPI);
		while(apSSP_DeviceBusyGet(APB_SPI));
		apSSP_DeviceDisable(APB_SPI);
		APB_GPIO->GPIO_DO.SET = SD_CS;
		apSSP_ReadFIFO(APB_SPI);
		while(apSSP_RxFifoNotEmpty(APB_SPI)){
			apSSP_ReadFIFO(APB_SPI);
		}
		delay_us(5);
	}while(i!=num);
	i=0;
}

uint8_t CC1101_ReceiveData(uint8_t *rxBuffer){
	uint8_t size, status[2], i;
	if(CC1101_SpiReadStatus(CC1101_RXBYTES)){
		size = CC1101_SpiReadReg(CC1101_RXFIFO);
		CC1101_SpiReadBurstReg(CC1101_RXFIFO,rxBuffer,size);
		CC1101_SpiReadBurstReg(CC1101_RXFIFO,status,2);
		CC1101_SpiStrobe(CC1101_SFRX);
		printf("Rx Buffer: ");
		for(i=0; i<size; i++){
			printf("%.2x ", rxBuffer[i]);
		}
		printf("\n");		
		return size;		
	}
}

void CC1101_ReadDummyData(void){
	uint8_t size;
	if(CC1101_SpiReadStatus(CC1101_RXBYTES)){
		CC1101_SpiDummyReadBurstReg(CC1101_RXFIFO,32);
		CC1101_SpiDummyReadBurstReg(CC1101_RXFIFO,2);
		CC1101_SpiStrobe(CC1101_SFRX);
	}

}

uint8_t CC1101_CheckReceiveFlag(void){
	if((APB_GPIO->GPIO_DI & GDO0) == GDO0)			//receive data
	{
		while ((APB_GPIO->GPIO_DI & GDO0) == GDO0);
		return 1;
	}
	else							// no data
	{
		return 0;
	}	
	
	
}



