#pragma once
#include <Arduino.h>
#include <StatusLEDs.h>

#define DAT_BITS 11
#define DELAY_LEN 110

byte computeParity(uint8_t inByte);

struct Packet {
   byte parityBit;
   uint8_t dataByte;

   Packet(){};

   Packet(uint8_t data) {
      parityBit = computeParity(data);
      dataByte = data;
   }
};

extern volatile Packet outPacket;
extern volatile Packet inPacket;

extern volatile bool replyReady;
extern volatile bool transmitFinished;

void initialize(uint8_t cPin, uint8_t dPin);
void sendPacket(uint8_t toSend);
bool ping();

void recievePacket();
void transmitPacket();
