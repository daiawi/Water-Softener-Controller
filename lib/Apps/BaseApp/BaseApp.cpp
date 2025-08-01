#include "BaseApp.h"

void BaseApp::sleep(){
    display.clearDisplay();
    display.display();
}

void BaseApp::drawScreen(){
    display.clearDisplay();

    // Default Configuration
    display.setTextSize(1);    // Larger text size for time
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);  // Set cursor position for time

    // Call Render Function from Application
    render();

    // Draw Rendered Screen
    display.display();
}