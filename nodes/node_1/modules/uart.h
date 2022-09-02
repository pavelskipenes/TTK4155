#pragma once

#define FOSC 4915200 // Clock Speed
#define BAUD 9600
#define UBRR FOSC/16/BAUD-1

void USART_Init( );

void USART_Transmit( unsigned char data );

unsigned char USART_Receive( void );

void printf_uart( void );