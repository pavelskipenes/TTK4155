#include "GPIO.h"

typedef enum {
	A0, A1, A2, A3, A4, A5, A6, A7,
	B0, B1, B2, B3, B4, B5, B6, B7,
	C0, C1, C2, C3, C4, C5, C6, C7,
	D0, D1, D2, D3, D4, D5, D6, D7
} Pin;

void GPIOvalue(Pin pin, bool value){
	
	Pin ports[24] = { PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7,
					  PB0, PB1, PB2, PB3, PB4, PB5, PB6, PB7,
					  PC0, PC1, PC2, PC3, PC4, PC5, PC6, PC7,
					  PD0, PD1, PD2, PD3, PD4, PD5, PD6, PD7};
	
	switch(pin){
		case A0 || A1 || A2 || A3 || A4 || A5 || A6 || A7:
			port = PORTA;
			break;
		case B0 || B1 || B2 || B3 || B4 || B5 || B6 || B7:
			port = PORTB;
			break;
		case C0 || C1 || C2 || C3 || C4 || C5 || C6 || C7:
			port = PORTC;
			break;
		case D0 || D1 || D2 || D3 || D4 || D5 || D6 || D7:
			port = PORTD;
			break;
	}
	
	port = value << ports[pin];
	
}
