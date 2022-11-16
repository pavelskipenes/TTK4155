#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "can.h"
#include "gpio.h"

extern GPIO mcp2515;

void mcp2515_init();
void mcp2515_can_tx(uint16_t id, union can_data data);
void mcp2515_set_mode(enum can_mode);

// reception and interrupts are not supported
#if 0
void mcp2515_can_rx(can_frame *);
void handle_interrupt();
enum mcp2515_interrupt_source
{
	CMP2515_INT_RX0_FULL,
	CMP2515_INT_RX1_FULL,
	CMP2515_INT_TX0_EMPTY,
	CMP2515_INT_TX1_EMPTY,
	CMP2515_INT_TX2_EMPTY,
	CMP2515_INT_ERROR,
	CMP2515_INT_WAKE_UP,
	CMP2515_INT_MESSAGE_ERROR,
};
#endif