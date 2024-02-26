#include "PluggableUSBHID.h"
#include "USBKeyboard.h"

#define POSITION_3   0x54
#define POSITION_2   0x1C
#define POSITION_1   0x38
#define POSITION_M   0x70
#define POSITION_0   0x60
#define POSITION_P   0x62
#define POSITION_EPT 0x63

#define REVERSE_0 2
#define REVERSE_FORWARD 3
#define REVERSE_BACKWARD 1

#define UPPER 1
#define LOWER 0

USBKeyboard Keyboard;

void setup() {
  // put your setup code here, to run once:
  for (int i = 13; i <= 22; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
  }
  for (int i = 3; i <= 12; i++) pinMode(i, INPUT_PULLDOWN);
  Serial.begin(115200);
}

uint8_t getPosition() {
  uint8_t switches = 0x00;
  for (int i = 0; i < 7; i++) switches |= (digitalRead(i + 3) << i);
  return switches;
}

uint8_t getReverse() {
  if (digitalRead(11)) return REVERSE_FORWARD;
  if (digitalRead(10)) return REVERSE_BACKWARD;
  return REVERSE_0;
}

void processPositionMetrostroi(uint8_t position) {
  switch (position) {
    case POSITION_3:
      Keyboard.key_code('3', 0);
      break;
    case POSITION_2:
      Keyboard.key_code('2', 0);
      break;
    case POSITION_1:
      Keyboard.key_code('1', 0);
      break;
    case POSITION_M:
      break;
    case POSITION_0:
      Keyboard.key_code('4', 0);
      break;
    case POSITION_P:
      Keyboard.key_code('5', 0);
      break;
    case POSITION_EPT:
      Keyboard.key_code('7', 0);
      break;
  }
}

void processReverseMetrostroi(uint8_t direction) {
  if (direction == UPPER) Keyboard.key_code('0', 0);
  else if (direction == LOWER) Keyboard.key_code('9', 0);
}

void loop() {
  // put your main code here, to run repeatedly:
  uint8_t previousPosition = POSITION_0, currentPosition = POSITION_0, previousReverse = REVERSE_0, currentReverse = REVERSE_0;
  while (1) {
    currentPosition = getPosition();
    currentReverse = getReverse();
    if (currentPosition != previousPosition) {
      processPositionMetrostroi(currentPosition);
      previousPosition = currentPosition;
    }
    if (currentReverse > previousReverse) {
      processReverseMetrostroi(UPPER);
      previousReverse = currentReverse;
    }
    if (currentReverse < previousReverse) {
      processReverseMetrostroi(LOWER);
      previousReverse = currentReverse;
    }
    delay(50);
  }
}
