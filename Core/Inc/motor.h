/*
 * car.h
 *
 *  Created on: Oct 7, 2021
 *      Author: death
 */


#ifndef MOTOR_H
#define MOTOR_H
#include "stm32f1xx_hal.h"

class Motor{
private:
	uint32_t A,B;
	TIM_HandleTypeDef *timer;
public:
	Motor(uint32_t,uint32_t,TIM_HandleTypeDef* );
	void go(int);
	void back(int);
	void stop();
};


#endif

