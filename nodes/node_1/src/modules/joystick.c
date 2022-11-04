#include "joystick.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

static int16_t joystick_adc_offset_x = 0;
static int16_t joystick_adc_offset_y = 0;

void joystick_init(uint8_t adc_initial_value_x, uint8_t adc_initial_value_y)
{
    static bool initialized = false;
    if (initialized)
    {
        assert(false && "already initialized");
    }
    initialized = true;

    // save current readings as zero
    const float adc_midpoint = 255.0 / 2;
    joystick_adc_offset_x = (int16_t)(adc_midpoint - adc_initial_value_x);
    joystick_adc_offset_y = (int16_t)(adc_midpoint - adc_initial_value_y);
}

static int16_t voltage_to_angle(int16_t signed_voltage)
{
    int16_t ret = (int16_t)(signed_voltage * (90.0l / 255) - 45);
    assert(ret >= -100 && ret <= 100);
    return ret;
}

static int16_t voltage_to_percent(int16_t signed_voltage)
{
    int16_t ret = (int16_t)(signed_voltage * (200.0l / 255) - 100);
    assert(ret >= -100 && ret <= 100);
    return ret;
}

struct joystick_angle_t joystick_get_angle(uint8_t adc_voltage_x, uint8_t adc_voltage_y)
{
    struct joystick_angle_t tmp =
        {
            voltage_to_angle((int16_t)adc_voltage_x + joystick_adc_offset_x),
            voltage_to_angle((int16_t)adc_voltage_y + joystick_adc_offset_y),
        };
    assert(tmp.angle_x >= -45 && tmp.angle_x <= 45);
    assert(tmp.angle_y >= -45 && tmp.angle_y <= 45);

    return tmp;
}

struct joystick_percent_t joystick_get_percent(uint8_t adc_voltage_x, uint8_t adc_voltage_y)
{
    struct joystick_percent_t tmp =
        {
            voltage_to_percent((int16_t)adc_voltage_x + joystick_adc_offset_x),
            voltage_to_percent((int16_t)adc_voltage_y + joystick_adc_offset_y),
        };
    assert(tmp.percent_x >= -100 && tmp.percent_x <= 100);
    assert(tmp.percent_y >= -100 && tmp.percent_y <= 100);
    return tmp;
}