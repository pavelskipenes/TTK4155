#include "mcp2515.h"
#include "can.h"
#include "spi.h"
#include <assert.h>
#include <stdio.h>

GPIO mcp2515 = {
	4,
	&DDRB,
	&PORTB};

enum rx_buffer
{
	/**
	 * @brief Receive Buffer 0, Start at RXB0SIDH
	 *
	 */
	RX_BUFFER_RXB0SIDH = 0u,
	/**
	 * @brief Receive Buffer 0, Start at RXB0D0
	 */
	RX_BUFFER_RXB0D0 = 1u,
	/**
	 * @brief Receive Buffer 1, Start at RXB1SIDH
	 */
	RX_BUFFER_RXB1SIDH = 2u,
	/**
	 * @brief Receive Buffer 1, Start at RXB1D0
	 */
	RX_BUFFER_RXB1D0 = 3u,
};

enum mcp2515_instruction_t
{
	// source: table 12-1 page 66 mcp2515 datasheet

	/**
	 * @brief Resets internal registers to default state, set Configuration mode.
	 */
	INSTR_RESET = 0xC0u,

	/**
	 * @brief Read data from register beginning at selected address.
	 */
	INSTR_READ = 0x03u,
	/*
		1001 0nm0

		When reading a receive buffer, reduces the overhead of a normal
		Read command by placing the Address Pointer at one of four
		locations, as indicated by ‘n,m’. Note: The associated RX flag bit
		(CANINTF.RXnIF) will be cleared after bringing CS high.
	*/
	INSTR_READ_RX_BUFFER_MASK = 0x90u,

	/**
	 * @brief Write data to register beginning at selected address.
	 */
	INSTR_WRITE = 0x02u,

	/**
	 * @brief When loading a transmit buffer, reduces the overhead of a normal
	 * Write command by placing the Address Pointer at one of six
	 * locations as indicated by ‘a,b,c’.
	 * @see enum rx_buffer
	 */
	INSTR_LOAD_TX_BUFFER = 0x40u,

	/**
	 * @brief Instructs controller to begin message transmission sequence for
	 * any of the transmit buffers.
	 */
	INSTR_RTS_MASK = 0x80u,

	/**
	 * @brief Quick polling command that reads several status bits for transmit
	 * and receive functions.
	 */
	INSTR_READ_STATUS = 0xA0u,

	/**
	 * @brief Quick polling command that indicates filter match and message
	 * type (standard, extended and/or remote) of received message.
	 */
	INSTR_RX_STATUS = 0xB0u,

	/**
	 * @brief Allows the user to set or clear individual bits in a particular
	 * register. Note: Not all registers can be bit-modified with this
	 * command. Executing this command on registers that are not bit-
	 * modifiable will force the mask to FFh. See the register map in
	 * Section 11.0 “Register Map” for a list of the registers that apply.
	 */
	INSTR_BIT_MODIFY = 0x05u,
};

#pragma region registers

// can control buffer
#define CANCTRL 0x0F

// can configuration buffers
#define CNF1 0x2A
#define CNF2 0x29
#define CNF3 0x28

// can transmit buffer control
#define TXB0CTRL 0x30
#define TXB1CTRL 0x40
#define TXB2CTRL 0x50

// can transmit buffers
#define TXB0SIDH 0x31
#define TXB1SIDH 0x41
#define TXB2SIDH 0x51

#define TXB0SIDL 0X32
#define TXB1SIDL 0X42
#define TXB2SIDL 0X52

#define TXB0EID8 0x33
#define TXB1EID8 0x43
#define TXB2EID8 0x53

#define TXB0EID0 0x34
#define TXB1EID0 0x44
#define TXB2EID0 0x54

#define TXB0DLC 0x35
#define TXB1DLC 0x45
#define TXB2DLC 0x55

#define TXB0D0 0x36
#define TXB1D0 0x46
#define TXB2D0 0x56

// can receive buffer control
#define RXB0CTRL 0x60
#define RXB1CTRL 0x70

// can receive buffers
#define RXB0SIDH 0x61
#define RXB1SIDH 0x71

#define RXB0SIDL 0x62
#define RXB1SIDL 0x72

#define RXB0EID8 0X63
#define RXB1EID8 0X73

#define RXB0EID0 0x64
#define RXB1EID0 0x74

#define RXB0DLC 0x65
#define RXB1DLC 0x75

#define RXB0D0 0x66
#define RXB1D0 0x76

// can receive interrupt
#define CANINTE 0x2B // CANINTE.RXnIE enables interrupt on new message
#define CANINTF 0x2C // source of the interrupt

/* BFPCTRL */
#define B1BFS 5
#define B0BFS 4
#define B1BFE 3
#define B0BFE 2
#define B1BFM 1
#define B0BFM 0

/* TXRTSCTRL */
#define B2RTS 5
#define B1RTS 4
#define B0RTS 3
#define B2RTSM 2
#define B1RTSM 1
#define B0RTSM 0

