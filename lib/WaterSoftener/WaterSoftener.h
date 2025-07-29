#pragma once
#include <RTClib.h>
#include <Adafruit_SSD1306.h>
#include <ProgramData.h>
#include "BooleanBuffer.h"

class WaterSoftener{
    public:
        WaterSoftener(Adafruit_SSD1306& disp, eepromData& data):display(disp), programData(data), swBuffer(10){}
    
        static const char* titles[5];
        static TimeSpan timeDelays[5];
        
        void update(DateTime now);      // Called To Check Naturally
        void skipWait();                // Sets Flag to Skip Wait
        TimeSpan getCountdown();
        DateTime getCurrentTime();
        DateTime roundEventTo2AM(DateTime event);

    private:
        // Board Config
        const int pinOpto = 8;
        const int pinGear = 7;

        // Flags
        bool _skipFlag = false;
        
        // Custom Classes
        Adafruit_SSD1306& display;
        eepromData& programData;
        BooleanBuffer swBuffer;

        // Timing 
        DateTime clock;
        TimeSpan countdown;

        // Changing Methods
        bool readyToChange(DateTime now);
        void advanceSoftener();
        bool hasChanged();
        void changeScreen();

        // Helper Methods
        void advanceStageAndSave();

        // Changing State
        enum sequenceState {WAIT_FOR_CLOSE, WAIT_FOR_OPEN};
        sequenceState changeState = WAIT_FOR_CLOSE;
        bool prevState = 1;
};