#pragma once

#include <stdbool.h>
#include <stdint.h>



void mcp2515_init();

void mcp2515_reset();

uint8_t mcp2515_read(uint8_t);

void mcp2515_write(uint8_t, uint8_t);

void mcp2515_request_to_send(bool, bool, bool);

uint8_t mcp2515_read_status();

void mcp2515_bit_modify(uint8_t, uint8_t, bool);