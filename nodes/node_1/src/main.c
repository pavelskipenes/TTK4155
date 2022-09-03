#include <stdbool.h>
#include <util/delay.h>

#include "usart.h"

int main()
{
    usart_init(9600);
    usart_send_string("Epstein didn't kill himself\n");

    while (true)
    {
        _delay_ms(1000);
    }
}