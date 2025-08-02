#pragma once
#include <Arduino.h>

struct PinMap {
    volatile uint8_t* pinReg;   // PINx register
    volatile uint8_t* pcmskReg; // PCMSKx register
    uint8_t mask;               // Bit mask for pin
    uint8_t pcicrBit;           // Bit for PCICR register
};

class Encoder {
public:
    Encoder(uint8_t pinDT, uint8_t pinCLK, uint8_t pinBTN);

    void begin();

    int8_t readEncoder();
    bool readButton();

    // Public so ISRs can call them
    void updateEncoder();
    void updateButton();

    // Accessor for the static instance pointer
    static Encoder* getInstance();

private:
    static Encoder* _instance;

    uint8_t _pinDT, _pinCLK, _pinBTN;
    PinMap _dtMap, _clkMap;

    uint8_t _lastState;
    int8_t _direction;
    bool _rotated;
    uint8_t _stateChangeCounter;
    bool _buttonFlag;
    unsigned long lastBtn;

    static PinMap getPinMap(uint8_t pin);
};
