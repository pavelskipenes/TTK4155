#include <stdarg.h>
#include <stdio.h>

#include "can_controller.h"
#include "printf_stdarg.h"
#include "sam.h"
#include "uart.h"

int main()
{
    SystemInit();

    WDT->WDT_MR = WDT_MR_WDDIS;        // Disable Watchdog Timer
    PMC->PMC_WPMR &= ~(PMC_WPMR_WPEN); // PMC enable

    configure_uart();
    // brp = 42
    // phase_1 = 7
    // phase_2 = 6
    // propagation = 2
    can_init_def_tx_rx_mb(0x165 | (41 << CAN_BR_BRP_Pos));

    CAN_MESSAGE msg;
    while (1)
    {
        uint8_t status = can_receive(&msg, 0x00);
        if (!status)
        {
            printf("[NODE 2]: message received: %s\n\r", msg.data);
        }
    }
}