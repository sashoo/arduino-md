#include <SPI.h>
#include <DueTimer.h>
#include "waveform.h"
#include "common.h"

#include "Generator.h"
#include "VoltConvert.h"
#include "spi_helper.h"

//#define MOSI 11
//#define SCK  13
#define LD0   8
#define CS0  10
#define CLR  9

#define SQRT2 sqrt(2)
#define INV_AMP 0.4285496f // 1/3.3*sqrt(2)/1000.f

int wave_step = 0;

int local_step1 = 0;
int wave_shift1 = 0;
int local_step2 = 0;
int wave_shift2 = 0;

float value1 = 0.f;
float value2 = 0.f;

float amp1 = 1.f;
float amp2 = 1.f;

#define encoder0PinA  6
#define encoder0PinB  7

volatile unsigned int encoder0Pos = 0;


void doEncoderA() {
  /* If pinA and pinB are both high or both low, it is spinning
   * forward. If they're different, it's going backward.
   *
   * For more information on speeding up this process, see
   * [Reference/PortManipulation], specifically the PIND register.
   */
  if (digitalRead(encoder0PinA) == digitalRead(encoder0PinB)) {
    encoder0Pos++;
  } else {
    encoder0Pos--;
  }
  
  if (encoder0Pos < 0) encoder0Pos = 0;
  if (encoder0Pos > PHI_STEPS) encoder0Pos = PHI_STEPS;
  //Serial.println (encoder0Pos, DEC);
}

void doEncoderB() {
  /* If pinA and pinB are both high or both low, it is spinning
   * forward. If they're different, it's going backward.
   *
   * For more information on speeding up this process, see
   * [Reference/PortManipulation], specifically the PIND register.
   */
  if (digitalRead(encoder0PinA) == digitalRead(encoder0PinB)) {
    encoder0Pos--;
  } else {
    encoder0Pos++;
  }

  if (encoder0Pos < 0) encoder0Pos = 0;
  if (encoder0Pos > PHI_STEPS) encoder0Pos = PHI_STEPS;

  //Serial.println (encoder0Pos, DEC);
}


// Generator(float voltage_level, int output_type, int pin_output, int pin_feedback, int pin_ld=0);
Generator G1 = Generator(3.3f, DAC_INTERN, DAC0, A8);
//Generator G1 = Generator(3.3f, DAC_EXTERN, CS0,  A8, LD0);
float dt = 0.01;

void setup() {
  pinMode(MOSI, OUTPUT);
  pinMode(SCK, OUTPUT);
  pinMode(LD0, OUTPUT);
  pinMode(CS0, OUTPUT);
  pinMode(CLR, OUTPUT);
  pinMode(A8, INPUT);
  
  G1.init();
  
  //Timer3.attachInterrupt(generator_step);
  //Timer3.start(dt*1000000); // in us
  
  Timer4.attachInterrupt(waveform_step);
  Timer4.start(20);

  analogReadResolution(12); 
  analogWriteResolution(12);

  //SPI.setClockDivider(2);
  //SPI.begin(CS0);
  //SPI.setBitOrder(MSBFIRST);
  //SPI.setDataMode(SPI_MODE0);
  
  pinMode(encoder0PinA, INPUT); 
  digitalWrite(encoder0PinA, HIGH);       // turn on pullup resistor
  pinMode(encoder0PinB, INPUT); 
  digitalWrite(encoder0PinB, HIGH);       // turn on pullup resistor

  attachInterrupt(encoder0PinA, doEncoderA, CHANGE);  // encoder pin on interrupt 0 - pin 2
  attachInterrupt(encoder0PinB, doEncoderB, CHANGE);  // encoder pin on interrupt 0 - pin 2
}

void generator_step() {
  G1.iterate(dt);
}

void waveform_step() {
  analogWrite(DAC0, int(value1));
  analogWrite(DAC1, int(value2));
  //dacc_write_conversion_data(DACC_INTERFACE, uint32_t(0x4000));
  ++wave_step;
  if (wave_step >= PHI_STEPS) wave_step = 0;
  //G1.wave_output(G1.Eqs);  
  local_step1 = wave_step + encoder0Pos;
  if (local_step1 >= PHI_STEPS) local_step1 -= PHI_STEPS;
  //if (local_step1 < 0) local_step1 = PHI_STEPS;  
  
  local_step2 = wave_step + wave_shift2;
  if (local_step2 >= PHI_STEPS) local_step2 -= PHI_STEPS;
  //if (local_step2 < 0) local_step2 = PHI_STEPS;  
  
  //value = this->Eqs * INV_AMP * sine_wave[local_step]; 
  value1 = amp1 * sine_wave[local_step1];
  value2 = amp2 * sine_wave[local_step2];
  
  
/*
  if (this->output_type == DAC_INTERN) {
    //int lvl = level(value, this->voltage_level, 12);
    //analogWrite(this->pin_output, lvl);
    analogWrite(this->pin_output, int(value));
  }
  else if (this->output_type == DAC_EXTERN) {
    int lvl = level_8512(value);
    digitalWrite(this->pin_ld, HIGH);
    SPI_send12(this->pin_output, lvl);
    digitalWrite(this->pin_ld, LOW);
  }
  */
}

void generator_log() {
  //SerialUSB.println('working on due!'); 
}

void loop() {
}
