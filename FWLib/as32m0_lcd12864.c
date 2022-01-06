#include "as32m0_lcd12864.h"

void LCDInit(void){
	APB_GPIO->GPIO_DO.CLR = LCD_CS | LCD_SID | LCD_CLK;
	APB_GPIO->GPIO_OE.SET = LCD_CS | LCD_SID | LCD_CLK;
	delay_s(1);
	APB_GPIO->GPIO_DO.SET = LCD_CS;
	delay_us(500);
	LCDWriteCom(0x30);
	delay_ms(100);
	LCDWriteCom(0x30);
	delay_us(500);
	LCDWriteCom(0x0F);
	delay_us(500);
	LCDWriteCom(0x01);
	delay_ms(50);
	LCDWriteCom(0x06);  
	delay_us(500);	
}

void LCDSendByte(uint8_t byte){
	uint8_t i;
	APB_GPIO->GPIO_DO.CLR = LCD_CLK | LCD_SID;
	for(i=0; i<8; i++){
		APB_GPIO->GPIO_DO.CLR = LCD_CLK;
		if( (byte & 0x80) == 0x80)
			APB_GPIO->GPIO_DO.SET = LCD_SID;
		else
			APB_GPIO->GPIO_DO.CLR = LCD_SID;
		APB_GPIO->GPIO_DO.SET = LCD_CLK;
		byte = byte <<1;
	}
	APB_GPIO->GPIO_DO.CLR = LCD_CLK | LCD_SID;
}

void LCDWriteCom(uint8_t com){
	delay_us(100);
//	APB_GPIO->GPIO_DO.SET = LCD_CS;
	LCDSendByte(0xF8);
	LCDSendByte(0xF0 & com);
	LCDSendByte(0xF0 & (com<<4));
//	APB_GPIO->GPIO_DO.CLR = LCD_CS;
}

void LCDWriteData(uint8_t data){
	delay_us(100);
//	APB_GPIO->GPIO_DO.SET = LCD_CS;
	LCDSendByte(0xFA);
	LCDSendByte(0xF0 & data);
	LCDSendByte(0xF0 & (data<<4));
//	APB_GPIO->GPIO_DO.CLR = LCD_CS;
}

void address12864(uint8_t row, uint8_t col){
	switch(row)
	{
		case 1:  col|=0x80;break;
		case 2:  col|=0x90;break;
		case 3:  col|=0x88;break;
		case 4:  col|=0x98;break;
		default:         break;
	}
	LCDWriteCom(col);	
}

void stringDisp(uint8_t row, uint8_t *string, uint8_t len){
	uint8_t addr,i;					
	switch(row)
	{
		case 1:  addr = 0x80;break;
		case 2:  addr = 0x90;break;
		case 3:  addr = 0x88;break;
		case 4:  addr = 0x98;break;
		default:         break;
	}
	LCDWriteCom(addr); 
	delay_us(100);
	for(i=0;i<len;i++)
		{
			LCDWriteData(*string++);
			delay_us(50);
		}	
}
/*
void selectDisp(uint8_t m, uint8_t row, uint8_t col, uint8_t size, uint8_t *dat){
	uint8_t i;
	address12864(row,col);
	delay(500000);
	if(m == 0)	 					
	{
		for(i=0;i<size;i++)
		{
			LCDWriteData(0x30+(*dat++));
			delay(500000);
		}		
	}
	else
	{
		for(i=0;i<size;i++)
		{
			LCDWriteData(*(dat++));
			delay(500000);
		}		
	}	
}
*/
void Displayontrl(uint8_t displayon, uint8_t cursoron, uint8_t blinkon)
{
	uint8_t temp;
	displayon = displayon & 0x1;
	cursoron  = cursoron & 0x1;
	blinkon = blinkon & 0x1;
	temp = 0x08 | displayon<<2 | cursoron <<1 | blinkon;
	LCDWriteCom(temp);

}
void ChangeToBasicCMD(void){
	LCDWriteCom(0x30);
}

void LCDClear(void){
	LCDWriteCom(0x01);
	delay_ms(100);
}

void LCDdisplay_128x64(uint8_t *data){
	uint8_t xi, yj, x, y;
	uint16_t index;
	for(yj=0; yj<64; yj++){
		if(yj<32){
			x = 0x80;   
			y = yj + 0x80;
		}
		else{
			x = 0x88;
			y = yj - 32 + 0x80; 
		}
		LCDWriteCom(0x34);
		LCDWriteCom(y);
		LCDWriteCom(x);
		LCDWriteCom(0x30);
		index = yj * 16;
		for(xi=0; xi<16; xi++){
			LCDWriteData(data[index++]);
		}
	}
	LCDWriteCom(0x34);
	LCDWriteCom(0x36);
}
