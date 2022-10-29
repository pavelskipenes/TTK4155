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

ISR(BADISR_vect)
{
	assert(false && "received unhandled interrupt");
}

ISR(INT1_vect)
{
	// assert(false && "CAN frame received");
	printf("[INT] can message received\n");

	can_frame frame;
	frame.id = 0;
	frame.rtr = 0;
	frame.ide = 0;
	frame.data_length = 8;
	frame.ack = 0;
	for (int i = 0; i < 8; i++){
		frame.data[i] = 0;
	}
	
	can.rx(&frame);

	printf("id: 0x%X", frame.id);
	printf("\ndata: 0x");
	for(int i = 0; i < 8; i++){
		printf("%X", frame.data[i]);
	}
	printf("\n");

}

int main()
{

	FILE *uart = usart_init(9600);
	(void)uart;

	FILE *oled = oled_init();
	(void)oled;

	adc_init();
	
	can_config can = {
		.mode = CAN_MODE_LOOPBACK,
		.ctrl_init = &mcp2515_init,
		.ctrl_mode = &mcp2515_set_mode,
		.tx = &mcp2515_can_tx,
		.rx = &mcp2515_can_rx,
	};
	can_init(can);
	
	

	cli();
	sei();

	can.tx(0, 255);
	can.tx(69, 110);



	// enable interrupt
	SREG |= (1 << SREG_I);
	GICR |= (1 << INT1);

	while (true)
	{

		// channel_values adc_values = adc_read();

		// fprintf(uart, "[adc] 0x%X 0x%X 0x%X 0x%X\n", adc_values.channel[0], adc_values.channel[1], adc_values.channel[2], adc_values.channel[3]);
		// fprintf(oled, "%X %X %X %X\r\n", adc_values.channel[0], adc_values.channel[1], adc_values.channel[2], adc_values.channel[3]);
		// fprintf(uart, "\nok");

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