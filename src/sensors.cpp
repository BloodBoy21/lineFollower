#include <sensors.h>
#define V_MAX 100
#define V_MIN 30
extern bool isAnObstacule;
LineSensor ::LineSensor(sensors _sensors, Car* _car) {
  lilFord = _car;
  liSensorS = _sensors;
  for (int i = 0; i < 4; i++) {
    pinMode(liSensorS.sLineArray[i], INPUT);
  }
  create_cases();
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
/**
 * @brief 
 * Create sensor cases like a dictionary
 */
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
void LineSensor::run() {
  int sensorsValue = read_sensors();
  // int sensorCase = directionCases[sensorsValue];
  float pidValue = calculate_pid(liSensorS.get_error());
  Speed speedControlS = {int(vBase + pidValue), int(vBase - pidValue)};
  lilFord->set_speed(speedControlS);
  switch (sensorsValue) {
    case 6:
      lilFord->stop();
      break;
    default:
      lilFord->go();
      break;
  }
}
/**
 * @brief 
 * Read the distance to check if an object is front
 * and go back
 */
void LineSensor::radar() {
  int distance = get_distance();
#ifdef DEBUG
  Serial.Printl("The Distance is:" + String(distance));
#endif
  if (distance <= 20) {
    lilFord->set_speed({100, 100});
    lilFord->back();
    delay(1000);
    lilFord->right();
    delay(500);
    lilFord->go();
  }
}
/**
 * @brief 
 * Get the distance object
 * @return int 
 */
int get_distance() {
  digitalWrite(trigger, LOW);
  delayMicroseconds(5);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  int duration = pulseIn(echo, HIGH);
  return (duration * 0.034) / 2;
}
/**
 * @brief 
 * Calculate PID value 
 * @param error 
 * @return float 
 */
float LineSensor::calculate_pid(int error) {
  float Kv = 0.7;
  PID.P = error;
  PID.I = (PID.I + PID.P) * ((PID.I * PID.P) > 0);
  PID.D = PID.P - PID.oldError;
  PID.oldError = error;
  vBase = V_MIN + (V_MAX - V_MIN) * exp(-Kv * abs(PID.P * Kp));
  return (Kp * PID.P) + (Ki * PID.I) + (Kd * PID.D);
}