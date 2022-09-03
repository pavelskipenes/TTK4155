// #include "gpio.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include "usart.h"

void usart_init(uint16_t baud_rate)
{

#define BAUD 9600
#define MYUBRR (F_CPU / 16 / (BAUD - 1))
    UBRR0H = (uint8_t)(MYUBRR >> 8);
    UBRR0L = (uint8_t)MYUBRR;

    // one start bit
    // asynchronous mode
    // even parity bit
    // one stop bit
    // 8 data bits
    // in total 11 bits per frame
    // page 187 - 188
    UCSR0C = (1 << URSEL0) | (0 << UMSEL0) | (1 << UPM01) | (0 << USBS0) | (3 << UCSZ00);

    // enable transmit on USART0
    UCSR0B = (1 << TXEN0); // | (1<<RXEN0);
}
void usart_send_string(char *string)
{
    while (*string)
    {
        usart_send_char(*string++);
    }
    usart_send_char('\0');
}

void usart_send_char(char character)
{
    // if UDRE0 bit inside UCSRA0 register is set then transmission can begin
    while (!(UCSR0A & (1 << UDRE0)))
        ;
    UDR0 = character;
}
