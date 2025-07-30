#include "BaseApp.h"

// Store Null Pointers and Assign in Main
class AppRegistry{
    public:
        BaseApp* currentApp = nullptr;

        // Add Custom Apps Here ... 
        BaseApp* menuApp = nullptr;
        BaseApp* countdownApp = nullptr;
        BaseApp* diagnosticApp = nullptr;

        void update(bool pressed, int dir);
        void swapTo(BaseApp* newApp);
        void returnToMenu();

    // Timing
    private:
        bool isSleeping = false;
        unsigned long sleep_time = 3000;
        unsigned long lastInteraction = 0;
        unsigned long lastRenderTime = 0;
};

