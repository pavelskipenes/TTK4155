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
    const uint32_t brp = 42;
    // phase_1 = 7
    // phase_2 = 6
    // propagation = 2
    can_init_def_tx_rx_mb(0x165 | ((brp - 1) << CAN_BR_BRP_Pos));

    // Done: TODO: Program the PIO controller and assign the desired PWM pins to their peripheral function
    // Done: TODO: select a pin on Arduino that is easily accessible and is in the table on page 973.
    // Pin: PA19, peripheral B, Signal: PWMH1. PIN42
    struct pio_t pwm_pin =
        {
            .port = PIO_PORT_A,
            .pin_number = 19,
        };
    const double period = 0.00020; // 20ms
    const double duty_cycle = 0.045;
    pwm_init_pin(period, duty_cycle);

    // pwm_servo_set_pos(uint8_t pos, uint8_t channel);
    pwm_set_duty_cycle(duty_cycle, 6);
    printf("initialization completed\n");
    CAN_MESSAGE msg;
    while (1)
    {
        uint8_t status = can_receive(&msg, 0x00);
        if (!status)
        {
            printf("message received: %s\n", msg.data);
        }
    }
}