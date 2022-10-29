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

enum can_instruction
{
	CAN_INSTRUCTION_WRITE = 0x2,
	CAN_INSTRUCTION_READ = 0x3,
	CAN_INSTRUCTION_BIT_MODIFY = 0x5,
	CAN_INSTRUCTION_READ_STATUS = 0xA0,
	CAN_INSTRUCTION_RESET = 0xC0,
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

typedef struct can_config_t
{
	enum can_mode mode;
	void (*ctrl_init);
	void (*ctrl_mode)(can_mode);
	void (*tx)(uint16_t, uint64_t);
	void (*rx)(can_frame);
} can_config;


void can_init(struct can_config);