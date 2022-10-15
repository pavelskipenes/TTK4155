#include "memory.h"
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>

void memory_init()
{
    // enable external memory interface. page 30
    MCUCR |= (1 << SRE);

    // page 32
    SFIOR |= (1 << XMM2); // mask JTAG ports PORTC 7:4
}

void memory_test()
{
    printf("starting memory test...\n\r");
    volatile uint8_t *sram_start = (volatile uint8_t *)0x1800;
    uint16_t sram_size = 2048; // 2KB
    uint16_t write_errors = 0;
    uint16_t read_errors = 0;
    volatile uint8_t seed = (uint8_t)rand();
    // write random data to ram
    srand(seed);
    uint16_t write_success = 0;
    uint16_t i = 0;
    for (; i < sram_size; i++)
    {
        uint8_t write_value = (uint8_t)rand();
        sram_start[i] = write_value;        // write
        uint8_t read_value = sram_start[i]; // read
        if (read_value != write_value)
        {
            write_errors++;
        }
        else
        {
            write_success++;
        }
    }
    // assert(sram_size == write_errors + write_success);

    // test data integrity after write and that data write above was not written to one address only
    srand(seed);
    uint16_t read_success = 0;
    for (i = 0; i < sram_size; i++)
    {
        uint8_t written_value = (uint8_t)rand();
        uint8_t read_value = sram_start[i];
        if (read_value != written_value)
        {
            read_errors++;
        }
        else
        {
            read_success++;
        }
    }

    // assert(sram_size == read_errors + read_success);
    if (write_errors || read_errors)
    {
        printf("%u/%u write errors and %u/%u read errors\n\r\n\r", write_errors, sram_size, read_errors, sram_size);
    }
    else
    {
        printf("memory test ok\n\r");
    }
}
