#include <SPI.h>
#include <DueTimer.h>

#include "Generator.h"
#include "VoltConvert.h"
#include "spi_helper.h"

//#define MOSI 11
//#define SCK  13
#define LD0   8
#define CS0  10
#define CLR  9



// Generator(float voltage_level, int output_type, int pin_output, int pin_feedback, int pin_ld=0);
//Generator G1 = Generator(3.3f, DAC_INTERN, DAC0, A8);
Generator G1 = Generator(3.3f, DAC_EXTERN, CS0,  A8, LD0);
float dt = 0.01;

void setup() {
  pinMode(MOSI, OUTPUT);
  pinMode(SCK, OUTPUT);
  pinMode(LD0, OUTPUT);
  pinMode(CS0, OUTPUT);
  pinMode(CLR, OUTPUT);
  
  //G1 = Generator(A0, DAC0);
  G1.init();
  
  Timer3.attachInterrupt(generator_step);
  Timer3.start(dt*1000000); // in us

  analogReadResolution(12); 
  analogWriteResolution(12);
  
  SPI.begin(CS0);
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(CS0, 21);
  
  //analogWrite(DAC0, level(1.5f));
  G1.output(1.5f);
}

void generator_step() {
  G1.iterate(dt);
  //G1.output();
}

void loop() {
  //G1.output(1.5f);
  //analogWrite(DAC0, 2048);
}
