#include "pwm.h"

#include <assert.h>

#include "pio.h"
#include "pmc.h"
#include "sam.h"

// pulse width 0.9ms - 2.1ms
// signal period = 20ms
// duty cycle = 0.9/20 <-> 2.1 / 20
// duty cycle = 0.045 <-> 0.105

// Set the pulse value of the PWM
static inline void pwm_set_pulse(uint32_t pulse, uint8_t channel)
{
    // Set CDTYUPD register to update PWM pulse
    PWM->PWM_CH_NUM[channel].PWM_CDTYUPD = PWM_CDTY_CDTY(pulse);
    // Trigger duty cycle update on next period
    PWM->PWM_SCUC = PWM_SCUC_UPDULOCK;
}

void pwm_set_duty_cycle(uint8_t duty_cycle, uint8_t channel)
{
    // duty cycle in percentage
    if (duty_cycle > 100)
    {
        duty_cycle = 100;
    }
    uint32_t cprd = PWM->PWM_CH_NUM[channel].PWM_CPRD;
    uint32_t pulse = (duty_cycle * cprd) / 100;
    pwm_set_pulse(pulse, channel);
}

void pwm_servo_set_pos(uint8_t pos, uint8_t channel)
{
    // duty cycle in percentage
    if (pos > 180)
    {
        pos = 180;
    }
    uint32_t cprd = PWM->PWM_CH_NUM[channel].PWM_CPRD;
    uint32_t cprd_top = cprd / 10;
    uint32_t cprd_bottom = cprd_top / 2;
    uint32_t cprd_range = cprd_top - cprd_bottom;
    uint32_t pulse = ((pos * cprd_range) / 180) + cprd_bottom;
    pwm_set_pulse(pulse, channel);
}

