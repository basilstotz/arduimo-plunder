/*
  LPS22HB - Read Pressure

  This example reads data from the on-board LPS22HB sensor of the
  Nano 33 BLE Sense and prints the pressure sensor value to the
  Serial Monitor once a second.

  The circuit:
  - Arduino Nano 33 BLE Sense

  This example code is in the public domain.
*/

#include <Arduino_LPS22HB.h>

int n;
unsigned long t;
float sumX, sumX2, sumY, sumXY;


void reset() {
  n = 0;
  sumX = 0.0;
  sumX2 = 0.0;
  sumY = 0.0;
  sumXY = 0.0;
  t = millis();
}

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!BARO.begin()) {
    Serial.println("Failed to initialize pressure sensor!");
    while (1);
  }
  reset();
}



void loop() {
  // read the sensor value
  float y = BARO.readPressure();
  float x = (millis() - t) / 1000.0;

  if (n++ < 100) {
    sumX += x;
    sumX2 += x * x;
    sumY += y;
    sumXY += x * y;
  } else {
    float a = (n * sumXY - sumX * sumY) / (n * sumX2 - sumX * sumX); // slope
    float b = (sumY - a * sumX) / (1.0*n);                           // y-Axis


    
    Serial.print(sumY/(1.0*n), 4);
    Serial.print(": ");
    Serial.print(b, 4);
    Serial.print("  ");
    Serial.println(a, 4);
    
    reset();
  }

  //delay(1000);
}
