#pragma once

#include <stdbool.h>
#include <stdint.h>

#define CAN_STANDARD_MESSAGE_ID_BITS 11 // CAN2.0A
#define CAN_EXTENDED_MESSAGE_ID_BITS 29 // CAN2.0B


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
    uint8_t data[8];
    uint8_t ack;
} can_frame;


void can_init();

void can_tx(uint16_t, uint64_t);

can_frame can_rx();