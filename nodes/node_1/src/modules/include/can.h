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

typedef struct can_frame_t
{
    // TODO: identifier
    uint32_t arbitration;
    uint8_t control;
    uint8_t data;
    uint16_t crc;
    uint8_t ack;
    uint8_t eof;

} can_frame;
