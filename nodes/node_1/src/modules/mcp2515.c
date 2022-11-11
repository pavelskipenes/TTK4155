#include <stdio.h>
#include "can.h"
#include "mcp2515.h"
#include "spi.h"

GPIO mcp2515 = {
	4,
	&DDRB,
	&PORTB};

void mcp2515_init()
{
	static bool initialized = false;
	if (initialized)
	{
		return;
	}
	initialized = true;
	spi_init();
	spi_init_slave(&mcp2515);
	mcp2515_reset();
	
	// enable interrupt on RX0 and RX1
	mcp2515_write(CANINTE, 0x3);




	// TODO: set can timing (CNF registers)
	/*
	Fosc = 16MHz
	Fbr = 125kHz
	
	Tq = 2Tosc
	
	brp = Fosc/(2Fbr)

	*/

	uint8_t sjw = 1; // sync jump width length
	uint8_t brp = 16; // baud rate prescaler, 500kHz
	
	uint8_t btlmode = 1; // PS2 bit time length
	uint8_t prseg = 2; // propagation segment length
	uint8_t phseg1 = 7; // PS1 length
	uint8_t phseg2 = 6; // PS2 length
	
	mcp2515_write(CNF1, (sjw << 5)|(brp));
	mcp2515_write(CNF2, (btlmode << 7)|(phseg1 << 3)|(prseg));
	mcp2515_write(CNF3, (phseg2));


}

void mcp2515_reset()
{
	gpio_set(&mcp2515, GPIO_LOW);
	spi_send_byte(&mcp2515, CAN_INSTRUCTION_RESET);
	gpio_set(&mcp2515, GPIO_HIGH);
}

uint8_t mcp2515_read(uint8_t address)
{
	gpio_set(&mcp2515, GPIO_LOW);
	spi_send_byte(&mcp2515, CAN_INSTRUCTION_READ);
	spi_send_byte(&mcp2515, address);

	uint8_t data = spi_send_byte(&mcp2515, 0x69); // dummy byte
	gpio_set(&mcp2515, GPIO_HIGH);

	return data;
}

void mcp2515_write(uint8_t address, uint8_t data)
{
	gpio_set(&mcp2515, GPIO_LOW);
	spi_send_byte(&mcp2515, CAN_INSTRUCTION_WRITE);
	spi_send_byte(&mcp2515, address);
	spi_send_byte(&mcp2515, data);
	gpio_set(&mcp2515, GPIO_HIGH);
}

void mcp2515_request_to_send(bool txb2, bool txb1, bool txb0)
{
	uint8_t buffers = (txb2 << 2) | (txb1 << 1) | (txb0);

	gpio_set(&mcp2515, GPIO_LOW);
	uint8_t data = 0x80 | buffers;
	spi_send_byte(&mcp2515, data);
	gpio_set(&mcp2515, GPIO_HIGH);
}

uint8_t mcp2515_read_status()
{
	uint8_t data;
	gpio_set(&mcp2515, GPIO_LOW);
	spi_send_byte(&mcp2515, CAN_INSTRUCTION_READ_STATUS);
	spi_send_byte(&mcp2515, 0x69); 			// dummy byte
	data = spi_send_byte(&mcp2515, 0x69);	// dummy byte
	gpio_set(&mcp2515, GPIO_HIGH);
	return data;
}

void mcp2515_bit_modify(uint8_t register_address, uint8_t bit, bool state)
{
	uint8_t mask = (1 << bit);
	uint8_t data = state << bit;

	gpio_set(&mcp2515, GPIO_LOW);
	spi_send_byte(&mcp2515, CAN_INSTRUCTION_BIT_MODIFY);
	spi_send_byte(&mcp2515, register_address);
	spi_send_byte(&mcp2515, mask);
	spi_send_byte(&mcp2515, data);
	gpio_set(&mcp2515, GPIO_HIGH);
}

void mcp2515_set_mode(enum can_mode mode){
	uint8_t abat = 0u;
	uint8_t osm = 0;	   // one-shot disabled
	uint8_t clken = 1;	   // clkout pin enabled
	uint8_t clkpre = 0x04; // fclkout = fclk/8
	uint8_t ctrl_data = (mode << 5) | (abat << 4) | (osm << 3) | (clken << 2) | (clkpre << 1);
	mcp2515_write(CANCTRL, ctrl_data);
}

void mcp2515_can_tx(uint16_t id, uint64_t data)
{
	bool rtr = 0;	 // remote transmission request bit // 0 = data frame
	uint8_t ide = 0; // standard frame
	bool ack = 1;	 // acknowledge by receiving node
	// using TXB0 - mcp2515 page 20
	mcp2515_write(TXB0SIDH, (uint8_t)(id >> 3));								 // standard id bits 10:3
	mcp2515_write(TXB0SIDL, (uint8_t)((uint8_t)((id & 0x7) << 5)) | (ide << 3)); // standard id bits 2:0
	// mcp2515_write(TXB0EID8, 0);								 // extended id bits 15:8
	// mcp2515_write(TXB0EID0, 0);								 // extended id bits 7:0
	mcp2515_write(TXB0DLC, (rtr << 6) | (sizeof(data)));

	uint8_t buff[8];
	(*(uint64_t *)(buff)) = data;

	for (uint8_t i = 0; i < 8; i++)
	{
		mcp2515_write(TXB0D0 + i, buff[i]);
	}

	bool txb2 = 0;
	bool txb1 = 0;
	bool txb0 = 1;
	mcp2515_request_to_send(txb2, txb1, txb0);

	// wait until message has been sent
	while (TXB0CTRL & (1 << TXREQ));
}

void mcp2515_can_rx(can_frame *frame)
{

	// read CANSTAT 3:0 to find which RXB the message was loaded into
	//
	
	uint8_t sidl_register = mcp2515_read(RXB0SIDL);

	frame->id = mcp2515_read(RXB0SIDH) << 3;
	frame->id |= (sidl_register & 0xE0) >> 5;
	frame->ide = (sidl_register & 0x8) >> 3;

	frame->data_length = mcp2515_read(RXB0DLC) & 0x0F;

	// TODO: page 30 mcp2515 extended identifiers

	for (int i = 0; i < 8; i++)
	{
		frame->data[7-i] = mcp2515_read(RXB0D0 + i);
	}

	// clear interrupt registers
	mcp2515_write(CANINTF, 0);
}