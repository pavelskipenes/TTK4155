#include "mcp2515_private.h"
#include "spi.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#if 0

#if 0
#include <avr/interrupt.h>
ISR(INT1_vect)
{
	// TODO: check source of the interrupt

	handle_interrupt();

	can_frame frame;
	can_rx(&frame);

	printf("id: 0x%X\t", frame.id);
	printf("data: 0x");
	for (int i = 0; i < 8; i++)
	{
		printf("%c", frame.data.character[i]);
	}
	printf("\n");
}

static void (*can_rx)(can_frame *);
#endif

#define INTERRUPT_MASK 0b1110
#define INTERRUPT_ERROR 0b0010	 // Error Interrupt
#define INTERRUPT_WAKE_UP 0b0100 // Wake-up Interrupt
#define INTERRUPT_TXB0 0b0110	 // TXB0 Interrupt
#define INTERRUPT_TXB1 0b1000	 // TXB1 Interrupt
#define INTERRUPT_TXB2 0b1010	 // TXB2 Interrupt
#define INTERRUPT_RXB0 0b1100	 // RXB0 Interrupt
#define INTERRUPT_RXB1 0b1110	 // RXB1 Interrupt

static union can_data mcp2515_read_rx(enum rx_buffer buffer);

// receive entire can message and clear interrupt flag for that register
static union can_data mcp2515_read_rx(enum rx_buffer buffer)
{
	assert(false && "not supported");
	mcp2515_instruction(&mcp2515, INSTR_READ_RX_BUFFER_MASK | (uint8_t)buffer);
	union can_data data = {0};
	for (size_t i = 0; i < 8; i++)
	{
		data.blob <<= 8;
		uint8_t byte = spi_send_byte(&mcp2515, DUMMY_BYTE);
		data.blob |= byte;
	}

	return data;
}

void mcp2515_can_rx(can_frame *frame)
{
	assert(false && "not supported");
	// read CANSTAT 3:0 to find which RXB the message was loaded into

	uint8_t sidl_register = mcp2515_read(RXB0SIDL);

	frame->id = mcp2515_read(RXB0SIDH) << 3;
	frame->id |= (sidl_register & 0xE0) >> 5;
	frame->ide = (sidl_register & 0x8) >> 3;

	frame->data_length = mcp2515_read(RXB0DLC) & 0x0F;

	// TODO: page 30 mcp2515 extended identifiers

	frame->data = mcp2515_read_rx(RX_BUFFER_RXB0SIDH);
}


enum mcp2515_interrupt_source mcp2515_interrupt_source()
{

	// get interrupt flags
	uint8_t caninte = mcp2515_read(CANINTE);
	uint8_t canintf = mcp2515_read(CANINTF);
	uint8_t canstat = mcp2515_read(CANSTAT);
	printf("[DEBUG] [CANINTF]: %#2x\n", canintf);
	printf("[DEBUG] [CANINTE]: %#2x\n", caninte);
	printf("[DEBUG] [CANSTAT]: %#2x\n", canstat);

	assert((caninte == canintf | caninte) && "mismatch between received and enabled interrupts");

	if (canintf & (1 << MERRF))
	{
		printf("[CAN]: message error interrupt\n");
		mcp2515_bit_modify(CANINTF, (1 << MERRF), (0 << MERRF));
		return;
	}
	if (canintf & (1 << WAKIF))
	{
		printf("[CAN]: wake up interrupt\n");
		mcp2515_bit_modify(CANINTF, (1 << WAKIF), (0 << WAKIF));
		return;
	}
	if (canintf & (1 << ERRIF))
	{
		printf("[CAN]: error interrupt\n");
		// TODO: (multiple sources in EFLG register)
		mcp2515_bit_modify(CANINTF, (1 << ERRIF), (0 << ERRIF));
		return;
	}
	if (canintf & (1 << TX2IF))
	{
		printf("[CAN]: transmit buffer 2 empty\n");
		mcp2515_bit_modify(CANINTF, (1 << TX2IF), (0 << TX2IF));
		return;
	}
	if (canintf & (1 << TX1IF))
	{
		printf("[CAN]: transmit buffer 1 empty\n");
		mcp2515_bit_modify(CANINTF, (1 << TX1IF), (0 << TX1IF));
		return;
	}
	if (canintf & (1 << TX0IF))
	{
		printf("[CAN]: transmit buffer 0 empty\n");
		mcp2515_bit_modify(CANINTF, (1 << TX0IF), (0 << TX0IF));
		return;
	}
	if (canintf & (1 << RX1IF))
	{
		printf("[CAN]: receive buffer 1 full\n");
		mcp2515_bit_modify(CANINTF, (1 << RX1IF), (0 << RX1IF));
		return;
	}
	if (canintf & (1 << RX0IF))
	{
		printf("[CAN]: receive buffer 0 full\n");
		mcp2515_bit_modify(CANINTF, (1 << RX0IF), (0 << RX0IF));
		return;
	}
	assert(false && "Got an interrupt without source");
}

#endif

static void mcp2515_reset()
{
	mcp2515_instruction(&mcp2515, INSTR_RESET);
}

static uint8_t mcp2515_read(uint8_t address)
{
	mcp2515_instruction(&mcp2515, INSTR_READ);
	spi_send_byte(&mcp2515, address);
	return spi_send_byte(&mcp2515, DUMMY_BYTE);
}

