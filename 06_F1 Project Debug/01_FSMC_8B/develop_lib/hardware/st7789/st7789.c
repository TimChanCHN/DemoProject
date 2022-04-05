#include "st7789.h"

int st7789_send_byte(lcd_drv_t *p_dev, uint8_t data)
{
    IS_NULL(p_dev);
	#if defined (CONFIG_CTRL_GPIO)
    #ifndef LCD_DEBUG
    p_dev->ctrl_gpio.db_port->BSRR = (data & 0xFF);
    p_dev->ctrl_gpio.db_port->BRR  = (~(data) & 0xFF);

    #else
    GPIOB->BSRR &= ~(0x0f << 12);
    GPIOB->BRR  &= ~(0x0f << 12);
    GPIOC->BSRR &= ~(0x0f << 6);
    GPIOC->BRR  &= ~(0x0f << 6);

    uint8_t tmp1  = (data & 0x0f);
    uint8_t tmp2  = (~(data & 0x0f) & 0x0f);
    GPIOB->BSRR  |= ((tmp1) << 12);
    GPIOB->BRR   |= ((tmp2) << 12);

    tmp1 = ((data & 0xf0) >> 4);
    tmp2 = ((~(data & 0xf0) & 0xf0) >> 4);
    GPIOC->BSRR  |= ((tmp1) << 6);
    GPIOC->BRR   |= ((tmp2) << 6);
    #endif
	#endif
	
	return 0;
}

int st7789_write_cmd(lcd_drv_t *p_dev, uint16_t cmd)
{
    #if 0
    IS_NULL(p_dev);

    #if defined (CONFIG_CTRL_GPIO)
    gpio_output_set(&p_dev->ctrl_gpio.cs_pin, 0);
    gpio_output_set(&p_dev->ctrl_gpio.rs_pin, 0);             // rs = 0, send cmd
    gpio_output_set(&p_dev->ctrl_gpio.wr_pin, 0);
    st7789_send_byte(p_dev, (uint8_t)cmd);
    gpio_output_set(&p_dev->ctrl_gpio.wr_pin, 1);
    gpio_output_set(&p_dev->ctrl_gpio.cs_pin, 1);

    #elif defined (CONFIG_CTRL_FSMC)
    p_dev->lcd_addr->lcd_cmd = (uint8_t)cmd;
    #endif
    #else
    LCD->lcd_cmd = (uint8_t)cmd;
    #endif
	
	return 0;
}

int st7789_write_data(lcd_drv_t *p_dev, uint16_t data)
{
    #if 0
    IS_NULL(p_dev);

    #if defined (CONFIG_CTRL_GPIO)
    gpio_output_set(&p_dev->ctrl_gpio.cs_pin, 0);
    gpio_output_set(&p_dev->ctrl_gpio.rs_pin, 1);             // rs = 1, send data
    gpio_output_set(&p_dev->ctrl_gpio.wr_pin, 0);
    st7789_send_byte(p_dev, (uint8_t)data);
    gpio_output_set(&p_dev->ctrl_gpio.wr_pin, 1);
    gpio_output_set(&p_dev->ctrl_gpio.cs_pin, 1);
	
    #elif defined (CONFIG_CTRL_FSMC)
    p_dev->lcd_addr->lcd_data = (uint8_t)data;
    #endif
    #else 
    LCD->lcd_data = (uint8_t)data;
    #endif

	return 0;
}

int st7789_write_burst_data(lcd_drv_t *p_dev, uint8_t *buff, uint16_t count)
{
    IS_NULL(p_dev);

	#if defined (CONFIG_CTRL_GPIO)
    gpio_output_set(&p_dev->ctrl_gpio.cs_pin, 0);
    gpio_output_set(&p_dev->ctrl_gpio.rs_pin, 1);             // rs = 1, send data

    for (uint16_t i = 0; i < count; i++)
    {
        gpio_output_set(&p_dev->ctrl_gpio.wr_pin, 0);
        st7789_send_byte(p_dev, buff[i]);
        gpio_output_set(&p_dev->ctrl_gpio.wr_pin, 1);
    }
    gpio_output_set(&p_dev->ctrl_gpio.wr_pin, 1);
    gpio_output_set(&p_dev->ctrl_gpio.cs_pin, 1);
	#endif
	
	return 0;
}

