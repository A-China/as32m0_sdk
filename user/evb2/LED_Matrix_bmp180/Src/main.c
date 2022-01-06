#include <stdlib.h>
#include <string.h>
#include "printf_config.h"
#include "as32m0_gpio.h"
#include "as32m0_pinctrl.h"
#include "as32m0_ssp.h"
#include "as32m0_misc.h"

#include "as32m0_pwm.h"
#include "LedControl.h"
#include "max7219_config.h"
#include "as32m0_BMP180.h"


#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) ((bitvalue) ? bitSet(value, bit) : bitClear(value, bit))
#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#define pgm_read_byte_near(addr)  pgm_read_byte(addr)

//#define __SOFTSPI
//#define LED1	GPIOA_Pin_0
//#define CLK		GPIOB_Pin_1
//#define MOSI	GPIOB_Pin_3
//#define CS 		GPIOB_Pin_0


void _sys_exit(void);
void Spi_init(void);
void ledSetup(void);
void clearScreen(void);
void scrollFont(void);
void scrollMessage(const uint8_t * messageString);
void loadBufferLong(uint8_t ascii, uint8_t dir);
void rotateBufferLongLeft(void);
void rotateBufferLongRight(void);
void printBufferLong(void);
uint32_t init_beep(void);



const int numDevices = 16;      // number of MAX7219s used in this case 2
const long scrollDelay = SCROLL_DELAY;   // adjust scrolling speed
const uint8_t bufferLongSpace = 48;
unsigned long bufferLong [bufferLongSpace] = {0};  
unsigned long bufferLong2[bufferLongSpace] = {0};  
const unsigned char spaces[] ={"                           "};
const unsigned char scrollText[] ={"A-China       Semicon    "};
const unsigned char scrollText2[] ={" PUF MCU     MCU007 n8   "};
const unsigned char scrollText3[] ={"ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
const uint8_t cortex[] = {"                         Cortex-M0  "};

void ledSetup(void);




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
const uint8_t font5x7 [] = {      //Numeric Font Matrix (Arranged as 7x font data + 1x kerning data)
	//Space (Char 0x20)
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 3, 
	//!
    0x40, 0x40, 0x40, 0x40, 0x40, 0x00, 0x40, 2,
	//"
    0xA0, 0xA0, 0xA0, 0x00, 0x00, 0x00, 0x00, 4,
 
    0x50, //01010000,	//#
    0x50, //01010000,
    0xF8, //11111000,
    0x50, //01010000,
    0xF8, //11111000,
    0x50, //01010000,
    0x50, //01010000,
    6,
 
    0x20, //00100000,	//$
    0x78, //01111000,
    0xA0, //10100000,
    0x70, //01110000,
    0x28, //00101000,
    0xF0, //11110000,
    0x20, //00100000,
    6,
 
    0xC0, //11000000,	//%
    0xC8, //11001000,
    0x10, //00010000,
    0x20, //00100000,
    0x40, //01000000,
    0x98, //10011000,
    0x18, //00011000,
    6,
 
    0x60, //01100000,	//&
    0x90, //10010000,
    0xA0, //10100000,
    0x40, //01000000,
    0xA8, //10101000,
    0x90, //10010000,
    0x68, //01101000,
    6,
 
    0xC0, //11000000,	//'
    0x40, //01000000,
    0x80, //10000000,
    0x00, //00000000,
    0x00, //00000000,
    0x00, //00000000,
    0x00, //00000000,
    3,
 
    0x20, //00100000,	//(
    0x40, //01000000,
    0x80, //10000000,
    0x80, //10000000,
    0x80, //10000000,
    0x40, //01000000,
    0x40, //00100000,
    4,
 
    0x80, //10000000,	//)
    0x40, //01000000,
    0x20, //00100000,
    0x20, //00100000,
    0x20, //00100000,
    0x40, //01000000,
    0x80, //10000000,
    4,
 
    0x00, //00000000,	//*
    0x20, //00100000,
    0xA8, //10101000,
    0x70, //01110000,
    0xA8, //10101000,
    0x20, //00100000,
    0x00, //00000000,
    6,
 
    0x00, //00000000,	//+
    0x20, //00100000,
    0x20, //00100000,
    0xF8, //11111000,
    0x20, //00100000,
    0x20, //00100000,
    0x00, //00000000,
    6,
 
    0x00, //00000000,	//,
    0x00, //00000000,
    0x00, //00000000,
    0x00, //00000000,
    0xC0, //11000000,
    0x40, //01000000,
    0x80, //10000000,
    3,
 
    0x00, //00000000,	//-
    0x00, //00000000,
    0x00, //00000000,
    0xF8, //11111000,
    0x00, //00000000,
    0x00, //00000000,
    0x00, //00000000,
    6,
 
    0x00, //00000000,	//.
    0x00, //00000000,
    0x00, //00000000,
    0x00, //00000000,
    0x00, //00000000,
    0xC0, //11000000,
    0xC0, //11000000,
    3,
 
    0x00, //00000000,	///
    0x08, //00001000,
    0x10, //00010000,
    0x20, //00100000,
    0x40, //01000000,
    0x80, //10000000,
    0x00, //00000000,
    6,
 
    0x70, //01110000,	//0
    0x88, //10001000,
    0x98, //10011000,
    0xA8, //10101000,
    0xC8, //11001000,
    0x88, //10001000,
    0x70, //01110000,
    6,
 
    0x40, //01000000,	//1
    0xC0, //11000000,
    0x40, //01000000,
    0x40, //01000000,
    0x40, //01000000,
    0x40, //01000000,
    0xE0, //11100000,
    4,
 
    0x70, //01110000,	//2
    0x88, //10001000,
    0x08, //00001000,
    0x10, //00010000,
    0x20, //00100000,
    0x40, //01000000,
    0xF8, //11111000,
    6,
 
    0xF8, //11111000,	//3
    0x10, //00010000,
    0x20, //00100000,
    0x10, //00010000,
    0x08, //00001000,
    0x88, //10001000,
    0x70, //01110000,
    6,
 
    0x10, //00010000,	//4
    0x30, //00110000,
    0x50, //01010000,
    0x90, //10010000,
    0xF8, //11111000,
    0x10, //00010000,
    0x10, //00010000,
    6,
 
    0xF8, //11111000,	//5
    0x80, //10000000,
    0xF0, //11110000,
    0x08, //00001000,
    0x08, //00001000,
    0x88, //10001000,
    0x70, //01110000,
    6,
 
    0x30, //00110000,	//6
    0x40, //01000000,
    0x80, //10000000,
    0xF0, //11110000,
    0x88, //10001000,
    0x88, //10001000,
    0x70, //01110000,
    6,
 
    0xF8, //11111000,	//7
    0x88, //10001000,
    0x08, //00001000,
    0x10, //00010000,
    0x20, //00100000,
    0x20, //00100000,
    0x20, //00100000,
    6,
 
    0x70, //01110000,	//8
    0x88, //10001000,
    0x88, //10001000,
    0x70, //01110000,
    0x88, //10001000,
    0x88, //10001000,
    0x70, //01110000,
    6,
 
    0x70, //01110000,	//9
    0x88, //10001000,
    0x88, //10001000,
    0x78, //01111000,
    0x08, //00001000,
    0x10, //00010000,
    0x60, //01100000,
    6,
 
    0x00, //00000000,	//:
    0xC0, //11000000,
    0xC0, //11000000,
    0x00, //00000000,
    0xC0, //11000000,
    0xC0, //11000000,
    0x00, //00000000,
    3,
 
    0x00, //00000000,	//;
    0xC0, //11000000,
    0xC0, //11000000,
    0x00, //00000000,
    0xC0, //11000000,
    0x40, //01000000,
    0x80, //10000000,
    3,
 
    0x10, //00010000,	//<
    0x20, //00100000,
    0x40, //01000000,
    0x80, //10000000,
    0x40, //01000000,
    0x20, //00100000,
    0x10, //00010000,
    5,
 
    0x00, //00000000,	//=
    0x00, //00000000,
    0xF8, //11111000,
    0x00, //00000000,
    0xF8, //11111000,
    0x00, //00000000,
    0x00, //00000000,
    6,
 
    0x80, //10000000,	//>
    0x40, //01000000,
    0x20, //00100000,
    0x10, //00010000,
    0x20, //00100000,
    0x40, //01000000,
    0x80, //10000000,
    5,
 
    0x70, //01110000,	//?
    0x88, //10001000,
    0x08, //00001000,
    0x10, //00010000,
    0x20, //00100000,
    0x00, //00000000,
    0x20, //00100000,
    6,
 
    0x70, //01110000,	//@
    0x88, //10001000,
    0x08, //00001000,
    0x68, //01101000,
    0xA8, //10101000,
    0xA8, //10101000,
    0x70, //01110000,
    6,
 
    0x70, //01110000,	//A
    0x88, //10001000,
    0x88, //10001000,
    0x88, //10001000,
    0xF8, //11111000,
    0x88, //10001000,
    0x88, //10001000,
    6,
 
    0xF0, //11110000,	//B
    0x88, //10001000,
    0x88, //10001000,
    0xF0, //11110000,
    0x88, //10001000,
    0x88, //10001000,
    0xF0, //11110000,
    6,
 
    0x70, //01110000,	//C
    0x88, //10001000,
    0x80, //10000000,
    0x80, //10000000,
    0x80, //10000000,
    0x88, //10001000,
    0x70, //01110000,
    6,
 
    0xE0, //11100000,	//D
    0x90, //10010000,
    0x90, //10001000,
    0x90, //10001000,
    0x90, //10001000,
    0x90, //10010000,
    0xE0, //11100000,
    6,
 
    0xF8, //11111000,	//E
    0x80, //10000000,
    0x80, //10000000,
    0xF0, //11110000,
    0x80, //10000000,
    0x80, //10000000,
    0xF8, //11111000,
    6,
 
    0xF8, //11111000,	//F
    0x80, //10000000,
    0x80, //10000000,
    0xF0, //11110000,
    0x80, //10000000,
    0x80, //10000000,
    0x80, //10000000,
    6,
 
    0x70, //01110000,	//G
    0x88, //10001000,
    0x80, //10000000,
    0xB8, //10111000,
    0x88, //10001000,
    0x88, //10001000,
    0xF0, //01111000,
    6,
 
    0x88, //10001000,	//H
    0x88, //10001000,
    0x88, //10001000,
    0xF8, //11111000,
    0x88, //10001000,
    0x88, //10001000,
    0x88, //10001000,
    6,
 
    0xE0, //11100000,	//I
    0x40, //01000000,
    0x40, //01000000,
    0x40, //01000000,
    0x40, //01000000,
    0x40, //01000000,
    0xE0, //11100000,
    4,
 
    0x38, //00111000,	//J
    0x10, //00010000,
    0x10, //00010000,
    0x10, //00010000,
    0x10, //00010000,
    0x90, //10010000,
    0x60, //01100000,
    6,
 
    0x88, //10001000,	//K
    0x90, //10010000,
    0xA0, //10100000,
    0xC0, //11000000,
    0xA0, //10100000,
    0x90, //10010000,
    0x88, //10001000,
    6,
 
    0x80, //10000000,	//L
    0x80, //10000000,
    0x80, //10000000,
    0x80, //10000000,
    0x80, //10000000,
    0x80, //10000000,
    0xF8, //11111000,
    6,
 
    0x88, //10001000,	//M
    0xD8, //11011000,
    0xA8, //10101000,
    0xA8, //10101000,
    0x88, //10001000,
    0x88, //10001000,
    0x88, //10001000,
    6,
 
    0x88, //10001000,	//N
    0x88, //10001000,
    0xC8, //11001000,
    0xA8, //10101000,
    0x98, //10011000,
    0x88, //10001000,
    0x88, //10001000,
    6,
 
    0x70, //01110000,	//O
    0x88, //10001000,
    0x88, //10001000,
    0x88, //10001000,
    0x88, //10001000,
    0x88, //10001000,
    0x70, //01110000,
    6,
 
    0xF0, //11110000,	//P
    0x88, //10001000,
    0x88, //10001000,
    0xF0, //11110000,
    0x80, //10000000,
    0x80, //10000000,
    0x80, //10000000,
    6,
 
    0x70, //01110000,	//Q
    0x88, //10001000,
    0x88, //10001000,
    0x88, //10001000,
    0xA8, //10101000,
    0x90, //10010000,
    0x68, //01101000,
    6,
 
    0xF0, //11110000,	//R
    0x88, //10001000,
    0x88, //10001000,
    0xF0, //11110000,
    0xA0, //10100000,
    0x90, //10010000,
    0x88, //10001000,
    6,
 
    0x78, //01111000,	//S
    0x80, //10000000,
    0x80, //10000000,
    0x70, //01110000,
    0x08, //00001000,
    0x08, //00001000,
    0xF0, //11110000,
    6,
 
    0xF8, //11111000,	//T
    0x20, //00100000,
    0x20, //00100000,
    0x20, //00100000,
    0x20, //00100000,
    0x20, //00100000,
    0x20, //00100000,
    6,
 
    0x88, //10001000,	//U
    0x88, //10001000,
    0x88, //10001000,
    0x88, //10001000,
    0x88, //10001000,
    0x88, //10001000,
    0x70, //01110000,
    6,
 
    0x88, //10001000,	//V
    0x88, //10001000,
    0x88, //10001000,
    0x88, //10001000,
    0x88, //10001000,
    0x50, //01010000,
    0x20, //00100000,
    6,
 
    0x88, //10001000,	//W
    0x88, //10001000,
    0x88, //10001000,
    0xA8, //10101000,
    0xA8, //10101000,
    0xA8, //10101000,
    0x50, //01010000,
    6,
 
    0x88, //10001000,	//X
    0x88, //10001000,
    0x50, //01010000,
    0x20, //00100000,
    0x50, //01010000,
    0x88, //10001000,
    0x88, //10001000,
    6,
 
    0x88, //10001000,	//Y
    0x88, //10001000,
    0x88, //10001000,
    0x50, //01010000,
    0x20, //00100000,
    0x20, //00100000,
    0x20, //00100000,
    6,
 
    0xF8, //11111000,	//Z
    0x08, //00001000,
    0x10, //00010000,
    0x20, //00100000,
    0x40, //01000000,
    0x80, //10000000,
    0xF8, //11111000,
    6,
 
    0xE0, //11100000,	//[
    0x80, //10000000,
    0x80, //10000000,
    0x80, //10000000,
    0x80, //10000000,
    0x80, //10000000,
    0xE0, //11100000,
    4,
 
    0x00, //00000000,	//(0xackward Slash)
    0x80, //10000000,
    0x40, //01000000,
    0x20, //00100000,
    0x10, //00010000,
    0x08, //00001000,
    0x00, //00000000,
    6,
 
    0xE0, //11100000,	//]
    0x20, //00100000,
    0x20, //00100000,
    0x20, //00100000,
    0x20, //00100000,
    0x20, //00100000,
    0xE0, //11100000,
    4,
 
    0x20, //00100000,	//^
    0x50, //01010000,
    0x88, //10001000,
    0x00, //00000000,
    0x00, //00000000,
    0x00, //00000000,
    0x00, //00000000,
    6,
 
    0x00, //00000000,	//_
    0x00, //00000000,
    0x00, //00000000,
    0x00, //00000000,
    0x00, //00000000,
    0x00, //00000000,
    0xF8, //11111000,
    6,
 
    0x80, //10000000,	//`
    0x40, //01000000,
    0x20, //00100000,
    0x00, //00000000,
    0x00, //00000000,
    0x00, //00000000,
    0x00, //00000000,
    4,
 
    0x00, //00000000,	//a
    0x00, //00000000,
    0x70, //01110000,
    0x08, //00001000,
    0x78, //01111000,
    0x88, //10001000,
    0x78, //01111000,
    6,
 
    0x80, //10000000,	//b
    0x80, //10000000,
    0xB0, //10110000,
    0xC8, //11001000,
    0x88, //10001000,
    0x88, //10001000,
    0xF0, //11110000,
    6,
 
    0x00, //00000000,	//c
    0x00, //00000000,
    0x70, //01110000,
    0x80, //10001000,
    0x80, //10000000,
    0x80, //10001000,
    0x70, //01110000,
    6,
 
    0x08, //00001000,	//d
    0x08, //00001000,
    0x68, //01101000,
    0x98, //10011000,
    0x88, //10001000,
    0x88, //10001000,
    0x78, //01111000,
    6,
 
    0x00, //00000000,	//e
    0x00, //00000000,
    0x70, //01110000,
    0x88, //10001000,
    0xF8, //11111000,
    0x80, //10000000,
    0x70, //01110000,
    6,
 
    0x30, //00110000,	//f
    0x48, //01001000,
    0x40, //01000000,
    0xE0, //11100000,
    0x40, //01000000,
    0x40, //01000000,
    0x40, //01000000,
    6,
 
    0x00, //00000000,	//g
    0x78, //01111000,
    0x88, //10001000,
    0x88, //10001000,
    0x78, //01111000,
    0x08, //00001000,
    0x70, //01110000,
    6,
 
    0x80, //10000000,	//h
    0x80, //10000000,
    0xB0, //10110000,
    0xC8, //11001000,
    0x88, //10001000,
    0x88, //10001000,
    0x88, //10001000,
    6,
 
    0x40, //01000000,	//i
    0x00, //00000000,
    0xC0, //11000000,
    0x40, //01000000,
    0x40, //01000000,
    0x40, //01000000,
    0x70, //11100000,
    4,
 
    0x10, //00010000,	//j
    0x00, //00000000,
    0x30, //00110000,
    0x10, //00010000,
    0x10, //00010000,
    0x90, //10010000,
    0x60, //01100000,
    5,
 
    0x80, //10000000,	//k
    0x80, //10000000,
    0x90, //10010000,
    0xA0, //10100000,
    0xC0, //11000000,
    0xA0, //10100000,
    0x90, //10010000,
    5,
 
    0xC0, //11000000,	//l
    0x40, //01000000,
    0x40, //01000000,
    0x40, //01000000,
    0x40, //01000000,
    0x40, //01000000,
    0xE0, //11100000,
    4,
 
    0x00, //00000000,	//m
    0x00, //00000000,
    0xD0, //11010000,
    0xA8, //10101000,
    0xA8, //10101000,
    0x88, //10001000,
    0x88, //10001000,
    6,
 
    0x00, //00000000,	//n
    0x00, //00000000,
    0xB0, //10110000,
    0xC8, //11001000,
    0x88, //10001000,
    0x88, //10001000,
    0x88, //10001000,
    6,
 
    0x00, //00000000,	//o
    0x00, //00000000,
    0x70, //01110000,
    0x88, //10001000,
    0x88, //10001000,
    0x88, //10001000,
    0x70, //01110000,
    6,
 
    0x00, //00000000,	//p
    0x00, //00000000,
    0xF0, //11110000,
    0x88, //10001000,
    0xF0, //11110000,
    0x80, //10000000,
    0x80, //10000000,
    6,
 
    0x00, //00000000,	//q
    0x00, //00000000,
    0x68, //01101000,
    0x98, //10011000,
    0x78, //01111000,
    0x08, //00001000,
    0x08, //00001000,
    6,
 
    0x00, //00000000,	//r
    0x00, //00000000,
    0xB0, //10110000,
    0xC8, //11001000,
    0x80, //10000000,
    0x80, //10000000,
    0x80, //10000000,
    6,
 
    0x00, //00000000,	//s
    0x00, //00000000,
    0x70, //01110000,
    0x80, //10000000,
    0x70, //01110000,
    0x08, //00001000,
    0xF0, //11110000,
    6,
 
    0x40, //01000000,	//t
    0x40, //01000000,
    0xE0, //11100000,
    0x40, //01000000,
    0x40, //01000000,
    0x48, //01001000,
    0x30, //00110000,
    6,
 
    0x00, //00000000,	//u
    0x00, //00000000,
    0x88, //10001000,
    0x88, //10001000,
    0x88, //10001000,
    0x98, //10011000,
    0x68, //01101000,
    6,
 
    0x00, //00000000,	//v
    0x00, //00000000,
    0x88, //10001000,
    0x88, //10001000,
    0x88, //10001000,
    0x90, //01010000,
    0x20, //00100000,
    6,
 
    0x00, //00000000,	//w
    0x00, //00000000,
    0x88, //10001000,
    0xA8, //10101000,
    0xA8, //10101000,
    0xA8, //10101000,
    0x50, //01010000,
    6,
 
    0x00, //00000000,	//x
    0x00, //00000000,
    0x88, //10001000,
    0x50, //01010000,
    0x20, //00100000,
    0x50, //01010000,
    0x88, //10001000,
    6,
 
    0x00, //00000000,	//y
    0x00, //00000000,
    0x88, //10001000,
    0x88, //10001000,
    0x78, //01111000,
    0x08, //00001000,
    0x70, //01110000,
    6,
 
    0x00, //00000000,	//z
    0x00, //00000000,
    0xF8, //11111000,
    0x10, //00010000,
    0x20, //00100000,
    0x40, //01000000,
    0xF8, //11111000,
    6,
 
    0x20, //00100000,	//{
    0x40, //01000000,
    0x40, //01000000,
    0x80, //10000000,
    0x40, //01000000,
    0x40, //01000000,
    0x20, //00100000,
    4,
 
    0x80, //10000000,	//|
    0x80, //10000000,
    0x80, //10000000,
    0x80, //10000000,
    0x80, //10000000,
    0x80, //10000000,
    0x80, //10000000,
    2,
 
    0x80, //10000000,	//}
    0x40, //01000000,
    0x40, //01000000,
    0x20, //00100000,
    0x40, //01000000,
    0x40, //01000000,
    0x80, //10000000,
    4,
 
    0x00, //00000000,	//~
    0x00, //00000000,
    0x00, //00000000,
    0x68, //01101000,
    0x90, //10010000,
    0x00, //00000000,
    0x00, //00000000,
    6,
 
    0x60, //01100000,	// (Char 0x7F)
    0x90, //10010000,
    0x90, //10010000,
    0x60, //01100000,
    0x00, //00000000,
    0x00, //00000000,
    0x00, //00000000,
    5,
    
    0x00, //00000000,	// smiley
    0x60, //01100000,
    0x66, //01100110,
    0x00, //00000000,
    0x81, //10000001,
    0x66, //01100110,
    0x18, //00011000,
    5
};


void ledSetup()
{
  for (int x=0; x<numDevices+1; x++)
  {
		//printf("LED matrix setup %d device\n", x);
    shutdown(x,false);       //The MAX72XX is NOT in power-saving mode on startup
    setIntensity(x,2);       // Set the brightness to default value
    clearDisplay(x);         // and clear the display
  }
}

void clearScreen() {
	//printf("Device num %d\n", numDevices);
	for (int x=0; x<numDevices+1; x++)
  {
    clearDisplay(x);         // and clear the display
		APB_GPIO->GPIO_DO.SET = LED1;
		__NOP();
		__NOP();
		__NOP();
		__NOP();
  }
  for (int i=0; i <35; i++){
		bufferLong[i] = 0;
	}
	
}

//void scrollFont() {
//    for (uint8_t counter=0x20;counter<0x80;counter++){
//        loadBufferLong(counter);
//        delay_ms(500);
//    }
//}
 
// Scroll Message
void scrollMessage(const uint8_t * messageString) {
    uint8_t counter = 0;
    uint8_t myChar=0;
    do {
        // read back a char 
        myChar =  pgm_read_byte_near(messageString + counter); 
        if (myChar != 0){
            loadBufferLong(myChar, LEFT);
        }
        counter++;
				//printf("scroll message #%d char=0x%02X\n", counter, myChar);
    } 
    while (myChar != 0);
}

// Load character into scroll buffer
void loadBufferLong(uint8_t ascii, uint8_t dir ){
		
    if (ascii >= 0x20 && ascii <=0x7f){
        for (int a=0;a<7;a++){                      // Loop 7 times for a 5x7 font
            unsigned long c = pgm_read_byte(font5x7 + ((ascii - 0x20) * 8) + a);     // Index into character table to get row data
//					int  c = font5x7[(((ascii - 0x20) * 8) + a)];
            unsigned long x = bufferLong [a*5];     // Load current scroll buffer
//					printf("ASCII=x%02X,c=x%08X, x=x%08X \n", ascii, c, bufferLong [a*5]); 
            x = x | c;                              // OR the new character onto end of current
            bufferLong [a*5] = x;                   // Store in buffer
					
        }
        uint8_t count = pgm_read_byte_near(font5x7 +((ascii - 0x20) * 8) + 7);     // Index into character table for kerning data
				//printf("counter=%d\n", count);
        for (uint8_t x=0; x<count;x++){
					if(dir == LEFT){
            rotateBufferLongLeft();
					}
				else if(dir == RIGHT){
						rotateBufferLongRight();
				}
            printBufferLong();
            delay_ms(scrollDelay);
        }
    }
}
// Rotate the buffer Left
void rotateBufferLongLeft(){
		unsigned long x;
	
    for (int a=0;a<7;a++){                      // Loop 7 times for a 5x7 font
        x = bufferLong [a*5];     							// Get low buffer entry
        uint8_t b = bitRead(x,31);              				// Copy high order bit that gets lost in rotation
        x = x<<1;                               // Rotate left one bit
        bufferLong [a*5] = x;                   // Store new low buffer
        x = bufferLong [a*5+1];                 // Get high buffer entry
				uint8_t b1 = bitRead(x,31);
        x = x<<1;                               // Rotate left one bit
        bitWrite(x,0,b);                        // Store saved bit
        bufferLong [a*5+1] = x;                 // Store new high buffer
			
        x = bufferLong [a*5+2]; 
				uint8_t b2 = bitRead(x,31);
				x = x<<1;                               // Rotate left one bit
        bitWrite(x,0,b1);                        // Store saved bit
        bufferLong [a*5+2] = x;                 // Store new high buffer
			
        x = bufferLong [a*5+3];  
				uint8_t b3 = bitRead(x,31);
				x = x<<1;                               // Rotate left one bit
        bitWrite(x,0,b2);                        // Store saved bit
        bufferLong [a*5+3] = x;                 // Store new high buffer
				
        x = bufferLong [a*5+4];  
				x = x<<1;                               // Rotate left one bit
        bitWrite(x,0,b3);                        // Store saved bit
        bufferLong [a*5+4] = x;                 // Store new high buffer
    }
}  

// Rotate the buffer Right
void rotateBufferLongRight(){
    for (int a=0;a<7;a++){                      // Loop 7 times for a 5x7 font
        unsigned long x;
				x = bufferLong [a*5+4];     						// Get low buffer entry
        uint8_t b = bitRead(x,0);              	// Copy high order bit that gets lost in rotation
        x = x>>1;                               // Rotate left one bit
        bufferLong [a*5+4] = x;                 // Store new low buffer
        x = bufferLong [a*5+3];                 // Get high buffer entry
				uint8_t b1 = bitRead(x,0);
        x = x>>1;                               // Rotate left one bit
        bitWrite(x,31,b);                       // Store saved bit
        bufferLong [a*5+3] = x;                 // Store new high buffer
			
        x = bufferLong [a*5+2];  
				uint8_t b2 = bitRead(x,0);
				x = x>>1;                               // Rotate left one bit
        bitWrite(x,31,b1);                      // Store saved bit
        bufferLong [a*5+2] = x;                 // Store new high buffer
			
        x = bufferLong [a*5+1];  
				uint8_t b3 = bitRead(x,0);
				x = x>>1;                               // Rotate left one bit
        bitWrite(x,31,b2);                        // Store saved bit
        bufferLong [a*5+1] = x;                 // Store new high buffer
				
        x = bufferLong [a*5+0];  
//				uint8_t b4 = bitRead(x,0);
				x = x>>1;                               // Rotate left one bit
        bitWrite(x,31,b3);                        // Store saved bit
        bufferLong [a*5+0] = x;                 // Store new high buffer
    }
}  


// Display Buffer on LED matrix
void printBufferLong(){
	//printf("Print buffer long\n");
  for (int a=0;a<7;a++){                    		// Loop 7 times for a 5x7 font
    unsigned long x;
		uint8_t y;
		
		y = bufferLong [a*5+4];
		setRow(15,a,y);
		x = bufferLong [a*5+3];  
		y = (x>>24);
		setRow(14,a,y);
		y = (x>>16);
		setRow(13,a,y);
		y = (x>>8);
		setRow(12,a,y);
		y = x;
		setRow(11,a,y);
		
		x = bufferLong [a*5+2];  
		y = (x>>24);
		setRow(10,a,y);
		y = (x>>16);
		setRow(9,a,y);
		y = (x>>8);
		setRow(8,a,y);
		y = x;
		setRow(7,a,y);
		
		x = bufferLong [a*5+1];   									// Get high buffer entry
		y = (x>>24);                            		// Mask off second character
    setRow(6,a,y);                       				// Send row to relevent MAX7219 chip
		y = (x>>16);                            		// Mask off third character
    setRow(5,a,y);                       				// Send row to relevent MAX7219 chip
		y = (x>>8);                             		// Mask off forth character
    setRow(4,a,y);                       				// Send row to relevent MAX7219 chip
		
    y = x;                          						// Mask off first character
    setRow(3,a,y);                       				// Send row to relevent MAX7219 chip
    x = bufferLong [a*5];                   		// Get low buffer entry
    y = (x>>24);                            		// Mask off second character
    setRow(2,a,y);                       				// Send row to relevent MAX7219 chip
    y = (x>>16);                            		// Mask off third character
    setRow(1,a,y);                       				// Send row to relevent MAX7219 chip
    y = (x>>8);                             		// Mask off forth character
    setRow(0,a,y);                       				// Send row to relevent MAX7219 chip
//		y = (x & 0xFF);
//		setRow(0,a,y);
  }
}

void scrollLeft(uint16_t digit) {
	uint16_t i;
	
	for(i=0; i<digit; i++){
		rotateBufferLongLeft();
		printBufferLong();
		delay_ms(scrollDelay);
	}
}

void scrollRight(uint16_t digit) {
	uint16_t i;
	
	for(i=0; i<digit; i++){
		rotateBufferLongRight();
		printBufferLong();
		delay_ms(scrollDelay);
	}
}

void TempInfo(float T, float p){
	char str[16];
	sprintf(str, "Temp : %.1f C     ", T);;
	sprintf(str, "P  :%.1f hPa     ", p);;

}

/**
	* @brief  initial APB_PWM3 (PWM6) for beepper (4KHz, 78:22)
  * @param  void 
  */
uint32_t init_beep(void) {
	
	PWM_INFO pwm_info;
	int32_t ret;

	pwm_info.int_en = 0;				// pwm interrupt 0=disable, 1=enable
	pwm_info.div = 2;
	pwm_info.lmt = 3000;

	pwm_info.ch_1_info.ch_n_point = 2000;			//@ SPWM ch1 only
	pwm_info.ch_1_info.ch_p_point = 0;
	pwm_info.ch_1_info.oc_en      = 1;
	pwm_info.ch_1_info.oen_n      = 1;
	pwm_info.ch_1_info.oen_p      = 1;
	pwm_info.ch_1_info.out_n      = 1;
	pwm_info.ch_1_info.out_p      = 0;

	//printf("Configing APB_PWM3 beep \n");
	ret = apPWM_Config(APB_PWM3, &pwm_info);				// APB_PWM3 is pin PWM6
	if(ret < 0){
		//printf("[%d] Check CMD Status busy \n",__LINE__);
	}	
	// APB_PINC->PB6.FuncSel = 0xC0; // SWITCH TO GPIO
	APB_PINC->PB6.FuncSel = 0xF0; // SWITCH TO GPIO
	return ret;
}

/**
	* @brief  soft SPI init for MAX72xx LED
  * @param  void 
  */
void Spi_init(void){
#ifdef __SOFTSPI	
	// Set GPIO FUNC
		APB_PINC->PB0.FuncSel = GIO_FUNC0;
		APB_PINC->PB1.FuncSel = GIO_FUNC0;
		APB_PINC->PB2.FuncSel = GIO_FUNC0;
		APB_PINC->PB3.FuncSel = GIO_FUNC0;
	// SET Output As Default
		APB_GPIO->GPIO_OE.SET = CLK | MOSI | CS;
		APB_GPIO->GPIO_DO.CLR = CLK;			// CLK Set Default 0
		APB_GPIO->GPIO_DO.CLR = MOSI;			// MOSI Set Default 0
		APB_GPIO->GPIO_DO.SET = CS;	
#else 
	#ifndef __SOFTSPI
		PIN_INFO PIN_INFO;
		PIN_INFO.pin_func = GIO_FUNC1;
		PIN_INFO.pin_stat = GIO_PU;
		PIN_INFO.pin_ds		= GIO_DS_2_4;
		PIN_INFO.pin_od   = OD_OFF;
		PIN_INFO.pin_sonof = SONOF_ON;
		APB_GPIO->GPIO_DO.SET = GPIOB_Pin_0;
		APB_GPIO->GPIO_OE.SET = GPIOB_Pin_0;
		PinCtrl_GIOSet(PIN_CTL_GPIOB, GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 , &PIN_INFO);
		ssp_config(6);	
	#endif
#endif	
	
		
	//printf("logProccess Software SPI initized.\n");
}

void rotateBufferLongTest(void) {
		rotateBufferLongLeft();
		for (int a=0;a<7;a++){
			printf("bufLong rotated %d x%08X \n", a, bufferLong[a*5]);
		}
		printf("\n");
}

void bufferTest(int ascii) {
	if (ascii >= 0x20 && ascii <=0x7f){
        for (int a=0;a<7;a++){                      // Loop 7 times for a 5x7 font
					int  c = font5x7[(((ascii - 0x20) * 8) + a)];
          unsigned long x = bufferLong [a*5];     // Load current scroll buffer
          x = x | c;                              // OR the new character onto end of current
          bufferLong [a*5] = x;                   // Store in buffer
					printf("bufLong %d x%08X \n", a, bufferLong[a*5]); 
        }
		}
	rotateBufferLongTest();
	printBufferLong();
	
}

int main()
{
	// GPIO PA0 LED
	GPIO_InitTypeDef gpio_init;
	gpio_init.GPIO_Mode = GPIO_Mode_Out;
	gpio_init.GPIO_Pin	= GPIO_Pin_0;
	GPIO_Init(GPIOA, &gpio_init);
	APB_GPIO->GPIO_DO.SET = GPIOA_Pin_0;
	
	// prepare printf (uart0)
	config_uart0(OSC_CLK_FREQ, UART_SETBAUD, UART0_PD23);
	
	printf("==> \n");
	printf("==> \n");
	printf("==> BMP085/180 + MAX7219 control LED Matrix \n");
	printf("==> \n");
	printf("==> \n");
	// wait
	while(apUART_Check_BUSY(APB_UART0) == 1);

	// *********************************************
	init_beep();
	Spi_init();
	
	ledControl_init(MOSI, CLK, CS, numDevices);
	ledSetup();
	clearScreen();
	delay_ms(300);

	init_I2C_BMP180(BMP180_ADDR, 1, I2C_PB45);
	BMP180_read_calibration();
	
  float T, p, A, P0;	
	uint8_t str[20];
	uint16_t cin;
	uint16_t i;
	
	loadBufferLong('7', 0);
	delay_s(1);
	printBufferLong();


	while(1) {
		while(apUART_Check_RXFIFO_EMPTY(APB_UART0) == 1);
			cin = SER_GetChar();	
			switch(cin){
				case '1':
						rotateBufferLongLeft();
						printBufferLong();
					break;
				
				case '2':
						loadBufferLong('7', 0);
					break;
				
				case '4':
						scrollMessage(scrollText);   //scrollFont();
					break;
				
				case '5':
						loadBufferLong('A', 0);
						loadBufferLong('B', 0);
						loadBufferLong('C', 0);
					break;
				
				case '3':
						rotateBufferLongRight();
						printBufferLong();
					break;
				
				case '7':
						scrollMessage(scrollText2);
					break;
				
				case '8':
						printBufferLong();
					break;
				
				case '9':
					while(1){
						clearScreen();
						delay_s(1);
						scrollMessage(scrollText);   //scrollFont();
						delay_s(2);
						for(i=0; i<150; i++) {
							rotateBufferLongRight();
							printBufferLong();
						}
						delay_s(2);
						scrollMessage(scrollText2);   //scrollFont();
						delay_s(2);
						for(i=0; i<150; i++) {
							rotateBufferLongRight();
							printBufferLong();
						}
						delay_ms(2000);
						scrollMessage(cortex);   //scrollFont();
						delay_s(2);
						for(i=0; i<150; i++) {
							rotateBufferLongLeft();
							printBufferLong();
						}
						delay_ms(2000);
				}
				break;
				
				case 'a':
						printf("BMP180 temperature pressure sensor\n");
						T=BMP180_getTemperature();
						printf("\nT		:	%.1f C\n", T);
						p = BMP180_calpressure(0);
						p = p/100;
						printf("p		:	%.1f hPa\n", p);
						A=BMP100_altitude(p, 1008.2);// in meter
						P0=BMP100_sealevel(p, A);
						printf("Altitude	:	%f m\n", A);
						printf("Sealevel	:	%f m\n\n", P0);
					
						sprintf(str,"  %.1f C        %.1f Pa  ", T, p);;
						scrollMessage(str);
				
					
					break;
				
				default:
					break;
			}
	}
	
	//-------  System Config ----------
//	
//  printf("\n");
//	printf("\n");
//	printf("==> TEST SSD1306 PASSED \n");
//	printf("\n");
//	printf("\n");
//	printf("#"); // invoke TB $finish;
//	_sys_exit();
// 	return 0x12;
}

