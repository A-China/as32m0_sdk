#include "as32m0.h"
#include "as32m0_i2c.h"
#include "as32m0_pinctrl.h"

#if !defined SSD1306_128_32 && !defined SSD1306_96_16
#define SSD1306_128_64
#endif
#if defined SSD1306_128_64
  #define SSD1306_LCDWIDTH            128
  #define SSD1306_LCDHEIGHT            64
#endif
#if defined SSD1306_128_32
  #define SSD1306_LCDWIDTH            128
  #define SSD1306_LCDHEIGHT            32
#endif
#if defined SSD1306_96_16
  #define SSD1306_LCDWIDTH             96
  #define SSD1306_LCDHEIGHT            16
#endif

#define SSD1306_MEMORYMODE 0x20         
#define SSD1306_COLUMNADDR 0x21         
#define SSD1306_PAGEADDR 0x22           
#define SSD1306_SETCONTRAST 0x81        
#define SSD1306_CHARGEPUMP 0x8D         
#define SSD1306_SEGREMAP 0xA0 
#define SSD1306_SEGREMAP_INV 0xA1
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5       
#define SSD1306_NORMALDISPLAY 0xA6      
#define SSD1306_INVERTDISPLAY 0xA7      
#define SSD1306_SETMULTIPLEX 0xA8       
#define SSD1306_DISPLAYOFF 0xAE         
#define SSD1306_DISPLAYON 0xAF          
#define SSD1306_COMSCANINC 0xC0         
#define SSD1306_COMSCANDEC 0xC8         
#define SSD1306_SETDISPLAYOFFSET 0xD3   
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5 
#define SSD1306_SETPRECHARGE 0xD9       
#define SSD1306_SETCOMPINS 0xDA         
#define SSD1306_SETVCOMDETECT 0xDB      

#define SSD1306_RIGHT_HORIZONTAL_SCROLL 0x26              
#define SSD1306_LEFT_HORIZONTAL_SCROLL 0x27               
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29 
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A  
#define SSD1306_DEACTIVATE_SCROLL 0x2E                    
#define SSD1306_ACTIVATE_SCROLL 0x2F                      
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3             

void init_I2C_SSD1306(uint8_t taddr, uint8_t Speed, I2C_LOC i2c_loc);
void I2C_SSD1306_command(uint8_t command);
void I2C_SSD1306_data(uint8_t value);
void I2C_SSD1306_display_on(void);
void I2C_SSD1306_display_off(void);
void I2C_SSD1306_display_rotate(uint8_t r);		// rotate 0 or rotate 180
void I2C_SSD1306_chargepump_on(void);
void I2C_SSD1306_chargepump_off(void);
void I2C_SSD1306_set_precharge_period(uint8_t value);
void I2C_SSD1306_set_contrast(uint8_t value);
void I2C_SSD1306_setAddrmode(uint8_t value);
void I2C_SSD1306_setPageaddr(uint8_t start, uint8_t end);
void I2C_SSD1306_setColaddr(uint8_t start, uint8_t end);
void I2C_SSD1306_display_clear(void);
void I2C_SSD1306_initOLED(void);
void I2C_SSD1306_drawBitMap(uint8_t Bitmap []);
void I2C_SSD1306_drawBuffer(void);
void I2C_SSD1306_LeftHScrol(uint8_t start_page, uint8_t frame, uint8_t end_page);
void I2C_SSD1306_RightHScrol(uint8_t start_page, uint8_t frame, uint8_t end_page);
void I2C_SSD1306_StopScrol(void);
void I2C_SSD1306_InverseDisplay(uint8_t on);
