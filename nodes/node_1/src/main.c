#include <assert.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdio.h>
#include <util/delay.h>

#include "can.h"
#include "mcp2515.h"
#include "usart.h"

ISR(BADISR_vect)
{
	assert(false && "received unhandled interrupt");
}

int main()
{

	FILE *uart = usart_init(9600);
	(void)uart;

	can_config config = {
		.mode = CAN_MODE_NORMAL,
		.ctrl_init = mcp2515_init,
		.ctrl_mode = mcp2515_set_mode,
		.tx = mcp2515_can_tx,
		.rx = NULL,
	};
	tx_func_ptr can_tx = can_init(&config);

	SREG |= (1 << SREG_I);
	cli();
	sei();

	union can_data data = {.bytes = "FUCKFUCK"};
	_delay_ms(5000);
	can_tx(0x45, data);
	while (true)
	{
	}
}