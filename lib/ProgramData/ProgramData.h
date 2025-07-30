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

inline const char* errorToString(Error error_enum) {
    switch (error_enum) {
        case Error::NO_ERROR:               return "NONE";
        case Error::RTC_INIT_FAILED:        return "RTC_INIT_FAIL";
        case Error::RTC_STOPPED:            return "RTC_STOPPED";
        case Error::CHANGE_TIMEOUT:         return "CHANGE_TIMEOUT";
        default:     return "UNDEFINED";
    }
}
