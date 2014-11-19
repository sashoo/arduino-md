#include <DueTimer.h>

#include "Generator.h"
#include "VoltConvert.h"

#define FIRST_SEGMENT 14 
#define NUM_SEGMENTS   8   

#define FIRST_DIGIT 22 
#define NUM_DIGITS   4

#define DATA  26
#define LATCH 27
#define CLOCK 28

Generator G1 = Generator(A8, DAC0);
float dt = 0.01;
float voltmeter = 0.f;
/*
byte segments[10] = {
  0b11111100, 0b01100000, 0b11011010, 0b11110010, 0b01110110,
  0b10110110, 0b10111110, 0b11100000, 0b11111111, 0b11110110 
};
*/
byte segments[10] = {
  0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110,
  0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01101111 
};

byte empty = 0b00000000;

void setup() {
  
  //G1 = Generator(A0, DAC0);
  G1.init();
  
  Timer3.attachInterrupt(generator_step);
  Timer3.start(dt*1000000);
  Timer4.attachInterrupt(voltmeter_step);
  Timer4.start(250000);

  for (int i = 0; i < NUM_DIGITS; i++) {
    pinMode(FIRST_DIGIT + i, OUTPUT);
    digitalWrite(FIRST_DIGIT + i, HIGH);
  }
  
  pinMode(DATA, OUTPUT);
  pinMode(LATCH, OUTPUT);
  pinMode(CLOCK, OUTPUT);
  
  analogReadResolution(12); 
  analogWriteResolution(12);
  
  //analogWrite(DAC0, level(1.5f));
  G1.output();
}

void generator_step() {
  G1.iterate(dt);
  //G1.output();
}

void voltmeter_step() {
  int r = analogRead(A8);
  voltmeter = volts(r);  
}
  

void clearSegments() {
    digitalWrite(LATCH, LOW);
    shiftOut(DATA, CLOCK, LSBFIRST, empty);
    digitalWrite(LATCH, HIGH);
    
    for (int i = 0; i < NUM_DIGITS; i++) {    
      digitalWrite(FIRST_DIGIT + i, HIGH);
    }
  /*
  for (int j = 0; j < NUM_SEGMENTS; j++) {
    digitalWrite(FIRST_SEGMENT + j, HIGH);
  }
  
  for (int i = 0; i < NUM_DIGITS; i++) {
    digitalWrite(FIRST_DIGIT + i, LOW);
  }**/
}

int printSegments(float value) {
  
  String s = String(value, 3);
  int len = s.length();
  int k = 0;
  clearSegments();
    
  for (int i = 0; i < NUM_DIGITS; i++) {
    digitalWrite(FIRST_DIGIT + i, LOW);
 
    k %= len;    
    int digit = s[k] - '0';
    int mask = segments[digit];
        
    if (s[(k+1)%len] == '.') {
      k++;
      mask |= 0b10000000; 
    }
  
    k++; 
    
    digitalWrite(LATCH, LOW);
    shiftOut(DATA, CLOCK, LSBFIRST, mask);
    digitalWrite(LATCH, HIGH);
    
    delay(1);
    
    digitalWrite(LATCH, LOW);
    shiftOut(DATA, CLOCK, LSBFIRST, empty);
    digitalWrite(LATCH, HIGH);
    
    digitalWrite(FIRST_DIGIT + i, HIGH);     
  }
}

void loop() {
  printSegments(voltmeter);
}
