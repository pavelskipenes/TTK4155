#include "mcp2515.h"
#include "spi.h"

GPIO mcp2515 = {
	4, &DDRB, &PORTB
};


void mcp2515_init(){
	spi_init();
	spi_init_slave(&mcp2515);
	mcp2515_reset();
}

void mcp2515_reset(){
	spi_send_byte(&mcp2515, 0xC0);
}

uint8_t mcp2515_read(uint8_t address){
	spi_send_byte(&mcp2515, 0x03);
	spi_send_byte(&mcp2515, address);
	uint8_t data = spi_send_byte(&mcp2515, 0xFF); // dummy byte
	
	return data;
}

void mcp2515_write(uint8_t address, uint8_t data){
	spi_send_byte(&mcp2515, 0x02);
	spi_send_byte(&mcp2515, address);
	spi_send_byte(&mcp2515, data);
}

void mcp2515_request_to_send(bool txb2, bool txb1, bool txb0){
	uint8_t buffers = (txb2 << 2)|(txb1 << 1)|(txb0);
	
	uint8_t data = 0x80 | buffers;
	spi_send_byte(&mcp2515, data);
}

uint8_t mcp2515_read_status(){
	uint8_t data;
	spi_send_byte(&mcp2515, 0xA0);
	data = spi_send_byte(&mcp2515, 0xFF); // dummy byte
	data = spi_send_byte(&mcp2515, 0xFF); // dummy byte
	return data;
}

void mcp2515_bit_modify(uint8_t register_address, uint8_t bit, bool state){
	uint8_t mask = (1 << bit);
	uint8_t data = state << bit;
	
	spi_send_byte(&mcp2515, 0x05);
	spi_send_byte(&mcp2515, register_address);
	spi_send_byte(&mcp2515, mask);
	spi_send_byte(&mcp2515, data);
	
}