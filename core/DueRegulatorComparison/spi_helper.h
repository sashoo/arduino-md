#include <SPI.h>

inline void SPI_send12(int cs, unsigned int value) { 
  SPI.transfer(cs, value >> 8, SPI_CONTINUE);
  SPI.transfer(cs, value & 0xff);
} 
