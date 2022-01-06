#include "math.h"
#include "as32m0_i2c.h"
#include "as32m0_pinctrl.h"
#include "as32m0_misc.h"
#include "printf_config.h"

#ifndef __as32m0_bmp180_h
#define __as32m0_bmp180_h

#define BMP180_ADDR 0x77 // 7-bit address

#define	BMP180_REG_CONTROL 0xF4
#define	BMP180_REG_RESULT 0xF6

#define	BMP180_COMMAND_TEMPERATURE 0x2E
#define	BMP180_COMMAND_PRESSURE0 0x34
#define	BMP180_COMMAND_PRESSURE1 0x74
#define	BMP180_COMMAND_PRESSURE2 0xB4
#define	BMP180_COMMAND_PRESSURE3 0xF4

//typedef enum{
//	I2C_PB45 = 0,
//	I2C_PC01 = 1,
//	I2C_PC67 = 2,
//	I2C_PD23 = 3
//}I2C_LOC;

void init_I2C_BMP180(uint8_t taddr, uint8_t Speed, I2C_LOC i2c_loc);
uint16_t BMP180_read16_bit(uint8_t address);
int16_t BMP180_readsigned16_bit(uint8_t address);
void BMP180_read_calibration(void);
void BMP180_getUT(void);
void BMP180_getUP(uint8_t oss); //Oss (register F4h <7:6>): controls the oversampling ratio of the pressure measurement (00b: single, 01b: 2 times, 10b: 4 times, 11b: 8 times).
float BMP180_getTemperature(void);
float BMP180_calpressure(uint8_t oss);
float BMP100_altitude(float P, float P0);
float BMP100_sealevel(float P, float A);


#endif

