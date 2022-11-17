#include "pwm.h"
#include "pio.h"
#include "pmc.h"
#include "sam.h"

// pulse width 0.9ms - 2.1ms
// signal period = 20ms
// duty cycle = 0.9/20 <-> 2.1 / 20
// duty cycle = 0.045 <-> 0.105

void pwm_init_pin(pio_t *pin)
{

    /*
    * I/O Lines p. 973

    The pins used for interfacing the PWM are multiplexed with PIO lines. The programmer must first program the PIO
    controller to assign the desired PWM pins to their peripheral function. If I/O lines of the PWM are not used by the
    application, they can be used for other purposes by the PIO controller.
    All of the PWM outputs may or may not be enabled. If an application requires only four channels, then only four
    PIO lines will be assigned to PWM outputs.
    */
    //! Table with PIO configuration is on page 973
    // TODO: Program the PIO controller and assign the desired PWM pins to their peripheral function
    // TODO: select a pin on Arduino that is easily accessible and is in the table

    /*
    * Power Management p. 974

    The PWM is not continuously clocked. The programmer must first enable the PWM clock in the Power
    Management Controller (PMC) before using the PWM. However, if the application does not require PWM
    operations, the PWM clock can be stopped when not needed and be restarted later. In this case, the PWM will
    resume its operations where it left off.
    In the PWM description, Master Clock (MCK) is the clock of the peripheral bus to which the PWM is connected.
    */

    // DONE: TODO: Enable PWM Clock in Power management Controller (PMC) before using the PWM
    // DONE: TODO: find Peripheral identifier for PWM.
    const uint8_t pwm_id = 36;
    pmc_peripheral_enable(pwm_id);
}

/*
* Interrupt Sources p.974

The PWM interrupt line is connected on one of the internal sources of the Nested Vectored Interrupt Controller
(NVIC). Using the PWM interrupt requires the NVIC to be programmed first.

* Fault Inputs p.974

The PWM has the FAULT inputs connected to the different modules. Please refer to the implementation of these
module within the product for detailed information about the fault generation procedure. The PWM receives faults
from PIO inputs, PMC, ADC controller, and Timer/Counters

* PWM Clock Generator p. 976

The PWM master clock (MCK) is divided in the clock generator module to provide different clocks available for all
channels. Each channel can independently select one of the divided clocks.

Each linear divider can independently divide one of the clocks of the modulo n counter. The selection of the clock
to be divided is made according to the PREA (PREB) field of the PWM Clock register (PWM_CLK). The resulting
clock clkA (clkB) is the clock selected divided by DIVA (DIVB) field value.

After a reset of the PWM controller, DIVA (DIVB) and PREA (PREB) are set to 0. This implies that after reset clkA
(clkB) are turned off.

*/

/*
Pin Multiplexing

Each pin is configurable, according to product definition as either a general-purpose I/O line only, or as an I/O line
multiplexed with one or two peripheral I/Os. As the multiplexing is hardware defined and thus product-dependent,
the hardware designer and programmer must carefully determine the configuration of the PIO controllers required
by their application. When an I/O line is general-purpose only, i.e. not multiplexed with any peripheral I/O,
programming of the PIO Controller regarding the assignment to a peripheral has no effect and only the PIO
Controller can control how the pin is driven by the product.
*/

/*
Power Management

The Power Management Controller controls the PIO Controller clock in order to save power. Writing any of the
registers of the user interface does not require the PIO Controller clock to be enabled. This means that the
configuration of the I/O lines does not require the PIO Controller clock to be enabled.
However, when the clock is disabled, not all of the features of the PIO Controller are available, including glitch
filtering. Note that the Input Change Interrupt, Interrupt Modes on a programmable event and the read of the pin
level require the clock to be validated.
After a hardware reset, the PIO clock is disabled by default.
The user must configure the Power Management Controller before any access to the input line information.
*/

