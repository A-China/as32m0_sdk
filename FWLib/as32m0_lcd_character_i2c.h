#include "as32m0.h"
#include "as32m0_i2c.h"
#include "as32m0_pinctrl.h"
#include "as32m0_misc.h"
#define LCD_char_ADDR 0x27 // 7-bit address
// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x01
#define LCD_ENTRYSHIFTINCREMENT 0x02
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00
// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00
#define En 0x4  // Enable bit
#define Rw 0x2  // Read/Write bit
#define Rs 0x1  // Register select bit

void LCD_char_write4bit(uint8_t data);
void LCD_char_send(uint8_t data, uint8_t mode);
void LCD_char_backlight_on(void);
void LCD_char_backlight_off(void);
void LCD_char_command(uint8_t data);
void LCD_char_write(uint8_t data);
void LCD_char_init(uint8_t number_of_line, uint8_t font);
void LCD_char_display_clear(void);
void LCD_char_display(void);
void LCD_char_displayon(void);
void LCD_char_displayoff(void);
void LCD_char_return_home(void);
void LCD_char_set_blinkon(void);
void LCD_char_set_blinkoff(void);
void LCD_char_set_cursoron(void);
void LCD_char_set_cursoroff(void);
void LCD_char_entrymode_increment(void);
void LCD_char_entrymode_decrement(void);
void LCD_char_entrymode_toright(void);
void LCD_char_entrymode_toleft(void);
void LCD_char_shiftLeft(void);
void LCD_char_shiftRight(void);
void LCD_char_setCursor(uint8_t col, uint8_t row);
void LCD_char_printstr(uint8_t *str, uint8_t len);

