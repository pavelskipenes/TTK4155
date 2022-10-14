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

void spi_init_slave(GPIO *slave)
{
    *(slave->ddr) |= (GPIO_OUTPUT << (slave->pin));
}

uint8_t spi_send_byte(GPIO *slave, uint8_t data)
{
    gpio_set(slave, GPIO_LOW);
    /* Start transmission */
    SPDR = data;
    /* Wait for transmission complete */
    while (!(SPSR & (1 << SPIF)))
    {
    };
    gpio_set(slave, GPIO_HIGH);

    return SPDR;
}