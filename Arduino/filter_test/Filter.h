#ifndef Filter_h
#define Filter_h

#include <Arduino.h>

#define LENGTH 256
#define MAXINT_L 32767L
#define MAXINT_F 32767.0

class Filter {

  public:

    Filter();
    ~Filter();

    void update(int16_t value);

    int32_t readRaw();
    float read();
    int16_t window[LENGTH];

  private:

    int32_t summe;
    int32_t accel;
    int32_t weights;
    int32_t factor;

    int writeIndex;
    int readIndex;
    int filterIndex;

    int16_t buffer[LENGTH];



};

#endif
