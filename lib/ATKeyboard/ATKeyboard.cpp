#include "ATKeyboard.h"

#include <Arduino.h>

volatile Packet outPacket;
volatile Packet inPacket;
uint8_t dataPin;
uint8_t clockPin;

volatile uint8_t bitcount = 0;
volatile bool ackBit = false;
volatile bool replyReady = false;
volatile bool transmitFinished = false;

void initialize(uint8_t cPin, uint8_t dPin) {
   clockPin = cPin;
   dataPin = dPin;
   pinMode(clockPin, INPUT);
   pinMode(dataPin, INPUT);
   attachInterrupt(digitalPinToInterrupt(clockPin), recievePacket, FALLING);
}

byte computeParity(uint8_t inByte) {
   uint8_t bitCount = 1;
   for (uint8_t ii = 0; ii < 8; ii++) {
      if ((inByte & 0x1) == 1) {
         bitCount++;
      }
      inByte = inByte >> 1;
   }
   return bitCount % 2;
}

void sendPacket(uint8_t toSend) {
   outPacket.dataByte = toSend;
   outPacket.parityBit = computeParity(toSend);
   detachInterrupt(digitalPinToInterrupt(clockPin));
   replyReady = false;
   transmitFinished = false;

   pinMode(clockPin, OUTPUT);
   digitalWrite(clockPin, LOW);

   delayMicroseconds(DELAY_LEN);

   bitcount = 0;
   pinMode(dataPin, OUTPUT);
   digitalWrite(clockPin, HIGH);
   delayMicroseconds(DELAY_LEN);
   pinMode(clockPin, INPUT);
   attachInterrupt(digitalPinToInterrupt(clockPin), transmitPacket, FALLING);

   return;
}

/* TODO: Fix this damn function!
Implement proper sending for values
I.e., if on bit 8 send parity, 9 send 0x01; */
void transmitPacket() {
   switch (bitcount) {
      case 0:
         digitalWrite(dataPin, LOW);
         break;

      case 1 ... 8:
         digitalWrite(dataPin, outPacket.dataByte & 0x01);
         outPacket.dataByte = outPacket.dataByte >> 1;
         break;

      case 9:
         digitalWrite(dataPin, outPacket.parityBit);
         break;

      case 10:
         digitalWrite(dataPin, HIGH);
         transmitFinished = true;
         ackBit = true;
         bitcount = 0;
         pinMode(clockPin, INPUT);
         pinMode(dataPin, INPUT);
         attachInterrupt(digitalPinToInterrupt(clockPin), recievePacket, FALLING);
         return;

      default:
         break;
   }
   bitcount++;
}

void recievePacket() {
   if (ackBit) {
      switch (bitcount) {
         case 3 ... 9:
            digitalWrite(10, HIGH);
            inPacket.dataByte = inPacket.dataByte >> 1;
            digitalWrite(10, LOW);

         case 2:
            digitalWrite(10, HIGH);
            inPacket.dataByte |= (digitalRead(dataPin) & 0x01) << 7;
            digitalWrite(10, LOW);
            break;

         case 10:
            inPacket.parityBit = digitalRead(dataPin);
            break;

         case 11:
            digitalWrite(10, HIGH);
            bitcount = 0;
            replyReady = true;
            ackBit = false;
            digitalWrite(10, LOW);
            return;

         default:
            break;
      }
      bitcount++;

   } else {
      switch (bitcount) {
         case 2 ... 8:
            digitalWrite(10, HIGH);
            inPacket.dataByte = inPacket.dataByte >> 1;
            digitalWrite(10, LOW);

         case 1:
            digitalWrite(10, HIGH);
            inPacket.dataByte |= (digitalRead(dataPin) & 0x01) << 7;
            digitalWrite(10, LOW);
            break;

         case 9:
            inPacket.parityBit = digitalRead(dataPin);
            break;

         case 10:
            digitalWrite(10, HIGH);
            bitcount = 0;
            replyReady = true;
            ackBit = false;
            digitalWrite(10, LOW);
            return;

         default:
            break;
      }
      bitcount++;
   }
}