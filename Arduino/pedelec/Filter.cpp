#include "Filter.h"

Filter::Filter() {
  int n;
  float l = ((float)LENGTH - 1.0) / 2.0;
  writeIndex = 0;
  for (n = 0; n < LENGTH; n++) {
    window[n] = round(MAXINT * exp(-0.5 * pow( ((float)n - l) / (0.4 * l) , 2.0)) );
  }
}


void Filter::update(int16_t value) {

  if (filterIndex == LENGTH ) {
    accel = summe;
    summe = 0;
    readIndex = writeIndex;
    filterIndex = 0;
  }

  for (int i = 0; i < 4; i++) {
    summe += (uint32_t)(buffer[readIndex++] * (int32_t)window[filterIndex++]);
    readIndex %= LENGTH;
  }

  buffer[writeIndex++] = value;
  writeIndex %= LENGTH;

}

int32_t Filter::read() {
  return accel;
}
