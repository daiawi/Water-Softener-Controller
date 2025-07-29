#include <Arduino.h>
#include <Adafruit_SSD1306.h>

// Predefined Classes / Structs
#include "AppRegistry.h"
#include "ProgramData.h"
#include "Encoder.h"
#include "WaterSoftener.h"

// List of Included Apps
#include "BaseApp.h"
#include "MenuApp.h"
#include "CountdownApp.h"

// Shared Objects
AppRegistry AppHub;
Adafruit_SSD1306 display(128, 32, &Wire);
eepromData programData;
RTC_DS1307 rtc;

// Custom Objects
Encoder encoder(3, 4, 2);
WaterSoftener softener(display, programData);

// App Setup
MenuApp _menuApp(display, programData, mainMenu, mainMenuSize);
CountdownApp _countdownApp(display, programData, softener);

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Attach Apps to AppHub
  AppHub.menuApp = &_menuApp;
  AppHub.countdownApp = &_countdownApp;
  AppHub.swapTo(AppHub.countdownApp); 

  // Initialize the OLED display 
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Using SSD1306_SWITCHCAPVCC
    Serial.println("SSD1306 allocation failed"); // non fatal error
  }

  // Start Encoder
  encoder.begin();

  // Start Real Time Clock
  rtc.begin();

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
  softener.update(clock);
}

