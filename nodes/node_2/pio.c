#include "pio.h"
#include "pmc.h"
#include "sam.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

/*
* Pin Multiplexing p. 620
Each pin is configurable, according to product definition as either a general-purpose I/O line only, or as an I/O line
multiplexed with one or two peripheral I/Os. As the multiplexing is hardware defined and thus product-dependent,
the hardware designer and programmer must carefully determine the configuration of the PIO controllers required
by their application. When an I/O line is general-purpose only, i.e. not multiplexed with any peripheral I/O,
programming of the PIO Controller regarding the assignment to a peripheral has no effect and only the PIO
Controller can control how the pin is driven by the product.
*/
/*
* Power Management p. 620
The Power Management Controller controls the PIO Controller clock in order to save power. Writing any of the
registers of the user interface does not require the PIO Controller clock to be enabled. This means that the
configuration of the I/O lines does not require the PIO Controller clock to be enabled.
However, when the clock is disabled, not all of the features of the PIO Controller are available, including glitch
filtering. Note that the Input Change Interrupt, Interrupt Modes on a programmable event and the read of the pin
level require the clock to be validated.
After a hardware reset, the PIO clock is disabled by default.
The user must configure the Power Management Controller before any access to the input line information.

*/
// TODO: Configure the Power Management Controller (PMC) before accessing the input lines (whatever that means)
/*

* Interrupt Generation p. 620
The PIO COntroller is connected on one of the sources of the Nested Vectored Interrupt Controller (NVIC). Using
the PIO Controller requires the NVIC to be programmed first.
The PIO Controller interrupt can be generated only if the PIO Controller clock is enabled.

*/

/*
* I/O Line or Peripheral Function Selection
When a pin is multiplexed with one or two peripheral functions, the selection is controlled with the registers
PIO_PER (PIO Enable Register) and PIO_PDR (PIO Disable Register). The register PIO_PSR (PIO Status
Register) is the result of the set and clear registers and indicates whether the pin is controlled by the
corresponding peripheral or by the PIO Controller. A value of 0 indicates that the pin is controlled by the
corresponding on-chip peripheral selected in the PIO_ABSR (AB Select Register). A value of 1 indicates the pin is
controlled by the PIO controller.

If a pin is used as a general purpose I/O line (not multiplexed with an on-chip peripheral), PIO_PER and PIO_PDR
have no effect and PIO_PSR returns 1 for the corresponding bit.

After reset, most generally, the I/O lines are controlled by the PIO controller, i.e. PIO_PSR resets at 1. However, in
some events, it is important that PIO lines are controlled by the peripheral (as in the case of memory chip select
lines that must be driven inactive after reset or for address lines that must be driven low for booting out of an
external memory). Thus, the reset value of PIO_PSR is defined at the product level, depending on the multiplexing
of the device.
*/

void pio_set_as_peripheral(uint8_t pin_number, enum pio_port_t port)
{
    if (PIO_PORT_A)
    {
        REG_PIOA_PER = (1 << pin_number);
        return;
    }
    if (PIO_PORT_B)
    {
        REG_PIOB_PER = (1 << pin_number);
        return;
    }

    if (PIO_PORT_C)
    {
        REG_PIOC_PER = (1 << pin_number);
        return;
    }
    REG_PIOD_PER = (1 << pin_number);
}

void pio_set_as_IO(uint8_t pin_number, enum pio_port_t port)
{
    if (port == PIO_PORT_A)
    {
        REG_PIOA_PDR = (1 << pin_number);
        return;
    }
    if (port == PIO_PORT_B)
    {
        REG_PIOB_PDR = (1 << pin_number);
        return;
    }
    if (port == PIO_PORT_C)
    {
        REG_PIOC_PDR = (1 << pin_number);
        return;
    }

    REG_PIOD_PDR = (1 << pin_number);
}

void pio_select_peripheral(uint8_t pin_number, enum pio_peripheral_t alphabet, enum pio_port_t port)
{
    uint32_t *reg;
    switch (port)
    {
    case PIO_PORT_A:
        reg = REG_PIOA_ABSR;
        break;
    case PIO_PORT_B:
        reg = REG_PIOB_ABSR;
        break;
    case PIO_PORT_C:
        reg = REG_PIOC_ABSR;
        break;
    case PIO_PORT_D:
        reg = REG_PIOD_ABSR;
        break;

    default:
        assert(false);
    }
    if (alphabet)
    {
        *reg |= (1 << pin_number);
    }
    else
    {
        *reg &= ~(1 << pin_number);
    }
}