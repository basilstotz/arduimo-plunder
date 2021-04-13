#ifndef Motor_h
#define Motor_h

#include <Arduino.h>



class Motor {

  public:
    Motor();
    ~Motor();

    void set(float _power);
    void update();

  private:
    float powerSoll;
    float powerIst;


};

#endif
