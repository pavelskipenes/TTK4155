#include <assert.h>
#include <avr/interrupt.h>
#include <avr/signature.h>
#include <stdbool.h>
#include <stdio.h>
#include <util/delay.h>

#include "adc.h"
#include "oled.h"
#include "usart.h"
#include "memory.h"
#include "timer.h"
#include "can.h"
#include "spi.h"
#include "mcp2515.h"
#include "can.h"

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
	
	spi_init();
	spi_init_slave(&mcp2515);
	//mcp2515_init();
	//can_init();

    cli();
    sei();
	
	uint8_t data = 255;
	spi_send_byte(&mcp2515, data);
	
    while (true)
    {
        channel_values adc_values = adc_read();
		
        fprintf(uart, "[adc] 0x%X 0x%X 0x%X 0x%X\n", adc_values.channel[0], adc_values.channel[1], adc_values.channel[2], adc_values.channel[3]);
        fprintf(oled, "%X %X %X %X\r\n", adc_values.channel[0], adc_values.channel[1], adc_values.channel[2], adc_values.channel[3]);
        
		
		
		
		_delay_ms(200);
		
    }
}