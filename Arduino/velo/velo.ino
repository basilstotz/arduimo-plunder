#include <EEPROM.h>

#include "Lights.h"


#define OUT_PIN 9
#define LED_COUNT 24

#define LEFT_PIN 12
#define RIGHT_PIN 11


Lights *lights;

int lastRight, lastLeft;
unsigned long frameCount, timer;
int longLeft, longRight;

// setup() function -- runs once at startup --------------------------------

void setup() {

  pinMode(LEFT_PIN, INPUT_PULLUP);
  pinMode(RIGHT_PIN, INPUT_PULLUP);

  //float u=atan2(7.0,1.0);  

  longLeft = 0;
  longRight = 0;

  lights = new Lights(LED_COUNT, OUT_PIN);

  lights->blinkParam(50, 4, BLINK_MODE_2);

  lights->resetSpecial();
  lights->resetLight();

  //lights->phare(true);
}


void loop() {

  frameCount++;

  if (digitalRead(LEFT_PIN) == LOW) {
    if (lastLeft == HIGH) {
      timer = millis();
      if (!lights->warn() && !lights->rainbow() && !lights->illum() && !lights->phare() && !lights->torch()) {
        if (lights->left()) {
          lights->left(false);
        } else {
          lights->left(true);
          lights->right(false);
        }
      } else {
        lights->resetSpecial();
      }
      lastLeft = LOW;
    }
    longLeft++;
    if (longLeft == 300) {
      longLeft = 0;
      lights->nextLight();
      lights->left(false);
    }
  } else {
    if (millis() - timer > 1000)lastLeft = HIGH;
    longLeft = 0;
  }

  if (digitalRead(RIGHT_PIN) == LOW) {
    if (lastRight == HIGH) {
      timer = millis();
      if (lights->right()) {
        lights->right(false);
      } else {
        lights->right(true);
        lights->left(false);
      }
      lastRight = LOW;
    }
    longRight++;
    if (longRight == 300) {
      longRight = 0;
      lights->nextSpecial();
      lights->right(false);
    }
  } else {
    if (millis() - timer > 1000)lastRight = HIGH;
    longRight = 0;
  }

  if (lights->left() || lights->right()) {
    if ( millis() - timer > 60000) {
      lights->right(false);
      lights->left(false);
    }
  }

  lights->draw();

  //digitalWrite(LED_PIN, LOW);
  delay(DELAY);
  //digitalWrite(LED_PIN, HIGH);

}



