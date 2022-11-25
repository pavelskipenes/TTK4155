/*
 * PI_controller.h
 *
 * Created: 25.11.2022 04:38:41
 *  Author: krist
 */
#include "stdint.h"


#ifndef PI_CONTROLLER_H_
#define PI_CONTROLLER_H_

static float kp;
static float ki;
static uint16_t error;

void PI_init(float k_p, float k_i);

uint16_t PI_controller(uint16_t pos);



#endif /* PI_CONTROLLER_H_ */