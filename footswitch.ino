#include <Keyboard.h>


//#define DEBUG // define DEBUG to enable USB debugging
#define LED_ENABLE // define LED_ENABLE to enable LEDs on switch press (the default behavior is annoying so this is better, trust me)
#define KEY_BACKSLASH 0x5C

struct Switch {
  const uint8_t key_layer_1;
  const uint8_t key_layer_2;
  const uint8_t pin;
  const int potentiometerThreshold;
  const bool polarity;
  bool lastPotentiometerValue;
};

const int SWITCH_COUNT = 3;
Switch switches[SWITCH_COUNT] = {
  Switch{KEY_F14, KEY_F15, 0, 180, false, false},
  Switch{KEY_BACKSLASH, KEY_BACKSLASH, 1, 512, true, false},
  Switch{KEY_F16, KEY_F16, 2, 512, true, false},
};

// mode switch pin
const int MODE_SWITCH_PIN = 9;

void setup() {
  // put your setup code here, to run once:
  pinMode(MODE_SWITCH_PIN, INPUT_PULLUP); // default high, switch grounds the pin
  Keyboard.begin();
  #ifdef DEBUG
    Serial.begin(115200);
  #endif
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int switchIndex = 0; switchIndex < SWITCH_COUNT; switchIndex++) {
    Switch *currentSwitch = &switches[switchIndex];
    const bool currentPotentiometerValue = readPot(currentSwitch->pin, currentSwitch->potentiometerThreshold) != currentSwitch->polarity;

    bool switchState = digitalRead(MODE_SWITCH_PIN) == LOW;
    uint8_t key = switchState ? currentSwitch->key_layer_1 : currentSwitch->key_layer_2;
    if (currentPotentiometerValue && !currentSwitch->lastPotentiometerValue) { // rising edge
      Keyboard.press(key);
    } else if (!currentPotentiometerValue && currentSwitch->lastPotentiometerValue) { // falling edge
      Keyboard.release(key);
    }

    #ifdef LED_ENABLE
      // we have finite LEDs so these are special cases
      if (switchIndex == 0) {
        writeLed0(currentPotentiometerValue);
      } else if (switchIndex == 1) {
        writeLed1(currentPotentiometerValue);
      }
    #endif

    currentSwitch->lastPotentiometerValue = currentPotentiometerValue;
  }

  delay(1); // ms delay
}

bool readPot(const uint8_t pin, const int threshold) {
  #ifdef DEBUG
    int tmp = analogRead(pin);
    Serial.print(pin, DEC);
    Serial.print(": ");
    Serial.println(tmp, DEC); // you must call Serial.begin(9600) once before using this
    return tmp > threshold;
  #else
    // analogRead() returns [0, 1023]
    return analogRead(pin) > threshold;
  #endif
}

void writeLed0(bool on) {
  if (on) {
    TXLED0;
  } else {
    TXLED1;
  }
}

void writeLed1(bool on) {
  if (on) {
    RXLED0;
  } else {
    RXLED1;
  }
}
