#include "adc.h"

#include <avr/interrupt.h>
#include <stdbool.h>

#include "gpio.h"
#include "timer.h"

static volatile uint8_t *ADC_ADDRESS = (uint8_t *)0x1400;
static volatile bool adc_busy = true;

void adc_init()
{
    memory_init();
    timer_enable_perif_clock();
    // External Interrupt Request 2 Enable page 84
    GICR |= (1 << INT2);
    // Interrupt on rising edge page 85
    EMCUCR |= (1 << ISC2);
}

ISR(INT2_vect)
{
    adc_busy = false;
}

adc_sample adc_read()
{
    adc_busy = true;
    // write to adc starts a sample
    *ADC_ADDRESS = 69;

    // booo... AVR does not have wfi
    while (adc_busy)
    {
    };

    return (adc_sample){
        .touch_bar_left = *ADC_ADDRESS,
        .joystick[0] = *ADC_ADDRESS,
        .joystick[1] = *ADC_ADDRESS,
        .touch_bar_right = *ADC_ADDRESS,
    };
}
