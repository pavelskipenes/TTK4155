#pragma once

#include <stdio.h>

#define OLED_NUM_COLUMNS_PER_PAGE 8
#define OLED_NUM_PAGES 8
#define OLED_NUM_SEGMENTS_PER_PAGE 128
#define OLED_SEGMENTS_PER_CHAR 8

FILE* oled_init();
