#ifndef SENSORS_H
#define SENSORS_H
#include "stm32f1xx_hal.h"
#include "motor.h"
int get_distance();
typedef struct {
	GPIO_TypeDef* port;
	uint16_t pin;
}STMPIN;
struct speed {
	int speedA;
	int speedB;

};

struct IRpins {
	STMPIN IR1;
	STMPIN IR2;
	STMPIN IR3;
	STMPIN IR4;
	STMPIN pinArray[4]={IR1,IR2,IR3,IR4};
};

class Car {
private:
	Motor *motorA, *motorB;
public:
	Car(Motor*, Motor*);
	void go();
	void back();
	void stop();
	void left();
	void right();
	void go_back();
	speed carSpeed = { 199, 199 };
};

class CarIR: public Car {
private:
	float Kp=0.15,Ki=0.0003,Kd=0.2;
	float P=0,I=0,D=0;
	float vBase;
	int oldError;
	IRpins lineDetector;
	int read_error();
	float calculate_PID(int);
	int read_sensors();
public:
	CarIR(Motor*,Motor*,IRpins);
	void run();
};

#endif
