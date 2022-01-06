

#include "as32m0_i2c.h"
#define I2C_OSC_CLK  OSC_CLK_FREQ/1000000
#define TX_FIFO_DEPTH 8
#define RX_FIFO_DEPTH 8

void I2C_DISABLE(I2C_TypeDef *I2C,uint32_t data)
{
	I2C->IC_ENABLE &= ~(1<<data);
}
void I2C_ENABLE(I2C_TypeDef *I2C,uint32_t data)
{
	I2C->IC_ENABLE |= (1<<data);
}
void I2C_TargerAdd(I2C_TypeDef *I2C,uint32_t data)
{
	I2C->IC_TAR = data;
}
void I2C_SlaveAdd(I2C_TypeDef *I2C,uint32_t data)
{
	I2C->IC_SAR = data;
}
void I2C_SCL_HCNT(I2C_TypeDef *I2C,uint8_t speed,uint8_t fastmodeplus)
{
  uint32_t MIN_SCL_HighTime=0;
  float temp;
  if(speed == 0x01 )
  {
     MIN_SCL_HighTime=4000;//ns  //@ 4000H+4700L=8700ns => 115K
     temp=I2C_OSC_CLK*MIN_SCL_HighTime*0.001;
     I2C->IC_SS_SCL_HCNT=(uint32_t)temp;
  }
  else if(speed == 0x02)
  {
    if(fastmodeplus == 0x01)
     MIN_SCL_HighTime=260;//ns  @ 1.3M
    else
     MIN_SCL_HighTime=600;//ns //@ 526K
      
     temp=I2C_OSC_CLK*MIN_SCL_HighTime*0.001;
     I2C->IC_FS_SCL_HCNT=(uint32_t)temp;
  }
  else if(speed == 0x03)
  {
     MIN_SCL_HighTime=120;//ns
     temp=I2C_OSC_CLK*MIN_SCL_HighTime*0.001;
  //   I2C->IC_HS_SCL_HCNT=(uint32_t)temp;
     I2C->IC_FS_SCL_HCNT=0;
     I2C->IC_HS_SCL_HCNT=0;
     I2C->IC_HS_SPKLEN = 2;
     I2C->IC_FS_SPKLEN = 2;
  }
  else
    printf("ERROR:I2C_SCL_HCNT is not exist !");

}

void I2C_SCL_LCNT(I2C_TypeDef *I2C,uint8_t speed,uint8_t fastmodeplus)
{
  uint32_t MIN_SCL_LowTime=0;
  float temp;
  if(speed == 0x01 )
  {   
     MIN_SCL_LowTime=4700;//ns
     temp=I2C_OSC_CLK*MIN_SCL_LowTime*0.001;
     I2C->IC_SS_SCL_LCNT=(uint32_t)temp;
  }
  else if(speed == 0x02)
  { 
    if(fastmodeplus == 0x01)
     MIN_SCL_LowTime=500;//ns
    else
      MIN_SCL_LowTime=1300;//ns
     temp=I2C_OSC_CLK*MIN_SCL_LowTime*0.001;
     I2C->IC_FS_SCL_LCNT=(uint32_t)temp;
  }
  else if(speed == 0x03)
  {
     MIN_SCL_LowTime=320;//ns
     temp=I2C_OSC_CLK*MIN_SCL_LowTime*0.001;
    // I2C->IC_HS_SCL_LCNT=(uint32_t)temp;	
     I2C->IC_FS_SCL_LCNT=0;
     I2C->IC_HS_SCL_LCNT=0;
  }
  else
   printf("ERROR:I2C_SCL_LCNT is not exist !");
}
void I2C_TransFifoSet(I2C_TypeDef *I2C,uint8_t data)
{
   if(data < (TX_FIFO_DEPTH+1))
    I2C->IC_TX_TL=data;
   else
      printf("ERROR:The IC_TX_TL must be less than %x \n ",TX_FIFO_DEPTH);	
}
void I2C_ReceFifoSet(I2C_TypeDef *I2C,uint8_t data)
{
   if(data < (RX_FIFO_DEPTH+1))
    I2C->IC_RX_TL=data;
   else
      printf("ERROR:The IC_TX_TL must be less than %x \n ",TX_FIFO_DEPTH);	
}

