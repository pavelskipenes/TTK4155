#define FOSC 4915200 // Clock Speed
#define BAUD 9600
#define UBRR FOSC/16/BAUD-1

void USART_Init( )
{
	/* Set baud rate */
	UBRRH = (unsigned char)(UBRR>>8);
	UBRRL = (unsigned char)UBRR;
	
	/* Enable receiver and transmitter */
	UCSRB = (1<<RXEN)|(1<<TXEN);
	
	/* Set frame format: 8data, 2stop bit */
	UCSRC = (1<<URSEL)|(1<<USBS)|(3<<UCSZ0);
}

void USART_Transmit( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSRA & (1<<UDRE)) );
	
	/* Put data into buffer, sends the data */
	UDR = data;
}

unsigned char USART_Receive( void )
{
	/* Wait for data to be received */
	while ( !(UCSRA & (1<<RXC)) );
	
	/* Get and return received data from buffer */
	return UDR;
}