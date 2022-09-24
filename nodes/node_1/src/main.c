#include <stdbool.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "usart.h"
#include "memory.h"
#include "timer.h"
#include "adc.h"

ISR(BADISR_vect)
{
    printf("bad int received GIFR: 0x%x\n", GIFR);
}

int main()
{

    FILE *uart0 = usart_init(9600);
    memory_init();
    memory_test();
    adc_init();

    char buffer[256] = {0};
    channel_value adc_values = {0};
    while (true)
    {
        adc_values = adc_read();
        printf("acd channel 0: 0x%x\n", adc_values.channel_0);
        printf("acd channel 1: 0x%x\n", adc_values.channel_1);
        printf("acd channel 2: 0x%x\n", adc_values.channel_2);
        printf("acd channel 3: 0x%x\n", adc_values.channel_3);
        // printf("Listening...\n\r");
        // fgets(buffer, 30, uart0);
        // printf("received: %s\n\r", buffer);
    }
}
