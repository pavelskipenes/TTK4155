#pragma once

#include <stdint.h>

#include "can.h"
#include "gpio.h"

// extern GPIO mcp2515;

/**
 * @brief Initialize mcp2515 can transceiver
 *
 */
void mcp2515_init();

/**
 * @brief can transmit handler
 *
 * @param id of the receiver
 * @param data payload
 */
void mcp2515_can_tx(uint16_t id, union can_data data);

/**
 * @brief change can mode
 * @see enum can_mode
 *
 */
void mcp2515_set_mode(enum can_mode);

