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

int main()
{

	FILE *uart = usart_init(9600);
	(void)uart;

	FILE *oled = oled_init();
	(void)oled;

	adc_init();
	
	can_config config = {
		.mode = CAN_MODE_LOOPBACK,
		.ctrl_init = mcp2515_init,
		.ctrl_mode = mcp2515_set_mode,
		.tx = mcp2515_can_tx,
		.rx = mcp2515_can_rx,
	};

	void (*can_tx)(uint16_t, uint64_t) = can_init(&config);

	cli();
	sei();

	can_tx(0, 255);
	can_tx(69, 111);

	while (true)
	{

		// channel_values adc_values = adc_read();

		// fprintf(uart, "[adc] 0x%X 0x%X 0x%X 0x%X\n", adc_values.channel[0], adc_values.channel[1], adc_values.channel[2], adc_values.channel[3]);
		// fprintf(oled, "%X %X %X %X\r\n", adc_values.channel[0], adc_values.channel[1], adc_values.channel[2], adc_values.channel[3]);
		fprintf(uart, "\nok");

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