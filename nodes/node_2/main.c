#include <stdarg.h>
#include <stdio.h>

#include "can_controller.h"
#include "printf_stdarg.h"
#include "sam.h"
#include "uart.h"

int main()
{
	SystemInit();

	WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer
	PMC->PMC_WPMR &= ~(PMC_WPMR_WPEN); // PMC enable

	uint8_t can_rx = PIO_PA1A_CANRX0;
	uint8_t can_tx = PIO_PA0A_CANTX0;
	uint32_t can_br = 167; // calculated
	uint32_t can_ph1 = 7;
	uint32_t can_ph2 = 6;
	uint32_t can_prop = 2;
	
	uint16_t msg_id = 0;
	char data[8] = "1234567";
	char msg_length = '8';

	configure_uart();
	printf("Hello World\n\r");
	can_init_def_tx_rx_mb(can_br, can_ph1, can_ph2,can_prop);
	
	CAN_MESSAGE msg = {
		.id = msg_id,
		.data_length = msg_length,
		.data = data,
	};
	
	while (1)
	{
		
		//can_send(&msg, 0);
		while(can_receive(&msg, 0));
		//printf("%s",msg.data);
		
		for (int i = 0; i < 8; i++)
		{
			printf("%d\n\r", msg.data[i]);
		}
	
	}
	
	
}