
#ifndef Lights_h
#define Lights_h

#include <Adafruit_NeoPixel.h>


#define LIGHT_PIN 0
#define LED_PIN 13

#define LED_BLOCK (ledCount/6)

#define LED0 (0*LED_BLOCK)
#define LED1 (1*LED_BLOCK)
#define LED2 (2*LED_BLOCK)
#define LED3 (3*LED_BLOCK)
#define LED4 (4*LED_BLOCK)
#define LED5 (5*LED_BLOCK)
#define LED6 (6*LED_BLOCK)

#define ON true
#define OFF false

#define RIGHT true
#define LEFT false

#define BLINK_MODE_0 0
#define BLINK_MODE_1 1
#define BLINK_MODE_2 2

#define LIGHT_MEAN 1

//value=(r0/(r+r0))*1023
//         _____     _____
// gnd ---|_r0__|---|__r__|----5v    
//                |
//                |
//               a0
//
// with                                  r0=5.0k, r0=10K
#define AMB_HIGH (931*LIGHT_MEAN) //1k       931, 930
#define AMB_MIDH (850*LIGHT_MEAN)  //2k       731, 850
#define AMB_MIDL (170*LIGHT_MEAN)
#define AMB_LOW (20*LIGHT_MEAN)  //50k       96, 170



#define LIGHT_AUTO 0
#define LIGHT_OFF 1
#define LIGHT_ON 2
#define LIGHT_FULL 3


#define DELAY 10

// in 100mW units. eg 100 -> 10W
#define MAX_POWER 100
// in 100mW units. eg 3 -> 300mW
#define LED_POWER 3
#define MAX_BRIGHT 255

class Lights {

  public:
    Lights(int _ledCount, int _pin);

    void left(boolean state);
    void right(boolean state);
    void brake(boolean state);
    void front(boolean state);
    void side(boolean state);
    void back(boolean state);

    void warn(boolean state);
    void rainbow(boolean state);
    void illum(boolean state);
    void phare(boolean state);
    void torch(boolean state);

    boolean left();
    boolean right();
    boolean brake();
    boolean front();
    boolean side();
    boolean back();

    boolean warn();
    boolean phare();
    boolean rainbow();
    boolean torch();
    boolean illum();


    void resetSpecial();
    void nextSpecial();
    void resetLight();
    void nextLight();
    void draw();
    void blinkParam(int _blinkPeriod, int _blinkAnimation, int _blinkMode);

  private:
    Adafruit_NeoPixel *strip;
    int pin;
    int ledCount;

    uint32_t frameCounter;

    long firstPixelHue;

    //light measurement
    int ambientLight;
    int lightMode;
    int lightState;
    int oldLightState;


    int leftPhase;
    int rightPhase;
    int brakePhase;
    int frontPhase;
    int sidePhase;
    int backPhase;
    int warnPhase;

    int blinkPeriod;
    int blinkAnimation;
    int blinkMode;

    int brakeAnimation;

    boolean leftState;
    boolean rightState;
    boolean brakeState;
    boolean frontState;
    boolean sideState;
    boolean backState;
    boolean warnState;

    boolean rainbowState;
    boolean illumState;
    boolean phareState;
    boolean torchState;

    boolean leftIntState;
    boolean rightIntState;
    boolean warnIntState;
    boolean sideIntState;
    boolean phareIntState;
    boolean frontIntState;
    boolean backIntState;

    void fill(int start, int stop, uint32_t color);
    void fill(int start, int stop, uint32_t color, boolean what, boolean dir, int phase );

    uint32_t white;
    uint32_t orange;
    uint32_t red;
    uint32_t off;

    int brightness;

};

#endif

