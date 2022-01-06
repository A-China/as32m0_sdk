/*
	
  ******************************************************************************
  * @file 			( file ):   UC1609C.h
  * @brief 		( description ):  	
  ******************************************************************************
  * @attention 	( attention ):	author: 
  ******************************************************************************
  
*/

#ifndef _UC1609C_H
#define _UC1609C_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ----------------------------------------------------------*/

#include "fonts.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "as32m0_gpio.h"
#include "as32m0_pinctrl.h"
#include "as32m0_ssp.h"
#include "as32m0_misc.h"

/*
	connection LCD UC1609C:

	CS   ->   CS ( chip select )
	RST  ->   RST ( Reset )
	CD   ->   DC ( date / command )
	SCK	 ->   SCL
	SDA	 ->   MOSI
	VDD  ->   +3.3V
	VSS	 ->   GND
	A	 ->   anode led ( +3.3V or PWM )
	K	 ->   katode led ( GND )

*/
#define GPIO_PortB	8
#define DC_GPIO_Port 8
#define RST_GPIO_Port 8
#define CS_GPIO_Port 8
#define CS_Pin			GPIO_Pin_0
#define SPI_CLK			GPIO_Pin_1
#define SPI_MISO		GPIO_Pin_2
#define SPI_MOSI		GPIO_Pin_3
#define DC_Pin			GPIO_Pin_4
#define RST_Pin			GPIO_Pin_5
#define GPIO_PIN_RESET 	0
#define GPIO_PIN_SET		1
#define HAL_Delay( t ) delay_ms( t )


//#######  SETUP  ##############################################################################################

			// specify the SPI port for HAL -------------------
			//============================================================================
		
			//=== we indicate the ports (if they are called DC RES CS in the cube, then there is nothing to indicate)
			
			
			
			
			
			
			//=============================================================================
			
			// def: 192 x 64
			#define  UC1609C_width			192		// 192
			#define	 UC1609C_height			64		// 64
			
//##############################################################################################################

// Display  Size ------------------------------------------------

#define UC1609C_WIDTH	UC1609C_width
#define UC1609C_HEIGHT	UC1609C_height

/* Absolute value */
#define ABS(x)   ((x) > 0 ? (x) : -(x))


// UC1609C Read registers ---------------------------------------------
#define UC1609C_GET_STATUS 					0x01 	// Not used v1.0

// UC1609C Write registers --------------------------------------------
#define UC1609C_SYSTEM_RESET 				0xE2 	// Not used v1.0

#define UC1609C_POWER_CONTROL 				0x2F 
#define UC1609C_PC_SET 						0x06 	// PC[2:0] 110 Internal V LCD (7x charge pump) + 10b: 1.4mA

#define UC1609C_ADDRESS_CONTROL 			0x88 	// set RAM address control
#define UC1609_ADDRESS_SET 					0x02 	// FOR UC1609  Set AC [2:0] Program registers  for RAM address control.
#define UC1609C_ADDRESS_SET 				0x01 	// FOR UC1609C  Set AC [2:0] Program registers  for RAM address control.

#define UC1609C_SET_PAGEADD 				0xB0 	// Page address Set PA[3:0]
#define UC1609C_SET_COLADD_LSB 				0x00 	// Column Address Set CA [3:0]
#define UC1609C_SET_COLADD_MSB 				0x10 	// Column Address Set CA [7:4]

#define UC1609C_TEMP_COMP_REG 				0x27 	// Temperature Compensation Register
#define UC1609C_TEMP_COMP_SET 				0x00 	// TC[1:0] = 00b= -0.00%/ C

#define UC1609C_FRAMERATE_REG 				0xA0 	// Frame rate
#define UC1609C_FRAMERATE_SET 				0x01  	// Set Frame Rate LC [4:3] 01b: 95 fps

#define UC1609C_BIAS_RATIO 					0xE8 	// Bias Ratio. The ratio between V-LCD and V-D .
#define UC1609C_BIAS_RATIO_SET 				0x03 	//  Set BR[1:0] = 11 (set to 9 default)

#define UC1609C_GN_PM 						0x81 	// Set V BIAS Potentiometer to fine tune V-D and V-LCD  (double-byte command)
#define UC1609C_DEFAULT_GN_PM 				0x49 	// default only used if user does not specify Vbias

#define UC1609C_LCD_CONTROL 				0xC0 	// Rotate map control
#define UC1609C_DISPLAY_ON 					0xAE 	// enables display
#define UC1609C_ALL_PIXEL_ON 				0xA4 	// sets on all Pixels on
#define UC1609C_INVERSE_DISPLAY 			0xA6 	// inverts display
#define UC1609C_SCROLL 						0x40 	// scrolls , Set the scroll line number. 0-64

// Rotate -------------------------------------------------------------
#define UC1609C_ROTATION_FLIP_TWO 			0x06
#define UC1609C_ROTATION_NORMAL 			0x04
#define UC1609C_ROTATION_FLIP_ONE 			0x02
#define UC1609C_ROTATION_FLIP_THREE 		0x00