int st7789_init(driver_info_t *p_drv)
{
    // 1. 初始化管脚
    // 1.1 GPIO控制方式

    IS_NULL(p_drv);
    lcd_drv_t *p_dev = p_drv->dev;

#if 0
    #if defined (CONFIG_CTRL_GPIO)
    #ifdef  LCD_DEBUG
    gpio_config(&p_dev->ctrl_gpio.gpio_port1);
    gpio_config(&p_dev->ctrl_gpio.gpio_port2);

    gpio_output_set(&p_dev->ctrl_gpio.gpio_port1, 0);
    gpio_output_set(&p_dev->ctrl_gpio.gpio_port2, 0);
    #else

    for (uint8_t i = 0; i < 8; i++)
    {
        gpio_config(&p_dev->ctrl_gpio.db[i]);
        gpio_output_set(&p_dev->ctrl_gpio.db[i], 0);
    }
    #endif

    gpio_config(&p_dev->ctrl_gpio.cs_pin);
    gpio_config(&p_dev->ctrl_gpio.rst_pin);
    gpio_config(&p_dev->ctrl_gpio.rs_pin);
    gpio_config(&p_dev->ctrl_gpio.wr_pin);
    gpio_config(&p_dev->ctrl_gpio.rd_pin);

    gpio_output_set(&p_dev->ctrl_gpio.cs_pin, 1);
    gpio_output_set(&p_dev->ctrl_gpio.rst_pin, 1);
    gpio_output_set(&p_dev->ctrl_gpio.rs_pin, 1);
    gpio_output_set(&p_dev->ctrl_gpio.wr_pin, 1);
    gpio_output_set(&p_dev->ctrl_gpio.rd_pin, 1);

    #elif defined (CONFIG_CTRL_FSMC)

    // 1.2 FSMC控制方式
    gpio_config(&p_dev->ctrl_fsmc.rst_pin);
    gpio_config(&p_dev->ctrl_fsmc.back_light);
    gpio_config(&p_dev->ctrl_fsmc.fsmc_pin1);
    gpio_config(&p_dev->ctrl_fsmc.fsmc_pin2);

    fsmc_conig(0, BUS_WIDTH_8b);

	gpio_output_set(&p_dev->ctrl_fsmc.rst_pin, 0);
    delay_ms(1000);
    gpio_output_set(&p_dev->ctrl_fsmc.rst_pin, 1);

    p_dev->lcd_base_addr = ((uint32_t)((FSMC_BASE_ADDR + BANK1_SECTOR1_OFFSET) | BANK_8B_A16_OFFSET));
    p_dev->lcd_addr = ((LCD_Type_8b_t *)(p_dev->lcd_base_addr));
	

    #endif
#else
// 管脚分配:
/*
    D0 -- PD14
    D1 -- PD15
    D2 -- PD0
    D3 -- PD1
    D4 -- PE7
    D5 -- PE8
    D6 -- PE9
    D7 -- PE10

    D8 -- PE11
    D9 -- PE12
    D10 -- PE13
    D11 -- PE14
    D12 -- PE15
    D13 -- PD8
    D14 -- PD9
    D15 -- PD10

    RD/NOE -- PD4
    WD/NEW -- PD5
    RS/A16 -- PD11
    CS/NE1 -- PD7

    BL -- PB12
    RST -- PB6
 */
    GPIO_InitTypeDef GPIO_InitStructure;
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
    FSMC_NORSRAMTimingInitTypeDef  readWriteTiming; 
	FSMC_NORSRAMTimingInitTypeDef  writeTiming;
	
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC,ENABLE);	//使能FSMC时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE,ENABLE);//使能PORTB,D,E,G以及AFIO复用功能时钟

 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				 //PE1 RST
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

 	//PORTD复用推挽输出  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7|GPIO_Pin_11|GPIO_Pin_14|GPIO_Pin_15;				 //	//PORTD复用推挽输出  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //复用推挽输出   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure); 
    
	//PORTE复用推挽输出  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;				 //	//PORTE复用推挽输出  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //复用推挽输出   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);    	    	 											 

    // HCLK = 72Mhz, 14ns
	readWriteTiming.FSMC_AddressSetupTime = 0x07;	 //地址建立时间（ADDSET）
    readWriteTiming.FSMC_AddressHoldTime = 0x00;	 //地址保持时间（ADDHLD）模式A未用到	
    readWriteTiming.FSMC_DataSetupTime = 26;		 // 数据保存时间为16个HCLK,因为液晶驱动IC的读数据的时候，速度不能太快，尤其对1289这个IC。
    readWriteTiming.FSMC_BusTurnAroundDuration = 0x00;
    readWriteTiming.FSMC_CLKDivision = 0x00;
    readWriteTiming.FSMC_DataLatency = 0x00;
    readWriteTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //模式A 
    

	writeTiming.FSMC_AddressSetupTime = 0x04;	 //地址建立时间（ADDSET）为1个HCLK  
    writeTiming.FSMC_AddressHoldTime = 0x00;	 //地址保持时间（A		
    writeTiming.FSMC_DataSetupTime = 8;		 ////数据保存时间为4个HCLK	
    writeTiming.FSMC_BusTurnAroundDuration = 0x00;
    writeTiming.FSMC_CLKDivision = 0x00;
    writeTiming.FSMC_DataLatency = 0x00;
    writeTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //模式A 

    // BTCR为寄存器BCRx,BTRx, x=(NE1~NE4)
    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;//  这里我们使用NE1 ，也就对应BTCR[0],[1]。
    // BCRx[1] --> MUXEN地址/数据复用使能位
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable; // 不复用数据地址
    // BCRx[3:2] --> MTYP存储器类型
    FSMC_NORSRAMInitStructure.FSMC_MemoryType =FSMC_MemoryType_SRAM;// FSMC_MemoryType_SRAM;  //SRAM   
    // BCRx[5:4] --> MWID：存储器数据总线宽度
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_8b;//存储器数据宽度为16bit   
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode =FSMC_BurstAccessMode_Disable;// FSMC_BurstAccessMode_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
    FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;   
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;  
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;	//  存储器写使能
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;   
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable; // 读写使用不同的时序
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &readWriteTiming; //读写时序
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &writeTiming;  //写时序

    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  //初始化FSMC配置

 	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);  // 使能BANK1 

	delay_ms(50);           // delay 50 ms 
    //复位
    set_gpio_value(GPIOE, GPIO_Pin_1, 0);
    delay_ms(1000);
    set_gpio_value(GPIOE, GPIO_Pin_1, 1);


