#include "gpio.h"

#include <assert.h>

void gpio_set(GPIO *gpio, enum gpio_state state)
{
    assert(gpio->pin >= 0);
    assert(gpio->pin < 8);
    if (state)
    {
        *(gpio->port) |= (GPIO_HIGH << gpio->pin);
    }
    else
    {
        *(gpio->port) &= ~(1 << gpio->pin);
    }
}

void gpio_direction(GPIO *gpio, enum gpio_direction direction)
{
    assert(gpio->pin >= 0);
    assert(gpio->pin < 8);
    if (direction == GPIO_OUTPUT)
    {
        *(gpio->ddr) |= (GPIO_OUTPUT << gpio->pin);
    }
    else
    {
        *(gpio->ddr) &= ~(GPIO_INPUT << gpio->pin);
    }
}

void gpio_toggle(GPIO *gpio)
{
    *(gpio->port) ^= (1 << gpio->pin);
}
