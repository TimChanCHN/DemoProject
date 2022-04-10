#include "board_config.h"
// #include "nt_shell_handler.h"
//#include "tft_lcd_4_3_handler.h"
//#include "lcd_handler.h"
// #include "fsmc_8b.h"
#include "letter_shell_handler.h"

static gpio_object_t m_gpio_led = 
{
	.gpio_port_periph_clk 	= RCC_APB2Periph_GPIOB,
	.p_gpio_port			= GPIOB,
	.gpio_pin				= GPIO_Pin_0,
	.gpio_dir				= GPIO_DIR_OUTPUR,
};


int main(void)
{
	mid_system_tick_init();
	gpio_config(&m_gpio_led);
	gpio_output_set(&m_gpio_led, 1);

	init_letter_shell();

	uint32_t i = mid_get_systick();
	uint8_t flag = 0;

	Shell shell = get_shell();

	while(1)
	{
		#if 0
		if (mid_get_systick() - i > 1000)
		{
			flag = !flag;
			i = mid_get_systick();
		}

		if (flag)
		{
			gpio_output_set(&m_gpio_led, 0);
		}
		else
		{
			gpio_output_set(&m_gpio_led, 1);
		}
		#else
		gpio_output_set(&m_gpio_led, 0);
		delay_ms(1000);
		gpio_output_set(&m_gpio_led, 1);
		delay_ms(1000);
		#endif
	}

}

