#pragma once
#include "BaseApp.h"
#include "RTClib.h"

class RtcApp : public BaseApp{
    public:
        RtcApp(Adafruit_SSD1306& disp, eepromData& state, RTC_DS1307& RTC)
        : BaseApp(disp,state), rtc(RTC){}
        
        void init() override;
        void render() override;
        void onRotate(int dir) override;
        void onPress() override;

        // DateTime
        DateTime inputDateTime;

        // State Definition
        enum sequenceState {DATE_INPUT, TiME_INPUT, FINISHED};
        sequenceState state;

    private:
        RTC_DS1307& rtc;
        uint8_t index;

        uint8_t date_input[3];
        uint8_t time_input[3];

        // Date Input
        void processDatePress();
        void processDateRender();
        void wrapDateInput(uint8_t date[3]);

        // Time Input
        void processTimePress();
        void processTimeRender();
        void wrapTimeInput(uint8_t time[3]);

        // Load RTC Data
        void loadDateTime();
        void setDateTime();
        void adjustDateTime(int dir);


};
