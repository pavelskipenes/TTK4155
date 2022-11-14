#pragma once

#include <stdbool.h>
#include <stdint.h>

struct joystick_percent_t
{
    int16_t percent_x;
    int16_t percent_y;
};

bool joystick_init(uint8_t adc_initial_value_x, uint8_t adc_initial_value_y);
void joystick_test_voltage_to_percent();
struct joystick_percent_t joystick_get_percent(uint8_t voltage_x, uint8_t voltage_y);
