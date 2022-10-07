#pragma once
#include <stdbool.h>
#include <stdint.h>

enum gpio_direction
{
    GPIO_INPUT,
    GPIO_OUTPUT
};

enum gpio_state
{
    GPIO_LOW,
    GPIO_HIGH
};

typedef struct GPIO_t
{
    int pin;
    volatile uint8_t *ddr;
    volatile uint8_t *port;
} GPIO;

void gpio_set(GPIO *gpio, enum gpio_state state);
void gpio_direction(GPIO *gpio, enum gpio_direction direction);
void gpio_toggle(GPIO *gpio);
