/*
 * sensors.cpp
 *
 *  Created on: Oct 7, 2021
 *      Author: death
 */
#include "stm32f1xx_hal.h"
#include <cstdio>
#include<math.h>
#include "sensors.h"
#define V_MAX 180
#define V_MIN 80
int distance = 0;
extern STMPIN echo;
extern STMPIN trig;
extern TIM_HandleTypeDef htim2;
Car::Car(Motor *_A, Motor *_B) {
	motorA = _A;
	motorB = _B;
}
void Car::go() {
	motorA->go(carSpeed.speedA);
	motorB->go(carSpeed.speedB);
}
void Car::back() {
	motorA->back(carSpeed.speedA);
	motorB->back(carSpeed.speedB);
}
void Car::stop() {
	motorA->stop();
	motorB->stop();
}
void Car::left() {
	motorA->go(carSpeed.speedA);
	motorB->back(carSpeed.speedB);
}
void Car::right() {
	motorA->back(carSpeed.speedA);
	motorB->go(carSpeed.speedB);
}
void Car::go_back(){}

/* Car with sensors */
CarIR::CarIR(Motor *_A, Motor *_B, IRpins _pins) :
		Car(_A, _B) {
	lineDetector = _pins;
}
int CarIR::read_sensors() {
	int status = 0;
	for (int i = 3; i >= 0; i++) {
		if (HAL_GPIO_ReadPin(lineDetector.pinArray[i].port,
				lineDetector.pinArray[i].pin)) {
			status += pow(2, i);
		}
	}
	return status;
}
int CarIR::read_error() {
	int error = 0;
	for (int i = 0; i < 4; i++) {
		STMPIN ir = lineDetector.pinArray[i];
		if (i < 2) {
			error += HAL_GPIO_ReadPin(ir.port, ir.pin) * i + 1;
		} else {
			error += -(HAL_GPIO_ReadPin(ir.port, ir.pin) * i - 1);
		}
	}
	return error;
}

float CarIR::calculate_PID(int error) {
	float Kv = 0.7;
	P = error;
	I = (I + P) * ((I * P) > 0);
	D = error - oldError;
	oldError = error;
	vBase = V_MIN + (V_MAX - V_MIN) * exp(-Kv * abs(P * Kp));
	return (Kp * P) + (Ki * I) + (Kd * D);
}
void CarIR::run() {
	int sensorsRead = read_sensors();
	float pidValue = calculate_PID(read_error());
	carSpeed = { int(vBase + pidValue), int(vBase - pidValue) };
	if (sensorsRead != 6)
		go();
	else
		stop();

}
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
	uint32_t icVal1 = 0;
	uint32_t icVal2 = 0;
	uint32_t difference = 0;
	bool wasCaptured = 0;
	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4) // if the interrupt source is channel1
			{
		if (wasCaptured == 0) // if the first value is not captured
				{
			icVal1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4); // read the first value
			wasCaptured = 1;  // set the first captured as true
			// Now change the polarity to falling edge
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_4,
					TIM_INPUTCHANNELPOLARITY_FALLING);
		}

		else if (wasCaptured == 1)   // if the first is already captured
				{
			icVal2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4); // read second value
			__HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter

			if (icVal2 > icVal1) {
				difference = icVal2 - icVal1;
			}

			else if (icVal1 > icVal2) {
				difference = (0xffff - icVal1) + icVal2;
			}

			distance = difference * .034 / 2;
			wasCaptured = 0; // set it back to false

			// set polarity to rising edge
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_4,
					TIM_INPUTCHANNELPOLARITY_RISING);
			__HAL_TIM_DISABLE_IT(&htim2, TIM_IT_CC1);
		}
	}
}
int get_distance() {
	HAL_GPIO_WritePin(trig.port, trig.pin, GPIO_PIN_SET); // pull the TRIG pin HIGH
	HAL_Delay(0.01);  // wait for 10 us
	HAL_GPIO_WritePin(trig.port, trig.pin, GPIO_PIN_RESET); // pull the TRIG pin low
	__HAL_TIM_ENABLE_IT(&htim2, TIM_IT_CC1);
	return distance;
}