//---------------------------------------------------------------------


// Display pixel color definition
// (1): white on blue, FG = white BG = blue
// ERM19264SBS-4 LCD white on blue
// (2): black on white, FG = black BG = white
// ERM19264FS-4 LCD, black on white
// (3): white on black, FG = white, BG = black
// ERM19264DNS-4 LCD White on Black 

#define FOREGROUND  						0
#define BACKGROUND 							1
#define INVERSE								2

//---------------------------------------------------------------------


/* Functions prototypes ----------------------------------------------*/

/*
	******************************************************************************
	* @brief	( description ):  asc32m0 spi initialization
	* @param	( options ):			spi speed, clock divider
	* @return  	( returns ):	
	******************************************************************************
*/

void init_spi(uint8_t speed);

/*
	******************************************************************************
	* @brief	( description ):  display initialization
	* @param	( options ):	
	* @return  	( returns ):	
	******************************************************************************
*/
void UC1609C_init( void );
//----------------------------------------------------------------------------------


/*
	******************************************************************************
	* @brief	( description ):  setting display contrast
	* @param	( options ):	value from 0 .... 255 (default 30)
	* @return  	( returns ):	
	******************************************************************************
*/
void UC1609C_contrast (uint8_t bits);
//----------------------------------------------------------------------------------


/*
	******************************************************************************
	* @brief	( description ):  enabling and disabling information on the display (with saving information)
	* @param	( options ):	1 - ON    0 - OFF
	* @return  	( returns ):	
	******************************************************************************
*/
void UC1609C_enable (uint8_t bits);
//----------------------------------------------------------------------------------


/*
	******************************************************************************
	* @brief	( description ):  scrolling the display vertically
	* @param	( options ):	value from 0 to 64
									how many lines do we shift
										from 0 to 64 move up
										from 64 to 0 move down
	* @return  	( returns ):	
	******************************************************************************
*/
void UC1609C_scroll (uint8_t bits);
//----------------------------------------------------------------------------------


/*
	******************************************************************************
	* @brief	( description ):  display rotation
	* @param	( options ):	Param1: 4 possible values 000 010 100 110 (defined)
								specify the rotation parameter:
									UC1609C_ROTATION_FLIP_TWO
									UC1609C_ROTATION_NORMAL
									UC1609C_ROTATION_FLIP_ONE
									UC1609C_ROTATION_FLIP_THREE
	* @return  	( returns ):	
	******************************************************************************
*/
void UC1609C_rotate(uint8_t rotatevalue);
//----------------------------------------------------------------------------------


/*
	******************************************************************************
	* @brief	( description ):  display inversion
	* @param	( options ):	Param1: bits, 1 invert , 0 normal
	* @return  	( returns ):	
	******************************************************************************
*/
void UC1609C_invertDisplay (uint8_t bits);
//----------------------------------------------------------------------------------


/*
	******************************************************************************
	* @brief	( description ):  output to the display of all points at once (paint over the entire display)
	* @param	( options ): 	parameter: 1 - all points are on, 0 - all points are off
	* @return  	( returns ):	
	******************************************************************************
*/
void UC1609C_allPixelsOn(uint8_t bits);
//----------------------------------------------------------------------------------


/*
	******************************************************************************
	* @brief	( description ):  fill the entire display with bits (only do not touch the display buffer)
								( for example, if 0x00 then everything is empty, 0xFF then everything is painted over, 0x55 (0v01010101) then everything is through a line)
	* @param	( options ):	1- command, 2 - data
	* @return  	( returns ):	
	******************************************************************************
*/
void UC1609C_fillScreen(uint8_t dataPattern);
//----------------------------------------------------------------------------------


/*
	******************************************************************************
	* @brief	( description ):  output of information from the buffer to the display
								( called every time you need to display data)
								( for example, we brought out the text and then call the UC1609C_update (void) function)
	* @param	( options ):	
	* @return  	( returns ):	
	******************************************************************************
*/
void UC1609C_update( void );
//----------------------------------------------------------------------------------


/*
	******************************************************************************
	* @brief	( description ):  draws a pixel at the specified coordinates (do not forget to call UC1609C_update ();)
	* @param	( options ):	Param1: x offset 0-192
								Param2: y offset 0-64
								Param3: pixel view ( FOREGROUND or BACKGROUND  or INVERSE )
	* @return  	( returns ):	
	******************************************************************************
*/

void UC1609C_drawPixel( int16_t x, int16_t y, uint8_t colour );
//----------------------------------------------------------------------------------


/*
	******************************************************************************
	* @brief	( description ):  clear the framebuffer (but does not clear the display itself)
	* @param	( options ):
	* @return  	( returns ):	
	******************************************************************************
*/
void UC1609C_clearBuffer( void );
//----------------------------------------------------------------------------------


