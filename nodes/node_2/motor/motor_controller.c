/*
 * motor_controller.c
 *
 * Created: 25.11.2022 00:20:11
 *  Author: krist
 */ 


#include "motor_controller.h"
#include "sam.h"
#include "printf-stdarg.h"

/*uint8_t NOT_OE = PIO_PD0;//		= PD0;
uint8_t NOT_RST = PIO_PD1;//		= PD1;
uint8_t SEL = PIO_PD2;//			= PD2;
uint8_t MOTOR_ENABLE = PIO_PD9;// = PD9;
uint8_t DIR = PIO_PD10;//			= PD10;
*/
// uint8_t DO0 - DO7 = PIN33 - PIN40; = PC1 - PC8;


// initializing DAC(speed) and Direction for the motor
void motor_init(){
	
	// DACC
	PMC->PMC_PCR |= PMC_PCR_EN | PMC_PCR_DIV_PERIPH_DIV_MCK | (ID_DACC << PMC_PCR_PID_Pos);
	PMC->PMC_PCER1 |= 1 << 6;
	
	DACC->DACC_MR |= DACC_MR_REFRESH(1) | DACC_MR_USER_SEL_CHANNEL1 | DACC_MR_WORD_HALF | DACC_MR_TRGEN_DIS | DACC_MR_STARTUP_0 ;
	
	DACC->DACC_CHER |= DACC_CHER_CH1;
	DACC->DACC_CDR = 0;
	
	// MJ1 -> output to motorbox
	PIOD->PIO_PER |= NOT_RST | NOT_OE | SEL | MOTOR_ENABLE | DIR;
	PIOD->PIO_OER |= NOT_RST | NOT_OE | SEL | MOTOR_ENABLE | DIR;
	
	// PIOD->PIO_SODR |= MOTOR_ENABLE;
	
	// MJ2 INPUT FROM ENCODER
	PIOC->PIO_PER |= PIO_PC1 | PIO_PC2 | PIO_PC3 | PIO_PC4 | PIO_PC5 | PIO_PC6 | PIO_PC7 | PIO_PC8;
	PIOC->PIO_ODR |= PIO_PC1 | PIO_PC2 | PIO_PC3 | PIO_PC4 | PIO_PC5 | PIO_PC6 | PIO_PC7 | PIO_PC8;

	PMC->PMC_PCR = PMC_PCR_EN | PMC_PCR_DIV_PERIPH_DIV_MCK | (ID_PIOC << PMC_PCR_PID_Pos);
	PMC->PMC_PCER0 |= 1 << (ID_PIOC);
	
	PI_init(20.0, 40.0);
	
	
}

void motor_set_speed(uint16_t speed){
	DACC->DACC_CDR = speed;
}

void motor_set_position(uint16_t position){
	uint16_t heading = PI_controller(position);
	uint8_t direction = 1;
	if(heading < 0){
		heading = -heading;
		direction = 0;
	}
	motor_set_direction(direction);
	motor_set_speed(heading);
}

// direction --> 1 or 0
void motor_set_direction(uint8_t direction){
	if(direction == 1){
		PIOD->PIO_CODR = DIR;
	}
	else if (direction == 0){
		PIOD->PIO_SODR = DIR;
	}

}

void motor_stop(){
	PIOD->PIO_CODR = MOTOR_ENABLE;
}

void motor_start(){
	PIOD->PIO_SODR = MOTOR_ENABLE;
}


// hacky attempt at sleep(ms) 
volatile void wait(int time){
	volatile int i = 0;
	while(i < 84000*time){
		i++;
	}
}

uint16_t motor_read_encoder(){
	
	PIOD->PIO_CODR |= NOT_OE;
	PIOD->PIO_CODR |= SEL;
	// Wait 20 ms????
	wait(20);
	// reset?
	// PIOD->PIO_CODR |= NOT_RST;
	// PIOD->PIO_SODR |= NOT_RST;

	uint8_t HIGH_BYTE = PIOC->PIO_PDSR & PIO_PC1 & PIO_PC2 & PIO_PC3 & PIO_PC4 & PIO_PC5 & PIO_PC6 & PIO_PC7 & PIO_PC8;

	PIOD->PIO_SODR |= SEL;
	// wait 20 ms again? 
	wait(20);
	uint8_t LOW_BYTE = PIOC->PIO_PDSR & PIO_PC1 & PIO_PC2 & PIO_PC3 & PIO_PC4 & PIO_PC5 & PIO_PC6 & PIO_PC7 & PIO_PC8;

	
	PIOD->PIO_SODR |= NOT_OE;

	
	uint16_t BYTES = (HIGH_BYTE << 8) | LOW_BYTE;
	if (BYTES & (1 << 15)) {
		return ((uint16_t) (~BYTES) + 1);
	}
	return -BYTES;
}