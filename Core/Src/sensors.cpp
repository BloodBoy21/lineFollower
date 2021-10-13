/*
 * sensors.cpp
 *
 *  Created on: Oct 7, 2021
 *      Author: death
 */
#include "stm32f1xx_hal.h"
#include "sensors.h"
#include <string.h>
#include<cmath>

#define V_MAX 180
#define V_MIN 80
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
void Car::go_back() {
	motorA->go(carSpeed.speedA);
	motorB->back(carSpeed.speedB);
}

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
			status += pow(2, i + 1);
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

void uart_printl(UART_HandleTypeDef *huart, char data[]) {
	HAL_UART_Transmit(huart, (uint8_t*) data, strlen(data), 1000);
	HAL_UART_Transmit(huart, (uint8_t*) "\n", strlen("\n"), 1000);
}

