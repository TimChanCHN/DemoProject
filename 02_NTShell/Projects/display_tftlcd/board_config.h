/**
 * @brief 根据不同的芯片 来 选择所包含的头文件，和宏管理
 * 
 */
#ifndef __BOARD_CONFIG_H
#define __BOARD_CONFIG_H





/**
 * FT32 平台的宏管理 和 头文件选择
 */
#ifdef  FT32
#include "ft32f0xx.h"

/* GPIO */
#define LCD_BACK_LIGHT_PIN              GPIO_Pin_4
#define LCD_BACK_LIGHT_PORT             GPIOB
#define LCD_BACK_LIGHT_PORT_PERIPH_CLK  RCC_AHBPeriph_GPIOB

#define BK9531_CE_PIN                   GPIO_Pin_5
#define BK9531_CE_PORT                  GPIOB
#define BK9531_CE_PERIPH_CLK            RCC_AHBPeriph_GPIOB

#define POWER_ON_PIN                    GPIO_Pin_2
#define POWER_ON_PORT                   GPIOA
#define POWER_ON_PORT_PERIPH_CLK        RCC_AHBPeriph_GPIOA

#define POWER_SW_KEY_PIN                GPIO_Pin_3
#define POWER_SW_KEY_PORT               GPIOA
#define POWER_SW_KEY_PORT_PERIPH_CLK    RCC_AHBPeriph_GPIOA           

/* TIMER */
#define TIM3_CLK                        RCC_APB1Periph_TIM3
#define TIM3_IRQ                        TIM3_IRQn

#define USER_TIMER_CLK                  TIM3_CLK
#define USER_TIMER_IRQ                  TIM3_IRQ
#define USER_TIMER                      TIM3

#define USER_TIMER_CH                   TIM_IT_CC1
#define USER_TIMER_FREQ                 1000000     //定时器经分频后获得的频率
#define USER_TIMER_PERIOD               1000        //定时器溢出计数


/* UART */
#define FT_UART1                        USART1
#define FT_UART1_CLK                    RCC_APB2Periph_USART1

#define FT_UART1_TX_PIN                 GPIO_Pin_9
#define FT_UART1_TX_GPIO_PORT           GPIOA
#define FT_UART1_TX_GPIO_CLK            RCC_AHBPeriph_GPIOA
#define FT_UART1_TX_SOURCE              GPIO_PinSource9
#define FT_UART1_TX_AF                  GPIO_AF_1

#define FT_UART1_RX_PIN                 GPIO_Pin_10
#define FT_UART1_RX_GPIO_PORT           GPIOA
#define FT_UART1_RX_GPIO_CLK            RCC_AHBPeriph_GPIOA
#define FT_UART1_RX_SOURCE              GPIO_PinSource10
#define FT_UART1_RX_AF                  GPIO_AF_1

/* I2C */

#define VIRT1_SCL_PIN                    GPIO_Pin_6                  /* 模拟I2C-SCL */
#define VIRT1_SCL_GPIO_PORT              GPIOB                       /* GPIOB */
#define VIRT1_SCL_GPIO_CLK               RCC_AHBPeriph_GPIOB
#define VIRT1_SDA_PIN                    GPIO_Pin_7                  /* 模拟I2C-SDA */
#define VIRT1_SDA_GPIO_PORT              GPIOB                       /* GPIOB */
#define VIRT1_SDA_GPIO_CLK               RCC_AHBPeriph_GPIOB

/* EXIT */
#define EXIT_GPIO_PORT_CLK              RCC_AHBPeriph_GPIOA
#define EXIT_GPIO_PIN                   GPIO_Pin_8
#define EXIT_GPIO_PORT_RESOURCE         EXTI_PortSourceGPIOA
#define EXIT_GPIO_PIN_RESOURCE          EXTI_PinSource8
#define EXIT_TRIGGER_MODE               EXTI_Trigger_Falling
#define EXIT_GPIO_PORT                  GPIOA
#define EXIT_LINE                       EXTI_Line8

/* HT1621 */
#define HT1621_DATA_PIN                 GPIO_Pin_7
#define HT1621_DATA_PORT                GPIOA
#define HT1621_DATA_PORT_PERIPH_CLK     RCC_AHBPeriph_GPIOA

#define HT1621_WR_PIN                   GPIO_Pin_5
#define HT1621_WR_PORT                  GPIOA
#define HT1621_WR_PORT_PERIPH_CLK       RCC_AHBPeriph_GPIOA

