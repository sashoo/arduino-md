#include "waveform.h"
#include <SPI.h>
#include "TimerOne.h"

#define MOSI 11
#define SCK  13
#define LD0   8
#define CS0   9
#define CLR  10
#define SQ 7

int freq = FREQ;
int i = 0;

volatile unsigned long sine_point = 0;
volatile unsigned long dti  = 0;
volatile unsigned long t1 = 0;
volatile unsigned long t2 = 0;
volatile boolean toggle0 = false;

void setup() {
  pinMode(MOSI, OUTPUT);
  pinMode(SCK, OUTPUT);
  pinMode(LD0, OUTPUT);
  pinMode(CS0, OUTPUT);
  pinMode(CLR, OUTPUT);
  //pinMode(SQ, OUTPUT);  
  digitalWrite(CS0, HIGH);
  digitalWrite(LD0, HIGH);

  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV2);

  Timer1.initialize(5);
  Timer1.attachInterrupt(pushSine); // blinkLED to run every 0.15 seconds
  
  //Serial.begin(9600);
    DDRD = B11111111;

}

void pushSine() {
  
  //setValue( sine_wave[sine_point % PHI_STEPS] );

/*
digitalWrite(LD0, LOW);
digitalWrite(LD0, HIGH);

sine_point++;
if (sine_point >= PHI_STEPS)
  sine_point = 0;

setValue(sine_wave[sine_point]);
*/

  if (toggle0){
    //setValue(2048);
    //digitalWrite(SQ, HIGH);
        PORTD = B11110000;
    toggle0 = 0;
  }
  else{
    //setValue(0);
    //digitalWrite(SQ, LOW);
        PORTD = B00001111;
    toggle0 = 1;
  }  
  //dti += 1;
  //if (dti > 2) {
   // sine_point += 1;  
   //digitalWrite(LD0, LOW);
  // toggle0 = true;

  
    //setValue( (dti+1)*200 );
  //  dti = 0;
  //}
  

  //unsigned long int micr = micros();

  //dti = t2 - t1;
  
  //setValue(dti);
  //t1 = millis();
  //Serial.print("micros  passed = ");
  //Serial.println(dti);

  // just because I don't want to deal with
  // integer overflow 
  //if (sine_point > PHI_STEPS)
  //  sine_point = 0;
}

void loop() {


 
  //noInterrupts();
  //t2 = millis();
  //interrupts();
  //dti = micros();

  //delay (1000);
  //Serial.print("micros  passed = ");
  //Serial.println(dti);


  /*
  unsigned int t = micros();
  unsigned int sine_step = t/DT;
  int s = sine_wave[sine_step % PHI_STEPS];

  delayMicroseconds(20);
  setValue(s);
  */
  
  //delay(100);
}

void setValue(unsigned int value) { 
  //digitalWrite(CLR, HIGH);
  //digitalWrite(CLR, LOW);
  //digitalWrite(CLR, LOW);
  //digitalWrite(CLR, HIGH);

    // Example values
  int ctrl_reg = 0; // 0000
  int level = value; // 111010111000

  // Shift 'ctrl_reg' left by 4 bits
  int shifted_ctrl_reg = ctrl_reg << 4; // 00010000

  // Shift 'level' 8 bits to the right
  int shifted_right_level = level >> 8; // 00001110

  // Create byte 1 by doing a bitwise or of 'shifted_ctrl_reg' and 'shifted_right_level'
  int byte1 = shifted_ctrl_reg;
  byte1 = byte1 | shifted_right_level; // 00011110

// Now in byte2 I only want the last 8 bits of 'level' as the first 4 were in byte1 so I just delete the first 4 bits from 'level' by making them 0
  int byte2 = level;
  bitWrite(byte2, 8, 0);
  bitWrite(byte2, 9, 0);
  bitWrite(byte2, 10, 0);
  bitWrite(byte2, 11, 0);

  //digitalWrite(CS0, LOW);  
  //SPI.transfer(0);
  SPI.transfer(byte1);
  SPI.transfer(byte2);
  //digitalWrite(CS0, HIGH);
  
  //delay(1);
  //digitalWrite(LD0, LOW);
  //digitalWrite(LD0, LOW);
  //delayMicroseconds(1000);
  //digitalWrite(LD0, HIGH);

  //digitalWrite(CLR, HIGH);
  //digitalWrite(LD0, HIGH);
}



