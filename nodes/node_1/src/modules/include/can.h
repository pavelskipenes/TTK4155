#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define CAN_STANDARD_MESSAGE_ID_BITS 11 // CAN2.0A
#define CAN_EXTENDED_MESSAGE_ID_BITS 29 // CAN2.0B

enum can_frame_type
{
	CAN_FRAME_DATA,		// send data
	CAN_FRAME_REMOTE,	// request data
	CAN_FRAME_ERROR,	// bus error signaling
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

union can_data
{
	uint8_t bytes[8];
	char character[8];
	uint64_t blob;
};

typedef struct can_frame_t
{
	uint16_t id;
	bool rtr;
	bool ide;
	uint8_t data_length;
	union can_data data;
	uint8_t ack;
} can_frame;

typedef void (*tx_func_ptr)(uint16_t id_remote, union can_data data);

typedef struct can_config_t
{
	enum can_mode mode;
	void (*ctrl_init)(void);
	void (*ctrl_mode)(enum can_mode);
	tx_func_ptr tx;			 // frame transmitter
	void (*rx)(can_frame *); // frame receiver. Not supported
} can_config;

tx_func_ptr can_init(can_config *);