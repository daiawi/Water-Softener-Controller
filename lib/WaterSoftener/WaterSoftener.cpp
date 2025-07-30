#include "WaterSoftener.h"

extern Error errorStatus;

const char* WaterSoftener::titles[5] = {
    "Fill:",
    "Brine Rinse:",
    "Back Wash:",
    "Rinse:",
    "Service:"
};

// Water Softner Time Delays
TimeSpan WaterSoftener::timeDelays[5] = {
  TimeSpan(0,0,8,36),       // Fill: 8 minutes 36 seconds
  TimeSpan(0,0,38,0),       // Brine Rinse: 38 minutes   
  TimeSpan(0,1,26,0),       // Back Wash: 1 Hour, 26 Minutes
  TimeSpan(0,0,15,0),       // Rinse: 15 minutes
  TimeSpan(4,0,0,0)         // Service 4 Days
};

void WaterSoftener::begin(){
    pinMode(pinOpto, OUTPUT);
    pinMode(pinGear, INPUT_PULLUP);
}

TimeSpan WaterSoftener::getCountdown(){
    return countdown;
}

DateTime WaterSoftener::getCurrentTime(){
    return clock;
}

void WaterSoftener::skipWait(){
    _skipFlag = true;
}

bool WaterSoftener::readyToChange(DateTime now){
    countdown = programData.nextEvent - now;
    return (countdown.totalseconds() <= 0 || _skipFlag);
}

void WaterSoftener::update(DateTime now){
    clock = now;
    if (readyToChange(now)) advanceSoftener();
}

void WaterSoftener::advanceSoftener(){
    if (_skipFlag){_skipFlag = false;}

    // Update Program Data 
    advanceStageAndSave();

    unsigned long startTime = millis();
    const int TIMEOUT = 3000;
    changeScreen(); // This will not return the screen back, need to find a better way.

    while (!hasChanged()){
        if (millis() - startTime > TIMEOUT){
            changeState = WAIT_FOR_CLOSE;
            errorStatus = Error::CHANGE_TIMEOUT;
            break;
        }
    }
}

void WaterSoftener::advanceStageAndSave(){
    // Update Program Data with New Stage/Event
    programData.stage = (programData.stage + 1) % 5;
    programData.nextEvent = clock + timeDelays[programData.stage];
  
    // Round Softener Stage Start to Night Time (Add Cycle Count)
    if (programData.stage == 4){
        programData.cycleCount += 1;
        programData.nextEvent = roundEventTo2AM(programData.nextEvent);
    }

    // Save New Program Data
    programData.save();
}

bool WaterSoftener::hasChanged(){    
    // Evaluate Switch state through a Buffer
    swBuffer.add(digitalRead(pinGear));
    bool state = swBuffer.isStableTrue;

    const bool OPEN = 1; 
    const bool CLOSED = 0;

    bool changed = false;
    // Change Logic
    if (state!=prevState){
        prevState = state;
        if (changeState == WAIT_FOR_CLOSE && state==CLOSED){
            changeState = WAIT_FOR_OPEN;
        }
        else if (changeState == WAIT_FOR_OPEN && state==OPEN){
            changeState = WAIT_FOR_CLOSE;
            changed = true;
        }
    }

    // Return whether finished or not
    return changed;
}

void WaterSoftener::changeScreen(){
  display.clearDisplay();
  display.setTextSize(2);    // Larger text size for time
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);  // Set cursor position for time
  display.println("Changing");
  display.println("State");
  
  // Update the display
  display.display();
}

DateTime WaterSoftener::roundEventTo2AM(DateTime event){
    // Define 2 AM and 6 AM for the day of the event
    DateTime twoAM(event.year(), event.month(), event.day(), 2, 0, 0);
    DateTime sixAM(event.year(), event.month(), event.day(), 6, 0, 0);

    // Define 2 AM for the next day
    DateTime twoAMNextDay = twoAM + TimeSpan(1, 0, 0, 0);  // Adding one day

    // If the event time is before 6 AM, round to 2 AM today
    if (event < sixAM) {
        return twoAM;
    }
    // Otherwise, round to 2 AM on the next day
    else {
        return twoAMNextDay;
    }
}