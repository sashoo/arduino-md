float volts(int lvl) {
  return float(lvl/4096.f*3.3f);
}

int level(float v) {
  return int(v/3.3f*4096.f);
}



void setup() {
  analogReadResolution(12); 
  analogWriteResolution(12);
}

void loop() {  

  analogWrite(DAC0, level(0.250f));
}
