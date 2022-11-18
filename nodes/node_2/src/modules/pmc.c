#include "pmc.h"
#include "sam.h"

void pmc_peripheral_enable(uint8_t peripheral_id)
{
    if (peripheral_id > 31)
    {
        PMC->PMC_PCER1 |= (1 << (peripheral_id - 31));
        return;
    }
    PMC->PMC_PCER0 |= (1 << peripheral_id);
}

void pmc_peripheral_disable(uint8_t peripheral_id)
{
    if (peripheral_id > 31)
    {
        PMC->PMC_PCER1 |= (1 << (peripheral_id - 31));
        return;
    }
    PMC->PMC_PCER0 |= (1 << peripheral_id);
}

bool pmc_peripheral_status(uint8_t peripheral_id)
{
    if (peripheral_id > 31)
    {
        return PMC->PMC_PCSR1 & (1 << (peripheral_id - 31));
    }
    return PMC->PMC_PCSR0 & (1 << peripheral_id);
}