#include "joystick.h"

#include <assert.h>

static int16_t voltage_to_percent(const uint8_t adc_value, const uint8_t midpoint)
{
    // y(x) = a_1 * x + b_1, for x > midpoint
    // y(x) = a_2 * x + b_2, for x < midpoint

    // a_1 = (100 / 255 - midpoint)
    // b_1 = 100 / (255 - m) * m

    // a_2 = (100 / midpoint)
    // b_2 = -100

    if (adc_value == midpoint)
    {
        return 0;
    }

    if (adc_value > midpoint)
    {
        return ((100 * adc_value) / (255 - midpoint)) - ((100 * midpoint) / (255 - midpoint));
    }
    return ((100 * adc_value) / midpoint) - 100;
}

void joystick_test_voltage_to_percent()
{
    assert(voltage_to_percent(0xFF / 2, 0xFF / 2) == 0);
    assert(voltage_to_percent(0xFF, 0xFF / 2) == 100);
    assert(voltage_to_percent(0x00, 0xFF / 2) == -100);
}

struct joystick_percent_t joystick_get_percent(uint8_t adc_voltage_x, uint8_t adc_voltage_y)
{
    struct joystick_percent_t tmp =
        {
            voltage_to_percent(adc_voltage_x, 0xA5),
            voltage_to_percent(adc_voltage_y, 0xC9),
        };
    assert(tmp.percent_x >= -100 && tmp.percent_x <= 100);
    assert(tmp.percent_y >= -100 && tmp.percent_y <= 100);
    return tmp;
}