/*
Peripheral Clock Controller

The Power Management Controller controls the clocks of each embedded peripheral by means of the Peripheral
Clock Controller. The user can individually enable and disable the Clock on the peripherals.
The user can also enable and disable these clocks by writing Peripheral Clock Enable 0 (PMC_PCER0),
Peripheral Clock Disable 0 (PMC_PCDR0), Peripheral Clock Enable 1 (PMC_PCER1) and Peripheral Clock
Disable 1 (PMC_PCDR1) registers. The status of the peripheral clock activity can be read in the Peripheral Clock
Status Register (PMC_PCSR0) and Peripheral Clock Status Register (PMC_PCSR1).
When a peripheral clock is disabled, the clock is immediately stopped. The peripheral clocks are automatically
disabled after a reset.

In order to stop a peripheral, it is recommended that the system software wait until the peripheral has executed its
last programmed operation before disabling the clock. This is to avoid data corruption or erroneous behavior of the
system.

The bit number within the Peripheral Clock Control registers (PMC_PCER0-1, PMC_PCDR0-1, and
PMC_PCSR0-1) is the Peripheral Identifier defined at the product level. The bit number corresponds to the
interrupt source number assigned to the peripheral.
In order to save power consumption, the clock of CAN0, CAN1 peripherals can be MCK divided by a division factor
of 1, 2, 4.
This is done by setting the PMC_PCR register. It features a command and acts like a mailbox. To write the division
factor, the user needs to write a WRITE command, the peripheral ID and the chosen division factor. To read the
current division factor, the user just needs to write the READ command and the peripheral ID. Then a read access
on PMC_PCR must be performed.
DIV must not be changed while peripheral is in use or when the peripheral clock is enabled.To change the clock
division factor (DIV) of a peripheral, its clock must first be disabled by writing either EN to 0 for the corresponding
PID (DIV must be kept the same if this method is used), or writing to PMC_PCDR register. Then a second write
must be performed into PMC_PCR with the new value of DIV and a third write must be performed to enable the
peripheral clock (either by using PMC_PCR or PMC_PCER register).

*/

// Power management controller user interface (PMC)
// TODO: Find PIO peripheral inside one of these status registers
/*
    enable
    REG_PMC_PCER0;
    REG_PMC_PCER1;

    disable
    REG_PMC_PCDR0;
    REG_PMC_PCDR1;

    status
    REG_PMC_PCSR0;
    REG_PMC_PCSR1;
*/
// Register overview 538
// Peripheral Clock Enable Register 0 page 542
// Peripheral Clock Enable Register 1 page 563
//! PID 36 spotted in REG_PCER1, REG_PCDR1 and REG_PCSR1 on bit location 4;
//! Note: This register can only be written if the WPEN bit is cleared in “PMC Write Protect Mode Register”. (Check out later if problems occur)

//! PIO's bit location is it's own id number.

// Note: To get PIDx, refer to identifiers as defined in the section “Peripheral Identifiers” in the product datasheet.

// TODO: find Peripheral identifier for PWM
// TODO: find Peripheral identifier for PIO
//! Peripheral identifier for PWM is 36
//! Peripheral identifier for PIO A is 11
//! Peripheral identifier for PIO B is 12
//! Peripheral identifier for PIO C is 13
//! Peripheral identifier for PIO D is 14
//! Peripheral identifier for PIO F is 15
//! Peripheral identifier for PIO E is 16
/*
 * Peripheral Identifiers
 *
 * Page: 38
 *
 * Interesting peripherals below
 *
 * ID - Name
 * 2 - RTC      Real-time Clock
 * 3 - RTT      Real-time Timer
 * 4 - WDG      Watchdog Timer
 * 5 - PMC      Power Management Controller
 * 11 - PIOA    Parallel I/O Controller A
 * 12 - PIOB    Parallel I/O Controller B
 * 13 - PIOC    Parallel I/O Controller C
 * 14 - PIOD    Parallel I/O Controller D
 * 15 - PIOE    Parallel I/O Controller E
 * 16 - PIOF    Parallel I/O Controller F
 * 27 - TC0     Timer Counter Channel 0
 * 28 - TC1     Timer Counter Channel 1
 * 29 - TC2     Timer Counter Channel 2
 * 30 - TC3     Timer Counter Channel 3
 * 31 - TC4     Timer Counter Channel 4
 * 32 - TC5     Timer Counter Channel 5
 * 33 - TC6     Timer Counter Channel 6
 * 34 - TC7     Timer Counter Channel 7
 * 35 - TC8     Timer Counter Channel 8
 * 36 - PWM     Pulse Width Modulation Controller
 */
