#ifndef __MOTOR__H
#define __MOTOR__H
#include <Arduino.h>
int speed_converter(int speed);
class Motor {
 private:
  int a, b;

 public:
  Motor(int, int);
  void go(int);
  void back(int);
  void stop();
};
struct Speed {
  int speedA;
  int speedB;
};

struct Car {
  Motor *wheelA;
  Motor *wheelB;
  Speed speedControlS;
  void set_speed(Speed _speedControl) {
    speedControlS = _speedControl;
  }
  void stop() {
    wheelA->stop();
    wheelB->stop();
  }
  void go() {
    wheelA->go(speed_converter(speedControlS.speedA));
    wheelB->go(speed_converter(speedControlS.speedB));
  }
  void back() {
    wheelA->back(speed_converter(speedControlS.speedA));
    wheelB->back(speed_converter(speedControlS.speedB));
  }
  void right() {
    wheelA->go(speed_converter(speedControlS.speedA));
    wheelB->back(speed_converter(speedControlS.speedB));
  }
  void left() {
    wheelA->back(speed_converter(speedControlS.speedA));
    wheelB->go(speed_converter(speedControlS.speedB));
  }
};

#endif