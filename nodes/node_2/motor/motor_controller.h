/*
 * motor_controller.h
 *
 * Created: 25.11.2022 00:22:19
 *  Author: krist
 */ 
#include "stdint.h"

#ifndef MOTOR_CONTROLLER_H_
#define MOTOR_CONTROLLER_H_

#define NOT_OE			PIO_PD0
#define NOT_RST			PIO_PD1
#define SEL				PIO_PD2
#define MOTOR_ENABLE	PIO_PD9
#define DIR				PIO_PD10

void motor_init();

void motor_set_speed(uint16_t speed);

void motor_set_direction(uint8_t direction);

void motor_stop();

void motor_start();

void motor_set_position(uint16_t position);

volatile void wait(int time);

uint16_t motor_read_encoder();



#endif /* MOTOR_CONTROLLER_H_ */