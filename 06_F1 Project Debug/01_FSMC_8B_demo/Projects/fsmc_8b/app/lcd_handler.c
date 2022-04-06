#include "lcd_handler.h"
#include "font.h"

//LCD的画笔颜色和背景色	   
uint16_t POINT_COLOR=0x0000;	//画笔颜色
uint16_t BACK_COLOR=0xFFFF;  //背景色 

//管理LCD重要参数
//默认为竖屏
_lcd_dev lcddev;

//写寄存器函数
//regval:寄存器值
void LCD_WR_REG(uint8_t regval)
{   
	LCD->LCD_REG=regval;//写入要写的寄存器序号	 
}
//写LCD数据
//data:要写入的值
void LCD_WR_DATA(uint8_t data)
{	 
	LCD->LCD_RAM=data;		
}
//读LCD数据
//返回值:读到的值
uint16_t LCD_RD_DATA(void)
{
	uint16_t ram;			//防止被优化
	ram=LCD->LCD_RAM;	
	return ram;	 
}					   
//写寄存器
//LCD_Reg:寄存器地址
//LCD_RegValue:要写入的数据
void LCD_WriteReg(uint16_t LCD_Reg,uint16_t LCD_RegValue)
{	
	LCD->LCD_REG = (uint8_t)LCD_Reg;		//写入要写的寄存器序号	 
	LCD->LCD_RAM = (uint8_t)LCD_RegValue;//写入数据	    	
}	   
//读寄存器
//LCD_Reg:寄存器地址
//返回值:读到的数据
uint16_t LCD_ReadReg(uint16_t LCD_Reg)
{										   
	LCD_WR_REG(LCD_Reg);		//写入要读的寄存器序号
	delay_us(5);		  
	return LCD_RD_DATA();		//返回读到的值
}   
//开始写GRAM
void LCD_WriteRAM_Prepare(void)
{
    LCD->LCD_REG=(uint8_t)lcddev.wramcmd;	
}	 
//LCD写GRAM
//RGB_Code:颜色值
void LCD_WriteRAM(uint16_t RGB_Code)
{							    
	LCD->LCD_RAM = (uint8_t)(RGB_Code >> 8);//写十六位GRAM
	LCD->LCD_RAM = (uint8_t)(RGB_Code & 0xff);//写十六位GRAM
}


//设置光标位置
//Xpos:横坐标
//Ypos:纵坐标
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{	 	
    LCD_WriteReg(lcddev.setxcmd, Xpos);
    LCD_WriteReg(lcddev.setycmd, Ypos); 
} 		 
    
//画点
//x,y:坐标
//POINT_COLOR:此点的颜色
void LCD_DrawPoint(uint16_t x,uint16_t y)
{
	LCD_SetCursor(x,y);		//设置光标位置 
	LCD_WriteRAM_Prepare();	//开始写入GRAM
	LCD->LCD_RAM = (uint8_t)(POINT_COLOR >> 8);//写十六位GRAM
	LCD->LCD_RAM = (uint8_t)(POINT_COLOR & 0xff);//写十六位GRAM
}


//设置LCD显示方向
//dir:0,竖屏；1,横屏
void LCD_Display_Dir(uint8_t dir)
{
	if(dir==0)			//竖屏
	{
		lcddev.dir=0;	//竖屏
		lcddev.width=240;
		lcddev.height=320;
	}
    else 				//横屏
	{	  				
		lcddev.dir=1;	//横屏
		lcddev.width=320;
		lcddev.height=240;
	} 
    lcddev.wramcmd=0X2C;
    lcddev.setxcmd=0X2A;
    lcddev.setycmd=0X2B;  

    LCD_WR_REG(lcddev.setxcmd); 
    LCD_WR_DATA(0);
    LCD_WR_DATA(0);
    LCD_WR_DATA((lcddev.width-1)>>8);
    LCD_WR_DATA((lcddev.width-1)&0XFF);

    LCD_WR_REG(lcddev.setycmd); 
    LCD_WR_DATA(0);
    LCD_WR_DATA(0);
    LCD_WR_DATA((lcddev.height-1)>>8);
    LCD_WR_DATA((lcddev.height-1)&0XFF);  

}	


//初始化lcd
//该初始化函数可以初始化各种ILI93XX液晶,但是其他函数是基于ILI9320的!!!
//在其他型号的驱动芯片上没有测试! 

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

