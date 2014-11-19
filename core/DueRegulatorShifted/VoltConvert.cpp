float volts(int lvl) {
  return float(lvl/4096.f*3.3f);
}

int level(float v) {
  return int(v/3.3f*4096.f);
}

