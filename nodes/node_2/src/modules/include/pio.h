#pragma once

#include <stdint.h>

enum pio_peripheral_t
{
    PIO_PERIPHERAL_A,
    PIO_PERIPHERAL_B
};

enum pio_port_t
{
    PIO_PORT_A,
    PIO_PORT_B,
    PIO_PORT_C,
    PIO_PORT_D,
};

struct pio_t
{
    uint8_t pin_number;
    enum pio_port_t port;
};

void pio_select_peripheral(uint8_t pin_number, enum pio_peripheral_t alphabet, enum pio_port_t port);
void pio_set_as_IO(uint8_t pin_number, enum pio_port_t port);
void pio_set_as_peripheral(uint8_t pin_number, enum pio_port_t port);
