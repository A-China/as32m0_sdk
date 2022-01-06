#include "as32m0_BMP180.h"

static int16_t AC1;
static int16_t AC2;
static int16_t AC3;
static uint16_t AC4;
static uint16_t AC5;
static uint16_t AC6;
static int16_t B1;
static int16_t B2;
static int32_t B3;
static uint32_t B4;
static int32_t B6;
static int32_t B7;
static int16_t MB;
static int16_t MC;
static int16_t MD;
static int32_t UT;
static int32_t UP;
static float X1;
static float X2;
static float X3;

static float B5;
void init_I2C_BMP180(uint8_t taddr, uint8_t Speed, I2C_LOC i2c_loc){
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

uint16_t BMP180_read16_bit(uint8_t address){
	uint16_t tmp;
	while((APB_I2C->IC_STATUS & (1<<0)) == 0x1);
  APB_I2C->IC_DATA_CMD =address;//set offset address
  APB_I2C->IC_DATA_CMD =0x100;//send read command	
	APB_I2C->IC_DATA_CMD =0x100;//send read command	
	while((APB_I2C->IC_STATUS & (1<<0)) == 0x1);
	tmp = (APB_I2C->IC_DATA_CMD) << 8;
	tmp = (APB_I2C->IC_DATA_CMD) | tmp;	
	return tmp;
}
int16_t BMP180_readsigned16_bit(uint8_t address){
	int16_t tmp;
	while((APB_I2C->IC_STATUS & (1<<0)) == 0x1);
  APB_I2C->IC_DATA_CMD =address;//set offset address
  APB_I2C->IC_DATA_CMD =0x100;//send read command	
	APB_I2C->IC_DATA_CMD =0x100;//send read command	
	while((APB_I2C->IC_STATUS & (1<<0)) == 0x1);
	tmp = (APB_I2C->IC_DATA_CMD) << 8;
	tmp = (APB_I2C->IC_DATA_CMD) | tmp;	
	return tmp;
}

void BMP180_read_calibration(void){
	AC1 = BMP180_readsigned16_bit(0xAA);
	AC2 = BMP180_readsigned16_bit(0xAC);
	AC3 = BMP180_readsigned16_bit(0xAE);
	AC4 = BMP180_read16_bit(0xB0);
	AC5 = BMP180_read16_bit(0xB2);
	AC6 = BMP180_read16_bit(0xB4);
	B1	= BMP180_readsigned16_bit(0xB6);
	B2	= BMP180_readsigned16_bit(0xB8);
	MB	= BMP180_readsigned16_bit(0xBA);
	MC	= BMP180_readsigned16_bit(0xBC);
	MD	= BMP180_readsigned16_bit(0xBE);
/*	
	printf("AC1: %d\n", AC1);
	printf("AC2: %d\n", AC2);
	printf("AC3: %d\n", AC3);
	printf("AC4: %d\n", AC4);
	printf("AC5: %d\n", AC5);
	printf("AC6: %d\n", AC6);
	printf("B1: %d\n", B1);
	printf("B2: %d\n", B2);
	printf("MB: %d\n", MB);
	printf("MC: %d\n", MC);
	printf("MD: %d\n", MD);
*/
}

void BMP180_getUT(void){
	uint8_t data[2];
	data[0] = BMP180_REG_CONTROL;
	data[1] = BMP180_COMMAND_TEMPERATURE;
	I2CWirteBytes(APB_I2C, data, 2);
  delay(500000);
	UT = BMP180_readsigned16_bit(BMP180_REG_RESULT);
}
void BMP180_getUP(uint8_t oss){
	uint8_t data[3];
	oss = oss & 0x3;
	data[0] = BMP180_REG_CONTROL;
	switch (oss){
		case 0:
			data[1] = BMP180_COMMAND_PRESSURE0;
			break;
		case 1:
			data[1] = BMP180_COMMAND_PRESSURE1;
			break;
		case 2:
			data[1] = BMP180_COMMAND_PRESSURE2;
			break;
		case 3:
			data[1] = BMP180_COMMAND_PRESSURE3;
			break;	
		default:
			data[1] = BMP180_COMMAND_PRESSURE0;
			break;		
	}
	I2CWirteBytes(APB_I2C, data, 2);	
	delay(500000);
	//UP = BMP180_readsigned16_bit(BMP180_REG_RESULT);
	I2CReadBytes(APB_I2C, BMP180_REG_RESULT, data, 3);
	UP = data[0] <<16 | data[1] <<8 | data[2];
	UP = UP>> (8-oss);
}
float BMP180_getTemperature(void){
	float T;
	BMP180_getUT();
	X1 = UT;
	X1 = X1 - AC6;
	X1 = X1 * AC5;
	X1 /= 32768;
	X2 = MC;
	X2 *= 2048;
	X2 /=(X1 + MD);
	B5 = X1 + X2;
	T = (B5 + 8)/16;
	T *= 0.1;
	return T;
}
	
float BMP180_calpressure(uint8_t oss){
	float p;
	BMP180_getUP(oss);
	B6 = B5 - 4000;
	X1 =  B6 * B6;
	X1 /= 4096;
	X1 *= B2;
	X1 /= 2048;
	X2 = AC2 * B6;
	X2 /= 2048;
	X3 = X1 + X2;
	B3 = AC1*4 +X3;
	B3 = (B3 << oss) + 2;
	B3 = B3 / 4;
	X1 = AC3*B6;
	X1 /= 8192;
	X2 = B6 * B6;
	X2 /= 4096;
  X2 *= B1;	
	X2 /=65536;
	X3 = X1 + X2 + 2;
	X3/=4;
	B4 = X3 + 32768;
	B4 *= AC4;
	B4 /= 32768;
	B7 = (uint32_t)UP - B3;
	B7 *= 50000 >> oss;
	if(B7 < 0x80000000){
		p = (uint32_t)(B7 * 2)/B4;
	}
	else{
		p = B7/B4;
		p *= 2;
	}
	X1 = p/256;
	X1 *= X1;
	X1 = X1 * 3038 / 65536;
	X2 = (-7357 * p)/ 65536;
	p = p + (X1 + X2 + 3791)/16;
	return p;
}	

float BMP100_altitude(float P, float P0){
	return(44330.0*(1-pow(P/P0,1/5.255)));
}

float BMP100_sealevel(float P, float A){
	return(P/pow(1-(A/44330.0),5.255));
}

