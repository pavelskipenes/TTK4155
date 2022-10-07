#include "adc.h"
#include "memory.h"
#include "timer.h"
#include "usart.h"
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdio.h>
#include <util/delay.h>

ISR(BADISR_vect)
{
    printf("error: received unhandled interrupt\nhalting\n");
    // halt
    while (true)
    {
        __asm__ __volatile__("nop");
    }
}

int main()
{

    usart_init(9600);
    memory_init();
    memory_test();
    adc_init();

    while (true)
    {
        channel_values adc_values = adc_read();

        printf("acd 0x%X\t\t0x%X\t\t0x%X\t\t0x%X\n", adc_values.channel[0], adc_values.channel[1], adc_values.channel[2], adc_values.channel[3]);
        _delay_ms(200);
    }
}