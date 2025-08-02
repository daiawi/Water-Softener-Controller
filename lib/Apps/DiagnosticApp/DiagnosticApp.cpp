#include "AppRegistry.h"
#include "DiagnosticApp.h"
#include "RTClib.h"
#include "timeUtils.h"

extern AppRegistry AppHub;
extern Error errorStatus;
extern RTC_DS1307 rtc;

void DiagnosticApp::init(){
    continuousRender = true;
}

void DiagnosticApp::render() {
    // Get Current Time
    DateTime clock = rtc.now();

    // Display RTC Time
    char timeString[9]; 
    formatTime(clock, timeString);
    display.print("Time: ");
    display.println(timeString);

    // Display Error State
    display.print("Error: ");
    const char* errStr = errorToString(errorStatus);
    display.println(errStr);

    // Display Current Cycle
    display.print("Cycle: ");
    display.println(programData.cycleCount);

    // Display Switch State
    display.print("Switch: ");
    const char* switch_status = digitalRead(pinGear) ? "Open" : "Closed";
    display.println(switch_status);
}

void DiagnosticApp::onRotate(int dir){
    return;
}

void DiagnosticApp::onPress(){
    AppHub.returnToMenu();
}
