#include "MenuApp.h"

// Constructor definition
MenuApp::MenuApp(Adafruit_SSD1306& disp, eepromData& state, MenuItem* main, int count)
    : BaseApp(disp, state) {
    // Initialize your members here
    mainMenu = main;
    mainMenuSize = count;
    currentMenuLevel = mainMenu;
    currentSelection = 0;
    currentMenuSize = count;
}


void MenuApp::init(){
    currentMenuLevel = mainMenu;
    currentMenuSize = mainMenuSize;
    currentSelection = 0; 
}

void MenuApp::render() {
    constexpr int itemsPerPage = 3;
    constexpr int lineHeight = 10;
    constexpr uint8_t downArrowChar = 25;

    display.clearDisplay();
    display.setTextSize(1);  // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);
     
    // Calculate start index for rendering based on the current selection
    int startIndex = (currentSelection / itemsPerPage) * itemsPerPage;
    int endIndex = startIndex + itemsPerPage;

    // Ensure endIndex does not exceed the number of items
    if (endIndex > currentMenuSize) {
        endIndex = currentMenuSize;
    }

    // Render the menu items in the window
    for (int i = startIndex; i < endIndex; ++i) {
        if (i == currentSelection) {
            display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);  // Highlight selected menu item
        } else {
            display.setTextColor(SSD1306_WHITE);  // Normal display for other items
        }

        display.setCursor(0, (i - startIndex) * lineHeight);  // Move cursor to the correct line (8 pixels height per line)
        display.println(currentMenuLevel[i].getLabel());
    }

    if (endIndex<currentMenuSize){
        display.setTextColor(SSD1306_WHITE);  // Normal display for other items
        display.setCursor(120, 22); // Set cursor to the bottom-right position
        display.write(downArrowChar);  // Decimal code for the down arrow
    }

    display.display();  // Display the updated menu
}

void MenuApp::onRotate(int dir){
    currentSelection += dir;

    // Constrain Current Selection to Bounds
    if (currentSelection > currentMenuSize - 1) {
        currentSelection = currentMenuSize - 1;
    } else if (currentSelection < 0) {
        currentSelection = 0;
    }
}

void MenuApp::onPress() {
    const MenuItem& item = currentMenuLevel[currentSelection];

    // If submenu exists → enter submenu
    if (item.isSubMenu()) {
        currentMenuLevel = item.getSubMenu();
        currentMenuSize = item.getSubMenuCount();
        currentSelection = 0;
        return;
    }

    // Otherwise, must be action, so execute (safe for nullptr)
    item.execute();

    // Return to main menu if we're not already there
    if (currentMenuLevel != mainMenu) {
        init();
    }
}

