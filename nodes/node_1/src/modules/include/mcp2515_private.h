#ifndef __MCP2515_PRIVATE__
#define __MCP2515_PRIVATE__

#else
#error "'mcp2515_private.h' included multiple times. This is a private header."
#endif

#include "mcp2515.h"

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