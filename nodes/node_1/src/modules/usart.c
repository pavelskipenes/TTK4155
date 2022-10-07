#include "usart.h"

#include <avr/io.h>
#include <stdio.h>

FILE *usart_init(uint16_t baud_rate)
{

    const uint16_t my_ubrr = (uint16_t)(F_CPU / 16 / (baud_rate - 1));

    UBRR0H = (uint8_t)(my_ubrr >> 8);
    UBRR0L = (uint8_t)(my_ubrr);

    // one start bit
    // asynchronous mode
    // even parity bit
    // one stop bit
    // 8 data bits
    // in total 11 bits per frame
    // page 187 - 188
    UCSR0C = (1 << URSEL0) | (0 << UMSEL0) | (1 << UPM01) | (0 << USBS0) | (3 << UCSZ00);

    // enable transmit on USART0
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);

    usart_flush_receive_buffer();

    return fdevopen(usart_put_char, usart_get_char);
}

int usart_put_char(char character, FILE *fd __attribute__((unused)))
{
    // if UDRE0 bit inside UCSRA0 register is set then transmission can begin
    while (!(UCSR0A & (1 << UDRE0))){

    };
    UDR0 = (uint8_t)character;
    return 0;
}

int usart_get_char(FILE *fd __attribute__((unused)))
{
    return usart_read_char();
}

void usart_flush_receive_buffer()
{
    while (UCSR0A & (1 << RXC0))
    {
        uint8_t _ __attribute__((unused)) = UDR0;
    }
}

char usart_read_char()
{
    while (!(UCSR0A & (1 << RXC0)))
    {
    };
    return (char)UDR0;
}