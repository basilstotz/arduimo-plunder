#include "Filter.h"


Filter::Filter() {

  writeIndex = 0;
  readIndex = 0;
  filterIndex = 0;

  //create window
  weights = 0L;
  float l = ((float)LENGTH - 1.0) / 2.0;
  for (int n = 0; n < LENGTH; n++) {
    //rectangular window:
    //window[n]=32767;
    //gaussian window:
    window[n] = (int16_t)(MAXINT_F * exp(-0.5 * pow( ((float)n - l) / (0.4 * l) , 2.0)) );
    weights += (int32_t)window[n];
  }
  weights /= MAXINT_L;
}


void Filter::update(int16_t value) {

  // store filtered datum when ready
  if (filterIndex >= LENGTH ) {
    //accel = summe / (int32_t)LENGTH;
    accel = summe / weights;
    summe = 0L;
    readIndex = writeIndex;
    filterIndex = 0;
  }

  //update filtered datum
  for (int i = 0; i < 4; i++) {
    summe += ((int32_t)(buffer[readIndex++] * (int32_t)window[filterIndex++]))/MAXINT_L;
    if (readIndex >= LENGTH)readIndex = 0;
  }

  buffer[writeIndex++] = value;
  if (writeIndex >= LENGTH)writeIndex = 0;

}

int32_t Filter::readRaw() {
  return accel;
}

float Filter::read() {
  return (20.0*(float)accel / 32767.0 );
}
