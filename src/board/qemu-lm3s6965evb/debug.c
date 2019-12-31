
#include <stdint.h>

volatile uint32_t * const UART0DR = (uint32_t *)0x4000C000;

char send_char(uint8_t *ch)
{
    *UART0DR = *ch;
    return *ch;
}

int printf_char(char ch)
{
    uint8_t c1 = (uint8_t)'\r';
    uint8_t c2 = 0;
    c2 = (uint8_t)ch;
    send_char(&c2);

    if (((uint8_t)'\n') == c2) {
        send_char(&c1);
    }

    return 0;
}

int printf_str(char *str)
{
    while (str && (*str != (char)'\0')) {
        printf_char(*str);
        str++;
    }

    return 0;
}
