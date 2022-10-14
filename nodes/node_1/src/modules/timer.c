#include "timer.h"

#include <avr/io.h>
#include <stdint.h>

#include "gpio.h"
#include <avr/io.h>

void timer_enable_perif_clock()
{
    GPIO pb0 = {
        0,
        &DDRB,
        &PORTB,
    };
    gpio_direction(&pb0, GPIO_OUTPUT);
}

void timer1_ctc(uint8_t interrupt_frequency)
{
    // prescaler: 1024, Clear Timer on Compare Match (CTC) with OCR1A register
    TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);

    // output compare interrupt enable
    TIMSK |= (1 << OCIE1A);
    TIFR |= (1 << OCF1A);

    uint16_t counter = (F_CPU / 1024) * interrupt_frequency;

    OCR1AH = (uint8_t)(counter >> 8);
    OCR1AL = (uint8_t)(counter);
}

void timer0_ctc(uint8_t interrupt_frequency)
{
    // note: no result when using external clock on PB0
    // internal clock prescalar 1024
    TCCR0 |= (1 << CS02) | (0 << CS01) | (1 << CS00);

    // CTC mode, no port operation
    TCCR0 |= (1 << WGM01);

    // Output Compare interrupt enable on Timer 0
    SREG |= (1 << OCIE0);
    TIFR |= (1 << OCF0);
    TIMSK |= (1 << OCIE0);

    uint8_t counter = (uint8_t)((F_CPU / 1024) * interrupt_frequency);
    OCR0 = counter;
}
