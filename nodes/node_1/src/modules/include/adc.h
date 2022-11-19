#pragma once

#include <stdint.h>

#define ADC_JOYSTICK_X 0
#define ADC_JOYSTICK_Y 1

typedef struct adc_sample_t
{
    uint8_t touch_bar_left;
    uint8_t joystick[2];
    uint8_t touch_bar_right;
} adc_sample;

/**
 * @brief Initialize adc.
 *
 * Side effects:
 * - Initialize external memory access.
 *
 */
void adc_init();

/**
 * @brief
 *
 * @return adc_sample containing sampled values
 *
 * @note Enable interrupts. Internal interrupt handler will sample the values and return them. Blocking function.
 */
adc_sample adc_read();
