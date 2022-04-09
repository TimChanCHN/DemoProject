#ifndef __TFT_LCD_HANDLER_H
#define __TFT_LCD_HANDLER_H


void tft_lcd_init(void);
void Display_UI1(void);
void Display_Dynamic(void);
void test_new_func(void);
void db_value_update(char ch, char val);
void temp_value_update(char val);
void voice_level_update(char ch, char rate);
void temp_level_update(char rate);

#endif

