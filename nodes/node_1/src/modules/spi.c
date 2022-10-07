#include "spi.h"

#include <avr/io.h>

#include "gpio.h"

void spi_init()
{
    /* Set MOSI and SCK output, all others input */
    DDRB |= (GPIO_OUTPUT << PORT5) | (GPIO_OUTPUT << PORT7);

    /* Enable SPI, Master, set clock rate fck/16 */
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

void spi_init_slave(struct spi_slave *slave)
{
    (*(slave->ss.ddr)) |= (GPIO_OUTPUT << slave->ss.pin);
}

uint8_t spi_send_byte(struct spi_slave *slave, uint8_t data)
{
    gpio_set(&(slave->ss), GPIO_LOW);
    /* Start transmission */
    SPDR = data;
    /* Wait for transmission complete */
    while (!(SPSR & (1 << SPIF)))
    {
    };
    gpio_set(&(slave->ss), GPIO_HIGH);

    return SPDR;
}