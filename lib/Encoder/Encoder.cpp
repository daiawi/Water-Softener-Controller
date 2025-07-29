#include "Encoder.h"

Encoder* Encoder::_instance = nullptr;

Encoder::Encoder(uint8_t pinDT, uint8_t pinCLK, uint8_t pinBTN)
    : _pinDT(pinDT), _pinCLK(pinCLK), _pinBTN(pinBTN),
      _lastState(0), _direction(0), _rotated(false), _stateChangeCounter(0),
      _buttonFlag(false)
{
    _instance = this;
}

void Encoder::begin() {
    pinMode(_pinDT, INPUT_PULLUP);
    pinMode(_pinCLK, INPUT_PULLUP);
    pinMode(_pinBTN, INPUT_PULLUP);

    _lastState = (digitalRead(_pinDT) << 1) | digitalRead(_pinCLK);

    attachPCINT(digitalPinToPCINT(_pinDT), Encoder::handleInterrupt, CHANGE);
    attachPCINT(digitalPinToPCINT(_pinCLK), Encoder::handleInterrupt, CHANGE);
    attachInterrupt(digitalPinToInterrupt(_pinBTN), Encoder::handleButtonInterrupt, FALLING);
}

void Encoder::updateEncoder() {
    uint8_t currentState = (digitalRead(_pinDT) << 1) | digitalRead(_pinCLK);

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
    if (now - lastBtn > 250){
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

// Static ISR for rotation
void Encoder::handleInterrupt() {
    if (_instance) _instance->updateEncoder();
}

// Static ISR for button
void Encoder::handleButtonInterrupt() {
    if (_instance) _instance->updateButton();
}
