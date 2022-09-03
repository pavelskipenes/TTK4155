#pragma once

#include <stdint.h>

void usart_init(uint16_t baud_rate);

void usart_send_string(char *string);

void usart_send_char(char character);
