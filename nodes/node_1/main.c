
#include "stdio.h"
#include "avr/iom162.h"
#include "uart.h"

void main(){
	
	USART_Init();
	
	while(true){
		
		USART_Transmit("A");
		
		PORTB = (1<<PB0);
		
		
	}
	


}