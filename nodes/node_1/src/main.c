#include <assert.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>

#include "adc.h"
#include "can.h"
#include "game.h"
#include "joystick.h"
#include "mcp2515.h"
#include "oled.h"
#include "uart.h"

ISR(BADISR_vect)
{
	assert(false && "received unhandled interrupt");
}

int main()
{
	// setup
	FILE *uart = uart_init(9600);
	(void)uart;
	adc_init();

	can_config config = {
		.mode = CAN_MODE_NORMAL,
		.ctrl_init = mcp2515_init,
		.ctrl_mode = mcp2515_set_mode,
		.tx = mcp2515_can_tx,
		.rx = NULL,
	};
	tx_func_ptr can_tx = can_init(&config);

	FILE *oled = oled_init();
	(void)oled;

	enum state_t next_state = PLAY; // default state
	joystick_init(0xA5, 0xC9);

	// enable interrupts
	SREG |= (1 << SREG_I);
	cli();
	sei();

	while (true)
	{
		next_state = game_menu(next_state);
		if (next_state == PLAY)
		{
			adc_sample sample = adc_read();
			struct joystick_percent_t percent = joystick_get_percent(sample.joystick[ADC_JOYSTICK_X], sample.joystick[ADC_JOYSTICK_Y]);
			// send data to CAN bus
			// TODO: create a protocol on top of CAN
			char buffer[4];
			sprintf(buffer, "x:%d", percent.percent_x);
			union can_data data;
			memcpy(&data.blob, buffer, sizeof(uint64_t));
			can_tx(0x00, data);
			char buffer[4];
			sprintf(buffer, "y:%d", percent.percent_x);
			union can_data data;
			memcpy(&data.blob, buffer, sizeof(uint64_t));
			can_tx(0x00, data);
		}
	}
}