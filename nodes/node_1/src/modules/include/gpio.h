#pragma once
#include <avr/io.h>
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

/**
 * @brief GPIO structure
 *
 * Use address of data direction registers and address of port to identify correct pin.
 * Example:
 * ```
 * GPIO my_pin = { .pin = 5, .ddr= &DDRA, .port = &PORTB };
 * ```
 */
typedef struct GPIO_t
{
    int pin;
    volatile uint8_t *ddr;
    volatile uint8_t *port;
} GPIO;

/**
 * @brief set GPIO state
 * @param gpio pin to modify
 * @param gpio_state GPIO_LOW or GPIO_HIGH
 */
void gpio_set(GPIO *gpio, enum gpio_state state);

/**
 * @brief set GPIO direction
 *
 * @param gpio pin to modify
 * @param direction GPIO_INPUT or GPIO_OUTPUT
 */
void gpio_direction(GPIO *gpio, enum gpio_direction direction);

/**
 * @brief toggle a pin
 *
 * @param gpio
 */
void gpio_toggle(GPIO *gpio);
