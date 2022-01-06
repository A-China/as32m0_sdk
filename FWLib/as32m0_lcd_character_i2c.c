#include <as32m0_lcd_character_i2c.h>

static uint8_t backlight_val = LCD_BACKLIGHT;
static uint8_t displaycontrol_val = LCD_DISPLAYOFF;
static uint8_t entry_mode_val = LCD_ENTRYSHIFTINCREMENT;

void LCD_char_write4bit(uint8_t data){
	APB_I2C->IC_DATA_CMD = data | backlight_val;//En=0, D7-D4 = data
	while((APB_I2C->IC_STATUS & IC_STATUS_TFE) == 0x0);
	APB_I2C->IC_DATA_CMD = data | En | backlight_val;//En=1, D7-D4 = data
	while((APB_I2C->IC_STATUS & IC_STATUS_TFE) == 0x0);
	delay_ms(1);
	APB_I2C->IC_DATA_CMD = data | backlight_val;//En=0, D7-D4 = data
	while((APB_I2C->IC_STATUS & IC_STATUS_TFE) == 0x0);	
	delay_us(3700);
}

void LCD_char_send(uint8_t data, uint8_t mode){
	uint8_t temp;
	temp = (data & 0xF0);
	LCD_char_write4bit(temp | mode );
	temp = (data <<4) & 0xF0; 
	LCD_char_write4bit(temp | mode);
}
void LCD_char_backlight_on(void){
	backlight_val = LCD_BACKLIGHT;
  APB_I2C->IC_DATA_CMD = backlight_val;//En=0, D7-D4 = data
  while((APB_I2C->IC_STATUS & IC_STATUS_TFE) == 0x0);	
	delay_ms(50);
}

void LCD_char_backlight_off(void){
	backlight_val = LCD_NOBACKLIGHT;
  APB_I2C->IC_DATA_CMD = backlight_val;//En=0, D7-D4 = data
  while((APB_I2C->IC_STATUS & IC_STATUS_TFE) == 0x0);	
	delay_ms(50);
}

void LCD_char_command(uint8_t data){
	LCD_char_send(data, 0);
}

void LCD_char_write(uint8_t data){
	LCD_char_send(data, 1);
}

void LCD_char_init(uint8_t number_of_line, uint8_t font){
  APB_I2C->IC_DATA_CMD = 0x00;//En=0, D7-D4 = 0 Rs=0
  while((APB_I2C->IC_STATUS & IC_STATUS_TFE) == 0x0);
	delay_ms(1);
	LCD_char_write4bit(0x03<<4);
	delay_ms(5);
	LCD_char_write4bit(0x03<<4);
	delay_ms(1);	
	LCD_char_write4bit(0x03<<4);
	delay_us(1500);
	LCD_char_write4bit(0x02<<4);
	LCD_char_command(LCD_FUNCTIONSET | number_of_line | font);
	LCD_char_displayoff();
	LCD_char_display_clear();
	LCD_char_entrymode_increment();
	LCD_char_displayon();
	LCD_char_return_home();
}
	
void LCD_char_display_clear(void){
	LCD_char_command(LCD_CLEARDISPLAY);
	delay_ms(2000);
}
void LCD_char_display(void){
	LCD_char_command(LCD_DISPLAYCONTROL | displaycontrol_val);
	delay_ms(50);
}
void LCD_char_displayon(void){
	displaycontrol_val |= LCD_DISPLAYON;
  LCD_char_display();		
}
void LCD_char_displayoff(void){
	displaycontrol_val &= ~LCD_DISPLAYON;
  LCD_char_display();	
}
void LCD_char_return_home(void){
	LCD_char_command(LCD_RETURNHOME);
}

void LCD_char_set_blinkon(void){
	displaycontrol_val |= LCD_BLINKON;
  LCD_char_display();	
}
void LCD_char_set_blinkoff(void){
	displaycontrol_val &= ~LCD_BLINKON;
  LCD_char_display();		
}
void LCD_char_set_cursoron(void){
	displaycontrol_val |= LCD_CURSORON;
  LCD_char_display();		
}
void LCD_char_set_cursoroff(void){
	displaycontrol_val &= ~LCD_CURSORON;
  LCD_char_display();			
}

void LCD_char_entrymode_increment(void){
	entry_mode_val |= LCD_ENTRYSHIFTINCREMENT;
	LCD_char_command(LCD_ENTRYMODESET | entry_mode_val);
	delay_ms(50);	
}
void LCD_char_entrymode_decrement(void){
	entry_mode_val &= ~LCD_ENTRYSHIFTINCREMENT;
	LCD_char_command(LCD_ENTRYMODESET | entry_mode_val);
	delay_ms(50);		
}
void LCD_char_entrymode_toright(void){
	entry_mode_val &= ~LCD_ENTRYLEFT;
	LCD_char_command(LCD_ENTRYMODESET | entry_mode_val);
	delay_ms(50);		
}
void LCD_char_entrymode_toleft(void){
	entry_mode_val |= LCD_ENTRYLEFT;
	LCD_char_command(LCD_ENTRYMODESET | entry_mode_val);
	delay_ms(50);	
}

void LCD_char_shiftLeft(void){
	LCD_char_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void LCD_char_shiftRight(void){
	LCD_char_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}
void LCD_char_setCursor(uint8_t col, uint8_t row){
	uint8_t temp;
	if(col == 0)
		temp = row;
	else
		temp = 0x40 + row;
	
	LCD_char_command(LCD_SETDDRAMADDR | temp);
}

void LCD_char_printstr(uint8_t *str, uint8_t len){
 uint8_t i;
 for(i=0; i<len; i++){
		LCD_char_write(*str);
	  str++;
 }

}
