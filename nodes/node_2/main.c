#include <stdio.h>
#include <stdarg.h>
#include "uart.h"
#include "printf-stdarg.h"

#include "sam.h"


int main()
{
	SystemInit();

	WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer
	PMC->PMC_WPMR &= ~(PMC_WPMR_WPEN); // PMC enable

	uint8_t can_rx = PIO_PA1A_CANRX0;
	uint8_t can_tx = PIO_PA0A_CANTX0;
	uint32_t can_br = 167; // calculated
	uint32_t can_ph1 = 4;
	uint32_t can_ph2 = 4;
	uint32_t can_prop = 2;
	
	configure_uart();
	printf("Hello World\n\r");
	can_init_def_tx_rx_mb(can_br, can_ph1, can_ph2,can_prop);
	

	while (1)
	{
		/* code */
	}
	
}