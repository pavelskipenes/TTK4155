#pragma once

#include "gpio.h"
#include <stdint.h>


void spi_init_slave(GPIO *);

uint8_t spi_send_byte(GPIO *, uint8_t);

void spi_init();
