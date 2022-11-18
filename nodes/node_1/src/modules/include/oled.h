#pragma once

#include <stdio.h>

#define OLED_NUM_COLUMNS_PER_PAGE 8
#define OLED_NUM_PAGES 8
#define OLED_NUM_SEGMENTS_PER_PAGE 128
#define OLED_SEGMENTS_PER_CHAR 8

FILE *oled_init();

void oled_reset();
void oled_home(uint8_t selection_index);
uint8_t oled_goto_line(uint8_t line);
void oled_goto_column(uint8_t column);
void oled_clear_line(uint8_t line);
void oled_pos(uint8_t row, uint8_t column);
void oled_print(char *character);
void oled_flush();