/* CANSTAT */
#define CANSTAT 0x0E
#define OPMOD2 7
#define OPMOD1 6
#define OPMOD0 5
#define ICOD2 3
#define ICOD1 2
#define ICOD0 1

/* CANCTRL */
#define REQOP2 7
#define REQOP1 6
#define REQOP0 5
#define ABAT 4
#define OSM 3
#define CLKEN 2
#define CLKPRE1 1
#define CLKPRE0 0

/* CNF3 */
#define SOF 7
#define WAKFIL 6
#define PHSEG22 2
#define PHSEG21 1
#define PHSEG20 0

/* CNF2 */
#define BTLMODE 7
#define SAM 6
#define PHSEG12 5
#define PHSEG11 4
#define PHSEG10 3
#define PRSEG2 2
#define PRSEG1 1
#define PRSEG0 0

/* CNF1 */
#define SJW1 7
#define SJW0 6
#define BRP5 5
#define BRP4 4
#define BRP3 3
#define BRP2 2
#define BRP1 1
#define BRP0 0

/* CANINTE - Interrupt enable */
#define MERRE 7 // Message Error interrupt enable bit
#define WAKIE 6 // Wake-up Interrupt Enable bit
#define ERRIE 5 // Error Interrupt Enable bit (multiple sources in EFLG register)
#define TX2IE 4 // Transmit Buffer 2 Empty Interrupt Enable bit
#define TX1IE 3 // Transmit Buffer 1 Empty Interrupt Enable bit
#define TX0IE 2 // Transmit Buffer 1 Empty Interrupt Enable bit
#define RX1IE 1 // Receive Buffer 1 Full Interrupt Enable bit
#define RX0IE 0 // Receive Buffer 0 Full Interrupt Enable bit

/* CANINTF - Interrupt flag */
#define MERRF 7 // Message Error interrupt pending bit
#define WAKIF 6 // Wake-up Interrupt pending bit
#define ERRIF 5 // Error Interrupt pending bit (multiple sources in EFLG register)
#define TX2IF 4 // Transmit Buffer 2 Empty Interrupt pending bit
#define TX1IF 3 // Transmit Buffer 1 Empty Interrupt pending bit
#define TX0IF 2 // Transmit Buffer 1 Empty Interrupt pending bit
#define RX1IF 1 // Receive Buffer 1 Full Interrupt pending bit
#define RX0IF 0 // Receive Buffer 0 Full Interrupt pending bit

/* EFLG */
#define RX1OVR 7
#define RX0OVR 6
#define TXBO 5
#define TXEP 4
#define RXEP 3
#define TXWAR 2
#define RXWAR 1
#define EWARN 0

/* TXB0CTRL */
/* TXB1CTRL */
/* TXB2CTRL */
#define ABTF 6
#define MLOA 5
#define TXERR 4
#define TXREQ 3
#define TXP1 1
#define TXP0 0

/* RXB0CTRL */
/* RXB1CTRL */
#define RXM1 6
#define RXM0 5
#define RXRTR 3
#define BUKT 1
#define FILHIT2 2
#define FILHIT1 1
#define FILHIT0 0

#pragma endregion registers

#define TX_BUFFER_0_RDY 0
#define TX_BUFFER_1_RDY 1
#define TX_BUFFER_2_RDY 2
#define DUMMY_BYTE 69

static uint8_t mcp2515_read_status();
static uint8_t mcp2515_read(uint8_t address);
static void mcp2515_bit_modify(uint8_t register_address, uint8_t mask, uint8_t data);
static void mcp2515_reset();
static void mcp2515_write(uint8_t address, uint8_t data);
static void mcp2515_instruction(GPIO *mcp2515, enum mcp2515_instruction_t instruction);
static void mcp2515_wait_tx_complete();

#if 0

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

#if 0

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
#endif

	// TODO: set can timing (CNF registers)
	/*
	Fosc = 16MHz
	Fbr = 125kHz
	Tq = 2Tosc
	brp = Fosc/(2Fbr)
	*/
	// mcp2515_bit_modify(CNF1, 0x03, 0x03); // set T_Q to 500ns and syncreg to 1
	// mcp2515_bit_modify(CNF2, 0xBF, 0xB1); // set PS1 to 7 and propseg to 2
	// mcp2515_bit_modify(CNF3, 0x07, 0x05); // set PS2 to 6

	uint8_t sjw = 0;	 // sync jump width length 1 x T_q
	uint8_t btlmode = 1; // PS2 bit time length

	uint8_t brp = 4;	// baud rate prescaler, 500kHz
	uint8_t prseg = 2;	// propagation segment length
	uint8_t phseg1 = 7; // PS1 length
	uint8_t phseg2 = 6; // PS2 length

	mcp2515_write(CNF1, (sjw << 6) | (brp - 1));
	mcp2515_write(CNF2, (btlmode << 7) | ((phseg1 - 1) << 3) | (prseg - 1));
	mcp2515_write(CNF3, (phseg2 - 1));
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
	printf("[CAN] sending message\n");

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
	printf("[CAN] message sent\n");
}
