#include "Filter.h"

Filter *filter;
int counter;

void setup() {
  Serial.begin(9600);
  filter = new Filter();
  pinMode(13, OUTPUT);
  for(int i=0;i<LENGTH;i++){
    //Serial.println(filter->window[i]);
  }
}

void loop() {
  digitalWrite(13, HIGH);
  //filter->update((int16_t)random(-16000,16000));
  filter->update(8000+(int16_t)random(-4000,4000));
  digitalWrite(13, LOW);
  if (counter++ % 128 == 0) {
    Serial.println(filter->read());
  }
  delay(1);
}
