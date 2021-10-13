#pragma once
#include <Arduino.h>

#define SET_INDICATOR 0xED
#define ACK 0xFA
#define RESEND 0xFE

void sendLEDStatus();
void setStatusLEDs();

void setCaps();
void setNum();
void setScroll();

void toggleCaps();
void toggleNum();
void toggleScroll();

void clearCaps();
void clearNum();
void clearScroll();