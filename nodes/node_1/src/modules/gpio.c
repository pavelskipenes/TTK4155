#include "gpio.h"

// this will not work because _MMIO_BYTE macro inside avr/sfr_defs.h dereferences all addresses making it impossible to pass it around as an address

// void gpio_set_direction(volatile uint8_t *direction_register_addr, uint8_t data_direction_bit_field, bool state)
// {
//     *direction_register_addr = (state << data_direction_bit_field);
// }
// 
// void gpio_set_output(volatile uint8_t* port_addr, uint8_t pin_bit_field, bool direction)
// {
//     *port_addr = (direction << pin_bit_field);
// }