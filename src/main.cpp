#include <Arduino.h>
#include <Adafruit_SSD1306.h>

// Predefined Classes / Structs
#include "AppRegistry.h"
#include "ProgramData.h"
#include "Encoder.h"
#include "WaterSoftener.h"

// List of Included Apps
#include "MenuApp.h"
#include "CountdownApp.h"
#include "DiagnosticApp.h"

// Shared Objects
AppRegistry AppHub;
Adafruit_SSD1306 display(128, 32, &Wire);
eepromData programData;
Error errorStatus = Error::NO_ERROR;
RTC_DS1307 rtc;

// Custom Objects
Encoder encoder(3, 4, 2);
WaterSoftener softener(display, programData);

// App Setup
MenuApp _menuApp(display, programData, mainMenu, mainMenuSize);
CountdownApp _countdownApp(display, programData, softener);
DiagnosticApp _diagnosticApp(display, programData);

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Attach Apps to AppHub
  AppHub.menuApp = &_menuApp;
  AppHub.countdownApp = &_countdownApp;
  AppHub.diagnosticApp = &_diagnosticApp;
  AppHub.swapTo(AppHub.countdownApp); 

  // Initialize the OLED display 
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  // Start Softener
  softener.begin();

  // Start Encoder
  encoder.begin();

  // Start Real Time Clock and Check for Errors
  if (!rtc.begin()) errorStatus = Error::RTC_INIT_FAILED;
  else if (!rtc.isrunning()) errorStatus = Error::RTC_STOPPED;

  // Load ProgramData
  programData.load();

  // Render the initial menu
  AppHub.currentApp->render();
}

void loop() {
  DateTime clock = rtc.now();
  bool pressed = encoder.readButton();
  int dir = encoder.readEncoder();

  // Update AppHub with Inputs
  AppHub.update(pressed, dir);

  // Update Water Softener
  if (errorStatus == Error::NO_ERROR){
    softener.update(clock);
  }
}

