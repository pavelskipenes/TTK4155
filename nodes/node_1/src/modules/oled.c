#include "oled.h"
#include "fonts.h"
#include "memory.h"

#include <stdbool.h>
#include <stdlib.h>
#include <util/delay.h>

static void oled_carriage_return();
static void oled_reset_page_counter();
static void oled_reset_cursor();
static void write_c(uint8_t command_byte);
static volatile uint8_t *oled_cmd = (volatile uint8_t *)0x1000;
static volatile uint8_t *oled_data = (volatile uint8_t *)0x1200;

void oled_flush()
{
    for (int i = 0; i < OLED_NUM_SEGMENTS_PER_PAGE * OLED_NUM_PAGES; i++)
    {
        *oled_data = 0;
    }
}

void oled_init()
{
    static bool initialized = false;
    if (initialized)
    {
        return;
    }
    initialized = true;
    printf("[info]: initializing oled\n");

    memory_init();
    memory_add_delay();

    // display off
    write_c(0xae);
    // segment remap
    write_c(0xa1);
    // common pads hardware: alternative
    write_c(0xda);
    write_c(0x12);
    // common output scan direction:com63~com0
    write_c(0xc8);
    // multiplex ration mode:63
    write_c(0xa8);
    write_c(0x3f);
    // display divide ratio/osc. freq. mode
    write_c(0xd5);
    write_c(0x80);
    // contrast control
    write_c(0x81);
    write_c(0x50);
    // set pre-charge period
    write_c(0xd9);
    write_c(0x21);
    // Set Horizontal Memory Addressing Mode
    write_c(0x20);
    write_c(0x00);

    oled_reset_cursor();

    // start reading at RAM address = 0;
    write_c(0x40);

    // VCOM deselect level mode
    write_c(0xdb);
    write_c(0x30);
    // master configuration
    write_c(0xad);
    write_c(0x00);
    // out follows RAM content
    write_c(0xa4);
    // set normal display
    write_c(0xa6);
    // display on
    write_c(0xaf);

    oled_flush();

    return;
}

static void write_c(uint8_t command_byte)
{
    *oled_cmd = command_byte;
}

static void oled_carriage_return()
{
    write_c(0x21);
    write_c(0);
    write_c(127);
}
static void oled_reset_page_counter()
{
    write_c(0x20);
    write_c(0);
    write_c(7);
}

static void oled_reset_cursor()
{
    oled_carriage_return();
    oled_reset_page_counter();
}
