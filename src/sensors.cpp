#include <sensors.h>
LineSensor ::LineSensor(sensors _sensors, Car* _car) {
  lilFord = _car;
  liSensorS = _sensors;
}

/**
 * @brief 
 *  Read all sensors pins and return the value on an int
 * @return int - Sensors status value
 */
int LineSensor::read_sensors() {
  int count = 0;
  for (int i = 0; i < 4; i++) {
    if (digitalRead(liSensorS.sLineArray[i]) == 1) {
      count += pow(2, i);
    }
  }
  return count;
}

void LineSensor::create_cases() {
  for (int i = 0; i < 5; i++) {
    const int* insertCase = insertionCases[i];
    const int arrSize = sizeof(insertCase) / sizeof(insertCase[0]);
    for (int j = 0; j < arrSize; j++) {
      directionCases[insertCase[i]] = i + 1;
    }
  }
}

/**
 * @brief 
 * Change the direction of the robot with the sensors pins values
 */
void LineSensor::change_direction() {
  int sensors_value = read_sensors();
  switch (sensors_value) {
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    default:
      break;
  }
}
