#pragma once
/**
 * @file adc.h
 * @author Pavel Skipenes (pavelgs@stud.ntnu.no)
 * @brief driver MAX155 IC ADC
 * @version 0.1
 * @date 2022-09-24
 * 
 * @copyright Copyright Pavel Skipenes (c) 2022
 * 
 * ADC is configured in hard wired mode, 8 channel, Single-Ended, Unipolar Conversion.
 * Connection details is in figure 8b on page 16 in the datasheet.
 * One slight difference: REFIN is connected to 5v.
 * 
 * ADC BUSY pin is connected back to MCU on pin PE0 
 * 
 */

#include <stdint.h>
static int const ADC_NUM_CHANNELS = 4;

typedef struct channels_value_t{
    uint8_t channel[4];
} channel_values;

void adc_init();

channel_values adc_read();

