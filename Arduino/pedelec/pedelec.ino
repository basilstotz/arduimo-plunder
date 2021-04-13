
#include "Motion.h"
#include "Controller.h"

int counter;
float p;

Motion *Motion;
Controller *controller;

void setup() {

  Motion =new Motion();
  Motion->begin(9,2.23);

  controller= new Controller();
  
  Serial.begin(9600);

}

void loop() {

  p=controller->power(Motion->velo(),Motion->accel());

  counter++;
  
  if (counter % 1000 == 0) {
    Serial.println(p);
    counter = 0;
  }


}








