#include <stdio.h>
#include "util.h"
#include "stdbool.h"

#ifdef F0
#include "f0_lib.h"
#endif


#if !defined(__ICCARM__)
struct __FILE
{
  int handle;
};
#endif

FILE __stdout;
FILE __stdin;

#if defined(__CC_ARM) || defined(__ICCARM__)
// int fgetc(FILE *p_file)
// {
//   uint8_t input;
//   input = uart_get();
//   return input;
// }

int fputc(int ch, FILE *p_file)
{
  UNUSED_PARAMETER(p_file);

  UNUSED_VARIABLE(f0_uart_put(F0_UART_1, (uint8_t)ch));

  return ch;
}

#elif defined(__GNUC__)

// int _write(int file, const char *p_char, int len)
// {
//   int i;

//   UNUSED_PARAMETER(file);

//   for (i = 0; i < len; i++)
//   {
//     UNUSED_VARIABLE(uart_put(*p_char++));
//   }

//   return len;
// }

// int _read(int file, char *p_char, int len)
// {
//   UNUSED_PARAMETER(file);
//   uart_get((uint8_t *)p_char) return 1;
// }
// #endif

// #if defined(__ICCARM__)

// __ATTRIBUTES size_t __write(int file, const unsigned char *p_char, size_t len)
// {
//   int i;

//   UNUSED_PARAMETER(file);

//   for (i = 0; i < len; i++)
//   {
//     UNUSED_VARIABLE(uart_put(*p_char++));
//   }
//   return len;
// }

#endif
