#include "tft_lcd_4_3_handler.h"
#include "board_config.h"


#define LCD_WIDTH   480
#define LCD_HEIGHT  800
#define CMD_WRAM    0X2C00
#define CMD_SETX    0X2A00
#define CMD_SETY    0x2B00


static lcd_drv_t m_nt35510_obj = 
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

static driver_info_t m_tftlcd_4_3_display_obj = 
{
    .dev                =   &m_nt35510_obj,
    .lcd_param          =   &m_lcd_obj,
    .init               =   nt35510_init,
    .write_cmd          =   nt35510_write_cmd,
    .write_data         =   nt35510_write_data,
    // .write_burst_data   =   nt35510_write_burst_data,
    .write_reg          =   nt35510_write_reg,
    .read_data          =   nt35510_read_data,
    .read_reg           =   nt35510_read_reg,
    .point_color        =   YELLOW,
    .background_color   =   BLACK,
};

void tftlcd_4_3_init(void)
{
    m_tftlcd_4_3_display_obj.init(&m_tftlcd_4_3_display_obj);
}


void test_UI(void)
{

    char str[] = "I love funa Chan";

    string_info_t string_info = 
    {
        .str        = str,
        .size       = 24,  
        .width      = m_lcd_obj.width, 
        .height     = 24,
        .coord      = {10, 10},
    };

    rect_info_t tmp_rect = 
    {
        .width   = 5,
        .coord_s = {210, 210},
        .coord_e = {320, 320},
        .color   = GREEN,
    };

    line_info_t tmp_line = 
    {
        .width   = 5,
        .coord_s = {100, 100},
        .coord_e = {200, 200},
        .color   = RED,
    };
    
    LCD_ShowString(&m_tftlcd_4_3_display_obj, string_info);
    LCD_DrawRectangle(&m_tftlcd_4_3_display_obj, tmp_rect);
    LCD_DrawLine(&m_tftlcd_4_3_display_obj, tmp_line);
}
