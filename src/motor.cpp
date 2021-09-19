#include <Arduino.h>
#include <motor.h>
int speed_converter(int speed) {
  return map(speed, 0, 100, 0, 65535);
}
Motor::Motor(int sideA, int sideB) {
  a = sideA, b = sideB;
  pinMode(a, OUTPUT);
  pinMode(b, OUTPUT);
}
void Motor::go(int speed) {
  analogWrite(a, speed);
  analogWrite(b, speed);
}
void Motor::back(int speed) {
  analogWrite(a, speed);
  analogWrite(b, speed);
}
void Motor::stop() {
  digitalWrite(a, 0);
  digitalWrite(b, 0);
}