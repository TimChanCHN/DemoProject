#include "fsmc_8b.h"
#include "board_config.h"

#define LCD_WIDTH   320
#define LCD_HEIGHT  240
#define CMD_WRAM    0x2C
#define CMD_SETX    0x2A
#define CMD_SETY    0x2B

static lcd_drv_t m_fsmc_8b_obj = 
{
    .ctrl_fsmc.rst_pin.gpio_port_periph_clk = RESET_CLK,
    .ctrl_fsmc.rst_pin.p_gpio_port          = RESET_PORT,
    .ctrl_fsmc.rst_pin.gpio_dir             = GPIO_DIR_OUTPUR,
    .ctrl_fsmc.rst_pin.gpio_pin             = RESET_PIN,
    .ctrl_fsmc.rst_pin.flag                 = MODE_IO,

    .ctrl_fsmc.back_light.gpio_port_periph_clk = BACK_LIGHT_CLK,
    .ctrl_fsmc.back_light.p_gpio_port          = BACK_LIGHT_PORT,
    .ctrl_fsmc.back_light.gpio_dir             = GPIO_DIR_OUTPUR,
    .ctrl_fsmc.back_light.gpio_pin             = BACK_LIGHT_PIN,
    .ctrl_fsmc.back_light.flag                 = MODE_IO,

    .ctrl_fsmc.fsmc_pin1.gpio_port_periph_clk = NT35510_PORTD_CLK,
    .ctrl_fsmc.fsmc_pin1.p_gpio_port          = NT35510_PORTD,
    .ctrl_fsmc.fsmc_pin1.gpio_dir             = GPIO_DIR_OUTPUR,
    .ctrl_fsmc.fsmc_pin1.gpio_pin             = NT35510_PORTD_AF_PIN,
    .ctrl_fsmc.fsmc_pin1.flag                 = MODE_AF,

    .ctrl_fsmc.fsmc_pin2.gpio_port_periph_clk = NT35510_PORTE_CLK,
    .ctrl_fsmc.fsmc_pin2.p_gpio_port          = NT35510_PORTE,
    .ctrl_fsmc.fsmc_pin2.gpio_dir             = GPIO_DIR_OUTPUR,
    .ctrl_fsmc.fsmc_pin2.gpio_pin             = NT35510_PORTE_AF_PIN,
    .ctrl_fsmc.fsmc_pin2.flag                 = MODE_AF,

};


static lcd_dev_t    m_lcd_obj = 
{
    .width      =   LCD_WIDTH,
    .height     =   LCD_HEIGHT,
    .id         =   0,
    .dir        =   0,
    .wramcmd    =   CMD_WRAM,
    .setxcmd    =   CMD_SETX,
    .setycmd    =   CMD_SETY,
};

static driver_info_t m_fsmc_8b_display_obj = 
{
    .dev                =   &m_fsmc_8b_obj,
    .lcd_param          =   &m_lcd_obj,
    .init               =   st7789_init,
    .write_cmd          =   st7789_write_cmd,
    .write_data         =   st7789_write_data,
    // .write_burst_data   =   nt35510_write_burst_data,
    // .write_reg          =   nt35510_write_reg,
    // .read_data          =   nt35510_read_data,
    // .read_reg           =   nt35510_read_reg,
    .point_color        =   YELLOW,
    .background_color   =   BLUE,
};



void fsmc_8b_init(void)
{
    m_fsmc_8b_display_obj.init(&m_fsmc_8b_display_obj);
}


void test_UI(void)
{
    
}
