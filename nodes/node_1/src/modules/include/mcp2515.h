#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "gpio.h"

// can control buffer
#define CANCTRL 0x0F

// can configuration buffers
#define CNF1 0x2A
#define CNF2 0x29
#define CNF3 0x28

// can transmit buffer control
#define TXB0CTRL 0x30
#define TXB1CTRL 0x40
#define TXB2CTRL 0x50

#define TXREQ 0x03

// can transmit buffers
#define TXB0SIDH 0x31
#define TXB1SIDH 0x41
#define TXB2SIDH 0x51

#define TXB0SIDL 0X32
#define TXB1SIDL 0X42
#define TXB2SIDL 0X52

#define TXB0EID8 0x33
#define TXB1EID8 0x43
#define TXB2EID8 0x53

#define TXB0EID0 0x34
#define TXB1EID0 0x44
#define TXB2EID0 0x54

#define TXB0DLC 0x35
#define TXB1DLC 0x45
#define TXB2DLC 0x55

#define TXB0D0 0x36
#define TXB1D0 0x46
#define TXB2D0 0x56

// can receive buffer control
#define RXB0CTRL 0x60
#define RXB1CTRL 0x70

// can receive buffers
#define RXB0SIDH 0x61
#define RXB1SIDH 0x71

#define RXB0SIDL 0x62
#define RXB1SIDL 0x72

#define RXB0EID8 0X63
#define RXB1EID8 0X73

#define RXB0EID0 0x64
#define RXB1EID0 0x74

#define RXB0DLC 0x65
#define RXB1DLC 0x75

#define RXB0D0 0x66
#define RXB1D0 0x76

// can receive interrupt
#define CANINTE 0x2B // CANINTE.RXnIE enables interrupt on new message
#define CANINTF 0x2C // source of the interrupt

extern GPIO mcp2515;


void mcp2515_init();

void mcp2515_reset();

uint8_t mcp2515_read(uint8_t);

void mcp2515_write(uint8_t, uint8_t);

void mcp2515_request_to_send(bool, bool, bool);

uint8_t mcp2515_read_status();

void mcp2515_bit_modify(uint8_t, uint8_t, bool);

void mcp2515_set_mode(enum can_mode);

void mcp2515_can_tx(uint16_t, uint64_t);

void mcp2515_can_rx(can_frame *);
