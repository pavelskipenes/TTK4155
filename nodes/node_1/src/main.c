#include "adc.h"
#include "memory.h"
#include "timer.h"
#include "usart.h"
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdio.h>

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

    char buffer[256] = {0};
    channel_value adc_values = {0};
    while (true)
    {
        adc_values = adc_read();
        printf("acd channel 0: 0x%x\n", adc_values.channel_0);
        // printf("acd channel 1: 0x%x\n", adc_values.channel_1);
        // printf("acd channel 2: 0x%x\n", adc_values.channel_2);
        // printf("acd channel 3: 0x%x\n", adc_values.channel_3);
        // printf("Listening...\n\r");
        // fgets(buffer, 30, uart0);
        // printf("received: %s\n\r", buffer);
    }
}