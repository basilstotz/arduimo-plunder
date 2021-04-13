#include <ArduinoBLE.h>       //Bluetooth
#include <Arduino_LSM9DS1.h>  //Motion
#include <Arduino_APDS9960.h> //Gesture

#include "Filter.h"
#include "Controller.h"

//Velo Vars
float umfang;


#define motionPin 9


//Motion vars
volatile float strecke, geschwindigkeit, beschleunigung, steigung;
volatile float lastV, lastD, lastO;
volatile long lastT;

Filter *filter;

Controller *controller;

float power;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Serial Started");

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  Serial.println("IMU Started");

  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
    while (1);
  }
  Serial.println("BLE Started");

  //pinMode(motionPin, INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(motionPin), handleMotionInterrupt, FALLING);
  Serial.println("Motion Started");

  Serial.begin(9600);
  while (!Serial);

  if (!APDS.begin()) {
    Serial.println("Error initializing APDS9960 sensor!");
  }


  // for setGestureSensitivity(..) a value between 1 and 100 is required.
  // Higher values makes the gesture recognition more sensible but less accurate
  // (a wrong gesture may be detected). Lower values makes the gesture recognition
  // more accurate but less sensible (some gestures may be missed).
  // Default is 80
  APDS.setGestureSensitivity(80);

  Serial.println("Detecting gestures ...");

  //pinMode(LED_BUILTIN, OUTPUT);

  umfang = 0.6;
  lastT = 0L;
  lastV = 0.0;
  strecke = 0.0;

  controller = new Controller();
  filter = new Filter(1);
}

void loop() {
  float x, y, z, tmp;
  float xm;
  char text[30];

  //digitalWrite(LED_BUILTIN, HIGH);

  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);
    filter->update(x);
  }

  if (filter->dataReady()) {
    xm = filter->read();
    sprintf(text, "%8.4f", xm);
    Serial.println(text);
    steigung = xm - beschleunigung;
    power = controller->power(geschwindigkeit, beschleunigung, steigung);
    if (abs(power) < 10.0) {
      controller->calib(geschwindigkeit, beschleunigung, steigung);
    }
  }

  if (APDS.gestureAvailable()) {
    // a gesture was detected, read and print to serial monitor
    int gesture = APDS.readGesture();

    switch (gesture) {
      case GESTURE_UP:
        Serial.println("Detected UP gesture");
        break;

      case GESTURE_DOWN:
        Serial.println("Detected DOWN gesture");
        break;

      case GESTURE_LEFT:
        Serial.println("Detected LEFT gesture");
        break;

      case GESTURE_RIGHT:
        Serial.println("Detected RIGHT gesture");
        break;

      default:
        // ignore
        break;
    }
  }

  //digitalWrite(LED_BUILTIN, LOW);
  //delay(10);
}

void handleMotionInterrupt() {

  long now = millis();
  long delta = now - lastT;
  lastT = now;

  int ok = (abs(lastD - delta) < (delta / 10L)) && (delta < 10000L);

  if (ok && lastO) {
    strecke += umfang;
    geschwindigkeit = umfang / float(delta);
    beschleunigung = (geschwindigkeit - lastV) / float(delta);
  } else {
    geschwindigkeit = 0.0;
    beschleunigung = 0.0;
  }

  lastV = geschwindigkeit;
  lastD = delta;
  lastO = ok;
}


