#include <Arduino.h>
#define DEBUG true
#include <sensors.h>
int trigger = PB10;
int echo = PB11;

sensors lineSensorS = {PA8, PA12, PB12, PB14};
Motor motorA(PB9, PB8);
Motor motorB(PB7, PB6);
Car robotS = {&motorA, &motorB};
LineSensor robotWithSensor(lineSensorS, &robotS);
void setup() {
  Serial.begin(9600);
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
}

void loop() {
  robotWithSensor.change_direction();
  robotWithSensor.radar();
}
