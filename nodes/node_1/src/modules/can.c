#include "can.h"

#include <assert.h>
#include <avr/io.h>
#include <stdio.h>
#include <string.h>

#if 0
#include <avr/interrupt.h>
ISR(INT1_vect)
{
	// TODO: check source of the interrupt

	handle_interrupt();

	can_frame frame;
	can_rx(&frame);

	printf("id: 0x%X\t", frame.id);
	printf("data: 0x");
	for (int i = 0; i < 8; i++)
	{
		printf("%c", frame.data.character[i]);
	}
	printf("\n");
}

static void (*can_rx)(can_frame *);
#endif

tx_func_ptr can_init(can_config *config)
{
	static bool initialized = false;
	if (initialized)
	{
		return NULL;
	}
	initialized = true;

	// interrupts are diabled
	// enable INT1
	// GICR |= (1 << INT1);
	// can_rx = config->rx;

	config->ctrl_init();
	config->ctrl_mode(config->mode);

	return config->tx;
}
