#include <Keyboard.h>

#define KEY_1 KEY_F14
#define KEY_2 0x5C // backslash
#define POT_THRESHHOLD 10
#define BUTTON_THRESHOLD 20

const int potentiometerPin = 0;
const int buttonPin = 9;

// potientometer states
bool currentPotentiometerValue; // could be local, but why not?
bool lastPotentiometerValue = 0; // stored pot state

// button states
uint8_t buttonOnCount = 0; // how many loops the button has been depressed for (for debouncing)
uint8_t selectedKey = KEY_1;
bool buttonState; // could be local, but why not?
bool lastButtonState = false;

void setup() {
  // put your setup code here, to run once:
  pinMode(buttonPin, INPUT_PULLUP);
  Keyboard.begin();
  Serial.begin(9600);
}

void loop() {
  buttonState = digitalRead(buttonPin) == LOW;
  Serial.println(buttonState);

  if (buttonState) {
    if (buttonOnCount < BUTTON_THRESHOLD) {
      buttonOnCount += 1;
    } else {
      toggleKey();
    }
  } else {
    buttonOnCount = 0;
  }

  lastButtonState = buttonState;
  
  // put your main code here, to run repeatedly:
  currentPotentiometerValue = readPot();

  if (currentPotentiometerValue && !lastPotentiometerValue) { // rising edge
    Keyboard.press(selectedKey);
  } else if (!currentPotentiometerValue && lastPotentiometerValue) { // falling edge
    Keyboard.release(selectedKey);
  }

  writeLed(currentPotentiometerValue);
  
  lastPotentiometerValue = currentPotentiometerValue;
  
  delay(1); // ms delay
}

void toggleKey() {
  // release previous key if necessary
  if (lastPotentiometerValue) {
     Keyboard.release(selectedKey);
  }
  
  if (selectedKey == KEY_1) {
    selectedKey = KEY_2;
  } else {
    selectedKey = KEY_1;
  }

  // press new key if necessary
  if (lastPotentiometerValue) {
     Keyboard.press(selectedKey);
  }
}

bool readPot(){
  // analogRead() returns [0, 1023]
  return analogRead(potentiometerPin) > POT_THRESHHOLD;
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

