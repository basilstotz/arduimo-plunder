#include "Controller.h"

Controller::Controller() {
  roll = 0.005;
  luft = 0.2;
  mass = 110.0;
}

Controller::Controller(float _roll, float _luft, float _mass) {
  roll = _roll;
  luft = _luft;
  mass = _mass * G;
}

float Controller::power( float velo, float accel, float inclin) {
  return (mass * (accel + roll + inclin) + luft * velo * velo) * velo; //mass *(accel+roll)=-luft*velo*velo
}                                                             //roll=(-luft*velo*velo)/mass-accel
 

void Controller::calib(float velo, float accel, float inclin) {
  if (velo > 10.0) {
    luft = -(mass * (accel + roll+inclin*velo)) / (velo * velo);
  } else if (velo < 3.0) {
    roll = ((-luft * velo * velo) / mass) - accel;
  }
}


