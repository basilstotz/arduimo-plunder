#ifndef Motion_h
#define Motion_h;

class Motion {

  public:
    void begin(int _pin, float _umfang);

    float path();
    float velo();
    float accel();

    float inclin();


  private:

    static void handleInterrupt();


};

#endif
