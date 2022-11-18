#include "pio.h"
#include "pmc.h"
#include "sam.h"

#include <assert.h>
#include <stdbool.h>

void pio_init()
{
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
    // Nice, PMC not necessary
}

void pio_set_as_peripheral(uint8_t pin_number, enum pio_port_t port)
{
    /*
        * I/O Line or Peripheral Function Selection
        When a pin is multiplexed with one or two peripheral functions, the selection is controlled with the registers
        PIO_PER (PIO Enable Register) and PIO_PDR (PIO Disable Register). The register PIO_PSR (PIO Status
        Register) is the result of the set and clear registers and indicates whether the pin is controlled by the
        corresponding peripheral or by the PIO Controller. A value of 0 indicates that the pin is controlled by the
        corresponding on-chip peripheral selected in the PIO_ABSR (AB Select Register). A value of 1 indicates the pin is
        controlled by the PIO controller.
    */

    if (PIO_PORT_A)
    {
        PIOA->PIO_PER &= ~(1 << pin_number);
        return;
    }
    if (PIO_PORT_B)
    {
        PIOB->PIO_PER &= ~(1 << pin_number);
        return;
    }

    if (PIO_PORT_C)
    {
        PIOC->PIO_PER &= ~(1 << pin_number);
        return;
    }
    PIOD->PIO_PER &= ~(1 << pin_number);
}

void pio_set_as_IO(uint8_t pin_number, enum pio_port_t port)
{
    /*
        * I/O Line or Peripheral Function Selection
        When a pin is multiplexed with one or two peripheral functions, the selection is controlled with the registers
        PIO_PER (PIO Enable Register) and PIO_PDR (PIO Disable Register). The register PIO_PSR (PIO Status
        Register) is the result of the set and clear registers and indicates whether the pin is controlled by the
        corresponding peripheral or by the PIO Controller. A value of 0 indicates that the pin is controlled by the
        corresponding on-chip peripheral selected in the PIO_ABSR (AB Select Register). A value of 1 indicates the pin is
        controlled by the PIO controller.
    */
    if (port == PIO_PORT_A)
    {
        PIOA->PIO_PER |= (1 << pin_number);
        return;
    }
    if (port == PIO_PORT_B)
    {
        PIOB->PIO_PER |= (1 << pin_number);
        return;
    }
    if (port == PIO_PORT_C)
    {
        PIOC->PIO_PER |= (1 << pin_number);
        return;
    }

    PIOD->PIO_PER |= (1 << pin_number);
}

void pio_select_peripheral(uint8_t pin_number, enum pio_peripheral_t alphabet, enum pio_port_t port)
{

    RwReg *reg;
    switch (port)
    {
    case PIO_PORT_A:
        reg = &REG_PIOA_ABSR;
        break;
    case PIO_PORT_B:
        reg = &REG_PIOB_ABSR;
        break;
    case PIO_PORT_C:
        reg = &REG_PIOC_ABSR;
        break;
    case PIO_PORT_D:
        reg = &REG_PIOD_ABSR;
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

/*
    * Interrupt Generation p. 620
    The PIO COntroller is connected on one of the sources of the Nested Vectored Interrupt Controller (NVIC). Using
    the PIO Controller requires the NVIC to be programmed first.
    The PIO Controller interrupt can be generated only if the PIO Controller clock is enabled.
*/
