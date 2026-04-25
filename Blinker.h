#ifndef BLINKER_H
#define BLINKER_H

#include "BaseBlinker.h"

class Blinker : public BaseBlinker {
public:
  Blinker(byte pin)
    : pin(pin) {
    pinMode(pin, OUTPUT);
  }

  void setState(bool state) {
    BaseBlinker::setState(state);
    digitalWrite(pin, state ? HIGH : LOW);
  }

private:
  byte pin;
};

#endif
