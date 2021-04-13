#include "Lights.h"

Lights::Lights(int _ledCount, int _pin) {
  ledCount = _ledCount;
  pin = _pin;
  strip = new Adafruit_NeoPixel(ledCount, pin, NEO_GRB + NEO_KHZ800);
  strip->begin();

  white = strip->Color(255, 255, 255);
  orange = strip->Color(255, 255 / 3, 0);
  red = strip->Color(255, 0, 0);
  off = strip->Color(0, 0, 0);

  pinMode(LED_PIN, OUTPUT);

  //Serial.begin(9600);


  //ambientLight = 500;
  lightState = LIGHT_ON;
  //oldLightState = LIGHT_OFF;

  blinkPeriod = 50;
  blinkAnimation = 2;
  blinkMode = BLINK_MODE_0;

  frameCounter = 0;

  resetLight();
  resetSpecial();


  firstPixelHue = 0L;

  //units: 100mW=1 -> 10W->100; 300mW->3
  brightness = (MAX_BRIGHT * MAX_POWER) / (ledCount * LED_POWER);
  if (brightness > MAX_BRIGHT)brightness = MAX_BRIGHT;
  strip->setBrightness(brightness);
}

void Lights::blinkParam(int _blinkPeriod, int _blinkAnimation, int _blinkMode) {
  blinkPeriod = _blinkPeriod;
  blinkAnimation = _blinkAnimation;
  blinkMode = _blinkMode;
}

void Lights::fill(int start, int stop, uint32_t color) {
  for (int i = start; i < stop; i++) {
    strip->setPixelColor(i, color);
  }
}

void Lights::fill(int start, int stop,  uint32_t color, boolean what, boolean dir, int phase) {
  uint32_t fg, bg;

  int index, diff;

  if (what) {
    fg = color;
    bg = off;
  } else {
    fg = off;
    bg = color;
  }

  if (phase == -1) {
    fill(start, stop, fg);
  } else {
    diff = stop - start;
    for (int i = 0; i < diff; i++) {
      if (dir) {
        index = i;
      } else {
        index = diff - i - 1;
      }
      if (phase >= i) {
        strip->setPixelColor(start + index, fg);
      } else {
        strip->setPixelColor(start + index, bg);
      }
    }
  }
}


//setter

void Lights::left(boolean state) {
  if (state && ! leftState) {
    leftPhase = 0;
    leftIntState = OFF;
  }
  leftState = state;
}

void Lights::right(boolean state) {
  if (state && !rightState) {
    rightPhase = 0;
    rightIntState = OFF;
  }
  rightState = state;
}

void Lights::warn(boolean state) {

  if (state && !warnState) {
    warnPhase = 0;
    warnIntState = OFF;
  }
  warnState = state;
}



void Lights::front(boolean state) {
  if (state && !frontState) {
    frontPhase = 0;
    frontIntState = ON;
  }
  frontState = state;
}

void Lights::side(boolean state) {
  if (state && !sideState) {
    sidePhase = 0;
    sideIntState = ON;
  }
  sideState = state;
}

void Lights::back(boolean state) {
  if (state && !backState) {
    backPhase = 0;
    backIntState = ON;
  }
  backState = state;
}

void Lights::brake(boolean state) {
  brakeState = state;
}

void Lights::rainbow(boolean state) {
  rainbowState = state;
}

void Lights::illum(boolean state) {
  illumState = state;
}

void Lights::phare(boolean state) {
  phareIntState = ON;
  phareState = state;
}

void Lights::torch(boolean state) {
  torchState = state;
}

//getter

boolean Lights::left() {
  return leftState;
}

boolean Lights::right() {
  return rightState;
}

boolean Lights::warn() {
  return warnState;
}


boolean Lights::front() {
  return frontState;
}

boolean Lights::side() {
  return sideState;
}

boolean Lights::back() {
  return backState;
}

boolean Lights::brake() {
  return brakeState;
}

boolean Lights::rainbow() {
  return rainbowState;
}

boolean Lights::illum() {
  return illumState;
}

boolean Lights::phare() {
  return phareState;
}

boolean Lights::torch() {
  return torchState;
}


void Lights::resetSpecial() {
  warn(OFF);
  rainbow(OFF);
  illum(OFF);
  phare(OFF);
  torch(OFF);
}

void Lights::nextSpecial() {
  if (!warnState && !rainbowState && !illumState && !phareState && !torchState) {
    warn(ON);
  } else if (warnState) {
    warn(OFF);
    phare(ON);
  } else if (phareState) {
    phare(OFF);
    rainbow(ON);
  } else if (rainbowState) {
    rainbow(OFF);
    torch(ON);
  } else if (torchState) {
    torch(OFF);
    illum(ON);
  } else if (illumState) {
    illum(OFF);
  }
}


