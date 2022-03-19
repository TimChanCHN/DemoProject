#include <stdlib.h>
#include "mid_usart.h"
#include "board_config.h"

#define UNUSED_VARIABLE(N)  do { (void)(N); } while (0)

static ntshell_t ntshell;

f0_uart_config_t m_uart_config = 
{
    .baudrate   = 115200,
    .databits   = USART_WordLength_8b,            // 8位数据位
    .stopbit    = USART_StopBits_1,
    .parity     = USART_Parity_No,
    .hwfc       = USART_HardwareFlowControl_None,
    .mode       = USART_Mode_Rx|USART_Mode_Tx,
};

f0_uart_info_t m_uart_info = 
{
    .uart       = USART1,
    .tx_port    = GPIOA,
    .rx_port    = GPIOA,

    .clk        = RCC_APB2Periph_USART1,
    .tx_clk     = RCC_AHBPeriph_GPIOA,
    .rx_clk     = RCC_AHBPeriph_GPIOA,
    .tx_pin     = GPIO_Pin_9,
    .rx_pin     = GPIO_Pin_10,
    .tx_pin_source  = GPIO_PinSource9,
    .rx_pin_source  = GPIO_PinSource10,
    .tx_af      = GPIO_AF_1,
    .rx_af      = GPIO_AF_1,
};

static usart_object_t m_usart_obj = 
{
    .id = 0,
    .uart_info = &m_uart_info,
    .uart_config = &m_uart_config,
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

static int func_callback(const char *text, void *extobj)
{
    ntshell_t *ntshell = (ntshell_t *)extobj;
    UNUSED_VARIABLE(ntshell);
    UNUSED_VARIABLE(extobj);

    if (ntlibc_strlen(text) > 0) {
        usart_put_string(&m_usart_obj, "User input text:'");
        usart_put_string(&m_usart_obj, text);
        usart_put_string(&m_usart_obj, "'\r\n");
    }
    return 0;
}

int init_ntshell(void)
{
    usart_init(&m_usart_obj);
    // usart_put_string(&m_usart_obj, "Hello world.\r\n");
    ntshell_init(
        &ntshell,
        func_read,
        func_write,
        func_callback,
        (void *)&ntshell);

    ntshell_set_prompt(&ntshell, "ntshell>");
    ntshell_execute(&ntshell);
    return 0;
}
