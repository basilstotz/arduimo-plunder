#ifndef Filter_h
#define Filter_h

#include <Arduino.h>

#define LENGTH 128
#define MAXINT 16348.0

class Filter {

  public:
    Filter();
    ~Filter();

    void update(int16_t value);

    int32_t read();

    

  private:
    int16_t buffer[128];
    int16_t window[128];
    int writeIndex;
    int readIndex;
    int filterIndex;
    int32_t summe;
    int32_t accel;
};

#endif
