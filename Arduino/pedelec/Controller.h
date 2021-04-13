#ifndef Controller_h
#define Controller_h


#include "Motion.h"

#define G (9.81)

class Controller {

  public:
    Controller();
    Controller(float _aplha, float _beta, float _mass);

    float power(float velo, float accel);
    void calib(float velo, float accel);

  private:
    float roll;
    float luft;
    float mass;



};

















#endif
