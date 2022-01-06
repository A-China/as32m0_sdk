#ifndef __MAX7219_CONFIG_h
#define __MAX7219_CONFIG_h

//#define __SOFTSPI
#define LED1	GPIOA_Pin_0
#define CLK		GPIOB_Pin_1
#define MOSI	GPIOB_Pin_3
#define CS 		GPIOB_Pin_0

#ifndef __SOFTSPI
	#include "as32m0_ssp.h"
	
#endif	

//enum MAX_ROTATE_DIRCTION
//{
//    UP = 2,
//    DOWN = 3,
//    LEFT = 0,
//    RIGHT = 1
//} MAX_ROTATE_DIRCTION;

#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3

#define SCROLL_DELAY 2




#endif	//__MAX7219_CONFIG_h

