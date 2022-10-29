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

ISR(INT1_vect)
{
	//assert(false && "CAN frame received");
	printf("\ncan message received");
	
	static can_frame frame;
	frame.id = 0;
	frame.rtr = 0;
	frame.ide = 0;
	frame.data_length = 8;
	for(int i = 0; i < 8; i++) frame.data[i] = 0;
	//frame.data = {0};
	frame.ack = 0;
	
	can_rx(&frame);
	
	for(int i = 0; i < 8; i++){
		printf("\ndata: %c", frame.data[i]);
	}
	
}

int main()
{

	
    FILE *uart = usart_init(9600);
    (void)uart;
	
    FILE *oled = oled_init();
    (void)oled;

    adc_init();
	
	// enable interrupt
	SREG |= (1 << SREG_I);
	GICR |= (1 << INT1);


	mcp2515_init();
	can_init();

    cli();
    sei();
	
	uint64_t data = 255;
	can_tx(0, data);

    while (true)
    {
		
        //channel_values adc_values = adc_read();
		
        //fprintf(uart, "[adc] 0x%X 0x%X 0x%X 0x%X\n", adc_values.channel[0], adc_values.channel[1], adc_values.channel[2], adc_values.channel[3]);
        //fprintf(oled, "%X %X %X %X\r\n", adc_values.channel[0], adc_values.channel[1], adc_values.channel[2], adc_values.channel[3]);
		//fprintf(uart, "\nok");
		
		/*
		mcp2515_write(CANINTE, 0x3);
		_delay_ms(10);
		printf("0X%X\n", mcp2515_read(CANINTE));
		*/
		
		/*
		mcp2515_write(CANINTE, 0x3);
		printf("\n%X", mcp2515_read(CANINTE));
		*/
		
		
		_delay_ms(1000);
		
    }
}