#include "can.h"
#include "mcp2515.h"


void can_init(){
	enum can_mode mode;
	mode = CAN_MODE_LOOPBACK;
	
	uint8_t abat = 0;
	uint8_t osm = 0; 		// one-shot disabled
	uint8_t clken = 1; 		// clkout pin enabled
	uint8_t clkpre = 0x04; 	// fclkout = fclk/8
	uint8_t ctrl_data = 0|(mode << 5)|(abat << 4)|(osm << 3)|(clken << 2)|(clkpre << 1);
	
	mcp2515_write(CANCTRL, ctrl_data);
}

void can_tx(uint16_t id, char data[]){
	can_frame frame;
	
	frame.id = id;
	frame.rtr = 1; // remote frame
	frame.ide = 0; // standard frame
	frame.data_length = sizeof(data);
	
	strcpy(frame.data, data);
	//frame.data = data;
	frame.ack = 1; // acknowledge by receiving node
	
	// using TXB0
	mcp2515_write(TXB0SIDH, frame.id); 	// standard id bits 10:3
	mcp2515_write(TXB0SIDL, (frame.ide << 3));			// standard id bits 2:0
	mcp2515_write(TXB0EID8, 0); 			// extended id bits 15:8
	mcp2515_write(TXB0EID0, 0); 			// extended id bits 7:0
	mcp2515_write(TXB0DLC, 0|(frame.rtr << 6)|(frame.data_length << 3));
	//mcp2515_write(TXB0D0, frame.data);
	
	
	bool txb2 = 0;
	bool txb1 = 0;
	bool txb0 = 1;
	mcp2515_request_to_send(txb2, txb1, txb0);
		
}

can_frame can_rx(){
	can_frame frame;
	
	
	
	for(int i = 0; i < frame.data_length; i++){
		frame.data[i] = mcp2515_read(RXB0D0 + i);
	}
	
	
	
	return frame;
}