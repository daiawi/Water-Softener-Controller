#include "AppRegistry.h"
#include "CountdownApp.h"
#include "WaterSoftener.h"
#include "timeUtils.h"

extern AppRegistry AppHub;
extern Error errorStatus;

void CountdownApp::init(){
    continuousRender = true;
}

void CountdownApp::render() {
    if (errorStatus == Error::NO_ERROR) showCountdown();
    else showError();
}

void CountdownApp::onRotate(int dir){
    return;
}

void CountdownApp::onPress(){
    AppHub.returnToMenu();
}

void CountdownApp::showCountdown(){
    display.print(softener.titles[programData.stage]);

    // Calculate Countdown:
    TimeSpan countdown = softener.getCountdown();

    // Print Countdown
    display.setCursor(0, 10);  // Set cursor position for time
    display.setTextSize(2);    // Larger text size for time
    
    char timeString[9]; 
    formatTime(countdown, timeString);
    
    display.print(timeString);
}

void CountdownApp::showError(){
    display.setTextSize(2);    // Larger text size for time
    display.println("Error");
    display.println("Detected");
  
    // Update the display
    display.display();
}