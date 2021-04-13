#include "Motor.h"

Motor::Motor() {


}


void Motor::set(float _power) {
   powerSoll=_power;
}


void Motor::update(){
  int u=analogRead(9);
  int i=analogRead(10);
  int p=u*i;
  
}

