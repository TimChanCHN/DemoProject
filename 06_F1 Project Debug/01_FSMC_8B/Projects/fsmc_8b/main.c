#include "board_config.h"
#include "nt_shell_handler.h"
#include "tft_lcd_4_3_handler.h"
//#include "lcd_handler.h"

static gpio_object_t m_gpio_led = 
{
	.gpio_port_periph_clk 	= RCC_APB2Periph_GPIOB,
	.p_gpio_port			= GPIOB,
	.gpio_pin				= GPIO_Pin_0,
	.gpio_dir				= GPIO_DIR_OUTPUR,
};


// Black Light-->PD12
// RST --> PE1
int main(void)
{
	mid_system_tick_init();
	gpio_config(&m_gpio_led);

	tftlcd_4_3_init();
	test_UI();
	// gpio_config(&m_backlight);
	// gpio_output_set(&m_backlight, 1);

	//	LCD_Init();
	// init_ntshell();

	while(1)
	{
		gpio_output_set(&m_gpio_led, 0);
		delay_ms(1000);
		gpio_output_set(&m_gpio_led, 1);
		delay_ms(1000);
	}

}

