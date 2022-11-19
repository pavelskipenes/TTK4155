#include "can.h"

#include <assert.h>
#include <avr/io.h>
#include <stdio.h>
#include <string.h>


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
