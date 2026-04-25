#include "MIDI.h"
#include "LEDGroup.h"
#include "Blinker.h"

#define SR_LATCH_PIN 8
#define SR_CLOCK_PIN 12
#define SR_DATA_PIN 11

MIDI_CREATE_INSTANCE(HardwareSerial, Serial, MIDI);

LEDGroup ledGroup = LEDGroup(SR_LATCH_PIN, SR_CLOCK_PIN, SR_DATA_PIN);

uint16_t binValue = 0;

Blinker clock = Blinker(9);
Blinker general = Blinker(10);

Blinker channels1_8[8] = { A0, A1, A2, A3, A4, A5, 2, 3 };
BaseBlinker channels9_16[8];

void setup() {
  Serial.begin(31250);  //MIDI baud rate

  pinMode(LED_BUILTIN, OUTPUT);

  MIDI.setHandleNoteOn(midiHandleNoteOn);
  MIDI.setHandleNoteOff(midiHandleNoteOff);
  MIDI.setHandleControlChange(midiHandleControlChange);
  MIDI.setHandleProgramChange(midiHandleProgramChange);
  MIDI.setHandlePitchBend(midiHandlePitchBend);

  MIDI.setHandleStart(midiHandleStart);
  MIDI.setHandleStop(midiHandleStop);
  MIDI.setHandleClock(midiHandleClock);

  MIDI.turnThruOn();
  MIDI.begin(MIDI_CHANNEL_OMNI);  //check if there's no performance issues

  for (int i = 0; i < 8; i++) {
    channels1_8[i].setState(true);
  }

  clock.setState(true);
  general.setState(true);

  ledGroup.process(0xFF);

  delay(1000);

  for (int i = 0; i < 8; i++) {
    channels1_8[i].setState(false);
  }

  ledGroup.process(0x00);
  clock.setState(false);
  general.setState(false);
}

void setBit(uint16_t &c, int x, bool value) {
  c = (c & ~(1 << x)) | (value << x);
}

void loop() {
  static byte pinIndex = 0;
  unsigned long time = millis();

  MIDI.read();

  if (pinIndex < 8) {
    channels1_8[pinIndex].process(time);
    channels9_16[pinIndex].process(time);
    setBit(binValue, pinIndex, channels9_16[pinIndex].getState());
    ledGroup.process(binValue);
    
  } else {
    clock.process(time);
    general.process(time);
  }

  pinIndex++;
  if (pinIndex >= 9) pinIndex = 0;
}

void handleMidi(byte channel) {
  general.blink();
  if (channel >= 1 && channel <= 8) {
    channels1_8[channel - 1].blink();
  }
  if (channel >= 9 && channel <= 16) {
    channels9_16[channel - 9].blink();
  }
}

void midiHandleNoteOn(byte channel, byte note, byte velocity) {
  handleMidi(channel);
}

void midiHandleNoteOff(byte channel, byte note, byte velocity) {
  handleMidi(channel);
}

void midiHandleControlChange(byte channel, byte control, byte value) {
  handleMidi(channel);
}

void midiHandleProgramChange(byte channel, byte value) {
  handleMidi(channel);
}

void midiHandlePitchBend(byte channel, int bend) {
  handleMidi(channel);
}

void midiHandleStart() {
  general.blink();
  clock.blink();
}

void midiHandleStop() {
  general.blink();
  clock.blink();
}

void midiHandleClock() {
  general.blink();
  clock.blink();
}
