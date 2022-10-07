#include "joystick.h"

int joystick_8_bit_voltage_to_angle(uint8_t voltage)
{
    return voltage * 0.3529 - 45;
}

