#include <sensors.h>
#define toBlack 15
#define toWhite 0
extern bool isAnObstacule;

LineSensor ::LineSensor(sensors _sensors, Car* _car) {
  this->lilFord = _car;
  this->liSensorS = _sensors;
  for (int i = 0; i < 4; i++) {
    pinMode(this->liSensorS.sLineArray[i], INPUT);
  }
}

/**
 * @brief 
 *  Read all sensors pins and return the value on an int
 * @return int - Sensors status value
 */
int LineSensor::read_sensors() {
  int count = 0;
  for (int i = 0; i < 4; i++) {
    if (digitalRead(this->liSensorS.sLineArray[i]) == 1) {
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
    const int* insertCase = this->insertionCases[i];
    const int arrSize = sizeof(insertCase) / sizeof(insertCase[0]);
    for (int j = 0; j < arrSize; j++) {
      this->directionCases[insertCase[i]] = i + 1;
    }
  }
}
/**
 * @brief 
 * Calculate error from sensors case to will use in PID
 * @param sensorCase 
 * @return int 
 */
int LineSensor::get_error(int sensorCase) {
  switch (sensorCase) {
    case 1:
      return 0;
    case 2:
      return 0;
    case 3:
      return 1;
    case 4:
      return -1;
    default:
      if (this->lineChanged)
        return -1;
      return 1;
  }
}
/**
 * @brief 
 * Change the direction of the robot with the sensors pins values
 */
void LineSensor::change_direction() {
  int sensorsValue = this->read_sensors();
  int sensorCase = this->directionCases[sensorsValue];
  float pidValue = this->calculate_pid(this->get_error(sensorCase));
  Speed speedControlS = {int(float(100) - pidValue), int(float(100) - pidValue)};
  this->lilFord->set_speed(speedControlS);
  if (sensorsValue == toWhite)
    this->lineChanged = false;
  else if (sensorsValue == toBlack)
    this->lineChanged = true;
  switch (sensorCase) {
    case 1:
      this->lilFord->go();
      break;
    case 2:
      this->lilFord->stop();
      break;
    case 3:
      this->lilFord->speedControlS.speedA = int(float(50) - pidValue);
      this->lilFord->left();
      break;
    case 4:
      this->lilFord->speedControlS.speedB = int(float(50) - pidValue);
      this->lilFord->right();
      break;
    default:
      if (this->lineChanged) {
        this->lilFord->speedControlS.speedB = int(float(50) - pidValue);
        this->lilFord->right();
      } else {
        this->lilFord->speedControlS.speedA = int(float(50) - pidValue);
        this->lilFord->left();
      }
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
    this->lilFord->back();
    delay(1000);
    this->lilFord->right();
    delay(500);
    this->lilFord->go();
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
  int distanceRadar = (duration * 0.034) / 2;
  return distanceRadar;
}
/**
 * @brief 
 * Calculate PID value 
 * @param error 
 * @return float 
 */
float LineSensor::calculate_pid(int error) {
  this->PID.P = error;
  this->PID.I += error;
  this->PID.D = error - this->PID.oldError;
  this->PID.oldError = error;
  return (Kp * this->PID.P) + (Ki * this->PID.I) + (Kd * this->PID.D);
}