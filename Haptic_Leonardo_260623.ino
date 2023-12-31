//Including the keyboard library
#include <Keyboard.h>
#include <KeyboardLayout.h>
#include <Keyboard_da_DK.h>
#include <Keyboard_de_DE.h>
#include <Keyboard_es_ES.h>
#include <Keyboard_fr_FR.h>
#include <Keyboard_it_IT.h>
#include <Keyboard_sv_SE.h>

const int p1ButtonPin = 2;  // input pin for pushbutton // two buttonpins are configured, because the same script is used for two seperate controllers, each with own arduino. Buttonpin uses leonardos keyboard-function. p1 writes "1" to computer. p2. writes "2"
const int p2ButtonPin = 3;  // input pin for pushbutton
const int ledPin = 4;
const int motorPin = 5;  // motor connected to digital pin

// timer variables
unsigned char ledState = LOW;  // used in CheckTime, to calculate how long led's been on
unsigned long ledCameOn = 0;   // used in ChekTime, to calculate how long led's been on
unsigned long timerAmount = 1800000; //(1800000 = 30 min)


unsigned long last_time = 0;
int previousButtonState = 0;  // for checking the state of the pushButtons

void setup() {
  Serial.begin(9600);
  pinMode(p1ButtonPin, INPUT_PULLUP);  //sets p1ButtonPin as input - Buttons still need differentation in arduino script, because they use Keyboard-library.
  pinMode(p2ButtonPin, INPUT_PULLUP);  //sets p2ButtonPin as input
  pinMode(ledPin, OUTPUT);
  pinMode(motorPin, OUTPUT);  // uses same motorPin. Ardity uses different serial ports to differentiate between the two contollers.

  // makes sure, led and motor are turned off
  digitalWrite(ledPin, LOW);
  digitalWrite(motorPin, LOW);
}

void loop() {
  // Turns motorPin on when receiving '1' from unity or off when receiving '0' from unity. Sent through Ardity in Unity.
  if (Serial.available() > 0) {

    switch (Serial.read()) {

      case '1':
        digitalWrite(motorPin, HIGH);  // sets motor on
        delay(100);                    // delay in between writes for stability
        break;

      case '0':
        digitalWrite(motorPin, LOW);  // sets motor off
        delay(100);                   // delay in between writes for stability
        break;

      case '+':
        LedOn();
        break;

      case '-':
        LedOff();
        break;

      case 'X':  // Execute tear-down functionality
        TearDown();
        break;

      default:
        break;
    }
    
    CheckTime();
  }

  //Reads ButtonPins - uses leonardo keyboard library. does not use Ardity's serial controller.
  if (checkState(p1ButtonPin) == true) {
    Keyboard.write('1');
    BlinkLed();
  }
  if (checkState(p2ButtonPin) == true) {
    Keyboard.write('2');
    BlinkLed();
  }
}

void LedOn() {
  digitalWrite(ledPin, HIGH);
  ledState = HIGH;
  ledCameOn = millis();
}

void LedOff() {
  digitalWrite(ledPin, LOW);
  ledState = LOW;
}

void BlinkLed() {
  digitalWrite(ledPin, HIGH);
  delay(200);
  digitalWrite(ledPin, LOW);
}

void TearDown() {
  for (int i = 0; i < 10; i++) {
    digitalWrite(ledPin, HIGH);
    delay(100);
    digitalWrite(ledPin, LOW);
    delay(100);
  }
  digitalWrite(motorPin, LOW);
}

void CheckTime() {
  if (ledState = HIGH) {
    if (millis() - ledCameOn > timerAmount) {
      LedOff();
    }
  }
}

// Checks button state
boolean checkState(int tmpPin) {
  if (digitalRead(tmpPin) == LOW && previousButtonState == 0) {
    previousButtonState = tmpPin;
    return true;

  } else if (digitalRead(tmpPin) == HIGH && previousButtonState == tmpPin) {
    previousButtonState = 0;
    delay(200);  //needed for deboucing of the buttons!
  }
  return false;
}