void pwm_init_pin(double period, double duty_cycle)
{
    // Enable PWM in PMC and wait for register to be set
    PMC->PMC_PCER1 |= PMC_PCER1_PID36;
    while ((PMC->PMC_PCSR1 & PMC_PCSR1_PID36) == 0)
        ;
    // Disable PIOC18
    PIOC->PIO_PDR |= PIO_PDR_P18;
    // Choose peripheral B
    PIOC->PIO_ABSR |= PIO_ABSR_P18;
    // Disable PWM write protect for register group 0, 1, 2, 3, 4 and 5
    PWM->PWM_WPCR = (PWM_WPCR_WPCMD(0) | (0x3F << 1));
    // Disable PWM channel 6
    PWM->PWM_DIS = (1 << 6);
    // PWM clock config
    // clock A prescaler X = 128
    PWM->PWM_CLK = (PWM_CLK_DIVA(1) | PWM_CLK_PREA(0x07));
    // Choose clock A for channel 6 and invert output
    PWM->PWM_CH_NUM[6].PWM_CMR = PWM_CMR_CPRE_CLKA | PWM_CMR_CPOL;
    // Set sync mode
    PWM->PWM_SCM |= PWM_SCM_UPDM_MODE0;
    // Set Channel period
    // X = 128, CPRD = 13125
    PWM->PWM_CH_NUM[6].PWM_CPRD = PWM_CPRD_CPRD(0x3345);
    // Set channel duty cycle
    PWM->PWM_CH_NUM[6].PWM_CDTY = PWM_CDTY_CDTY(0);
    // Enable PWM Channel 6
    PWM->PWM_ENA = (1 << 6);
    // Wait for PWM channel enable
    while ((PWM->PWM_SR & (1 << 6)) == 0)
        ;
    // Return 0 (success) when PWM has been enabled
    return;

    /*
        * Power Management p. 974

        The PWM is not continuously clocked. The programmer must first enable the PWM clock in the Power
        Management Controller (PMC) before using the PWM. However, if the application does not require PWM
        operations, the PWM clock can be stopped when not needed and be restarted later. In this case, the PWM will
        resume its operations where it left off.
        In the PWM description, Master Clock (MCK) is the clock of the peripheral bus to which the PWM is connected.
    */
    PMC->PMC_PCER1 |= PMC_PCER1_PID36;
    while ((PMC->PMC_PCSR1 & PMC_PCSR1_PID36) == 0)
    {
    };

    // Multiplex PIOC19 to peripheral
    PIOC->PIO_PDR |= PIO_PDR_P19;
    // set peripheral B
    PIOC->PIO_ABSR |= PIO_ABSR_P19;
    // Disable PWM write protect for register group 0, 1, 2, 3, 4 and 5
    PWM->PWM_WPCR = (PWM_WPCR_WPCMD(0) | (0x3F << 1));
    // Disable PWM channel 1
    PWM->PWM_DIS = PWM_DIS_CHID1;

    /*
        * PWM Clock Generator p. 976

        The PWM master clock (MCK) is divided in the clock generator module to provide different clocks available for all
        channels. Each channel can independently select one of the divided clocks.

        Each linear divider can independently divide one of the clocks of the modulo n counter. The selection of the clock
        to be divided is made according to the PREA (PREB) field of the PWM Clock register (PWM_CLK). The resulting
        clock clkA (clkB) is the clock selected divided by DIVA (DIVB) field value.

        After a reset of the PWM controller, DIVA (DIVB) and PREA (PREB) are set to 0. This implies that after reset clkA
        (clkB) are turned off.
    */
    // clock A prescaler X = 1024
    PWM->PWM_CLK = (PWM_CLK_DIVA(1) | PWM_CLK_PREA(0b1010));

    /*
        the waveform polarity. At the beginning of the period, the signal can be at high or low level. This property is
        defined in the CPOL field of the PWM_CMRx register. By default the signal starts by a low level.
    */
    // Select clock A as clock source
    PWM->PWM_CH_NUM[1].PWM_CMR = PWM_CMR_CPRE_CLKA | PWM_CMR_CPOL;
    // Set sync mode
    PWM->PWM_SCM |= PWM_SCM_UPDM_MODE0;
    /*
        the waveform period. This channel parameter is defined in the CPRD field of the PWM_CPRDx register.
        If the waveform is left aligned, then the output waveform period depends on the counter source clock and
        can be calculated:
        By using the PWM master clock (MCK) divided by an X given prescaler value (with X being 1, 2, 4, 8, 16, 32,
        64, 128, 256, 512, or 1024), the resulting period formula will be:

        period = (X * CPRD) / MCK
        <=>
        CPRD = (period * MCK )/X
    */
    uint32_t const CLK_PRESCALAR = 1024;
    uint32_t const CPRD = period * CHIP_FREQ_CPU_MAX / CLK_PRESCALAR;
    PWM->PWM_CH_NUM[1].PWM_CPRD = PWM_CPRD_CPRD(CPRD);
    /*
        the waveform duty-cycle. This channel parameter is defined in the CDTY field of the PWM_CDTYx
        register.
        If the waveform is left aligned then:

        duty_cycle = (period - 1/(f_channel_1_clock * CDTY))/ period
        <=>
        CDTY = -1/(f_channel_1_clock * (duty_cycle * period - period))
     */
    PWM->PWM_CH_NUM[1].PWM_CMR |= PWM_CMR_CPOL;

    uint32_t f_channel_1_clock = CHIP_FREQ_CPU_MAX / CLK_PRESCALAR;

    PWM->PWM_CH_NUM[1].PWM_CDTY = -1 / (f_channel_1_clock * (duty_cycle * period - period));
    // PWM->PWM_CH_NUM[1].PWM_CDTY = PWM_CDTY_CDTY(0);
    // Enable PWM Channel 1
    PWM->PWM_ENA = PWM_ENA_CHID1;
    // block until enabled
    while ((PWM->PWM_SR & PWM_ENA_CHID6) == 0)
    {
    };
    return;
}

/*
* Interrupt Sources p.974

The PWM interrupt line is connected on one of the internal sources of the Nested Vectored Interrupt Controller
(NVIC). Using the PWM interrupt requires the NVIC to be programmed first.

* Fault Inputs p.974

The PWM has the FAULT inputs connected to the different modules. Please refer to the implementation of these
module within the product for detailed information about the fault generation procedure. The PWM receives faults
from PIO inputs, PMC, ADC controller, and Timer/Counters
*/