#ifndef SENSORS_H
#define SENSORS_H
#include "stm32f1xx_hal.h"
#include "motor.h"
#define MAX_SPEED 2000
int get_distance();
void uart_println(UART_HandleTypeDef *huart, char data[]);
typedef struct {
	GPIO_TypeDef *port;
	uint16_t pin;
} STMPIN;
bool read_pin(STMPIN pin);
struct speed {
	int speedA;
	int speedB;

};

struct IRpins {
	STMPIN IR1;
	STMPIN IR2;
	STMPIN IR3;
	STMPIN IR4;
	STMPIN pinArray[4] = { IR1, IR2, IR3, IR4 };
};

class Car {
public:
	Motor *motorA, *motorB;
	Car(Motor*, Motor*);
	void go();
	void back();
	void stop();
	void left();
	void right();
	speed carSpeed = { MAX_SPEED, MAX_SPEED };
};

class CarIR: public Car {
private:
	float P = 0, I = 1, D = 0;
	float vBase=0;
	int oldError = 0;
	IRpins lineDetector;
	int read_error();
	float calculate_PID(int);
	int read_sensors();
public:
	float Kp = 200, Ki = 0, Kd =(Kp-1)*10;
	CarIR(Motor*, Motor*, IRpins);
	void run();
	void  comeback();
};

#endif
