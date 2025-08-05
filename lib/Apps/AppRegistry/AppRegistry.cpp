#include "AppRegistry.h"

void AppRegistry::swapTo(BaseApp* newApp){
    if (newApp){
        currentApp = newApp;
        currentApp->init();  // call setup of new app
    }
}

void AppRegistry::returnToMenu(){
    if (menuApp){
        currentApp = menuApp;
    }
}

void AppRegistry::update(bool pressed, int dir) {
    // Handle User Input First
    if (pressed || dir != 0) {
        lastInteraction = millis();

        if (isSleeping) {
            isSleeping = false;
            currentApp->wake();
            currentApp->drawScreen();
            return;
        }

        if (pressed) currentApp->onPress();
        else currentApp->onRotate(dir);

        currentApp->drawScreen();
        return;
    }

    // Check for Sleep Condition
    if (!isSleeping && (millis() - lastInteraction >= sleep_time)) {
        isSleeping = true;
        swapTo(countdownApp);
        currentApp->sleep();
        return;
    }

    // Check for Continous Render
    if (!isSleeping && currentApp->continuousRender) {
        unsigned long now = millis();
        if (now - lastRenderTime >= currentApp->renderPeriod) {
            lastRenderTime = now;
            currentApp->drawScreen();
        }
    }
}

