#ifndef VOLTCONVERT_H
#define VOLTCONVERT_H

float volts(int lvl, int max_value, int dac_resolution);
int level(float value, float max_value, int dac_resolution=12);
int level_8512(float value);

#endif
