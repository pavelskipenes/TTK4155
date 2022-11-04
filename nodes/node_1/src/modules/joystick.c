#include "joystick.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

static int8_t joystick_voltage_offset_x = 0;
static int8_t joystick_voltage_offset_y = 0;

void joystick_init(struct joystick_config_t *config)
{
    static bool initialized = false;
    if (initialized)
    {
        assert(false && "already initialized");
    }
    initialized = true;

    // save current readings as zero
    const float midpoint = 255.0f / 2;
    joystick_voltage_offset_x = (int8_t)(midpoint - config->adc_initial_value_x);
    joystick_voltage_offset_y = (int8_t)(midpoint - config->adc_initial_value_y);
}

/**
 * @brief
 *
 * @param voltage voltage representation from adc using 8 bits.
 * @return
 */
static int8_t voltage_to_angle(int16_t voltage)
{
    // max value * max_multiplier -> 45
    // min value -> - 45
    // range <- max - min
    return (int8_t)(voltage * (90.0f / 255) - 45);
}

static int8_t voltage_to_percent(int16_t voltage)
{

    return (int8_t)(voltage * (200.0f / 255) - 100);
}

/**
 * @brief get joystick position in degrees.
 * @note adds a static offset calculated by joystick_calibrate()
 * @see joystick_calibrate()
 *
 * @param voltage_x uint8_t horizontal adc value
 * @param voltage_y uint8_t vertical adc value
 *
 * @return struct joystick_t with adjusted values
 */
struct joystick_t joystick_get_angle(uint8_t voltage_x, uint8_t voltage_y)
{
    struct joystick_t tmp =
        {
            voltage_to_angle(voltage_x + joystick_voltage_offset_x),
            voltage_to_angle(voltage_y + joystick_voltage_offset_y),
        };
    return tmp;
}

struct joystick_percent_t joystick_get_percent(uint8_t voltage_x, uint8_t voltage_y)
{
    struct joystick_percent_t tmp =
        {
            voltage_to_percent(voltage_x + joystick_voltage_offset_x),
            voltage_to_percent(voltage_y + joystick_voltage_offset_y),
        };
    return tmp;
}