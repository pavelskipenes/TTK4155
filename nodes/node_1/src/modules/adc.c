#include "adc.h"

#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdio.h>

#include "timer.h"
#include "gpio.h"

static volatile uint8_t *ADC_ADDRESS = (uint8_t *)0x1400;
static volatile bool adc_busy = true;

void adc_init()
{
    timer_enable_perif_clock();
    DDRE &= ~(1 << PE0); // PE0 as input
    // enable interrupt on PE0 on rising edge
    // SREG |= (1 << SREG_I);
    cli();
    GICR = (1 << INT2);
    EMCUCR = (1 << ISC2);
    sei();
}

ISR(INT1_vect)
{
    printf("External interrupt 1 handled\n");
    adc_busy = false;
}

channel_value adc_read()
{
    // write to adc starts a sample
    *ADC_ADDRESS = 69;
    
    // booo... AVR does not have wfi
    while(adc_busy);
    adc_busy = false;

    return (channel_value){
        *ADC_ADDRESS,
        *ADC_ADDRESS,
        *ADC_ADDRESS,
        *ADC_ADDRESS};
}
