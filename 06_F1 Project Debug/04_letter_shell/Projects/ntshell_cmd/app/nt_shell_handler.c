#include <stdlib.h>
#include "mid_usart.h"
#include "board_config.h"


static ntshell_t ntshell;

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

static cmd_table_t m_cmd_table_list[] = 
{
    {"led_control", "control LED2", led_control},
};


static int func_read(char *buf, int cnt, void *extobj)
{
    int i;
    UNUSED_VARIABLE(extobj);
    
    for (i = 0; i < cnt; i++) {
        buf[i] = usart_get_char(&m_usart_obj);
    }
    return cnt;
}

static int func_write(const char *buf, int cnt, void *extobj)
{
    int i;
    UNUSED_VARIABLE(extobj);
    for (i = 0; i < cnt; i++) {
        usart_put_char(&m_usart_obj, buf[i]);
    }
    return cnt;
}


int init_ntshell(void)
{
    usart_init(&m_usart_obj);
    
    ntshell_init(
        &ntshell,
        func_read,
        func_write,
        ntshell_usrcmd_execute,
        (void *)&m_usart_obj);

    // function register
    for (int i = 0; i < sizeof(m_cmd_table_list)/sizeof(m_cmd_table_list[0]); i++)
    {
        CMD_FUNCTION_REGISTER(m_cmd_table_list[i].cmd, m_cmd_table_list[i].desc, m_cmd_table_list[i].func);
    }

    ntshell_set_prompt(&ntshell, "ntshell>");
    ntshell_execute(&ntshell);
    return 0;
}
