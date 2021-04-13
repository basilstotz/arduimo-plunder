#include "Motion.h"


Motion::Motion() {}


void Motion::begin(int _pin, float _umfang) {

  pin = _pin;
  umfang = (int32_t)(FACTOR * _umfang);

  pinMode(pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pin), handleInterrupt, FALLING);

  //BMI160.begin(BMI160GenClass::I2C_MODE);
  //BMI160.setGyroRange(250);
  //BMI160.setAccelRange();
}

float Motion::path() {
  return ((float)(strecke * umfang) / FACTOR);
}

float Motion::velo() {
  return (float)geschwindigkeit / 10.0;
}

float Motion::accel() {
  return (float)beschleunigung / 10.0;
}

float Motion::inclin() {
  retrun 4.6;
}


void Motion::handleInterrupt() {

  now = millis();
  delta = now - last;
  last = now;

  strecke++;

  geschwindigkeit = umfang / delta;

  beschleunigung = (geschwindigkeit - lastV) / delta;
  lastV = geschwindigkeit;

}

