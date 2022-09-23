#pragma once
#include <avr/io.h>
#include "stdbool.h"

#define GPIO_OUTPUT 1
#define GPIO_INPUT 0

#define GPIO_HIGH 1
#define GPIO_LOW 0

typedef enum {
	A0, A1, A2, A3, A4, A5, A6, A7,
	B0, B1, B2, B3, B4, B5, B6, B7,
	C0, C1, C2, C3, C4, C5, C6, C7,
	D0, D1, D2, D3, D4, D5, D6, D7
} Pin;

void gpio_set_state(Pin, bool);
void gpio_set_direction(Pin, bool);