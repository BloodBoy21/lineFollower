#ifndef __SENSOR_H__
#define __SENSOR_H__
#include <Arduino.h>
#include <motor.h>

struct sensors {
  int sLine0;
  int sLine1;
  int sLine2;
  int sLine3;
  int sLineArray[4] = {sLine0, sLine1, sLine2, sLine3};
};
class LineSensor {
 private:
  sensors liSensorS;
  Car* lilFord;
  int directionCases[16] = {0};
  const int casesToGo[2] = {0, 15};
  const int casesToStop[2] = {6, 9};
  const int specialCases[2] = {3, 12};
  const int casesToLeft[4] = {4, 7, 8, 11};
  const int casesToRight[4] = {1, 2, 13, 14};
  const int* insertionCases[5] = {casesToGo, casesToStop, casesToLeft, casesToRight, specialCases};
  int read_sensors();
  void create_cases();

 public:
  LineSensor(sensors _sensors, Car* _car);
  void change_direction();
};

#endif
