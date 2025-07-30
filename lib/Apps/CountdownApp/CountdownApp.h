#pragma once
#include "BaseApp.h"
#include "WaterSoftener.h"

class CountdownApp : public BaseApp{
    public:
        CountdownApp(Adafruit_SSD1306& disp, eepromData& state, WaterSoftener& soft)
        : BaseApp(disp,state), softener(soft) {}
        
        void init() override;
        void render() override;
        void onRotate(int dir) override;
        void onPress() override;
    
    private:
        void showCountdown();
        void showError();
        WaterSoftener& softener;

};
