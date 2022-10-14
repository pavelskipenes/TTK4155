#include "oled.h"

#include <assert.h>
#include <stdbool.h>

#include "fonts.h"
#include "memory.h"

static int oled_write_char(char character, FILE *fd __attribute__((unused)));
static void oled_flush();
static void oled_increment_page_index();
static void oled_increment_segment_index();
static void oled_reset_cursor();
static void oled_set_page_counter(uint8_t page_index);
static void oled_set_segment_counter(uint8_t segment_index);
static void oled_write_c(uint8_t command_byte);

static uint8_t s_oled_page_index = 0;
static uint8_t s_oled_segment_index = 0;
static volatile uint8_t *s_oled_cmd = (volatile uint8_t *)0x1000;
static volatile uint8_t *s_oled_data = (volatile uint8_t *)0x1200;

FILE *oled_init()
{
    static bool initialized = false;
    if (initialized)
    {
        assert(!"oled is already initialized");
    }
    initialized = true;
    printf("[info]: initializing oled\n");
    memory_init();

    // display off
    oled_write_c(0xae);
    // segment remap
    oled_write_c(0xa1);
    // common pads hardware: alternative
    oled_write_c(0xda);
    oled_write_c(0x12);
    // common output scan direction:com63~com0
    oled_write_c(0xc8);
    // multiplex ration mode:63
    oled_write_c(0xa8);
    oled_write_c(0x3f);
    // display divide ratio/osc. freq. mode
    oled_write_c(0xd5);
    oled_write_c(0x80);
    // contrast control
    oled_write_c(0x81);
    oled_write_c(0x50);
    // set pre-charge period
    oled_write_c(0xd9);
    oled_write_c(0x21);
    // Set Horizontal Memory Addressing Mode
    oled_write_c(0x20);
    oled_write_c(0x00);

    // start reading at RAM address = 0;
    oled_write_c(0x40);

    // VCOM deselect level mode
    oled_write_c(0xdb);
    oled_write_c(0x30);
    // master configuration
    oled_write_c(0xad);
    oled_write_c(0x00);
    // out follows RAM content
    oled_write_c(0xa4);
    // set normal display
    oled_write_c(0xa6);
    // display on
    oled_write_c(0xaf);

    oled_reset_cursor();
    oled_flush();

    return fdevopen(oled_write_char, NULL);
}

static int oled_write_char(char character, FILE *fd __attribute__((unused)))
{
    switch (character)
    {
    case '\n':
        oled_increment_page_index();
        break;
    case '\r':
        oled_set_segment_counter(0);
        s_oled_segment_index = 0;
        break;
    case '\t':
        for (int i = 0; i < 4; i++)
        {
            oled_write_char(' ', fd);
        }
        break;
    default:
        char8 c = fonts_get_char8(character);
        for (int i = 0; i < 8; i++)
        {
            *s_oled_data = c.segments[i];
            oled_increment_segment_index();
        }
    }
    return 0;
}

static void oled_increment_segment_index()
{
    if (++s_oled_segment_index == OLED_NUM_SEGMENTS_PER_PAGE)
    {
        s_oled_segment_index = 0;
        oled_increment_page_index();
    }
    oled_set_segment_counter(s_oled_segment_index);
}

static void oled_increment_page_index()
{
    if (++s_oled_page_index == OLED_NUM_PAGES)
    {
        s_oled_page_index = 0;
    }
    oled_set_page_counter(s_oled_page_index);
}

static void oled_reset_cursor()
{
    oled_set_segment_counter(0);
    oled_set_page_counter(0);
}

static void oled_set_segment_counter(uint8_t segment_index)
{
    assert(segment_index < 128);
    oled_write_c(0x21);
    oled_write_c(segment_index);
    oled_write_c(127);
}

static void oled_set_page_counter(uint8_t page_index)
{
    assert(page_index < 8);
    oled_write_c(0x22);
    oled_write_c(page_index);
    oled_write_c(7);
}

static void oled_flush()
{
    for (int i = 0; i < OLED_NUM_SEGMENTS_PER_PAGE * OLED_NUM_PAGES; i++)
    {
        *s_oled_data = 0;
    }
}

static void oled_write_c(uint8_t command_byte)
{
    *s_oled_cmd = command_byte;
}