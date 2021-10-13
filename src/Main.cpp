#include <ATKeyboard.h>
#include <Arduino.h>

void setup() {
   delay(2500);  // This delay avoids the BAT test.
   pinMode(10, OUTPUT);
   digitalWrite(10, LOW);
   initialize(6, 5);
   sendLEDStatus();
}

void loop() {
   // Your computer will receive these characters from a USB keyboard.
   toggleNum();
   delay(100);
   toggleCaps();
   delay(100);
   toggleScroll();
   delay(100);
}