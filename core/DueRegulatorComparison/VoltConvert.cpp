#include "Arduino.h"

float volts(int lvl, int max_value, int dac_resolution) {
  return float(lvl/float(1 << dac_resolution) * max_value);
  //return float(lvl/4095.f * 3.3f);
}

int level(float value, float max_value, int dac_resolution=12) {
  return int(value/max_value * float(1 << dac_resolution));
  //return int(value/3.3f * 4095.f);
  //return int(value/3.3f * 4095);
}

int level_8512(float value) {
  return constrain(int(value*1000), 0, 4095);
}