void LCD_Init(void)
{ 					
    GPIO_InitTypeDef GPIO_InitStructure;
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
    FSMC_NORSRAMTimingInitTypeDef  readWriteTiming; 
	FSMC_NORSRAMTimingInitTypeDef  writeTiming;
	
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC,ENABLE);	//使能FSMC时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE,ENABLE);//使能PORTB,D,E,G以及AFIO复用功能时钟

 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				 //PD12 推挽输出 背光
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
	
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

	readWriteTiming.FSMC_AddressSetupTime = 0x02;	 //地址建立时间（ADDSET）为1个HCLK 1/72M=14ns
    readWriteTiming.FSMC_AddressHoldTime = 0x00;	 //地址保持时间（ADDHLD）模式A未用到	
    readWriteTiming.FSMC_DataSetupTime = 0x0f;		 // 数据保存时间为16个HCLK,因为液晶驱动IC的读数据的时候，速度不能太快，尤其对1289这个IC。
    readWriteTiming.FSMC_BusTurnAroundDuration = 0x00;
    readWriteTiming.FSMC_CLKDivision = 0x00;
    readWriteTiming.FSMC_DataLatency = 0x00;
    readWriteTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //模式A 
    

	writeTiming.FSMC_AddressSetupTime = 0x00;	 //地址建立时间（ADDSET）为1个HCLK  
    writeTiming.FSMC_AddressHoldTime = 0x00;	 //地址保持时间（A		
    writeTiming.FSMC_DataSetupTime = 0x03;		 ////数据保存时间为4个HCLK	
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
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_8b;//存储器数据宽度为8bit   
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

	delay_ms(500);           // delay 50 ms 



    // LCD_WR_REG(0X04);				   
    // lcddev.id=LCD_RD_DATA();	//dummy read 	
    // lcddev.id=LCD_RD_DATA();	//读到0X00
    // lcddev.id=LCD_RD_DATA();   	//读取93								   
    // lcddev.id<<=8;
    // lcddev.id|=LCD_RD_DATA();  	//读取41 


    LCD_WR_REG(0x36);      // 显示扫描方向
    LCD_WR_DATA(0xA0);     // 00:从左到右，从上到下
                            // A0:从下到上，从左到右
                            // 60:从上到下，从右到左

    LCD_WR_REG(0x3A);    // color format
    LCD_WR_DATA(0x05);    // 16bit/pixel

    LCD_WR_REG(0xB2);    // Porch Setting  
    LCD_WR_DATA(0x0C);   
    LCD_WR_DATA(0x0C);   
    LCD_WR_DATA(0x00);   
    LCD_WR_DATA(0x33);   
    LCD_WR_DATA(0x33);   

    LCD_WR_REG(0xB7);    // Gate Control
    LCD_WR_DATA(0x35);   

    LCD_WR_REG(0xBB);    // VCOM  
    LCD_WR_DATA(0x1C);   

    LCD_WR_REG(0xC0);    // LCM control   
    LCD_WR_DATA(0x2C);   

    LCD_WR_REG(0xC2);    // VDV and VRH Command Enable 
    LCD_WR_DATA(0x01);   

    LCD_WR_REG(0xC3);    //  VRH Set  
    LCD_WR_DATA(0x0B);       

    LCD_WR_REG(0xC4);    // VDV Set
    LCD_WR_DATA(0x20);   

    LCD_WR_REG(0xC6);    // Frame Rate    
    LCD_WR_DATA(0x0F);   

    LCD_WR_REG(0xD0);    // Power Control 1
    LCD_WR_DATA(0xA4);   
    LCD_WR_DATA(0xA1);   

    LCD_WR_REG(0xE0);    // Positive Voltage     
    LCD_WR_DATA(0xD0);   
    LCD_WR_DATA(0x06);   
    LCD_WR_DATA(0x0B);   
    LCD_WR_DATA(0x0A);   
    LCD_WR_DATA(0x09);   
    LCD_WR_DATA(0x06);   
    LCD_WR_DATA(0x2F);   
    LCD_WR_DATA(0x44);   
    LCD_WR_DATA(0x45);   
    LCD_WR_DATA(0x18);   
    LCD_WR_DATA(0x14);   
    LCD_WR_DATA(0x14);   
    LCD_WR_DATA(0x27);   
    LCD_WR_DATA(0x2D);   

    LCD_WR_REG(0xE1);    // Negative Voltage    
    LCD_WR_DATA(0xD0);   
    LCD_WR_DATA(0x06);   
    LCD_WR_DATA(0x0B);   
    LCD_WR_DATA(0x0A);   
    LCD_WR_DATA(0x09);   
    LCD_WR_DATA(0x05);   
    LCD_WR_DATA(0x2E);   
    LCD_WR_DATA(0x43);   
    LCD_WR_DATA(0x45);   
    LCD_WR_DATA(0x18);   
    LCD_WR_DATA(0x14);   
    LCD_WR_DATA(0x14);   
    LCD_WR_DATA(0x27);   
    LCD_WR_DATA(0x2D);   

    LCD_WR_REG(0x21);    // Display Inversion On
    LCD_WR_REG(0x11);    // Sleep Out
    delay_ms(5);
    LCD_WR_REG(0x29);    // Display On

    
	LCD_Display_Dir(1);		//默认为竖屏

	LCD_Clear(RED);
}  



//清屏函数
//color:要清屏的填充色
void LCD_Clear(uint16_t color)
{
	uint32_t index=0;      
	uint32_t totalpoint=lcddev.width;
	totalpoint*=lcddev.height; 			//得到总点数

    LCD_SetCursor(0x00,0x00);	//设置光标位置 
	LCD_WriteRAM_Prepare();     		//开始写入GRAM	 	  
	for(index=0;index<totalpoint;index++)
	{
        LCD->LCD_RAM = (uint8_t)(color >> 8);//写十六位GRAM
        LCD->LCD_RAM = (uint8_t)(color & 0xff);//写十六位GRAM
	}
}  



