#ifndef __MOTOR__H
#define __MOTOR__H
#include <Arduino.h>
int speed_converter(int);
class Motor {
 private:
  int a, b;

 public:
  Motor(int, int);
  void go(int);
  void back(int);
  void stop();
};
struct Car {
  Motor *wheelA;
  Motor *wheelB;
  void stop() {
    wheelA->stop();
    wheelB->stop();
  }
  void go(int speed = 100) {
    speed = speed_converter(speed);
    wheelA->go(speed);
    wheelB->go(speed);
  }
  void back(int speed = 100) {
    speed = speed_converter(speed);
    wheelA->back(speed);
    wheelB->back(speed);
  }
  void right(int speed = 100) {
    speed = speed_converter(speed);
    wheelB->back(speed);
    wheelA->go(speed);
  }
  void left(int speed = 100) {
    speed = speed_converter(speed);
    wheelA->back(speed);
    wheelB->go(speed);
  }
};

int speed_converter(int speed) {
  return map(speed, 0, 100, 0, 65535);
}

#endif