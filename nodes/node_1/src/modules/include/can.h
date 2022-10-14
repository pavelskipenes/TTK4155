#pragma once

#include <stdbool.h>
#include <stdint.h>

#define CAN_STANDARD_MESSAGE_ID_BITS 11 // CAN2.0A
#define CAN_EXTENDED_MESSAGE_ID_BITS 29 // CAN2.0B

// can control buffer
#define CANCTRL 0x0F

// can transmit buffers
#define TXB0SIDH 0x31
#define TXB1SIDH 0x41
#define TXB2SIDH 0x51

#define TCB0DISL 0X32
#define TCB1DISL 0X42
#define TCB2DISL 0X52

#define TXB0EID8 0x33
#define TXB1EID8 0x43
#define TXB2EID8 0x53

#define TXB0EID0 0x34
#define TXB1EID0 0x34
#define TXB2EID0 0x34

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
#define CANINTF 0x2C



enum can_frame_type
{
    CAN_FRAME_DATA,     // send data
    CAN_FRAME_REMOTE,   // request data
    CAN_FRAME_ERROR,    // bus error signaling
    CAN_FRAME_OVERLOAD, // increase time between data frames
};

enum can_error_condition
{
    CAN_ERROR_CRC,
    CAN_ERROR_ACK,
    CAN_ERROR_FORM,
    CAN_ERROR_BIT,
    CAN_ERROR_STUFF
};

enum can_state
{
    CAN_STATE_ERROR_ACTIVE,
    CAN_STATE_ERROR_PASSIVE,
    CAN_STATE_ERROR_BUS_OFF
};

enum can_mode
{
	CAN_MODE_NORMAL,
	CAN_MODE_SLEEP,
	CAN_MODE_LOOPBACK,
	CAN_MODE_LISTEN_ONLY,
	CAN_MODE_CONFIG
};

typedef struct can_frame_t
{
    uint16_t id;
	bool rtr;
    bool ide;
	uint8_t data_length;
    char data[8];
    uint8_t ack;
} can_frame;


void can_init();

void can_tx(uint16_t, *char[]);

can_frame can_rx();