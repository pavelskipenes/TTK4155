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

    channel_values adc_values_tmp = adc_read();
    struct joystick_config_t joystick_config = {
        .adc_initial_value_x = adc_values_tmp.channel[2],
        .adc_initial_value_y = adc_values_tmp.channel[1],
    };
    joystick_init(&joystick_config);

    while (true)
    {
        channel_values adc_values = adc_read();
        struct joystick_percent_t joystick = joystick_get_percent(adc_values.channel[2], adc_values.channel[1]);

        fprintf(uart, "[joystick] x: %d y: %d\n", joystick.percent_x, joystick.percent_y);
        _delay_ms(200);
    }
}