#include <Keyboard.h>

#define KEY KEY_F14

#define THRESHHOLD 10

const int potentiometerPin = 0;

bool currentPotentiometerValue;
bool lastPotentiometerValue = 0;

void setup() {
  // put your setup code here, to run once:
  Keyboard.begin();

  // turn the LEDs off (this may not work, as other stuff writes to them)
//  TXLED1;
//  RXLED1;
}

void loop() {
  // put your main code here, to run repeatedly:
  currentPotentiometerValue = readPot();

  if (currentPotentiometerValue && !lastPotentiometerValue) { // rising edge
    Keyboard.press(KEY);
  } else if (!currentPotentiometerValue && lastPotentiometerValue) { // falling edge
    Keyboard.release(KEY);
  }

  writeLed(currentPotentiometerValue);
  
  lastPotentiometerValue = currentPotentiometerValue;
  
  delay(1); // ms delay
}

bool readPot(){
  // analogRead() returns [0, 1023]
  return analogRead(potentiometerPin) > THRESHHOLD;
}

void writeLed(bool on) {
  if (on) {
    TXLED1;
    RXLED0;
  } else {
    TXLED1;
    RXLED1;
  }
}

//int tmp;
//bool debugReadPot(){
//  tmp = analogRead(potentiometerPin);
//  Serial.println(tmp, DEC); // you must call Serial.begin(9600) once before using this
//  return tmp > THRESHHOLD;
//}