void I2C_MasterConfig(I2C_TypeDef *I2C,
                        uint8_t TarAddress,
                        uint8_t IcSlaveDis,
                        uint8_t Speed,
                        uint8_t MasterMode,
                        uint8_t tx_fifo,
                        uint8_t rx_fifo,
                        uint8_t fastmodeplus
			)
{
     I2C_DISABLE(I2C,bwI2C_ENAble); 	// disable I2C
     I2C->IC_CON     &= ~(0xffff<< 0);//clear bit[15:0]
		 // master mode, disabled slave mode, speed=standard/fast, tx fifo, rx fifo, disabled fast plus
     I2C->IC_CON      = (IcSlaveDis<<6)|(Speed<<1)|(MasterMode<<0)|(1<<5)|(1<<8);
     APB_I2C->IC_INTR_MASK = 0x00;
     I2C_TargerAdd(I2C,TarAddress);
     I2C_SCL_LCNT(I2C,Speed,fastmodeplus);
     I2C_SCL_HCNT(I2C,Speed,fastmodeplus);
     I2C_TransFifoSet(I2C,tx_fifo);
     I2C_ReceFifoSet(I2C,tx_fifo);
     I2C_ENABLE(I2C,bwI2C_ENAble);
}
void I2C_SlaveConfig(I2C_TypeDef *I2C,
                        uint8_t SlaAddress,
                        uint8_t Speed,
                        uint8_t tx_fifo,
                        uint8_t rx_fifo,
                        uint8_t fastmodeplus
			)
{
     I2C_DISABLE(I2C,bwI2C_ENAble);
     I2C->IC_CON     &= ~(0xffff<< 0);//clear bit[15:0]
     I2C->IC_CON  = (Speed<<1);
     I2C_SlaveAdd(I2C,SlaAddress);
     I2C_SCL_LCNT(I2C,Speed,fastmodeplus);
     I2C_SCL_HCNT(I2C,Speed,fastmodeplus);
     I2C_TransFifoSet(I2C,tx_fifo);
     I2C_ReceFifoSet(I2C,tx_fifo);
		 I2C->IC_INTR_MASK &= ~(0x7fff);
		 I2C->IC_INTR_MASK |= 1<<11;
		 I2C->IC_INTR_MASK |= 1<<5;//REQ Interrupt
     I2C_ENABLE(I2C,bwI2C_ENAble);
}



void I2CWirteOneByte(I2C_TypeDef *I2C,uint8_t OffsetAddr,uint8_t data)
{
			while((APB_I2C->IC_STATUS & IC_STATUS_TFE) == 0x0);
		  while((APB_I2C->IC_STATUS & IC_STATUS_ACTIVITY) == 0x1);
      I2C->IC_DATA_CMD =OffsetAddr;//set offset address
      I2C->IC_DATA_CMD =data;//set offset address
      while((APB_I2C->IC_STATUS & IC_STATUS_ACTIVITY) == 0x1);
			
}
uint8_t I2CReadOneByte(I2C_TypeDef *I2C,uint8_t OffsetAddr)
{
			while((APB_I2C->IC_STATUS & IC_STATUS_TFE) == 0x0);
		  while((APB_I2C->IC_STATUS & IC_STATUS_ACTIVITY) == 0x1);
      I2C->IC_DATA_CMD =OffsetAddr;//set offset address
      I2C->IC_DATA_CMD =0x100;//send read command
      while((APB_I2C->IC_STATUS & IC_STATUS_ACTIVITY) == 0x1);
      return I2C->IC_DATA_CMD;
}
uint16_t I2CReadTwoBytes(I2C_TypeDef *I2C,uint8_t OffsetAddr)
{
			uint16_t tmp;
      I2C->IC_DATA_CMD =OffsetAddr;//set offset address
      I2C->IC_DATA_CMD =0x100;//send read command
			I2C->IC_DATA_CMD =0x100;//send read command
      while((I2C->IC_STATUS & IC_STATUS_RFNE) == 0x0);
			tmp = (APB_I2C->IC_DATA_CMD) << 8;
			tmp = (APB_I2C->IC_DATA_CMD) | tmp;	
      return tmp;
}

void I2CWirteOneByte_Loopback(I2C_TypeDef *S_I2C,uint8_t data)
{
	S_I2C->IC_DATA_CMD =data;
}

void I2CWirteBytes(I2C_TypeDef *I2C,uint8_t *data,uint8_t len){
	uint8_t i;
	while((I2C->IC_STATUS & IC_STATUS_ACTIVITY) == 0x1);
	for(i=0; i<len; i++, data++){
	 I2C->IC_DATA_CMD = *data;
	}
	while((I2C->IC_STATUS & IC_STATUS_TFE) == 0x0);
}

void I2CReadBytes(I2C_TypeDef *I2C,uint8_t OffsetAddr, uint8_t *data,uint8_t len){
	uint8_t i;
	while((I2C->IC_STATUS & IC_STATUS_ACTIVITY) == 0x1);
	I2C->IC_DATA_CMD = OffsetAddr;
	for(i=0; i<len; i++){
	 I2C->IC_DATA_CMD = 0x100;
	}
	while((I2C->IC_STATUS & IC_STATUS_ACTIVITY) == 0x1);
	for(i=0; i<len; i++, data++){
	 *data = I2C->IC_DATA_CMD;
	}	
}

