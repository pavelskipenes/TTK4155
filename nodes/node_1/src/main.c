#include <assert.h>
#include <avr/interrupt.h>
#include <avr/signature.h>
#include <avr/io.h>
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

// ISR(INT1_vect)
// {
// 	assert(false && "CAN frame received");
// 	printf("[INT] can message received\n");
// }

int main()
{

	FILE *uart = usart_init(9600);
	(void)uart;

	adc_init();

	FILE *oled = oled_init();
	(void)oled;

	can_config config = {
		.mode = CAN_MODE_LOOPBACK,
		.ctrl_init = mcp2515_init,
		.ctrl_mode = mcp2515_set_mode,
		.tx = mcp2515_can_tx,
		.rx = mcp2515_can_rx,
	};

	tx_func_ptr can_tx = can_init(&config);

	can_tx(69, 255);
	// can_tx(0, 111);

	cli();
	sei();
	SREG |= (1 << SREG_I);


	while (true)
	{

		// channel_values adc_values = adc_read();

		// fprintf(uart, "[adc] 0x%X 0x%X 0x%X 0x%X\n", adc_values.channel[0], adc_values.channel[1], adc_values.channel[2], adc_values.channel[3]);
		// fprintf(oled, "%X %X %X %X\r\n", adc_values.channel[0], adc_values.channel[1], adc_values.channel[2], adc_values.channel[3]);
		// fprintf(uart, "\nok");



		_delay_ms(1000);
	}
}