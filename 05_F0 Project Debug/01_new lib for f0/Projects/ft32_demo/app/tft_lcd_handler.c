#include "tft_lcd_handler.h"
#include "board_config.h"
#include "picture.h"

#define LCD_WIDTH   320
#define LCD_HEIGHT  240
#define CMD_WRAM    0X2C
#define CMD_SETX    0X2A
#define CMD_SETY    0x2B

static lcd_drv_t    m_st7789_obj = 
{
    // CS--PC0, RESET--PC1, RS--PC2, WR--PC3, RD--PC4
    .ctrl_gpio.cs_pin.gpio_port_periph_clk    = ST7789_CS_PORT_PERIPH_CLK,
    .ctrl_gpio.cs_pin.p_gpio_port             = ST7789_CS_PORT,
    .ctrl_gpio.cs_pin.gpio_dir                = GPIO_DIR_OUTPUR,
    .ctrl_gpio.cs_pin.gpio_pin                = ST7789_CS_PIN,
    .ctrl_gpio.cs_pin.flag                    = MODE_IO,

    .ctrl_gpio.rst_pin.gpio_port_periph_clk   = ST7789_RST_PORT_PERIPH_CLK,
    .ctrl_gpio.rst_pin.p_gpio_port            = ST7789_RST_PORT,
    .ctrl_gpio.rst_pin.gpio_dir               = GPIO_DIR_OUTPUR,
    .ctrl_gpio.rst_pin.gpio_pin               = ST7789_RST_PIN,
    .ctrl_gpio.rst_pin.flag                    = MODE_IO,

    .ctrl_gpio.rs_pin.gpio_port_periph_clk    = ST7789_RS_PORT_PERIPH_CLK,
    .ctrl_gpio.rs_pin.p_gpio_port             = ST7789_RS_PORT,
    .ctrl_gpio.rs_pin.gpio_dir                = GPIO_DIR_OUTPUR,
    .ctrl_gpio.rs_pin.gpio_pin                = ST7789_RS_PIN,
    .ctrl_gpio.rs_pin.flag                    = MODE_IO,

    .ctrl_gpio.wr_pin.gpio_port_periph_clk    = ST7789_WR_PORT_PERIPH_CLK,
    .ctrl_gpio.wr_pin.p_gpio_port             = ST7789_WR_PORT,
    .ctrl_gpio.wr_pin.gpio_dir                = GPIO_DIR_OUTPUR,
    .ctrl_gpio.wr_pin.gpio_pin                = ST7789_WR_PIN,
    .ctrl_gpio.wr_pin.flag                    = MODE_IO,

    .ctrl_gpio.rd_pin.gpio_port_periph_clk    = ST7789_RD_PORT_PERIPH_CLK,
    .ctrl_gpio.rd_pin.p_gpio_port             = ST7789_RD_PORT,
    .ctrl_gpio.rd_pin.gpio_dir                = GPIO_DIR_OUTPUR,
    .ctrl_gpio.rd_pin.gpio_pin                = ST7789_RD_PIN,
    .ctrl_gpio.rd_pin.flag                    = MODE_IO,

    // DB0--PB12, DB1--PB13, DB2--PB14, DB3--PB15, DB4--PC6, DB5--PC7, DB6--PC8, DB7--PC9
    .ctrl_gpio.gpio_port1.gpio_port_periph_clk    = ST7789_DB0_3_PORT_PERIPH_CLK,
    .ctrl_gpio.gpio_port1.p_gpio_port             = ST7789_DB0_3_PORT,
    .ctrl_gpio.gpio_port1.gpio_dir                = GPIO_DIR_OUTPUR,
    .ctrl_gpio.gpio_port1.gpio_pin                = ST7789_DB0_3_PIN,
    .ctrl_gpio.gpio_port1.flag                    = MODE_IO,

    .ctrl_gpio.gpio_port2.gpio_port_periph_clk    = ST7789_DB4_7_PORT_PERIPH_CLK,
    .ctrl_gpio.gpio_port2.p_gpio_port             = ST7789_DB4_7_PORT,
    .ctrl_gpio.gpio_port2.gpio_dir                = GPIO_DIR_OUTPUR,
    .ctrl_gpio.gpio_port2.gpio_pin                = ST7789_DB4_7_PIN,
    .ctrl_gpio.gpio_port2.flag                    = MODE_IO,

};  

// lcd屏幕参数
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

static driver_info_t m_tftlcd_display_obj = 
{
    .dev                =   &m_st7789_obj,
    .lcd_param          =   &m_lcd_obj,
    .init               =   st7789_init,
    .write_cmd          =   st7789_write_cmd,
    .write_data         =   st7789_write_data,
    .write_burst_data   =   st7789_write_burst_data,
    .point_color        =   RED,
    .background_color   =   BLACK,
};

// 矩形条信息，因为涉及动态显示，不能作为局部变量
static rect_info_t m_rect_info_ch1 = 
{
	.coord_s = {120, 75},
	.coord_e = {275, 105},
    .width = 3,
    .color = GREEN,
};

