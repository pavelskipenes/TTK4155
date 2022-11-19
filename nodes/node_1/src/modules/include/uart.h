#pragma once

#include <stdint.h>
#include <stdio.h>

/**
 * @brief initialize uart
 * 
 * connection details:
 * 1 stop bit
 * 1 parity bit
 * 8 data bits
 * 
 * @param baud_rate 
 * @return file stream
 */
FILE *uart_init(uint16_t baud_rate);

/**
 * @brief 
 * 
 *
 * @param character send
 * @param fd stream
 * @return 0
 */
int uart_put_char(char character, FILE *fd);

/**
 * @brief get char
 * 
 * @param fd stream
 * @return char as int
 */
int uart_get_char(FILE *fd);

/**
 * @brief flush receive buffer
 * 
 */
void uart_flush_receive_buffer();

/**
 * @brief read character
 * 
 * @return received character
 * 
 * TODO: add config as args. Baudrate num stop bits etc...
 */
char uart_read_char();
