#include "AppRegistry.h"
#include "ProgramData.h"
#include "WaterSoftener.h"
#include "MenuFunctions.h"
#include "MenuItem.h"

#define MENU_COUNT(x) (sizeof(x) / sizeof(MenuItem))

extern AppRegistry AppHub;
extern eepromData programData;
extern WaterSoftener softener;
extern Error errorStatus;

static void setStage(int stage) {
    programData.stage = stage;
    programData.nextEvent = softener.getCurrentTime() + softener.timeDelays[programData.stage];

    if (programData.stage == 4) {
        programData.nextEvent = softener.roundEventTo2AM(programData.nextEvent);
    }

    programData.save();
    AppHub.swapTo(AppHub.countdownApp);
}

static void showCountdown() {
    AppHub.swapTo(AppHub.countdownApp);
}

static void showDiagnostic() {
    AppHub.swapTo(AppHub.diagnosticApp);
}

static void showRtcApp(){
    AppHub.swapTo(AppHub.rtcApp);
}

static void runNow() {
    softener.skipWait();
    AppHub.swapTo(AppHub.countdownApp);
}

static void runTonight() {
    if (programData.stage == 4){
        programData.nextEvent = softener.roundEventTo2AM(softener.getCurrentTime());
        programData.save();
    }
}

static void clearError() {
    errorStatus = Error::NO_ERROR;
}

static MenuItem runOpts[] = {
    MenuItem("Run Now", runNow),
    MenuItem("Run Tonight", runTonight),
    MenuItem("Back", nullptr)
};

static MenuItem stageOpts[] = {
    MenuItem("Service", [](){setStage(4);}),
    MenuItem("Fill", [](){setStage(0);}),
    MenuItem("Brine", [](){setStage(1);}),
    MenuItem("Back Wash", [](){setStage(2);}),
    MenuItem("Rinse", []() {setStage(3);})
};

static MenuItem settingOpts[] = {
    MenuItem("Diagnostics", showDiagnostic),
    MenuItem("Clear Error", clearError),
    MenuItem("Set Stage", stageOpts, MENU_COUNT(stageOpts)),
    MenuItem("Set RTC", showRtcApp),
    MenuItem("Back", nullptr)
};

MenuItem mainMenu[] = {
    MenuItem("Display Info", showCountdown),
    MenuItem("Run Cycle", runOpts, MENU_COUNT(runOpts)),
    MenuItem("Settings", settingOpts, MENU_COUNT(settingOpts))
};

int mainMenuSize = MENU_COUNT(mainMenu);
