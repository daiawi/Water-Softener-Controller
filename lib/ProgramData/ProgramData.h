#pragma once
#include <RTClib.h>
#include <EEPROM.h>

// EEPROM DATA STRUCTURE
struct eepromData {
  int stage;
  DateTime nextEvent;
  int cycleCount;

  void save(){EEPROM.put(0, *this);}
  void load(){EEPROM.get(0, *this);}
};

enum class Error{
  NO_ERROR,
  RTC_INIT_FAILED,
  RTC_STOPPED,
  CHANGE_TIMEOUT
};
