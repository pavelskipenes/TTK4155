#pragma once
/*
 * fonts.h
 *
 * Library of fonts
 * Large: 8x8, normal: 5x7 and small: 4x7
 */

#include <avr/pgmspace.h>
#include <stdint.h>

extern const uint8_t font8[95][8];
extern const uint8_t font5[95][5];
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

/**
 * @brief get font bytes for display outputs
 *
 * @param c character of interest
 * @return pixel array of character
 */
char8 fonts_get_char8(char c);