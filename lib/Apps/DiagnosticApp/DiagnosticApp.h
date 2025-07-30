#pragma once
#include "BaseApp.h"

class DiagnosticApp : public BaseApp{
    public:
        DiagnosticApp(Adafruit_SSD1306& disp, eepromData& state)
        : BaseApp(disp,state){}
        
        void init() override;
        void render() override;
        void onRotate(int dir) override;
        void onPress() override;

    private:
        const int pinGear = 7;
};
