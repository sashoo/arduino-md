#include <SPI.h>

//#define LED_PIN     9
#define POT_PIN     A0
#define DAC_PIN     A1

//#include "TimerOne.h"

//#define MOSI 11
//#define SCK  13
#define LD0   8
#define CS0  4
#define CLR  9
#define SQ 7
 
void setup()
{
  //pinMode(LED_PIN, OUTPUT);
  //SerialUSB.begin(9600);  
  analogReadResolution(12); 
  analogWriteResolution(12);
  
  pinMode(POT_PIN, INPUT);
  pinMode(DAC_PIN, INPUT);
  pinMode(13, OUTPUT);
  
  pinMode(MOSI, OUTPUT);

  pinMode(SCK, OUTPUT);
  pinMode(LD0, OUTPUT);
  pinMode(CS0, OUTPUT);
  pinMode(CLR, OUTPUT);
  //pinMode(SQ, OUTPUT);  
  //digitalWrite(CS0, HIGH);
  digitalWrite(LD0, HIGH);


  SPI.begin(CS0);
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(CS0, 21);
  digitalWrite(CLR, LOW);
  digitalWrite(CLR, HIGH);

}
 
inline void setValue(unsigned int value) { 
  SPI.transfer(CS0, value >> 8, SPI_CONTINUE);
  SPI.transfer(CS0, value & 0xff);
  
  //SPI.transfer(CS0, 0b00000000, SPI_CONTINUE);
  //SPI.transfer(CS0, 0b00000000, SPI_LAST);
  
} 
 
void loop()
{
  //int rotation, output;  
  //rotation = analogRead(POT_PIN);
  //output = map(rotation, 0, 1023, 0, 3000);
  
  //digitalWrite(CLR, LOW);
  
  setValue(200); 
  digitalWrite(LD0, LOW);
  digitalWrite(LD0, HIGH);
  //digitalWrite(CLR, HIGH);
  
 
  //SerialUSB.println(rotation);
  //delay(250);
}
