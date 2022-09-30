#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdio.h>

#include "adc.h"
#include "memory.h"
#include "oled.h"
#include "timer.h"
#include "usart.h"

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

    memory_init();
    memory_test();
    adc_init();

    FILE *oled = oled_init();
    fprintf(oled, "fuck\nfuck");
    while (true)
    {
        channel_values adc_values = adc_read();
        printf("acd channel 0: 0x%x\n", adc_values.channel[0]);
    }
}