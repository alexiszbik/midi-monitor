#ifndef BASE_BLINKER_H
#define BASE_BLINKER_H

static const unsigned long blinkTime = 5;

class BaseBlinker {
public:
  BaseBlinker() {
  }

  virtual void setState(bool state) {
    internalState = state;
  }

  inline bool getState() {
    return internalState;
  }

  void blink() {
    trigger = true;
  }

  void process(unsigned long time) {
    if (trigger) {
      endTime = time + blinkTime;
      trigger = false;
      setState(true);
    }

    if (time > endTime && internalState) {
      setState(false);
    }
  }

private:
  bool trigger = false;
  bool internalState = false;

  unsigned long endTime = 0;
};

#endif
