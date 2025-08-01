// EncoderApp.h - Template for All SubApps
#pragma once
#include <Adafruit_SSD1306.h>
#include "ProgramData.h"

class BaseApp {
    protected:
        Adafruit_SSD1306& display;
        eepromData& programData;
        
    public:
        BaseApp(Adafruit_SSD1306& disp, eepromData& state)
        : display(disp), programData(state) {}
        
        virtual void init() {}                                  // Optional setup
        virtual void render() = 0;                              // Required
        virtual void onRotate(int dir) = 0;                     // Required
        virtual void onPress() = 0;                             // Required
        virtual ~BaseApp() {}                                   // Ensure proper cleanup
        
        bool continuousRender = false; 
        unsigned long renderPeriod = 1000;
        void sleep();      
        void drawScreen();  
};