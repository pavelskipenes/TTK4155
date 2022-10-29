#include "can.h"
#include "mcp2515.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>

void can_init(){
	static bool initialized = false;
    if (initialized)
    {
        return;
    }
	
	// enable interrupt on RX0 and RX1
	mcp2515_write(CANINTE, 0x3);
	
	enum can_mode mode = CAN_MODE_LOOPBACK;
	uint8_t abat = 0u;
	uint8_t osm = 0; 		// one-shot disabled
	uint8_t clken = 1; 		// clkout pin enabled
	uint8_t clkpre = 0x04; 	// fclkout = fclk/8
	uint8_t ctrl_data = (mode << 5)|(abat << 4)|(osm << 3)|(clken << 2)|(clkpre << 1);
	mcp2515_write(CANCTRL, ctrl_data);
	
	// write to CNF	to set timing
	//
	
	

	// start normal operation mode
	/*
	mode = CAN_MODE_NORMAL;
	mcp2515_write(CANCTRL, ctrl_data);
	*/
}

void can_tx(uint16_t id, uint64_t data){
	/*
	can_frame frame;
	frame.id = id;
	frame.rtr = 1; // remote frame
	frame.ide = 0; // standard frame
	frame.data_len = data_len;
	frame.data = *data;
	frame.ack = 1; // acknowledge by receiving node
	
	// using TXB0
	mcp2515_write(TXB0SIDH, frame.id); 	// standard id bits 10:3
	mcp2515_write(TXB0SIDL, (frame.ide << 3));			// standard id bits 2:0
	mcp2515_write(TXB0EID8, 0); 			// extended id bits 15:8
	mcp2515_write(TXB0EID0, 0); 			// extended id bits 7:0
	mcp2515_write(TXB0DLC, 0|(frame.rtr << 6)|(frame.data_length << 3));
	mcp2515_write(TXB0D0, frame->data);
	*/
	
	////////////////////////////////////////
	
	bool rtr = 0; // data frame
	bool ide = 0; // standard frame
	bool ack = 1; // acknowledge by receiving node
	
	// using TXB0
	// mcp2515 page 20
	mcp2515_write(TXB0SIDH, (uint8_t)(id >> 3)); 			// standard id bits 10:3
	mcp2515_write(TXB0SIDL, ((id | 0x7) << 5)|(ide << 3));	// standard id bits 2:0
	mcp2515_write(TXB0EID8, 0); 			// extended id bits 15:8
	mcp2515_write(TXB0EID0, 0); 			// extended id bits 7:0
	mcp2515_write(TXB0DLC, (rtr << 6)|(sizeof(data) << 3));
	
	uint8_t buff[8];
	(*(uint64_t*)(buff)) = data;
	
	for(uint8_t i = 0; i < 8; i++){
		mcp2515_write(TXB0D0 + i, buff[i]);
	}
	
	
	
	bool txb2 = 0;
	bool txb1 = 0;
	bool txb0 = 1;
	mcp2515_request_to_send(txb2, txb1, txb0);
	
	
	// wait until message has been sent
	while(TXB0CTRL & (1 << TXREQ));
	
		
}

void can_rx(can_frame * frame){

	// read CANSTAT 3:0 to find which RXB the message was loaded into
	//
	
	uint8_t src = mcp2515_read(CANINTF);
	printf("\nsrc: %X\n", src);

	for(int i = 0; i < 8; i++){
		frame->data[i] = mcp2515_read(RXB0D0 + i);
		//printf("\n%c", mcp2515_read(RXB0D0 + i));
	}
	
	// clear interrupt registers
	mcp2515_write(CANINTF, 0);
	
	
}