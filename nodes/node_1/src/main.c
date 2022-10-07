#include <assert.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>

// #include "adc.h"
#include "fb.h"
#include "memory.h"
// #include "oled.h"
// #include "timer.h"
#include "usart.h"

ISR(BADISR_vect)
{
    assert(false && "received unhandled interrupt");
}

int main()
{
    FILE *uart = usart_init(9600);
    (void)uart;
    // printf("__malloc_heap_start: %p\n", (void *)__malloc_heap_start);
    // printf("__malloc_heap_end: %p\n", (void *)__malloc_heap_end);

    memory_init();
    memory_test();
    // adc_init();

    FILE *oled = fb_init();
    (void)oled;

    while (true)
    {
        // channel_values adc_values = adc_read();
        // printf("[acd]\t0x%X\t0x%X\t0x%X\t0x%X\t\n", adc_values.channel[0], adc_values.channel[1], adc_values.channel[2], adc_values.channel[3]);
        _delay_ms(200);
    }
}