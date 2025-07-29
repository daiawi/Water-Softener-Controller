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
    bool changed = false;

    // Handle input events
    if (pressed) {
        currentApp->onPress();
        changed = true;
    } else if (dir) {
        currentApp->onRotate(dir);
        changed = true;
    }

    // Check continuous render
    if (currentApp->continuousRender) {
        unsigned long now = millis();
        if (now - lastRenderTime >= currentApp->renderPeriod) {
            currentApp->render();
            lastRenderTime = now;
            return;
        }
    }

    // Render if user input changed something
    if (changed) {
        currentApp->render();
    }
}
