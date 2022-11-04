#include <assert.h>
#include <avr/interrupt.h>
#include <avr/signature.h>
#include <stdbool.h>
#include <stdio.h>
#include <util/delay.h>

#include "adc.h"
#include "joystick.h"
#include "oled.h"
#include "usart.h"

ISR(BADISR_vect)
{
    assert(false && "received unhandled interrupt");
}

int main()
{
    FILE *uart = usart_init(9600);
    (void)uart;

    FILE *oled = oled_init();
    (void)oled;

    adc_init();

    // enable interrupts
    cli();
    sei();
    SREG |= (1 << SREG_I);

    adc_sample sample = adc_read();
    joystick_init(sample.joystick[JOYSTICK_X], sample.joystick[JOYSTICK_Y]);

    while (true)
    {
        sample = adc_read();
        struct joystick_percent_t joystick = joystick_get_percent(sample.joystick[JOYSTICK_X], sample.joystick[JOYSTICK_Y]);
        fprintf(uart, "[adc] x: %X y: %X\n", sample.joystick[JOYSTICK_X], sample.joystick[JOYSTICK_Y]);
        fprintf(uart, "[joystick] x: %d y: %d\n", joystick.percent_x, joystick.percent_y);
        _delay_ms(200);
    }
}