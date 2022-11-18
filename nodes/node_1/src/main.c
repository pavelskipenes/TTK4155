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
#include "menu.h"
#include "oled.h"
#include "usart.h"

ISR(BADISR_vect)
{
	assert(false && "received unhandled interrupt");
}

int main()
{

	FILE *uart = usart_init(9600);
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

	SREG |= (1 << SREG_I);
	cli();
	sei();

	enum state_t next_state = PLAY; // default state

	while (true)
	{
		next_state = game_menu(next_state);
		if (next_state == PLAY)
		{
			printf("play selected\n");
		}
		if (next_state == HIGHSCORE)
		{
			printf("highscore selected\n");
		}
		// read from adc

		// char buffer[8] = {'a', 'b', 'c', 'd', 'e', 'f'};
		// adc_sample sample = adc_read();
		// struct joystick_percent_t percent = joystick_get_percent(sample.joystick[JOYSTICK_X], sample.joystick[JOYSTICK_Y]);
		// sprintf(buffer, "%d,%d", percent.percent_x, percent.percent_y);
		// send data to CAN bus
		// union can_data data;
		// memcpy(&data.blob, buffer, sizeof(uint64_t));
		// can_tx(0x00, data);
		// _delay_ms(10);
	}
}