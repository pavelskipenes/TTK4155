#pragma once

#include <stdint.h>

int joystick_8_bit_voltage_to_angle(uint8_t voltage);

struct joystick_position
{
    uint8_t x;
    uint8_t y;
};

enum joystick_direction
{
    NEUTRAL,
    LEFT,
    RIGHT,
    UP,
    DOWN
};
