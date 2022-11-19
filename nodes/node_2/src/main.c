#include <assert.h>
#include <stdarg.h>
#include <stdio.h>

#include "can_controller.h"
#include "printf_stdarg.h"
#include "pwm.h"
#include "sam.h"
#include "uart.h"

int main()
{
    SystemInit();
    WDT->WDT_MR = WDT_MR_WDDIS;        // Disable Watchdog Timer
    PMC->PMC_WPMR &= ~(PMC_WPMR_WPEN); // PMC enable
    configure_uart();

    // phase_1 = 7
    // phase_2 = 6
    // propagation = 2
    const uint32_t BRP = 42;
    can_init_def_tx_rx_mb(0x165 | ((BRP - 1) << CAN_BR_BRP_Pos));

    pwm_init_pin();

    printf("initialization completed\n");
    CAN_MESSAGE msg;

    double duty_cycle = 0.045;
    while (1)
    {

        uint8_t status = can_receive(&msg, 0x00);
        if (!status)
        {
            // message handling
        }
        // reset value
        if (duty_cycle >= 0.105)
        {
            duty_cycle = 0.45;
        }
        duty_cycle++;
        pwm_set_duty_cycle(duty_cycle);
    }
}
