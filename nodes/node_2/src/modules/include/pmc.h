#pragma once

#include <stdbool.h>
#include <stdint.h>

void pmc_peripheral_enable(uint8_t peripheral_id);
void pmc_peripheral_disable(uint8_t peripheral_id);
bool pmc_peripheral_status(uint8_t peripheral_id);