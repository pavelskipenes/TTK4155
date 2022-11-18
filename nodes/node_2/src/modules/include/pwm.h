#pragma once

#include "pio.h"

void pwm_init_pin(double period, double duty_cycle);
void pwm_servo_set_pos(uint8_t pos, uint8_t channel);
void pwm_set_duty_cycle(uint8_t duty_cycle, uint8_t channel);