#include "timer.h"

#include "gpio.h"

void timer_enable_perif_clock()
{
    DDRB |= (GPIO_OUTPUT << DDB0);
}
