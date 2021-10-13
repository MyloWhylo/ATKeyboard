#include <Arduino.h>

#include "ATKeyboard.h"

byte currentStatus = 0x00;

void sendLEDStatus() {
   sendPacket(SET_INDICATOR);
   while (!replyReady) {
   }
   if (inPacket.dataByte == ACK) {
      delayMicroseconds(DELAY_LEN);
      sendPacket(0x00);
   } else if (inPacket.dataByte == RESEND) {
      delayMicroseconds(DELAY_LEN);
      sendLEDStatus();
   } else {
      digitalWrite(10, HIGH);
   }
}

void setCaps() {
   currentStatus |= 0b00000100;
   sendLEDStatus();
}

void setNum() {
   currentStatus |= 0b00000010;
   sendLEDStatus();
}

void setScroll() {
   currentStatus |= 0b00000001;
   sendLEDStatus();
}

void toggleCaps() {
   currentStatus ^= 0b00000100;
   sendLEDStatus();
}

void toggleNum() {
   currentStatus ^= 0b00000010;
   sendLEDStatus();
}

void toggleScroll() {
   currentStatus ^= 0b00000001;
   sendLEDStatus();
}

void clearCaps() {
   currentStatus &= ~0b00000100;
   sendLEDStatus();
}

void clearNum() {
   currentStatus &= ~0b00000010;
   sendLEDStatus();
}

void clearScroll() {
   currentStatus &= ~0b00000001;
   sendLEDStatus();
}