#include "adc.h"

#include <avr/interrupt.h>
#include <stdbool.h>

#include "timer.h"
#include "gpio.h"

static volatile uint8_t *ADC_ADDRESS = (uint8_t *)0x1400;
static volatile bool adc_busy = true;

void adc_init()
{
    timer_enable_perif_clock();
    // enable interrupt on PE0 on rising edge page 84
    cli();
    GICR |= (1 << INT2);
    EMCUCR |= (1 << ISC2);
    sei();
}

ISR(INT2_vect)
{
    adc_busy = false;
}

channel_value adc_read()
{
    // write to adc starts a sample
    *ADC_ADDRESS = 69;

    // booo... AVR does not have wfi
    while (adc_busy)
    {
        __asm__ __volatile__("nop");
    };
    adc_busy = false;

    return (channel_value){
        *ADC_ADDRESS,
        *ADC_ADDRESS,
        *ADC_ADDRESS,
        *ADC_ADDRESS};
}
