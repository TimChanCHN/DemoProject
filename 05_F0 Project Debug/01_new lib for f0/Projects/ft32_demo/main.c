#include "board_config.h"

#include "tft_lcd_handler.h"


int main(void)
{
	mid_system_tick_init();
	
	tft_lcd_init();
	// Display_UI1();
	// Display_Dynamic();
	test_new_func();
	
	while(1)
	{
		
	}
}

