#include "gpio.h"
#include <stdbool.h>
#include <stdio.h>
#include <avr/io.h>

void gpio_set_state(Pin pin, bool state)
{


	uint8_t port_b;




	const Pin port_lut[24] = {PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7,
							  PB0, PB1, PB2, PB3, PB4, PB5, PB6, PB7,
							  PC0, PC1, PC2, PC3, PC4, PC5, PC6, PC7,
							  PD0, PD1, PD2, PD3, PD4, PD5, PD6, PD7};

	if (pin >= A0 && pin <= A7)
	{
		PORTA = (state << port_lut[pin]);
		return;
	}
	if (pin >= B0 && pin <= B7)
	{
		
		if(state == GPIO_HIGH) set_bit(port_b, port_lut[pin]);
		if(state == GPIO_LOW) clear_bit(port_b, port_lut[pin]);
			uint8_t port = PORTB;
			PORTB = set_bit(port, 6); 
			PORTB = port_b;
		return;
	}
	if (pin >= C0 && pin <= D7)
	{
		PORTC = (state << port_lut[pin]);
		return;
	}
	if (pin >= D0 && pin <= B7)
	{
		PORTD = (state << port_lut[pin]);
		return;
	}
}

void gpio_set_direction(Pin pin, bool direction)
{

	const Pin port_lut[24] = {DDA0, DDA1, DDA2, DDA3, DDA4, DDA5, DDA6, DDA7,
							  DDB0, DDB1, DDB2, DDB3, DDB4, DDB5, DDB6, DDB7,
							  DDC0, DDC1, DDC2, DDC3, DDC4, DDC5, DDC6, DDC7,
							  DDD0, DDD1, DDD2, DDD3, DDD4, DDD5, DDD6, DDD7};

	if (pin >= A0 && pin <= A7)
	{
		PORTA = (direction << port_lut[pin]);
		return;
	}
	if (pin >= B0 && pin <= B7)
	{
		PORTB = (direction << port_lut[pin]);
		return;
	}
	if (pin >= C0 && pin <= D7)
	{
		PORTC = (direction << port_lut[pin]);
		return;
	}
	if (pin >= D0 && pin <= B7)
	{
		PORTD = (direction << port_lut[pin]);
		return;
	}
}




uint8_t set_bit(uint8_t byte, int pos){
	return byte |= (1<< pos);
}

uint8_t clear_bit(uint8_t byte, int pos){
	return byte &= ~(1<< pos);
}