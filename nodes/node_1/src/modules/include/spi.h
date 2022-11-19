#pragma once

#include "gpio.h"
#include <stdint.h>

/**
 * @brief initialize slave
 *
 * @param slave GPIO slave select pin
 */
void spi_init_slave(GPIO *slave);

/**
 * @brief exchange byte with slave device
 *
 * @param slave unused
 * @param data byte to slave
 * @return byte from slave
 */
uint8_t spi_send_byte(GPIO *_slave, uint8_t data);

/**
 * @brief initialize spi
 *
 */
void spi_init();
