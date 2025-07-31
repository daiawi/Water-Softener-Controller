#include "AppRegistry.h"
#include "RtcApp.h"

extern AppRegistry AppHub;
extern Error errorStatus;

// TODO: Init Inputs with RTC Current Time/Date
void RtcApp::init(){
    state = sequenceState::DATE_INPUT;
    index = 0;

    // Writes Input Buffers
    loadDateTime();
}

void RtcApp::render() {
    switch (state){
        case sequenceState::DATE_INPUT: processDateRender(); break;
        case sequenceState::TiME_INPUT: processTimeRender(); break;
        case sequenceState::FINISHED: return;
    }
}

void RtcApp::onRotate(int dir){
    if (state!=sequenceState::FINISHED) {adjustDateTime(dir);}
}

void RtcApp::onPress(){
    switch (state){
        case sequenceState::DATE_INPUT: processDatePress(); break;
        case sequenceState::TiME_INPUT: processTimePress(); break;
        case sequenceState::FINISHED: AppHub.returnToMenu(); break;
    }
}

void RtcApp::processDatePress(){
    if (index < 2) {index++;}
    else{state = sequenceState::TiME_INPUT; index=0;}
}

void RtcApp::processDateRender(){
    display.clearDisplay();
    display.setTextSize(1);    // Larger text size for time
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);  // Set cursor position for time
    display.print("Date Input:");

    // Print Countdown
    display.setCursor(0, 10);  // Set cursor position for time
    display.setTextSize(2);    // Larger text size for time

    char DateString[10]; 
    sprintf(DateString, "%02d/%02d/%02d", inputDateTime.month(), inputDateTime.day(), inputDateTime.year()-2000);

    display.print(DateString);

    // Update the display
    display.display();
}

void RtcApp::processTimePress(){
    if (index < 2) {index++;}
    else{
        setDateTime();
        AppHub.returnToMenu();
    }
}

void RtcApp::processTimeRender(){
    display.clearDisplay();
    display.setTextSize(1);    // Larger text size for time
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);  // Set cursor position for time
    display.print("Time Input:");

    // Print Countdown
    display.setCursor(0, 10);  // Set cursor position for time
    display.setTextSize(2);    // Larger text size for time

    char timeString[10]; 
    sprintf(timeString, "%02d:%02d:%02d", inputDateTime.hour(), inputDateTime.minute(), inputDateTime.second());

    display.print(timeString);

    // Update the display
    display.display();
}


void RtcApp::loadDateTime(){
    inputDateTime = rtc.now();
}

void RtcApp::setDateTime(){
    if (inputDateTime.isValid()){
        DateTime currentDateTime = rtc.now();
        TimeSpan diff = inputDateTime - currentDateTime;
        
        programData.nextEvent = programData.nextEvent + diff;
        programData.save();

        rtc.adjust(inputDateTime);
    }
}

void RtcApp::adjustDateTime(int dir){
    // Update Date Input Arrays
    date_input[0] = inputDateTime.month();
    date_input[1] = inputDateTime.day();
    date_input[2] = inputDateTime.year() - 2000;

    // Update Time Input Arrays
    time_input[0] = inputDateTime.hour();
    time_input[1] = inputDateTime.minute();
    time_input[2] = inputDateTime.second();

    switch (state) {
        case sequenceState::DATE_INPUT:
            date_input[index] += dir;
            wrapDateInput(date_input);
            break;

        case sequenceState::TiME_INPUT:
            time_input[index] += dir;
            wrapTimeInput(time_input);
            break;

        default:
            break;
    }

    // Normalize and update 
    inputDateTime = DateTime(
        date_input[2]+2000, 
        date_input[0], 
        date_input[1], 
        time_input[0], 
        time_input[1], 
        time_input[2]);
}

void RtcApp::wrapDateInput(uint8_t date[3]){
    // Year offset: wrap 0-99
    date[2] %= 100;  // 0 to 99

    // Month: wrap 1-12
    // Since date[1] is uint8_t, subtract 1 carefully
    uint8_t month = date[0];
    month = (month == 0) ? 12 : month; // 0 maps to 12
    month = ((month - 1) % 12) + 1;
    date[0] = month;

    // Day: wrap 1-31
    uint8_t day = date[1];
    day = (day == 0) ? 31 : day;  // 0 maps to 31
    day = ((day - 1) % 31) + 1;
    date[1] = day;
}

void RtcApp::wrapTimeInput(uint8_t time[3]){
    // Wrap hours 0-23
    time[0] %= 24;
    if (time[0] < 0) time[0] += 24;

    // Wrap minutes 0-59
    time[1] %= 60;
    if (time[1] < 0) time[1] += 60;

    // Wrap seconds 0-59
    time[2] %= 60;
    if (time[2] < 0) time[2] += 60;
}