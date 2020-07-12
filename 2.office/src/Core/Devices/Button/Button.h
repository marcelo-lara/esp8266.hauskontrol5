#pragma once
#include "Arduino.h"
typedef void SwCallback(int clickCount);

class Button{
public:
  Button(uint8_t _switchButton);
  void setup();
  void update();

  SwCallback *swCallback;  

private:
  byte inSwitch;
  unsigned long inSw_lastMs;
  unsigned long inSw_downMs;
  int inSwitchCount;
};