static rect_dynamic_info_t m_dynamic_rect_ch1 = 
{
    .rect = &m_rect_info_ch1,
    .rate = 0,
    .end_x = 0,
};

static rect_info_t m_rect_info_ch2 = 
{
	.coord_s = {120, 125},
	.coord_e = {275, 155},
    .width = 3,
    .color = BLUE,
};

static rect_dynamic_info_t m_dynamic_rect_ch2 = 
{
    .rect = &m_rect_info_ch2,
    .rate = 0,
    .end_x = 0,
};

// 温度标记条
static triangle_info_t m_triangle_info = 
{
    .coord   = {220, 230},
    .base    = 60,
    .height  = 25,
    .width   = 3,
    .color   = GREEN,
};

static triangle_fill_info_t m_fill_tri = 
{
    .triangle = &m_triangle_info,
    .rate = 88,
};

// 温度信息结构体
number_info_t m_temp_info = 
{
    .num = 28,
    .size = 16,
    .len = 2,
    .coord = {120, 218},
};

// 记录分贝大小
number_info_t m_num_ch1 = 
{
    .num = 68,
    .size = 16,
    .len = 2,
    .coord = {85, 82},
};

number_info_t m_num_ch2 = 
{
    .num = 65,
    .size = 16,
    .len = 2,
    .coord = {85, 133},
};


void tft_lcd_init(void)
{
    m_tftlcd_display_obj.init(&m_tftlcd_display_obj);
}

void set_string_coord(string_info_t *str, uint16_t x, uint16_t y)
{
    str->coord.x = x;
    str->coord.y = y;
}

void set_area_coord(line_info_t *area, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    area->coord_s.x = x1;
    area->coord_s.y = y1;
    area->coord_e.x = x2;
    area->coord_e.y = y2;
    
}

void db_value_update(char ch, char val)
{
    uint16_t tmp_color = m_tftlcd_display_obj.point_color;
    if (ch == 1)
    {
        m_tftlcd_display_obj.point_color = GREEN;
        m_num_ch1.num = val;
        LCD_ShowNum(&m_tftlcd_display_obj, m_num_ch1);
    }
    else if (ch == 2)
    {
        m_tftlcd_display_obj.point_color = BLUE;
        m_num_ch2.num = val;
        LCD_ShowNum(&m_tftlcd_display_obj, m_num_ch2);
    }
    m_tftlcd_display_obj.point_color = tmp_color;
}

void temp_value_update(char val)
{
    uint16_t tmp_color = m_tftlcd_display_obj.point_color;
    m_tftlcd_display_obj.point_color = GREEN;
    m_temp_info.num = val;
    LCD_ShowNum(&m_tftlcd_display_obj, m_temp_info);
    m_tftlcd_display_obj.point_color = tmp_color;
}

void temp_level_update(char rate)
{
    m_fill_tri.rate = rate;
    LCD_FillTriangle(&m_tftlcd_display_obj, &m_fill_tri);
}

void voice_level_update(char ch, char rate)
{
    if (ch == 1)
    {
        m_dynamic_rect_ch1.rate = rate;
        LCD_FillRectanglePercent(&m_tftlcd_display_obj, &m_dynamic_rect_ch1);
    }
    else if (ch == 2)
    {
        m_dynamic_rect_ch2.rate = rate;
        LCD_FillRectanglePercent(&m_tftlcd_display_obj, &m_dynamic_rect_ch2);
    }
}

