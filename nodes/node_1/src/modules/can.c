#include "can.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>

static void (*can_rx)(can_frame *);

FILE *can_init(can_config *config)
{
	static bool initialized = false;
	if (initialized)
	{
		return;
	}

	config->ctrl_init();
	config->ctrl_mode(config->mode);

	can_rx = config->rx;

	return fdevopen(config->tx, NULL);
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
	for (int i = 0; i < 8; i++)
	{
		frame.data[i] = 0;
	}

	can_rx(&frame);

	printf("id: 0x%X", frame.id);
	printf("\ndata: 0x");
	for (int i = 0; i < 8; i++)
	{
		printf("%X", frame.data[i]);
	}
	printf("\n");
}