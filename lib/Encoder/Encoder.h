#pragma once
#include "Arduino.h"
#include "PinChangeInterrupt.h"

class Encoder {
public:
    Encoder(uint8_t pinDT, uint8_t pinCLK, uint8_t pinBTN);

    void begin();
    int8_t readEncoder();     // -1, 0, or 1
    bool readButton();     // returns true once per press

    // Static ISR wrappers
    static void handleInterrupt();      // for rotation
    static void handleButtonInterrupt(); // for button

private:
    // Instance-specific data
    uint8_t _pinDT, _pinCLK, _pinBTN;
    uint8_t _lastState;
    
    // Rotation Trackers
    volatile int8_t _direction;
    volatile bool _rotated;
    volatile uint8_t _stateChangeCounter;

    // Button state
    volatile bool _buttonFlag;
    unsigned long lastBtn;

    // Internal Methods
    void updateEncoder();           // called from DT/CLK ISR
    void updateButton();            // called from sw ISR

    // Single instance pointer
    static Encoder* _instance;
};
