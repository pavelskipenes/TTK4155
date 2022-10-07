#pragma once

#include "gpio.h"
#include <stdint.h>

struct spi_slave
{
    GPIO ss;
};

void spi_init_slave(struct spi_slave *slave);

uint8_t spi_send_byte(struct spi_slave *slave, uint8_t data);

void spi_init();
