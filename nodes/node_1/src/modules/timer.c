#include "timer.h"

#include "gpio.h"
#include <avr/io.h>

void timer_enable_perif_clock()
{
    GPIO pb0 = {
        0,
        &DDRB,
        &PORTB,
    };
    gpio_direction(&pb0, GPIO_OUTPUT);
}
