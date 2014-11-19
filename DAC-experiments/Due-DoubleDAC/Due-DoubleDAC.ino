#include <DueTimer.h>

#include "waveform.h"

//#define oneHzSample 1000000/maxSamplesNum  // sample for the 1Hz signal expressed in microseconds 
#define oneHzSample 1000000/PHI_STEPS  // sample for the 1Hz signal expressed in microseconds 

//const int button0 = 2, button1 = 3;
//volatile int wave0 = 0, wave1 = 0;

int i = 0;
int sample = 20;
int maxSamplesNum = PHI_STEPS;

volatile bool toggle = true;

void square_handler() {
  if (toggle)
    analogWrite(DAC0, 4095);  // write the selected waveform on DAC0
  else 
    analogWrite(DAC0, 0);  // write the selected waveform on DAC0
    
  toggle = !toggle;
}

void sine_handler() {
  analogWrite(DAC0, sine_wave[i]);  // write the selected waveform on DAC0
  i++;
  if(i >= maxSamplesNum)  // Reset the counter to repeat the wave
    i = 0;
}


void setup() {
  analogWriteResolution(12);  // set the analog output resolution to 12 bit (4096 levels)
  analogReadResolution(12);   // set the analog input resolution to 12 bit 

  Timer3.attachInterrupt(sine_handler);
  //Timer3.attachInterrupt(square_handler);
  Timer3.start(5);
  //attachInterrupt(button0, wave0Select, RISING);  // Interrupt attached to the button connected to pin 2
  //attachInterrupt(button1, wave1Select, RISING);  // Interrupt attached to the button connected to pin 3
}


void loop() {
  
  float a = 2.0;
  for (int i = 0; i < 1000000; i++) {
    a = a * 7.1;
  }
  // Read the the potentiometer and map the value  between the maximum and the minimum sample available
  // 1 Hz is the minimum freq for the complete wave
  // 170 Hz is the maximum freq for the complete wave. Measured considering the loop and the analogRead() time
  
  //sample = map(analogRead(A0), 0, 4095, 0, oneHzSample);
  //sample = constrain(t_sample, 0, oneHzSample);
  //sample = 20;

  //analogWrite(DAC0, waveformsTable[wave0][i]);  // write the selected waveform on DAC0
  //analogWrite(DAC1, waveformsTable[wave1][i]);  // write the selected waveform on DAC1
  //analogWrite(DAC0, sine_wave[i]);  // write the selected waveform on DAC0


  //delayMicroseconds(sample);  // Hold the sample value for the sample time
}
/*
// function hooked to the interrupt on digital pin 2
void wave0Select() {
  wave0++;
  if(wave0 == 4)
    wave0 = 0;
}

// function hooked to the interrupt on digital pin 3
void wave1Select() {
  wave1++;
  if(wave1 == 4)
    wave1 = 0;
}*/
