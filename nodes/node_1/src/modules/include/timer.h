#pragma once

#include <stdint.h>

void timer_enable_perif_clock();

void timer1_ctc(uint8_t interrupt_frequency);
void timer0_ctc(uint8_t interrupt_frequency);
