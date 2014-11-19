// Timer interrupt experiment

// To model sine wave I send 12 bit integers to DAC8512 
// Each integer corresponds to the current sine point
// (from 0 mV to 4095 mV)

// Data is transferred over SPI, so there actually
// may be a performance issue, considering the speed
// at which I want to output values 
// (dt = 5 µs between two consecutive points)

// Atmel datasheet:
// http://www.atmel.com/Images/doc8161.pdf

// DAC8512 datasheet:
// http://www.analog.com/static/imported-files/data_sheets/DAC8512.pdf

#include "waveform.h"
#include <SPI.h>

#define MOSI 11 // master output slave input 
#define SCK  13 // serial clock (aka CLK, SCLK)
#define LD0   8 // load strobe; LOW causes DAC to output a value
#define CS0   9 // chip select (aka slave select, SS)
#define CLR  10 // clears DAC register, active low

#define SQ 7

int freq = FREQ;

volatile unsigned long sine_point = 0;
volatile unsigned long sine_data = 0;
volatile boolean toggle0 = 0;

void setup() {
  pinMode(MOSI, OUTPUT); 
  pinMode(SCK, OUTPUT);  
  pinMode(LD0, OUTPUT);  
  pinMode(CS0, OUTPUT);  
  pinMode(CLR, OUTPUT);  
  pinMode(SQ, OUTPUT);  

  digitalWrite(CS0, HIGH);
  digitalWrite(LD0, HIGH);

  SPI.begin();  
  SPI.setBitOrder(MSBFIRST);  // most significant bit first
  SPI.setDataMode(SPI_MODE0); // clock polarity and phase
  SPI.setClockDivider(SPI_CLOCK_DIV2); // half of system clock (16 MHz / 2 = 8 MHz)

  noInterrupts();

  TCCR1C = 0; // set entire TCCR1A register to 0
  TCCR1C = 0; // set entire TCCR1C register to 0

  // set compare match register for 200kHz (5µs) increments
  // easier way to calculate:
  // OCR1A = 16 * dt - 1;
  // where dt is the desired time step in µs

  //  5 µs -> 79
  // 20 µs -> 319
  // 25 µs -> 399
  // 40 µs -> 639
  // 50 µs -> 799
  OCR1A = 799; // = (16*10^6) / (1*200000) - 1 (must be <65536) 

  TCNT1  = 0; //initialize counter value to 0

  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  // turn on CTC mode (WGM12) and set CS10 for 1x prescaler
  // datasheet page 136
  TCCR1B = (1 << WGM12) | (1 << CS10);  
  
  interrupts();
}

ISR(TIMER1_COMPA_vect){//timer0 interrupt 2kHz toggles pin 8  
  
  // LOW causes DAC to read data from its register 
  // and output it, HIGH prevents reading
  digitalWrite(LD0, LOW);
  digitalWrite(LD0, HIGH);

  // uncomment for sine
  /*
  sine_point++;
  if (sine_point >= PHI_STEPS)
    sine_point = 0;

  setValue(sine_wave[sine_point]);
  */

  //uncomment for square wave  
  if (toggle0){
    //setValue(2048);
    digitalWrite(SQ, HIGH);
    toggle0 = 0;
  }
  else{
    //setValue(0);
    digitalWrite(SQ, LOW);
    toggle0 = 1;
  }  

  //should I use it?
  //TCNT1 = 0; 
}

void loop() {
  
}

// packages an int as 2 bytes and sends it
// over SPI to a 12 bit DAC (DAC8512)
// max value: 4095 (4095 mV)
inline void setValue(unsigned int value) { 
  //noInterrupts();

  // clears DAC register and forces it
  // to output zero

  //digitalWrite(CLR, HIGH);
  //digitalWrite(CLR, LOW);

  SPI.transfer(value >> 8);
  SPI.transfer(value & 0xff);
  
  // Load strobe to output value from DAC analog output
  // moved to ISR

  //digitalWrite(LD0, LOW);
  //digitalWrite(LD0, HIGH);

  //interrupts();
}