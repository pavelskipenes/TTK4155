
#include "uart.h"

void main(){
	
	USART_Init();
	
	while(1){
		
		USART_Transmit("A");
		
	}
	


}