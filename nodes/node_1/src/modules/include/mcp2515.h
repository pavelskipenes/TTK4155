#pragma once

#include <stdbool.h>
#include <stdint.h>



void mcp2515_init();

void mcp2515_reset();

char mcp2515_read(char);

void mcp2515_write(char, char);

void mcp2515_request_to_send(bool, bool, bool);

char mcp2515_read_status();

void mcp2515_bit_modify(char, uint8_t, bool);