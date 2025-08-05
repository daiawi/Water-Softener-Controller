#include "BaseApp.h"

void BaseApp::wake(){
    display.ssd1306_command(SSD1306_DISPLAYON);
}

void BaseApp::sleep(){
    display.ssd1306_command(SSD1306_DISPLAYOFF);
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