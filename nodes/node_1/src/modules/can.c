#include "can.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>

void can_init(can_config config)
{
	static bool initialized = false;
	if (initialized)
	{
		return;
	}

	can.ctrl_init();
	can.ctrl_mode(config.mode);
}

