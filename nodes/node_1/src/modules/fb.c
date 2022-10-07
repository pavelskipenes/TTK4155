#include "fb.h"

#include <assert.h>
#include <stdlib.h>
#include <util/atomic.h>
// #include <signal.h>
#include <stdbool.h>
#include <stdio.h>

#include "fonts.h"
#include "oled.h"

#define FB_FRONT 0
#define FB_BACK 1

typedef struct page_t
{
    uint8_t segment[OLED_NUM_SEGMENTS_PER_PAGE];
} page;

typedef struct fb_t
{
    page page[OLED_NUM_PAGES];
    int cursor_page;
    int cursor_segment;
} fb;

typedef struct fb_buffers
{
    fb *frame_buffers[2];
} fb_buffers;

static int fb_put_char8(char, FILE *);
static void fb_write8(char8, fb *);
static void fb_increment_page_counter(fb *);
static void fb_reset_segment_counter(fb *);

/**
 * @brief open an output stream to oled display.
 *
 * @note allocates memory.
 *
 * @return FILE* stream.
 */

FILE *fb_init()
{
    printf("[info]: creating frame buffer\n");
    oled_init();

    printf("[info]: attempting to allocate %d\n", sizeof(fb));
    fb *fb0 = (fb *)calloc(1, sizeof(fb));
    fb *fb1 = (fb *)calloc(1, sizeof(fb));
    assert(fb0 && "failed to allocate memory for frame buffer");
    assert(fb1 && "failed to allocate memory for frame buffer");

    FILE *fd = fdevopen(fb_put_char8, NULL);
    assert(fd && "failed to create stream");

    if (fdev_get_udata(fd) == (fb_buffers *)NULL)
    {
        fb_buffers *buffers = calloc(sizeof(fb_buffers), 1);
        assert(buffers && "failed to allocate memory for frame buffer container");
        fdev_set_udata(fd, buffers);

        fb_buffers *buffer = (fb_buffers *)fdev_get_udata(fd);
        assert(buffer && "failed to get udata");

        buffer->frame_buffers[FB_FRONT] = fb0;
        buffer->frame_buffers[FB_BACK] = fb1;
    }

    // TODO(pavel): start oled write interrupts

    return fd;
}

void fb_swap(FILE *fd)
{
    assert(fd);
    fb_buffers *buffers = ((fb_buffers *)fdev_get_udata(fd));
    assert(buffers);

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        // xor swap
        buffers->frame_buffers[FB_BACK] = (fb *)((uintptr_t)(buffers->frame_buffers[FB_BACK]) ^ (uintptr_t)(buffers->frame_buffers[FB_FRONT]));
        buffers->frame_buffers[FB_FRONT] = (fb *)((uintptr_t)(buffers->frame_buffers[FB_FRONT]) ^ (uintptr_t)(buffers->frame_buffers[FB_BACK]));
        buffers->frame_buffers[FB_BACK] = (fb *)((uintptr_t)(buffers->frame_buffers[FB_BACK]) ^ (uintptr_t)(buffers->frame_buffers[FB_FRONT]));

        buffers->frame_buffers[FB_BACK]->cursor_page = buffers->frame_buffers[FB_BACK]->cursor_page ^ buffers->frame_buffers[FB_FRONT]->cursor_page;
        buffers->frame_buffers[FB_FRONT]->cursor_page = buffers->frame_buffers[FB_FRONT]->cursor_page ^ buffers->frame_buffers[FB_BACK]->cursor_page;
        buffers->frame_buffers[FB_BACK]->cursor_page = buffers->frame_buffers[FB_BACK]->cursor_page ^ buffers->frame_buffers[FB_FRONT]->cursor_page;
    }
}

static void fb_write8(char8 character8, fb *frame_buffer)
{
    assert(frame_buffer);
    for (int i = 0; i < 8; i++)
    {
        // add one segment at a time and increment the cursor
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            frame_buffer->page[frame_buffer->cursor_page++].segment[frame_buffer->cursor_segment++] = (uint8_t)character8.segments[i];
            // Wrap cursor around on overflow
            if (frame_buffer->cursor_segment % (OLED_NUM_SEGMENTS_PER_PAGE - 1))
            {
                frame_buffer->cursor_segment += 1;
                if (frame_buffer->cursor_page % (OLED_NUM_PAGES - 1))
                {
                    frame_buffer->cursor_page += 1;
                }
            }
        }
    }
}

static void fb_increment_page_counter(fb *buffers)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        if (++(buffers->cursor_page) == OLED_NUM_PAGES)
        {
            buffers->cursor_page = 0;
        }
    }
}

static void fb_reset_segment_counter(fb *buffers)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        buffers->cursor_segment = 0;
    }
}

static int fb_put_char8(char c, FILE *fd)
{
    assert(fd);
    assert(c - ' ' >= 0);
    assert(c - ' ' < 95);
    // #error "results in run time error when included in transaction unit"
    // fb_buffers *buffers = (fb_buffers *)fdev_get_udata(fd);
    // ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    // {
    //     fb *fb_back = buffers->frame_buffers[FB_BACK];
    //
    //     switch (c)
    //     {
    //     case '\n':
    //         fb_increment_page_counter(fb_back);
    //         break;
    //
    //     case '\r':
    //         fb_reset_segment_counter(fb_back);
    //         break;
    //     case '\t':
    //         for (int i = 0; i < 4; i++)
    //         { // always 4 spaces in a tab. Always
    //             fb_put_char8(' ', fd);
    //         }
    //         break;
    //
    //     default:
    //         fb_write8(fonts_get_char8(c), fb_back);
    //     }
    // }
    return 0;
}
