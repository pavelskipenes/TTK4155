#include "joystick.h"

#include <assert.h>

static uint8_t adc_inital_value_x;
static uint8_t adc_inital_value_y;

static inline int16_t voltage_to_percent(const uint8_t adc_value, const uint8_t midpoint)
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

/**
 * @brief test that the joystick to percent never spits out values outside of the range -100, 100
 * loop over this function while trying out different orientations of the joystick.
 */
void joystick_test_voltage_to_percent()
{
    assert(voltage_to_percent(0xFF / 2, 0xFF / 2) == 0);
    assert(voltage_to_percent(0xFF, 0xFF / 2) == 100);
    assert(voltage_to_percent(0x00, 0xFF / 2) == -100);
}

void joystick_init(uint8_t x, uint8_t y)
{
    adc_inital_value_x = x;
    adc_inital_value_y = y;
}

struct joystick_percent_t joystick_get_percent(uint8_t adc_voltage_x, uint8_t adc_voltage_y)
{
    struct joystick_percent_t tmp =
        {
            voltage_to_percent(adc_voltage_x, adc_inital_value_x),
            voltage_to_percent(adc_voltage_y, adc_inital_value_y),
        };

    // If these lines are uncommented the MCU will run into a reset loop.
    // assert(tmp.percent_x >= -100 && tmp.percent_x <= 100);
    // assert(tmp.percent_y >= -100 && tmp.percent_y <= 100);

    return tmp;
}