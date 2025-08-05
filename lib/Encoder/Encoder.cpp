#include "Encoder.h"

Encoder* Encoder::_instance = nullptr;

Encoder::Encoder(uint8_t pinDT, uint8_t pinCLK, uint8_t pinBTN)
    : _pinDT(pinDT), _pinCLK(pinCLK), _pinBTN(pinBTN),
      _lastState(0), _direction(0), _rotated(false),
      _stateChangeCounter(0), _buttonFlag(false), lastBtn(0)
{
    _instance = this;
}

Encoder* Encoder::getInstance() {
    return _instance;
}

PinMap Encoder::getPinMap(uint8_t pin) {
    PinMap map;

    if (pin >= 8 && pin <= 13) {         // PORTB
        map.pinReg   = &PINB;
        map.pcmskReg = &PCMSK0;
        map.mask     = 1 << (pin - 8);
        map.pcicrBit = PCIE0;
    } else if (pin >= A0 && pin <= A5) { // PORTC
        map.pinReg   = &PINC;
        map.pcmskReg = &PCMSK1;
        map.mask     = 1 << (pin - A0);
        map.pcicrBit = PCIE1;
    } else {                             // PORTD (D0–D7)
        map.pinReg   = &PIND;
        map.pcmskReg = &PCMSK2;
        map.mask     = 1 << pin;
        map.pcicrBit = PCIE2;
    }

    return map;
}

void Encoder::begin() {
    pinMode(_pinDT, INPUT_PULLUP);
    pinMode(_pinCLK, INPUT_PULLUP);
    pinMode(_pinBTN, INPUT_PULLUP);

    _dtMap  = getPinMap(_pinDT);
    _clkMap = getPinMap(_pinCLK);

    // Enable PCINT groups and pins
    PCICR |= (1 << _dtMap.pcicrBit);
    *_dtMap.pcmskReg |= _dtMap.mask;

    if (_clkMap.pcicrBit != _dtMap.pcicrBit) {
        PCICR |= (1 << _clkMap.pcicrBit);
    }
    *_clkMap.pcmskReg |= _clkMap.mask;

    // Setup external interrupt for button if on D2 (INT0)
    if (_pinBTN == 2) {
        EICRA |= _BV(ISC01); // Falling edge triggers
        EIMSK |= _BV(INT0);
    } else {
        // If button on PCINT pin, enable that pin interrupt
        PinMap btnMap = getPinMap(_pinBTN);
        PCICR |= (1 << btnMap.pcicrBit);
        *btnMap.pcmskReg |= btnMap.mask;
    }

    _lastState =
        ((*_dtMap.pinReg & _dtMap.mask) ? 2 : 0) |
        ((*_clkMap.pinReg & _clkMap.mask) ? 1 : 0);
}

void Encoder::updateEncoder() {
    uint8_t currentState =
        ((*_dtMap.pinReg & _dtMap.mask) ? 2 : 0) |
        ((*_clkMap.pinReg & _clkMap.mask) ? 1 : 0);

    if (currentState != _lastState) {
        _stateChangeCounter++;
        if (currentState == 0b11) {
            if (_stateChangeCounter >= 4) {
                _direction = (_lastState == 0b10) ? -1 :
                             (_lastState == 0b01) ?  1 : _direction;
                _rotated = true;
            }
            _stateChangeCounter = 0;
        }
    }
    _lastState = currentState;
}

void Encoder::updateButton() {
    unsigned long now = millis();
    if (now - lastBtn > 250) {
        _buttonFlag = true;
        lastBtn = now;
    }
}

int8_t Encoder::readEncoder() {
    if (_rotated) {
        _rotated = false;
        return _direction;
    }
    return 0;
}

bool Encoder::readButton() {
    if (_buttonFlag) {
        _buttonFlag = false;
        return true;
    }
    return false;
}

// Global ISR functions
#define ISR_DEBOUNCE 1000
volatile unsigned long lastPCINT0;
ISR(PCINT0_vect) {
    unsigned long now = micros();
    if (now - lastPCINT0 > ISR_DEBOUNCE){
        if (Encoder::getInstance()) Encoder::getInstance()->updateEncoder();
        lastPCINT0 = now;
    }
}

volatile unsigned long lastPCINT1;
ISR(PCINT1_vect) {
    unsigned long now = micros();
    if (now - lastPCINT1 > ISR_DEBOUNCE){
        if (Encoder::getInstance()) Encoder::getInstance()->updateEncoder();
        lastPCINT1 = now;
    }
}

volatile unsigned long lastPCINT2;
ISR(PCINT2_vect) {
    unsigned long now = micros();
    if (now - lastPCINT2 > ISR_DEBOUNCE){
        if (Encoder::getInstance()) Encoder::getInstance()->updateEncoder();
        lastPCINT2 = now;
    }
}

volatile unsigned long lastINT0;
ISR(INT0_vect) {
    unsigned long now = micros();
    if (now - lastINT0 > ISR_DEBOUNCE){
        if (Encoder::getInstance()) Encoder::getInstance()->updateButton();
        lastINT0 = now;
    }
}