void Lights::resetLight() {
  lightMode = LIGHT_AUTO;
}

void Lights::nextLight() {
  lightMode = (lightMode + 1) % (LIGHT_FULL + 1);
}


void Lights::draw() {

  frameCounter++;

  //if (frameCounter % 64 == 0) {
  //  ambientLight += ((analogRead(LIGHT_PIN) * LIGHT_MEAN) - ambientLight) / LIGHT_MEAN;
  //}

  ambientLight = analogRead(LIGHT_PIN) * LIGHT_MEAN;



  if (lightMode == LIGHT_AUTO) {
    switch (lightState) {
      case LIGHT_OFF:
        if (ambientLight < AMB_MIDH) {
          lightState = LIGHT_ON;
        }
        break;
      case LIGHT_ON:
        if (ambientLight > AMB_HIGH) {
          lightState = LIGHT_OFF;
        } else if (ambientLight < AMB_LOW) {
          lightState = LIGHT_FULL;
        }
        break;
      case LIGHT_FULL:
        if (ambientLight > AMB_MIDL) {
          lightState = LIGHT_ON;
        }
        break;
      default:
        lightState = LIGHT_ON;
        break;
    }
  } else {
    lightState = lightMode;
  }

  //  if (frameCounter % 64 == 0) {
  //    Serial.print(ambientLight);
  //    Serial.print(" LM: ");
  //    Serial.print(lightMode);
  //    Serial.print(" LS: ");
  //    Serial.println(lightState);
  //  }

  //if (oldLightState != lightState) {
  //oldLightState = lightState;
  switch (lightState) {
    case LIGHT_OFF:
      front(OFF);
      back(OFF);
      side(OFF);
      digitalWrite(LED_PIN, LOW);
      break;
    case LIGHT_ON:
      front(ON);
      back(ON);
      side(OFF);
      if (frameCounter % 4 == 0) {
        digitalWrite(LED_PIN, HIGH);
      } else {
        digitalWrite(LED_PIN, LOW);
      }
      break;
    case LIGHT_FULL:
      front(ON);
      back(ON);
      side(ON);
      digitalWrite(LED_PIN, HIGH);
      break;
  }

  //}


  //  switch (lightState) {
  //    case LIGHT_OFF:
  //      digitalWrite(LED_PIN, OFF);
  //      break;
  //    case LIGHT_ON:
  //      if (frameCounter % 4 == 0) {
  //        digitalWrite(LED_PIN, HIGH);
  //      } else {
  //        digitalWrite(LED_PIN, LOW);
  //      }
  //      break;
  //    case LIGHT_FULL:
  //      digitalWrite(LED_PIN, ON);
  //      break;
  //  }


  int L1, L2, L4, L5;

  if (frontState) {
    L1 = LED1;
    L2 = LED2;
    L4 = LED4;
    L5 = LED5;
  } else {
    L1 = LED1 + (LED_BLOCK / 2);
    L2 = LED2 - (LED_BLOCK / 2);
    L4 = LED4 + (LED_BLOCK / 2);
    L5 = LED5 - (LED_BLOCK / 2);
  }

  // draw now!

  //if (frameCounter < brightness)strip->setBrightness(frameCounter);

  strip->clear();

  if (brakeState) {

    fill(LED3, L4, red);
    fill(L5, LED6, red);
    brakePhase++;
  }

  if (sideIntState ) {

    uint32_t c;

    if (sideState) {
      if (++sidePhase > 255)sidePhase = 255;
    } else {
      if (--sidePhase == 0)sideIntState = OFF;
    }
    c = strip->Color(sidePhase, sidePhase, sidePhase);
    fill(LED0, LED1, c);
    fill(LED2, LED3, c);

  }


  if (leftState && !warnState) {
    if (leftPhase == 0)leftIntState = !leftIntState;

    int s = -1;
    if (blinkAnimation > 0)s = leftPhase / blinkAnimation;

    if (leftIntState) {
      fill(LED0, L1, orange, ON, LEFT, s);
      fill(LED3, L4, orange, ON, LEFT, s);
    } else {
      switch (blinkMode) {
        case BLINK_MODE_0:
          fill(LED0, L1, orange, OFF, RIGHT, s);
          fill(LED3, L4, orange, OFF, RIGHT, s);
          break;
        case BLINK_MODE_1:
          fill(LED0, L1, orange, OFF, LEFT, s);
          fill(LED3, L4, orange, OFF, LEFT, s);
          break;
        case BLINK_MODE_2:
          fill(LED0, L1, off);
          fill(LED3, L4, off);
          break;
      }
    }
    leftPhase = (leftPhase + 1) % blinkPeriod;
  }

  if (rightState && !warnState) {
    if (rightPhase == 0)rightIntState = !rightIntState;

    int s = -1;
    if (blinkAnimation > 0)s = rightPhase / blinkAnimation;

    if (rightIntState) {
      fill(L2, LED3, orange, ON, RIGHT, s);
      fill(L5, LED6, orange, ON, RIGHT, s);
    } else {
      switch (blinkMode) {
        case BLINK_MODE_0:
          fill(L2, LED3, orange, OFF, LEFT, s);
          fill(L5, LED6, orange, OFF, LEFT, s);
          break;
        case BLINK_MODE_1:
          fill(L2, LED3 , orange, OFF, RIGHT, s);
          fill(L5, LED6 , orange, OFF, RIGHT, s);
          break;
        case BLINK_MODE_2:
          fill(L2, LED3, off);
          fill(L5, LED6, off);
          break;
      }
    }
    rightPhase = (rightPhase + 1) % blinkPeriod;
  }

  if (warnState) {
    if (warnPhase == 0)warnIntState = !warnIntState;

    int s = -1;
    if (blinkAnimation > 0)s = warnPhase / blinkAnimation;

    if (warnIntState) {
      fill(LED0, L1, orange, ON, LEFT, s);
      fill(LED3, L4, orange, ON, LEFT, s);
      fill(L2, LED3, orange, ON, RIGHT, s);
      fill(L5, LED6, orange, ON, RIGHT, s);
    } else {
      switch (blinkMode) {
        case BLINK_MODE_0:
          fill(LED0, L1, orange, OFF, RIGHT, s);
          fill(LED3, L4, orange, OFF, RIGHT, s);
          fill(L2, LED3, orange, OFF, LEFT, s);
          fill(L5, LED6, orange, OFF, LEFT, s);
          break;
        case BLINK_MODE_1:
          fill(LED0, L1, orange, OFF, LEFT, s);
          fill(LED3, L4, orange, OFF, LEFT, s);
          fill(L2, LED3 , orange, OFF, RIGHT, s);
          fill(L5, LED6 , orange, OFF, RIGHT, s);
          break;
        case BLINK_MODE_2:
          fill(LED0, L1, off);
          fill(LED3, L4, off);
          fill(L2, LED3 , off);
          fill(L5, LED6 , off);
          break;;
      }
    }
    warnPhase = (warnPhase + 1) % blinkPeriod;
  }


  if (frontIntState) {

    uint32_t c;

    if (frontState) {
      if (++frontPhase > 255)frontPhase = 255;
    } else {
      if (--frontPhase == 0)frontIntState = OFF;
    }
    c = strip->Color(frontPhase, frontPhase, frontPhase);
    fill(LED1, LED2, c);
  }

  if (backIntState){

    uint32_t c;

    if (backState) {
      if (++backPhase > 255)backPhase = 255;
    } else {
      if (--backPhase == 0)backIntState = OFF;
    }
    c = strip->Color(backPhase, 0, 0);
    fill(LED4, LED5, c);
  }

  ////////////////////////////////////////////////////////////////////

  if (rainbowState) {
    firstPixelHue += 128;
    for (int i = 0; i < strip->numPixels(); i++) {
      int pixelHue = firstPixelHue + (i * 65536L / (strip->numPixels() / 2) );
      strip->setPixelColor(i, strip->gamma32(strip->ColorHSV(pixelHue)));
    }
  }

  if (phareState) {
    uint32_t f;
    int wait, anz;

    wait = 2;  //or: 4 or 1
    anz = 2;   //or: 1 or 2

    if ( frameCounter % (wait * LED3) == 0)phareIntState = !phareIntState;

    int c = (frameCounter / wait) % LED3;
    for (int i = 0; i < LED3; i++) {
      if (abs(c - i) < anz) {
        f = white;
      } else {
        f = off;
      }
      if (phareIntState) {
        strip->setPixelColor(LED0  + i, f);
        strip->setPixelColor(LED3  + i, f);
      } else {
        strip->setPixelColor( LED3 - 1 - i, f);
        strip->setPixelColor( LED6 - 1 - i, f);
      }
    }
  }

  if (torchState) {
    fill(LED0, LED3, white);
    fill(LED3, LED6, off);
  }

  if (illumState)fill(LED0, LED6, white);

  ///////////////////////////////////////////////////////////////////////////////////77

  strip->show();
}

