#pragma once

#include "adc.h"
#include <stdint.h>

struct joystick_t
{
    int angle_x;
    int angle_y;
};

struct joystick_percent_t
{
    int8_t percent_x;
    int8_t percent_y;
};

struct joystick_config_t
{
    uint8_t adc_initial_value_x;
    uint8_t adc_initial_value_y;
};

void joystick_init(struct joystick_config_t *);

struct joystick_t joystick_get_angle(uint8_t voltage_x, uint8_t voltage_y);
struct joystick_percent_t joystick_get_percent(uint8_t voltage_x, uint8_t voltage_y);
