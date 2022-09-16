#pragma once

#include <stdint.h>
#include <stdio.h>

FILE *usart_init(uint16_t baud_rate);
int usart_put_char(char character, FILE *fd);
int usart_get_char(FILE *fd);
void usart_flush_receive_buffer();
char usart_read_char();