#endif

    
    // 

    st7789_write_cmd(p_dev, 0x36);      // 显示扫描方向
    st7789_write_data(p_dev, 0xA0);     // 00:从左到右，从上到下
                                        // A0:从下到上，从左到右
                                        // 60:从上到下，从右到左

    st7789_write_cmd(p_dev, 0x3A);    // color format
    st7789_write_data(p_dev, 0x05);    // 16bit/pixel

    st7789_write_cmd(p_dev, 0xB2);    // Porch Setting  
    st7789_write_data(p_dev, 0x0C);   
    st7789_write_data(p_dev, 0x0C);   
    st7789_write_data(p_dev, 0x00);   
    st7789_write_data(p_dev, 0x33);   
    st7789_write_data(p_dev, 0x33);   

    st7789_write_cmd(p_dev, 0xB7);    // Gate Control
    st7789_write_data(p_dev, 0x35);   

    st7789_write_cmd(p_dev, 0xBB);    // VCOM  
    st7789_write_data(p_dev, 0x1C);   

    st7789_write_cmd(p_dev, 0xC0);    // LCM control   
    st7789_write_data(p_dev, 0x2C);   

    st7789_write_cmd(p_dev, 0xC2);    // VDV and VRH Command Enable 
    st7789_write_data(p_dev, 0x01);   

    st7789_write_cmd(p_dev, 0xC3);    //  VRH Set  
    st7789_write_data(p_dev, 0x0B);       

    st7789_write_cmd(p_dev, 0xC4);    // VDV Set
    st7789_write_data(p_dev, 0x20);   

    st7789_write_cmd(p_dev, 0xC6);    // Frame Rate    
    st7789_write_data(p_dev, 0x0F);   

    st7789_write_cmd(p_dev, 0xD0);    // Power Control 1
    st7789_write_data(p_dev, 0xA4);   
    st7789_write_data(p_dev, 0xA1);   

    st7789_write_cmd(p_dev, 0xE0);    // Positive Voltage     
    st7789_write_data(p_dev, 0xD0);   
    st7789_write_data(p_dev, 0x06);   
    st7789_write_data(p_dev, 0x0B);   
    st7789_write_data(p_dev, 0x0A);   
    st7789_write_data(p_dev, 0x09);   
    st7789_write_data(p_dev, 0x06);   
    st7789_write_data(p_dev, 0x2F);   
    st7789_write_data(p_dev, 0x44);   
    st7789_write_data(p_dev, 0x45);   
    st7789_write_data(p_dev, 0x18);   
    st7789_write_data(p_dev, 0x14);   
    st7789_write_data(p_dev, 0x14);   
    st7789_write_data(p_dev, 0x27);   
    st7789_write_data(p_dev, 0x2D);   

    st7789_write_cmd(p_dev, 0xE1);    // Negative Voltage    
    st7789_write_data(p_dev, 0xD0);   
    st7789_write_data(p_dev, 0x06);   
    st7789_write_data(p_dev, 0x0B);   
    st7789_write_data(p_dev, 0x0A);   
    st7789_write_data(p_dev, 0x09);   
    st7789_write_data(p_dev, 0x05);   
    st7789_write_data(p_dev, 0x2E);   
    st7789_write_data(p_dev, 0x43);   
    st7789_write_data(p_dev, 0x45);   
    st7789_write_data(p_dev, 0x18);   
    st7789_write_data(p_dev, 0x14);   
    st7789_write_data(p_dev, 0x14);   
    st7789_write_data(p_dev, 0x27);   
    st7789_write_data(p_dev, 0x2D);   

    st7789_write_cmd(p_dev, 0x21);    // Display Inversion On
    st7789_write_cmd(p_dev, 0x11);    // Sleep Out
    delay_ms(5);
    st7789_write_cmd(p_dev, 0x29);    // Display On

    LCD_Param_Setting(p_drv);
    LCD_Clear(p_drv);
	
	return 0;
}