void I2C_init(uint8_t taddr, uint8_t Speed, I2C_LOC i2c_loc){
	PIN_INFO pin_info;
	switch (i2c_loc){
		case (I2C_PB45):
			pin_info.pin_func = GIO_FUNC1;
			pin_info.pin_stat = GIO_PU;
			PinCtrl_GIOSet(PIN_CTL_GPIOB, GPIO_Pin_4|GPIO_Pin_5, &pin_info);	
			break;
		case (I2C_PC01):
			pin_info.pin_func = GIO_FUNC3;
			pin_info.pin_stat = GIO_PU;
			PinCtrl_GIOSet(PIN_CTL_GPIOC, GPIO_Pin_0|GPIO_Pin_1, &pin_info);	
			break;		
		case (I2C_PC67):
			pin_info.pin_func = GIO_FUNC1;
			pin_info.pin_stat = GIO_PU;
			PinCtrl_GIOSet(PIN_CTL_GPIOC, GPIO_Pin_6|GPIO_Pin_7, &pin_info);	
			break;
		case (I2C_PD23):
			pin_info.pin_func = GIO_FUNC3;
			pin_info.pin_stat = GIO_PU;
			PinCtrl_GIOSet(PIN_CTL_GPIOD, GPIO_Pin_2|GPIO_Pin_3, &pin_info);	
			break;		
	}
	I2C_MasterConfig(APB_I2C,taddr,1,Speed,1,8,8,0); 
}

void I2C_SearchAddr (I2C_TypeDef *I2C, I2C_LOC i2c_loc){
	uint32_t i;
	PIN_INFO pin_info;
	switch (i2c_loc){
		case (I2C_PB45):
			pin_info.pin_func = GIO_FUNC1;
			pin_info.pin_stat = GIO_PU;
			PinCtrl_GIOSet(PIN_CTL_GPIOB, GPIO_Pin_4|GPIO_Pin_5, &pin_info);	
			break;
		case (I2C_PC01):
			pin_info.pin_func = GIO_FUNC3;
			pin_info.pin_stat = GIO_PU;
			PinCtrl_GIOSet(PIN_CTL_GPIOC, GPIO_Pin_0|GPIO_Pin_1, &pin_info);	
			break;		
		case (I2C_PC67):
			pin_info.pin_func = GIO_FUNC1;
			pin_info.pin_stat = GIO_PU;
			PinCtrl_GIOSet(PIN_CTL_GPIOC, GPIO_Pin_6|GPIO_Pin_7, &pin_info);	
			break;
		case (I2C_PD23):
			pin_info.pin_func = GIO_FUNC3;
			pin_info.pin_stat = GIO_PU;
			PinCtrl_GIOSet(PIN_CTL_GPIOD, GPIO_Pin_2|GPIO_Pin_3, &pin_info);	
			break;		
	}	
	for(i=0; i<128;i++){
		I2C_MasterConfig(I2C,i,1,1,1,8,8,0); 
		I2C->IC_DATA_CMD = 0;
		delay(500);
		while((I2C->IC_STATUS & IC_STATUS_ACTIVITY) == 0x1);
		if(I2C_CLR_TX_ABRT(I2C) == 0x00){
			printf("I2C Addr Found!! I2C Addr: 0x%X\n", i);
		}
	}
}
	

uint32_t  I2C_CLR_RX_UNDER  (I2C_TypeDef *I2C){
	return  I2C->IC_CLR_RX_UNDER;
} 

uint32_t  I2C_CLR_RX_OVER   (I2C_TypeDef *I2C){
	return  I2C->IC_CLR_RX_OVER;
}

uint32_t  I2C_CLR_TX_OVER   (I2C_TypeDef *I2C){
	return  I2C->IC_CLR_TX_OVER;
}

uint32_t  I2C_CLR_RD_REQ    (I2C_TypeDef *I2C){
	return  I2C->IC_CLR_RD_REQ;
}

uint32_t  I2C_CLR_TX_ABRT   (I2C_TypeDef *I2C){
	return	I2C->IC_CLR_TX_ABRT;
}

uint32_t  I2C_CLR_RX_DONE   (I2C_TypeDef *I2C){
	return	I2C->IC_CLR_RX_DONE;
}

uint32_t  I2C_CLR_ACTIVITY  (I2C_TypeDef *I2C){
	return	I2C->IC_CLR_ACTIVITY;
}

uint32_t  I2C_CLR_STOP_DET  (I2C_TypeDef *I2C){
	return  I2C->IC_CLR_STOP_DET;
}

uint32_t  I2C_CLR_START_DET (I2C_TypeDef *I2C){
	return  I2C->IC_CLR_START_DET;
}

uint32_t  I2C_CLR_GEN_CALL  (I2C_TypeDef *I2C){
	return	I2C->IC_CLR_GEN_CALL;
}

