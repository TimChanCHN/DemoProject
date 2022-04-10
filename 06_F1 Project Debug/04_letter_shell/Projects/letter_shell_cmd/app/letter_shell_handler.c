#include "mid_usart.h"
#include "letter_shell_handler.h"

f1_uart_config_t m_uart_config = 
{
    .baudrate   = 115200,
    .databits   = USART_WordLength_8b,            // 8位数据位
    .stopbit    = USART_StopBits_1,
    .parity     = USART_Parity_No,
    .hwfc       = USART_HardwareFlowControl_None,
    .mode       = USART_Mode_Rx|USART_Mode_Tx,
};

f1_uart_info_t m_uart_info = 
{
    .uart       = USART1,
    .tx_port    = GPIOA,
    .rx_port    = GPIOA,

    .clk        = RCC_APB2Periph_USART1,
    .tx_clk     = RCC_APB2Periph_GPIOA,
    .rx_clk     = RCC_APB2Periph_GPIOA,
    .tx_pin     = GPIO_Pin_9,
    .rx_pin     = GPIO_Pin_10,

    .uart_id    = F1_UART_1,
};

static usart_object_t m_usart_obj = 
{
    .uart_info = &m_uart_info,
    .uart_config = &m_uart_config,
};

static Shell shell;
static char shellBuffer[512];

/**
 * @brief 用户shell写
 * 
 * @param data 数据
 * @param len 数据长度
 * 
 * @return short 实际写入的数据长度
 */
short userShellWrite(char *data, unsigned short len)
{
    usart_put_chars(&m_usart_obj, (uint8_t *)data, len);
    return len;
}

/**
 * @brief 用户shell读
 * 
 * @param data 数据
 * @param len 数据长度
 * 
 * @return short 实际读取到
 */
short userShellRead(char *data, unsigned short len)
{
    usart_get_chars(&m_usart_obj, (uint8_t *)data, len);

    return len;
}


Shell get_shell(void)
{
    return shell;
}

int init_letter_shell(void)
{
	uint8_t buff[10];

    usart_init(&m_usart_obj);

    shell.write = userShellWrite;
    shell.read = userShellRead;
    shellInit(&shell, shellBuffer, 512);

    shellTask(&shell);

    // while (1)
    // {
    //     usart_get_chars(&m_usart_obj, buff, 10);
    //     usart_put_string(&m_usart_obj, "\n\r");
    //     usart_put_chars(&m_usart_obj, buff, 10);
    // }
    
}