void Display_UI1(void)
{
    string_info_t tmp_string = 
    {
        .size   = 16,  
        .width  = m_lcd_obj.width, 
        .height = 16,
    };

    rect_info_t tmp_rect = 
    {
        .width   = 3,
    };

    line_info_t tmp_line = 
    {
        .width   = 3,
    };

    /* 1. 固定布局 */
    char *s1 = "AC VOLTAGE   INPUT MODE";
    tmp_string.str = s1;
    set_string_coord(&tmp_string, 60, 10);
    m_tftlcd_display_obj.point_color = YELLOW;
    LCD_ShowString(&m_tftlcd_display_obj, tmp_string);

    char *s2 = "200V~240V";
    tmp_string.str = s2;
    set_string_coord(&tmp_string, 65, 35);
    m_tftlcd_display_obj.point_color = RED;
	LCD_ShowString(&m_tftlcd_display_obj, tmp_string);

    char *s3 = "PARALLEL";
    tmp_string.str = s3;
    set_string_coord(&tmp_string, 170, 35);
    m_tftlcd_display_obj.point_color = GBLUE;
	LCD_ShowString(&m_tftlcd_display_obj, tmp_string);

    char *s4 = "ANG1";
    tmp_string.str = s4;
    set_string_coord(&tmp_string, 15, 82);
    m_tftlcd_display_obj.point_color = GREEN;
    LCD_ShowString(&m_tftlcd_display_obj, tmp_string);

	/* 2. CH1 */
    set_area_coord(&tmp_rect, 10, 75, 50, 105);
    tmp_rect.color = GREEN;
    LCD_DrawRectangle(&m_tftlcd_display_obj, tmp_rect);

	//画线
    set_area_coord(&tmp_line, 50, 88, 68, 88);
    tmp_line.color = GREEN;
    LCD_DrawLine(&m_tftlcd_display_obj, tmp_line);

    set_area_coord(&tmp_line, 75, 88, 82, 88);
    tmp_line.color = GREEN;
    LCD_DrawLine(&m_tftlcd_display_obj, tmp_line);

    // 68dB
    char *s5 = "dB";
    tmp_string.str = s5;
    set_string_coord(&tmp_string, 102, 82);
    LCD_ShowString(&m_tftlcd_display_obj, tmp_string);

    char *s6 = "CH1";
    tmp_string.str = s6;
    set_string_coord(&tmp_string, 288, 82);
    LCD_ShowString(&m_tftlcd_display_obj, tmp_string);

    set_area_coord(&tmp_rect, 280, 75, 315, 105);
    tmp_rect.color = GREEN;
    LCD_DrawRectangle(&m_tftlcd_display_obj, tmp_rect);
    LCD_DrawRectangle(&m_tftlcd_display_obj, m_rect_info_ch1);


    /* 3. CH2 */
    tmp_line.color = BLUE;
    set_area_coord(&tmp_line, 60, 91, 60, 140);
    LCD_DrawLine(&m_tftlcd_display_obj, tmp_line);

    set_area_coord(&tmp_line, 60, 140, 68, 140);
    LCD_DrawLine(&m_tftlcd_display_obj, tmp_line);

    set_area_coord(&tmp_line, 75, 140, 82, 140);
    LCD_DrawLine(&m_tftlcd_display_obj, tmp_line);

    // 65dB
    m_tftlcd_display_obj.point_color = BLUE;

    char *s7 = "dB";
    tmp_string.str = s7;
    set_string_coord(&tmp_string, 102, 133);
    LCD_ShowString(&m_tftlcd_display_obj, tmp_string);


    char *s8 = "CH2";
    tmp_string.str = s8;
    set_string_coord(&tmp_string, 288, 133);
    LCD_ShowString(&m_tftlcd_display_obj, tmp_string);

    set_area_coord(&tmp_rect, 280, 125, 315, 155);
    tmp_rect.color = BLUE;
    LCD_DrawRectangle(&m_tftlcd_display_obj, tmp_rect);
    LCD_DrawRectangle(&m_tftlcd_display_obj, m_rect_info_ch2);

    // 温度显示
    m_tftlcd_display_obj.point_color = WHITE;
    char *s9 = "TEMP:";
    tmp_string.str = s9;
    set_string_coord(&tmp_string, 60, 218);
    LCD_ShowString(&m_tftlcd_display_obj, tmp_string);

    m_tftlcd_display_obj.point_color = GREEN;
    char *s10 = "C";
    tmp_string.str = s10;
    set_string_coord(&tmp_string, 160, 218);
    LCD_ShowString(&m_tftlcd_display_obj, tmp_string);

    bmp_info_t dot_info = 
    {
        .coord      = {138, 218},
        .size       = sizeof(gImage_dot)/sizeof(gImage_dot[0]),
        .height     = 20,
        .color      = GREEN,
        .bmp        = gImage_dot,
    };
    LCD_ShowBMP(&m_tftlcd_display_obj, dot_info);

    bmp_info_t fan_info = 
    {
        .coord      = {180, 210},
        .size       = sizeof(gImage_fan)/sizeof(gImage_fan[0]),
        .height     = 30,
        .color      = GREEN,
        .bmp        = gImage_fan,
    };
    // 风扇标志
    LCD_ShowBMP(&m_tftlcd_display_obj, fan_info);

    LCD_DrawTriangle(&m_tftlcd_display_obj, m_triangle_info);
}

void Display_Dynamic(void)
{
    uint8_t rate[] = {30, 34, 38, 42, 50, 46, 42, 38, 42, 46, 50, 42, 36, 30};
    uint8_t i = 0, j = 0;

    temp_value_update(33);
    db_value_update(1, 48);
    db_value_update(2, 99);
    temp_level_update(50);

    while (1)
    {
        voice_level_update(1, rate[i]);
        voice_level_update(2, rate[i]);
		i++;
		if (i >= (sizeof(rate)/sizeof(rate[0])))
		{
			i = 0;
            j++;
            if (j >= (sizeof(rate)/sizeof(rate[0])))
            {
                j = 0;
            }
            temp_value_update(rate[j]);
		}

		delay_ms(50);
    }
}

void test_new_func(void)
{
    line_info_t tmp_line = 
    {
        .coord_s = {0, 0},
        .coord_e = {319, 239},
        .width   = 3,
        .color   = RED,
    };

    LCD_DrawLine(&m_tftlcd_display_obj, tmp_line);
}