/*
	******************************************************************************
	* @brief	( description ):  displaying a picture (from an array) to the screen (do not forget to call UC1609C_update ();)
	* @param	( options ):	options:
									//Param1: x offset 0-192
									//Param2: y offset 0-64
									//Param3: width 0-192
									//Param4 height 0-64
									//Param5 array with picture
	* @return  	( returns ):	
	******************************************************************************
*/
void UC1609C_bitmap(int16_t x, int16_t y, uint8_t w, uint8_t h, const uint8_t* data, uint8_t colour);
//----------------------------------------------------------------------------------


/*
	******************************************************************************
	* @brief	( description ):  a list of the display itself (the frame buffer is not cleared)
	* @param	( options ):	
	* @return  	( returns ):	
	******************************************************************************
*/
void UC1609C_clearDisplay( void );			
//----------------------------------------------------------------------------------



/*
	******************************************************************************
	* @brief	( description ):  displaying a string (Latin and Cyrillic) (do not forget to call UC1609C_update ();)
	* @param	( options ):	1- x coordinate
								2- y-coordinate
								3- the string itself
								4- specify the font of the symbol
								5- symbol size multiplier
								6- pixel view (FOREGROUND or BACKGROUND or INVERSE)
	* @return  	( returns ):	
	******************************************************************************
*/
void UC1609C_Print(int16_t x, int16_t y, char* str, FontDef_t* Font, uint8_t multiplier, uint8_t color);
//----------------------------------------------------------------------------------


/*
	******************************************************************************
	* @brief	( description ):  draw a line (do not forget to call UC1609C_update ();)
	* @param	( options ):	1- coordinate x1
								2- coordinate y1
								3- coordinate x2
								4- coordinate y2
								5- pixel view ( FOREGROUND or BACKGROUND  or INVERSE )
	* @return  	( returns ):	
	******************************************************************************
*/
void UC1609C_DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t c);
//----------------------------------------------------------------------------------


/*
	******************************************************************************
	* @brief	( description ):  draw a hollow rectangle (do not forget to call UC1609C_update ();)
	* @param	( options ):	1- coordinate x1
								2- coordinate y1
								3- width
								4- height
								5- pixel view ( FOREGROUND or BACKGROUND  or INVERSE )
	* @return  	( returns ):	
	******************************************************************************
*/
void UC1609C_DrawRectangle(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t c);
//----------------------------------------------------------------------------------


/*
	******************************************************************************
	* @brief	( description ):  draw a filled rectangle (do not forget to call UC1609C_update ();)
	* @param	( options ):	1- coordinate x1
								2- coordinate y1
								3- width
								4- height
								5- pixel view ( FOREGROUND or BACKGROUND  or INVERSE )
	* @return  	( returns ):	
	******************************************************************************
*/
void UC1609C_DrawFilledRectangle(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t c);
//----------------------------------------------------------------------------------


/*
	******************************************************************************
	* @brief	( description ):  draw a hollow triangle (do not forget to call UC1609C_update ();)
	* @param	( options ):	1- coordinate x1
								2- coordinate y1
								3- coordinate x2
								4- coordinate y2
								5- coordinate x3
								6- coordinate y3
								7- pixel view ( FOREGROUND or BACKGROUND  or INVERSE )
	* @return  	( returns ):	
	******************************************************************************
*/
void UC1609C_DrawTriangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint8_t color);
//----------------------------------------------------------------------------------


/*
	******************************************************************************
	* @brief	( description ):  draw a filled triangle (do not forget to call UC1609C_update ();)
	* @param	( options ):	1- coordinate x1
								2- coordinate y1
								3- coordinate x2
								4- coordinate y2
								5- coordinate x3
								6- coordinate y3
								7- pixel view ( FOREGROUND or BACKGROUND  or INVERSE )
	* @return  	( returns ):	
	******************************************************************************
*/
void UC1609C_DrawFilledTriangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint8_t color);
//----------------------------------------------------------------------------------


/*
	******************************************************************************
	* @brief	( description ):  draw a hollow circle (do not forget to call UC1609C_update ();)
	* @param	( options ):	1- coordinate x1
								2- coordinate y1
								3- radius
								4- pixel view ( FOREGROUND or BACKGROUND  or INVERSE )
	* @return  	( returns ):	
	******************************************************************************
*/
void UC1609C_DrawCircle(int16_t x0, int16_t y0, int16_t r, uint8_t c);
//----------------------------------------------------------------------------------


/*
	******************************************************************************
	* @brief	( description ):  draw a filled circle (do not forget to call UC1609C_update ();)
	* @param	( options ):	1- coordinate x1
								2- coordinate y1
								3- radius
								4- pixel view ( FOREGROUND or BACKGROUND  or INVERSE )
	* @return  	( returns ):	
	******************************************************************************
*/
void UC1609C_DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, uint8_t c);
//----------------------------------------------------------------------------------

	
#ifdef __cplusplus
}
#endif

#endif	/*	_UC1609C_H */

/************************ (C) COPYRIGHT GKP *****END OF FILE****/
