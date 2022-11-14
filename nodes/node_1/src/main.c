#include <assert.h>
#include <avr/interrupt.h>
#include <avr/signature.h>
#include <stdbool.h>
#include <stdio.h>
#include <util/delay.h>

#include "adc.h"
#include "joystick.h"
#include "usart.h"

ISR(BADISR_vect)
{
    assert(false && "received unhandled interrupt");
}

int main()
{

    FILE *uart = usart_init(9600);
    (void)uart;

    adc_init();

    // enable interrupts
    cli();
    sei();
    SREG |= (1 << SREG_I);

    while (true)
    {
        adc_sample sample = adc_read();

        fprintf(uart, "ch0:0x%02X ch1:0x%02X ch2:0x%02X ch3:0x%02X\n", sample.joystick[JOYSTICK_X], sample.joystick[JOYSTICK_Y], sample.touch_bar_left, sample.touch_bar_right);
        struct joystick_percent_t joystick = joystick_get_percent(sample.joystick[JOYSTICK_X], sample.joystick[JOYSTICK_Y]);
        fprintf(uart, "[joystick] x: %d y: %d\n", joystick.percent_x, joystick.percent_y);
        _delay_ms(200);
    }
}