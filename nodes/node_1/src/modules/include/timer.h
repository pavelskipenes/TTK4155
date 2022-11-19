#pragma once

#include <stdint.h>

/**
 * @brief expose system clock on pin PB0
 *
 * @note requires fuse byte to be programmed in certain way. See make flash rule for details
 */
void timer_enable_perif_clock();

/**
 * @brief enable ctc1 timer
 *
 * @param interrupt_frequency
 */
void timer1_ctc(uint8_t interrupt_frequency);

/**
 * @brief enable ctc0 timer
 *
 * @param interrupt_frequency
 */
void timer0_ctc(uint8_t interrupt_frequency);
