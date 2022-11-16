#pragma once

#include "gpio.h"
#include <stdint.h>

void spi_init_slave(GPIO *slave);

uint8_t spi_send_byte(GPIO *_slave, uint8_t data);

void spi_init();

// TODO: void slave_select(GPIO* slave, bool enable);