/*
 * PI_controller.c
 *
 * Created: 25.11.2022 04:38:21
 *  Author: krist
 */ 

#include "PI_controller.h"
#include "sam.h"
#include "printf-stdarg.h"

static float kp;
static float ki;
static uint16_t error;

void PI_init(float k_p, float k_i){
	kp = k_p;
	ki = k_i;
	error = 0; 
}

uint16_t PI_controller(uint16_t position){
	uint16_t encoder = motor_read_encoder();
	uint16_t diff = position - encoder;
	error += diff;
	uint16_t heading = kp * error + ki * diff;
	
	return heading;
}