/*
 * motor.cpp
 *
 *  Created on: 7 oct 2021
 *      Author: death
 */
#include "motor.h"

Motor::Motor(uint32_t _A,uint32_t _B,TIM_HandleTypeDef *_timer){
	A = _A;
	B = _B;
	timer = _timer;
}

void Motor::go(int speed){
	__HAL_TIM_SET_COMPARE(timer,A,speed);
	__HAL_TIM_SET_COMPARE(timer,B,0);
}
void Motor::back(int speed){
	__HAL_TIM_SET_COMPARE(timer,A,0);
	__HAL_TIM_SET_COMPARE(timer,B,speed);
}
void Motor::stop(){
	__HAL_TIM_SET_COMPARE(timer,B,0);
	__HAL_TIM_SET_COMPARE(timer,A,0);

}
void Motor::set_pins(MotorpinS _pins){
	pins = _pins;
}


