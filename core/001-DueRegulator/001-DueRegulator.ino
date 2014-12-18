#define FIRST_SEGMENT 14 // in reverse
#define NUM_SEGMENTS   8   

#define FIRST_DIGIT 11  // in reverse
#define NUM_DIGITS   4
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

void setup() {
  // put your setup code here, to run once:
  for (int i = 0; i < NUM_DIGITS; i++) {
    pinMode(FIRST_DIGIT - i, OUTPUT);
    digitalWrite(FIRST_DIGIT - i, HIGH);
  }
    
  for (int j = 0; j < NUM_SEGMENTS; j++) {
    pinMode(FIRST_SEGMENT + j, OUTPUT);
    digitalWrite(FIRST_SEGMENT + j, LOW);
  }
  
  analogReadResolution(12); 
}

void clearSegments() {
  for (int j = 0; j < NUM_SEGMENTS; j++) {
    digitalWrite(FIRST_SEGMENT + j, HIGH);
  }
  
  for (int i = 0; i < NUM_DIGITS; i++) {
    digitalWrite(FIRST_DIGIT - i, LOW);
  }
}

int printSegments(float value) {
  int t = (millis() / 1000) % 10;
  //int rest = 0;
  //digitalWrite(14, HIGH);
  //value *= 1000.f;
  //int number = int(value);
  //number = 1234;
  //int digit, rem;
  String s = String(value, 3);
  int len = s.length();
  int k = 0;
    
  for (int i = 0; i < NUM_DIGITS; i++) {
    digitalWrite(FIRST_DIGIT - i, LOW);
    
    //digit  = number/pow(10, 3-i);
    //number = number - pow(10, 3-i);
    //digit  = number / int(pow(10, 3-i));
    //number = number % int(pow(10, 3-i));
    //int mask = 0;
    k %= len;
    int digit = s[k] - '0';
    int mask = segments[digit];
        
    if (s[(k+1)%len] == '.') {
      k++;
      mask |= 0b10000000; 
    }
  
    k++;    
    
    for (int j = 0; j < NUM_SEGMENTS; j++) {
      boolean enabled = bitRead(mask, j);
      digitalWrite(FIRST_SEGMENT + j, enabled);    
    }
    delay(1);
    
    for (int j = 0; j < NUM_SEGMENTS; j++) {
      //boolean enabled = bitRead(mask, j);
      digitalWrite(FIRST_SEGMENT + j, LOW);    
    }
    
    digitalWrite(FIRST_DIGIT - i, HIGH);
  }
}

void loop() {
  int r = analogRead(A8);
  float volts = float(r)/4095.f*3.3f;
  printSegments(volts);
  //delay(100);
  //delay(100);
  
  //int number = 1234;
  
  
  
    /*
    int digitValue = number / (  pow(10, (NUM_DIGITS - i -1 )));
    int mask = segments[digitValue];
    number = number % ( int(pow(10,  (NUM_DIGITS - i -1 ))));
    
    
    for (int j = 0; j < NUM_SEGMENTS; j++) {
      boolean enabled = bitRead(mask, j);
      digitalWrite(FIRST_SEGMENT + j, enabled);    
    }
    
    digitalWrite(FIRST_DIGIT - i, HIGH);
  } */
 

}
