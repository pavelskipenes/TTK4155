#include "memory.h"
#include <assert.h>
#include <avr/io.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void memory_init()
{
    static bool initialized = false;
    if (initialized)
    {
        return;
    }
    initialized = true;

    // enable external memory interface. page 30
    MCUCR |= (1 << SRE);

    // mask JTAG ports PORTC 7:4
    SFIOR |= (1 << XMM2);
    printf("[info]: memory initialized\n");
}

void memory_add_delay()
{
    // insert wait states when interacting with slow peripherals
    EMCUCR |= (1 << SRW01) | (1 << SRW00);
}

void memory_test()
{
    volatile uint8_t *sram_start = (volatile uint8_t *)0x1800;
    const uint16_t sram_size = 2048; // 2KB
    volatile uint8_t seed = (uint8_t)rand();
    // write random data to ram
    srand(seed);
    uint16_t i = 0;
    for (; i < sram_size; i++)
    {
        uint8_t write_value = (uint8_t)rand();
        sram_start[i] = write_value;        // write
        uint8_t read_value = sram_start[i]; // read
        assert(read_value == write_value);
    }

    // test data integrity after write and that data write above was not written to one address only
    srand(seed);
    for (i = 0; i < sram_size; i++)
    {
        uint8_t written_value = (uint8_t)rand();
        uint8_t read_value = sram_start[i];
        assert(read_value == written_value);
    }
}
