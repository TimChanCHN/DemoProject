#include "nt35510.h"
#include "board_config.h"

static uint32_t m_bank_base[] = 
{0x60000000, 0x64000000, 0x68000000, 0x6C000000};

// TODO: RS对应的Ax(x便是该数组的index)
static uint32_t m_bank_offset_8bit[] = 
{
    0x0000, 0x0001, 0x0003, 0x0007, 0x000F, 0x001F, 0x003F, 0x007F,         // A0~A7对应的offset
    0x00FF, 0x01FF, 0x03FF, 0x07FF, 0x0FFF, 0x1FFF, 0x3FFF, 0x7FFF,          // A8~A15对应的offset
};

static uint32_t m_bank_offset_16bit[] = 
{
    0x0000, 0x0002, 0x0006, 0x000E, 0x001E, 0x003E, 0x007E, 0x00FE,         // A0~A7对应的offset
    0x01FE, 0x03FE, 0x07FE, 0x0FFE, 0x1FFE, 0x3FFE, 0x7FFE, 0xFFFE,         // A8~A15对应的offset
    0x1FFFE,                                                                // A16对应offset
};

//使用NOR/SRAM的 Bank1.sector1,每个sector大小为64M，地址位HADDR[27,26]=00 A16作为数据命令区分线 
//注意设置时STM32内部会右移一位对其! 
// TODO: 针对不同的片选以及RS所接的线，LCD_BASE均需要作像对应的选择
// NE1 --> 0x60000000(BANK1基地址)
// NE2 --> 0x64000000(BANK2基地址)
// NE3 --> 0x68000000(BANK3基地址)
// NE4 --> 0x6C000000(BANK4基地址)
#define LCD_BASE        ((uint32_t)(m_bank_base[0] | m_bank_offset_16bit[16]))
#define LCD             ((LCD_Type_16b_t *) LCD_BASE)

void LCD_WR_REG(uint16_t regval)
{   
	LCD->LCD_REG = regval;//写入要写的寄存器序号	 
}

void LCD_WR_DATA(uint16_t data)
{	 
	LCD->LCD_RAM = data;		 
}

//读LCD数据
//返回值:读到的值
uint16_t LCD_RD_DATA(void)
{
	uint16_t ram;			//防止被优化
	ram = LCD->LCD_RAM;	
	return ram;	 
}

//写寄存器
//LCD_Reg:寄存器地址
//LCD_RegValue:要写入的数据
void LCD_WriteReg(uint16_t LCD_Reg,uint16_t LCD_RegValue)
{	
	LCD->LCD_REG = LCD_Reg;		//写入要写的寄存器序号	 
	LCD->LCD_RAM = LCD_RegValue;//写入数据	    		 
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
uint16_t nt35510_init(driver_info_t *p_drv)
{

}


