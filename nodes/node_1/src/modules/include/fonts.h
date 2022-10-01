#pragma once
/*
 * fonts.h
 *
 * Library of fonts
 * Large: 8x8, normal: 5x7 and small: 4x7
 */

#include <avr/pgmspace.h>
#include <stdint.h>

// Font 8x8 - Large
extern const uint8_t font8[95][8];
// Font 5x7 - normal
extern const uint8_t font5[95][5];
// Font 4x6 - Small
extern const uint8_t font4[95][4];

typedef struct character8
{
	const uint8_t segments[8];
} char8;

typedef struct character5
{
	const uint8_t segments[5];
} char5;

typedef struct character4
{
	const uint8_t segments[4];
} char4;

#define CHAR_TO_INDEX(X) X - ' '

char8 fonts_get_char8(char c);