#include "timer.h"

#include "gpio.h"

void timer_init_clock_signal_output()
{
    DDRB |= (GPIO_OUTPUT << DDB0);
}
