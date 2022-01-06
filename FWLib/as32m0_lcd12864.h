#include <as32m0.h>
#include <as32m0_gpio.h>
#include <as32m0_misc.h>

#define LCD_CS		GPIOA_Pin_4		
#define LCD_SID		GPIOA_Pin_5
#define LCD_CLK		GPIOA_Pin_6
#define LCD_RSTn	GPIOA_Pin_7
void LCDSendByte(uint8_t byte);
void LCDInit(void);
void LCDWriteCom(uint8_t com);
void LCDWriteData(uint8_t data);
void address12864(uint8_t row, uint8_t col);
void stringDisp(uint8_t row, uint8_t *string, uint8_t len);
void selectDisp(uint8_t m, uint8_t row, uint8_t col, uint8_t size, uint8_t *dat);
void Displayontrl(uint8_t displayon, uint8_t cursoron, uint8_t blinkon);
void ChangeToBasicCMD(void);
void LCDClear(void);
void LCDdisplay_128x64(uint8_t *data);

