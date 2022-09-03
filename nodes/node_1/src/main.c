#include <stdbool.h>
#include <util/delay.h>

#include "gpio.h"

#define DELAY 1000 // one second

int main()
{
    DDRB = (GPIO_OUTPUT << DDB0);
    while (true)
    {
        PORTB = (GPIO_LOW << PB0);
        _delay_ms(DELAY);
        PORTB = (GPIO_HIGH << PB0);
        _delay_ms(DELAY);
    }
}