static void mcp2515_write(uint8_t address, uint8_t data)
{
	mcp2515_instruction(&mcp2515, INSTR_WRITE);
	spi_send_byte(&mcp2515, address);
	spi_send_byte(&mcp2515, data);
}

static uint8_t mcp2515_read_status()
{
	mcp2515_instruction(&mcp2515, INSTR_READ_STATUS);
	spi_send_byte(&mcp2515, DUMMY_BYTE);
	return spi_send_byte(&mcp2515, DUMMY_BYTE);
}

static void mcp2515_bit_modify(uint8_t register_address, uint8_t mask, uint8_t data)
{
	mcp2515_instruction(&mcp2515, INSTR_BIT_MODIFY);
	spi_send_byte(&mcp2515, register_address);
	spi_send_byte(&mcp2515, mask);
	spi_send_byte(&mcp2515, data);
}

/**
 * @brief run instruction on mcp2515
 *
 * @param mcp2515 pointer to gpio slave
 * @param enum mcp2515_instruction_t to run
 */
static void mcp2515_instruction(GPIO *p_mcp2515, enum mcp2515_instruction_t instruction)
{
	// instructions are only valid if they follow a falling CS#
	// keep the signal low. This works as long as there are only one SPI device connected.
	gpio_set(p_mcp2515, GPIO_HIGH);
	gpio_set(p_mcp2515, GPIO_LOW);
	spi_send_byte(p_mcp2515, instruction);
}

static void mcp2515_wait_tx_complete()
{
	while (true)
	{
		mcp2515_instruction(&mcp2515, INSTR_READ_STATUS); // get status
		uint8_t status_register = spi_send_byte(&mcp2515, DUMMY_BYTE);
		if (status_register & (1 << 2)) // TXB0CTRL.TXREQ
		{
			break;
		}
	}
}

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

	/*
		Frequency crystal oscillator (F_osc) = 16MHz
		Baud rate of the bus (F_br) = 125kHz
		time quantum (T_q) = 2*T_osc
		Baudrate prescaler = F_osc/(2F_br)
	*/

	const uint8_t sjw = 0;	   // sync jump width length 1 x T_q
	const uint8_t btlmode = 1; // PS2 bit time length
	const uint8_t brp = 4;	   // baud rate prescaler, 500kHz
	const uint8_t prseg = 2;   // propagation segment length
	const uint8_t phseg1 = 7;  // PS1 length
	const uint8_t phseg2 = 6;  // PS2 length

	mcp2515_write(CNF3, ((phseg2 - 1) << PHSEG20));
	mcp2515_write(CNF2, (btlmode << BTLMODE) | ((phseg1 - 1) << PHSEG10) | ((prseg - 1) << PRSEG0));
	mcp2515_write(CNF1, (sjw << SJW0) | ((brp - 1) << BRP0));
}

void mcp2515_set_mode(enum can_mode mode)
{
	uint8_t abat = 0;
	uint8_t osm = 0;	   // one-shot disabled
	uint8_t clken = 1;	   // clkout pin enabled
	uint8_t clkpre = 0x04; // fclkout = fclk/8

	uint8_t ctrl_data = (uint8_t)(mode << REQOP0) | (abat << ABAT) | (osm << OSM) | (clken << CLKEN) | (clkpre << CLKPRE0);
	mcp2515_write(CANCTRL, ctrl_data);
}

void mcp2515_can_tx(uint16_t id, union can_data data)
{
	mcp2515_instruction(&mcp2515, INSTR_LOAD_TX_BUFFER);

	// frame metadata
	spi_send_byte(&mcp2515, (uint8_t)(id >> 3));					// TXB0SIDH
	spi_send_byte(&mcp2515, (uint8_t)((uint8_t)(id & 0b111) << 5)); // TXB0SIDL
	spi_send_byte(&mcp2515, 0);										// TXB0EID8
	spi_send_byte(&mcp2515, 0);										// TXB0EID0
	spi_send_byte(&mcp2515, sizeof(data));							// TXB0DLC
	// frame data
	for (size_t i = 0; i < 8; ++i)
	{
		spi_send_byte(&mcp2515, data.bytes[i]); // TXB0Di
	}

	// set buffer 0 ready for transmission
	mcp2515_instruction(&mcp2515, INSTR_RTS_MASK | (1 << TX_BUFFER_0_RDY));
	mcp2515_wait_tx_complete();
}

/**
 * By enabling interrupts on incoming messages (RX0 full) and sending one can message
 * you'll receive in total three interrupts:
 * - TX0 empty (so a message sent)
 * - RX0 full (message received)
 * - no source (!)
 *
 * There is a mismatch between interrupt enable flags and interrupt flags that are actually being set.
 * I've tried with different code base as well. For now CAN messages has to be polled or ignored. If you
 * want to have a go at fixing this update the number below when you give up.
 *
 * Hours spent on fixing this shit so far: 16
 */

// disable all interrupts.
// mcp2515_write(CANINTE, (1 << RX0IE));
// enable interrupt on RX0 and RX1
// mcp2515_bit_modify(CANINTE, (1 << MERRE) | (1 << ERRIE) | (1 << RX0IE), (1 << ERRIE) | (1 << MERRE) | (1 << RX0IE));
// RX0 -> 3 interrupts.
// TX0 -> 2 interrupts.
// TX1 -> 0 interrupts.
// TX2 -> 0 interrupts.
// RX1 -> 0 interrupts.
