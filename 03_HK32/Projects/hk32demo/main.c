#include "board_config.h"

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

	while(1)
	{
		gpio_output_set(&m_gpio_led, 0);
		delay_ms(1000);
		gpio_output_set(&m_gpio_led, 1);
		delay_ms(1000);
	}

}

