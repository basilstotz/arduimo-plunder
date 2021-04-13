#include "Filter.h"


Filter::Filter(int _count) {

  _count=count;

  writeIndex = 0;
  readIndex = 0;
  filterIndex = 0;

  //create window
  weights = 0.0;
  float l = (float(LENGTH) - 1.0) / 2.0;
  for (int n = 0; n < LENGTH; n++) {
    //rectangular window:
    window[n]=1.0;
    //gaussian window:
    //window[n] = exp(-0.5 * pow( ((float)n - l) / (0.4 * l) , 2.0)) ;
    weights += window[n];
  }
}


void Filter::update(float value) {

  // store filtered datum when ready
  if (filterIndex >= LENGTH ) {
    accel = summe / weights;
    dataOk=true;
    summe = 0.0;
    readIndex = writeIndex;
    filterIndex = 0;
  }

  //update filtered datum
  for (int i = 0; i < count; i++) {
    summe += buffer[readIndex++] * window[filterIndex++];
    if (readIndex >= LENGTH)readIndex = 0;
  }

  buffer[writeIndex++] = value;
  if (writeIndex >= LENGTH)writeIndex = 0;

}

int Filter::dataReady(){
  return dataOk;
}

float Filter::read() {
  dataOk=false;
  return accel;
}



