/*
 * sensors.cpp
 *
 *  Created on: Oct 7, 2021
 *      Author: death
 */
#include "stm32f1xx_hal.h"
#include "sensors.h"
#include <string.h>
#include <stdio.h>
#include<cmath>
#define V_MAX 1900
#define V_MIN 1100
extern TIM_HandleTypeDef htim2;
extern UART_HandleTypeDef huart1;
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

/* Car with sensors */
CarIR::CarIR(Motor *_A, Motor *_B, IRpins _pins) :
		Car(_A, _B) {
		lineDetector = _pins;
}
int CarIR::read_sensors() {
	int status = 0;
	for (int i = 0; i < 4; ++i) {
		if (HAL_GPIO_ReadPin(lineDetector.pinArray[i].port,
				lineDetector.pinArray[i].pin)) {
			status += pow(2, i + 1);
		}
	}
	return status;
}
int CarIR::read_error() {
	int error = 0;
	STMPIN sensorError[4] = {lineDetector.IR1,lineDetector.IR2,lineDetector.IR4,lineDetector.IR3};
#ifdef debug
	char buffer[20];
#endif
	for (int i = 0; i < 4; i++) {
		STMPIN ir = sensorError[i];
		int status = HAL_GPIO_ReadPin(ir.port, ir.pin);
#ifdef debug
		sprintf(buffer, "status of %d:%d", i, status);
		uart_println(&huart1, buffer);
		memset(buffer, 0, 20);
#endif
		if (!i+1%2) {
			error += i==1?status *1:status*-1;
		} else {
			error += i==0?status *2:status*-2;
		}
	}
#ifdef debug
	sprintf(buffer, "Error:%d", error);
	uart_println(&huart1, buffer);
#endif
	return error;
}

float CarIR::calculate_PID(int error) {
	float Kv = 0.6;
	P = error;
	I += P;
	D = error - oldError;
	oldError = error;
	vBase = V_MIN + (V_MAX - V_MIN) * exp(-Kv * abs(P * Kp));
	return (Kp * P) + (Ki * I) + (Kd * D);
}
void CarIR::run() {
	float pidValue = calculate_PID(read_error());
	carSpeed = { int(vBase + pidValue), int(vBase - pidValue) };
#ifdef debug
	char buffer[50];
	sprintf(buffer, "pid:%.2f\nSpeed:{%d,%d}", pidValue, carSpeed.speedA,
			carSpeed.speedB);
	uart_println(&huart1, buffer);
#endif
	if (read_pin(lineDetector.IR2)&&read_pin(lineDetector.IR3)) {
		stop();
	} else {
		go();
	}

}
void CarIR::comeback(){
	carSpeed = {MAX_SPEED,MAX_SPEED};
	stop();
	back();
	HAL_Delay(300);
	motorA->go(MAX_SPEED);
	motorB->back(MAX_SPEED/3);
	HAL_Delay(400);
	go();

}

bool read_pin(STMPIN pin){
	return HAL_GPIO_ReadPin(pin.port, pin.pin);
}

void uart_println(UART_HandleTypeDef *huart, char data[]) {
	HAL_UART_Transmit(huart, (uint8_t*) data, strlen(data), 1000);
	HAL_UART_Transmit(huart, (uint8_t*) "\n", strlen("\n"), 1000);
}

