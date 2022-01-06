#include "stdlib.h"
#include "printf_config.h"
#include "as32m0_pwm.h"
#include "as32m0_gpio.h"
#include "as32m0_pinctrl.h"
#include "as32m0_ssp.h"
#include "UC1609C.h"
#include "bitmap.h"

/*
	LCD UC1609C connection:

	CS   ->   PB0 ( chip select )
	RST  ->   PB5 ( Reset )
	CD   ->   PB4 ( date / command )
	SCK	 ->   PB1
	SDA	 ->   PB3
	VDD  ->   +3.3V
	VSS	 ->   GND
	A	 ->   anode led ( +3.3V or PWM )
	K	 ->   katode led ( GND )

*/

void _sys_exit(void);


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
		printf("[%d] Check CMD Status busy \n",__LINE__);
	}	
	// APB_PINC->PB6.FuncSel = 0xC0; // SWITCH TO GPIO
	APB_PINC->PB6.FuncSel = 0xF0; // SWITCH TO GPIO
	return ret;
}

int main()
{
	// prepare printf (uart0)
	config_uart0(OSC_CLK_FREQ, UART_SETBAUD, UART0_PD23);
	UC1609C_init();
	
	printf("==> \n");
	printf("==> \n");
	printf("==> TEST LCD19264 \n");
	printf("==> \n");
	printf("==> \n");
	// wait
	while(apUART_Check_BUSY(APB_UART0) == 1);
	
	init_beep();
	while(1) {
		UC1609C_fillScreen(0x00);
		UC1609C_clearDisplay();
		UC1609C_clearBuffer();
		UC1609C_update();
		HAL_Delay(1000);
		
		UC1609C_allPixelsOn(1);
		HAL_Delay(1000);
		UC1609C_allPixelsOn(0);
		HAL_Delay(1000);
		
		UC1609C_fillScreen(0x55);
		HAL_Delay(1000);
		
		UC1609C_clearBuffer();
		
		UC1609C_drawPixel( 5, 5, FOREGROUND);	// FOREGROUND or BACKGROUND  or INVERSE	
		UC1609C_drawPixel( 6, 6, BACKGROUND);	// FOREGROUND or BACKGROUND  or INVERSE		
		UC1609C_drawPixel( 7, 7, INVERSE);		// FOREGROUND or BACKGROUND  or INVERSE	
		UC1609C_update();
		HAL_Delay(1000);
		
		UC1609C_clearBuffer();
		UC1609C_bitmap( 30, 20, 128, 27, logo, FOREGROUND);		// FOREGROUND or BACKGROUND  or INVERSE
		UC1609C_update();
		HAL_Delay(1000);
		
		for( int i = 0; i < 64; i++){
			UC1609C_scroll(i);
			HAL_Delay ( 100 );
		}
		// 
		for( int i = 64; i >= 0; i--){
			UC1609C_scroll(i);
			HAL_Delay ( 100 );
		}
		
		UC1609C_contrast (100); // 0....255 ( def 30 )
	
		HAL_Delay ( 1000 );
		
		UC1609C_contrast (30); // 0....255 ( def 30 )
	
		HAL_Delay ( 1000 );
	
		//--------------
		UC1609C_invertDisplay( 1 );	// 
		
		HAL_Delay ( 1000 );
		
		UC1609C_invertDisplay( 0 );	// 
		HAL_Delay ( 1000 );
		//---------------
		
		// UC1609C_ROTATION_FLIP_TWO
		// UC1609C_ROTATION_NORMAL 
		// UC1609C_ROTATION_FLIP_ONE
		// UC1609C_ROTATION_FLIP_THREE 
		
		UC1609C_rotate( UC1609C_ROTATION_FLIP_TWO );
		HAL_Delay ( 1000 );
		
		UC1609C_rotate( UC1609C_ROTATION_FLIP_ONE );
		HAL_Delay ( 1000 );
		
		UC1609C_rotate( UC1609C_ROTATION_FLIP_THREE );
		HAL_Delay ( 1000 );
		
		UC1609C_rotate( UC1609C_ROTATION_NORMAL  );
		HAL_Delay ( 1000 );
		//------------------------------------------------------------------------------------
		
		//---------
		UC1609C_enable( 0 );	//
		
		HAL_Delay ( 1000 );
		
		UC1609C_enable( 1 );	//
		
		HAL_Delay ( 1000 );
		//----------
		
		UC1609C_clearDisplay();	// 
		UC1609C_clearBuffer();
		
		UC1609C_Print(4, 30, "UC1609C LCD Hello 1983 =)", &Font_6x8, 1, FOREGROUND);	// FOREGROUND or BACKGROUND  or INVERSE	
		UC1609C_update();		//
		
		HAL_Delay ( 1000 );
		UC1609C_clearBuffer();
		for( int16_t i = 0; i < 1001; i++){
			
			char buff [5];
			sprintf( buff, "%04d", i );
			
			UC1609C_Print(65, 25, buff, &Font_16x26, 1, FOREGROUND);	// FOREGROUND or BACKGROUND  or INVERSE	
			UC1609C_update();
			
			//HAL_Delay ( 1000 );
		}
		
//		UC1609C_DrawLine( 5, 5, 50, 50, FOREGROUND);							// FOREGROUND or BACKGROUND  or INVERSE	
//		UC1609C_DrawRectangle( 25, 15, 30, 30, FOREGROUND);					// FOREGROUND or BACKGROUND  or INVERSE	
//		UC1609C_DrawFilledRectangle( 85, 25, 20, 20, FOREGROUND);				// FOREGROUND or BACKGROUND  or INVERSE	
//		UC1609C_DrawTriangle( 20, 30, 40, 50, 50, 50, FOREGROUND);			// FOREGROUND or BACKGROUND  or INVERSE	
//		UC1609C_DrawFilledTriangle( 115, 35, 145, 30, 165, 60, FOREGROUND);	// FOREGROUND or BACKGROUND  or INVERSE	
//		UC1609C_DrawCircle( 10, 10, 40, FOREGROUND);							// FOREGROUND or BACKGROUND  or INVERSE	
//		UC1609C_DrawFilledCircle( 140, 30, 10, FOREGROUND);					// FOREGROUND or BACKGROUND  or INVERSE	
//		
		UC1609C_update();	
		
		HAL_Delay ( 1000 );
	}
	
	
	//-------  System Config ----------
	
	
  printf("\n");
	printf("\n");
	printf("==> TEST LCD19264 PASSED \n");
	printf("\n");
	printf("\n");
	printf("#"); // invoke TB $finish;
	_sys_exit();
 	return 0x12;
}

