#ifndef __SENSOR_H__
#define __SENSOR_H__
#include <Arduino.h>
#include <motor.h>
extern int trigger, echo;
extern bool isAnObstacle;
int get_distance();
struct pidS {
  float P = 0;
  float I = 0;
  float D = 0;
  float oldError = 0;
};

struct sensors {
  int sLine0;
  int sLine1;
  int sLine2;
  int sLine3;
  int sLineArray[4] = {sLine0, sLine1, sLine2, sLine3};
  int get_error() {
    int error = 0;
    for (int i = 0; i < 4; i++) {
      if (i < 2) {
        error += digitalRead(sLineArray[i]) * -(i + 1);
      } else {
        error += digitalRead(sLineArray[i]) * (i - 1);
      }
    }
    return error;
  }
};
class LineSensor {
 private:
  float Kp = 0.15, Ki = 0.0003, Kd = 0.2;
  int vBase = 0;
  sensors liSensorS;
  Car* lilFord;
  int directionCases[16] = {0};
  const int casesToGo[2] = {0, 15};
  const int casesToStop[2] = {6, 9};
  const int specialCases[2] = {3, 12};
  const int casesToLeft[4] = {4, 7, 8, 11};
  const int casesToRight[4] = {1, 2, 13, 14};
  const int* insertionCases[5] = {casesToGo, casesToStop, casesToLeft, casesToRight, specialCases};
  pidS PID = {};
  int read_sensors();
  void create_cases();
  float calculate_pid(int);

 public:
  LineSensor(sensors, Car*);
  void run();
  void radar();
};

#endif
