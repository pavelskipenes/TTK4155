#pragma once

#include <stdbool.h>
#include <stdint.h>

struct joystick_percent_t
{
    int16_t percent_x;
    int16_t percent_y;
};

/**
 * @brief define resting position from ADC
 *
 * @param adc_initial_value_x resting position of X axis
 * @param adc_initial_value_y resting position of Y axis
 */
void joystick_init(uint8_t adc_initial_value_x, uint8_t adc_initial_value_y);

/**
 * @brief
 *
 * @param x sampled voltage in x axis as unsigned 8 bit value
 * @param y sampled voltage in y axis as unsigned 8 bit value
 * @return x and y as percent exited. positive direction is defined as up and right.
 */
struct joystick_percent_t joystick_get_percent(uint8_t voltage_x, uint8_t voltage_y);
