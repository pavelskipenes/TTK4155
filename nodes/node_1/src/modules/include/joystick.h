#pragma once

#include <stdint.h>

#include "adc.h"

struct joystick_angle_t
{
    int16_t angle_x;
    int16_t angle_y;
};

struct joystick_percent_t
{
    int16_t percent_x;
    int16_t percent_y;
};

void joystick_init(uint8_t adc_initial_value_x, uint8_t adc_initial_value_y);

struct joystick_angle_t joystick_get_angle(uint8_t voltage_x, uint8_t voltage_y);
struct joystick_percent_t joystick_get_percent(uint8_t voltage_x, uint8_t voltage_y);
