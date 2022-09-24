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
 * ADC is configured in hard wired mode, 4 channel unipolar convertion.
 * Connection details is in figure 8a on page 16 in the datasheet. 
 * 
 */

#include <stdint.h>
static int const ADC_NUM_CHANNELS = 4;

typedef struct channels_value_t{
    uint8_t channel_0;
    uint8_t channel_1;
    uint8_t channel_2;
    uint8_t channel_3;
} channel_value;

void adc_init();

channel_value adc_read();

