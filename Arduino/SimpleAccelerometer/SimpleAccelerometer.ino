/*
  Arduino LSM9DS1 - Simple Accelerometer

  This example reads the acceleration values from the LSM9DS1
  sensor and continuously prints them to the Serial Monitor
  or Serial Plotter.

  The circuit:
  - Arduino Nano 33 BLE Sense

  created 10 Jul 2019
  by Riccardo Rizzo

  This example code is in the public domain.
*/

#include <Arduino_LSM9DS1.h>
//#include <avr/dtostrf.h>

//float mx, my, mz;
//float mvx, mvy, mvz;
float vx, vy, vz, sx, sy, sz;
int sign;
//float gg;
float mgg,ia;
float maxi, mini, delta;
int t1,t2,deltat;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  //Serial.println("Started");

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  //Serial.print("Accelerometer sample rate = ");
  //Serial.print(IMU.accelerationSampleRate());
  //Serial.println(" Hz");
  //Serial.println();
  //Serial.println("Acceleration in G's");
  //Serial.println("X\tY\tZ");
  t1=t2=deltat=0;
  vx = 0.0; vy = 0.0; vz = 0.0; sx = 0.0; sy = 0.0; sz = 0.0;
  //mvx = 0.0; mvy = 0.0; mvz = 0.0;
  sign = 1;
  //gg = 0.0;
  mgg = 0.9735;
  ia=0.0;
}

void loop() {
  float x, y, z;

  float ax, ay, az;
  float aa;
  float da;
  
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);
    aa = sqrt(x * x + y * y + z * z);

    //pi-controller
    da=mgg-aa;
    ia+=da;
    mgg += 0.001 * da - 0.01*ia; 
    //highpass
    az = mgg-aa;
    //integrate
    vz += az;
    sz += vz;



    if (sign == 1) {
      if (vz < -0.01) {
        sign = -1;
        mini = sz;
        t1=millis();
      }
    } else {
      if (vz > 0.01) {
        sign = 1;
        maxi = sz;
        t2=millis();
      }
    }
    delta = maxi - mini;
    deltat= abs(t1-t2);
    

    //    char str_az[20],str_vz[20],str_sz[20]
    //    dtostrf(az,8,5,str_az);
    //   dtostrf(vz,8,5,str_vz);
    //   dtostrf(sz,8,5,str_sz);


    //Serial.print(deltat/1000.0, 4);
    //Serial.print('\t');
    Serial.print(ia, 4);
    Serial.print('\t');
    Serial.print(100 * az, 4);
    Serial.print('\t');
    Serial.print(10 * vz, 4);
    Serial.print('\t');
    Serial.println(sz, 4);
  }

}