#define HT1621_CS_PIN                   GPIO_Pin_4
#define HT1621_CS_PORT                  GPIOA
#define HT1621_CS_PORT_PERIPH_CLK       RCC_AHBPeriph_GPIOA

/* ADC */
#define ADC_CHANNEL_1                   ADC_Channel_1

#define ADC_GPIO_PORT_CLK               RCC_AHBPeriph_GPIOA
#define ADC_GPIO_PIN_0                  GPIO_Pin_0
#define ADC_GPIO_PIN_1                  GPIO_Pin_1
#define ADC_GPIO_PORT                   GPIOA

#define ADC_SAMPLETIME_CYCLE            ADC_SampleTime_239_5Cycles
#define ADC_CONTINUOUS_CONV_MODE        DISABLE

// TFT LCD
#define PARALLEL_PORT
// #define SERIAL_IIC
// #define SERIAL_SPI


// CS--PC0, RESET--PC1, RS--PC2, WR--PC3, RD--PC4
#define ST7789_CS_PIN                   GPIO_Pin_0
#define ST7789_CS_PORT                  GPIOC
#define ST7789_CS_PORT_PERIPH_CLK       RCC_AHBPeriph_GPIOC

#define ST7789_RST_PIN                   GPIO_Pin_1
#define ST7789_RST_PORT                  GPIOC
#define ST7789_RST_PORT_PERIPH_CLK       RCC_AHBPeriph_GPIOC

#define ST7789_RS_PIN                   GPIO_Pin_2
#define ST7789_RS_PORT                  GPIOC
#define ST7789_RS_PORT_PERIPH_CLK       RCC_AHBPeriph_GPIOC

#define ST7789_WR_PIN                   GPIO_Pin_3
#define ST7789_WR_PORT                  GPIOC
#define ST7789_WR_PORT_PERIPH_CLK       RCC_AHBPeriph_GPIOC

#define ST7789_RD_PIN                   GPIO_Pin_4
#define ST7789_RD_PORT                  GPIOC
#define ST7789_RD_PORT_PERIPH_CLK       RCC_AHBPeriph_GPIOC

// 打开该开关，DB数据不使用连续pin脚
#define LCD_DEBUG           
// DB0--PB12, DB1--PB13, DB2--PB14, DB3--PB15, DB4--PC6, DB5--PC7, DB6--PC8, DB7--PC9
#define ST7789_DB0_PIN                  GPIO_Pin_12
#define ST7789_DB0_PORT                 GPIOB
#define ST7789_DB0_PORT_PERIPH_CLK      RCC_AHBPeriph_GPIOB

#define ST7789_DB1_PIN                  GPIO_Pin_13
#define ST7789_DB1_PORT                 GPIOB
#define ST7789_DB1_PORT_PERIPH_CLK      RCC_AHBPeriph_GPIOB

#define ST7789_DB2_PIN                  GPIO_Pin_14
#define ST7789_DB2_PORT                 GPIOB
#define ST7789_DB2_PORT_PERIPH_CLK      RCC_AHBPeriph_GPIOB

#define ST7789_DB3_PIN                  GPIO_Pin_15
#define ST7789_DB3_PORT                 GPIOB
#define ST7789_DB3_PORT_PERIPH_CLK      RCC_AHBPeriph_GPIOB

#define ST7789_DB4_PIN                  GPIO_Pin_6
#define ST7789_DB4_PORT                 GPIOC
#define ST7789_DB4_PORT_PERIPH_CLK      RCC_AHBPeriph_GPIOC

#define ST7789_DB5_PIN                  GPIO_Pin_7
#define ST7789_DB5_PORT                 GPIOC
#define ST7789_DB5_PORT_PERIPH_CLK      RCC_AHBPeriph_GPIOC

#define ST7789_DB6_PIN                  GPIO_Pin_8
#define ST7789_DB6_PORT                 GPIOC
#define ST7789_DB6_PORT_PERIPH_CLK      RCC_AHBPeriph_GPIOC

#define ST7789_DB7_PIN                  GPIO_Pin_9
#define ST7789_DB7_PORT                 GPIOC
#define ST7789_DB7_PORT_PERIPH_CLK      RCC_AHBPeriph_GPIOC

#endif


/* BK9531 */
#define BK953X_DEVICE_ID    0x25

/* 选择develop_lib库的头文件 */
#include "develop_lib.h"
#include "lib_error.h"
#include "util.h"

#endif
