#ifndef Filter_h
#define Filter_h

#include <Arduino.h>

#define LENGTH 256
#define MAXINT_L 32767L
#define MAXINT_F 32767.0

class Filter {

  public:

    Filter(int _count);
    ~Filter();

    void update(float value);
    int dataReady();
    float read();


  private:

    float summe;
    float accel; 
    float weights;

    int count;
    int writeIndex;
    int readIndex;
    int filterIndex;
    int dataOk;

    float buffer[LENGTH];
    float window[LENGTH];


};

#endif

