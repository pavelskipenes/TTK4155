#pragma once

#include <stdint.h>

#define JOYSTICK_X 1
#define JOYSTICK_Y 0

typedef struct adc_sample_t{
    uint8_t touch_bar_left;
    uint8_t joystick[2];
    uint8_t touch_bar_right;
} adc_sample;

void adc_init();

adc_sample adc_read();

