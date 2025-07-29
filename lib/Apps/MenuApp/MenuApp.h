#pragma once
#include "BaseApp.h"
#include "MenuItem.h"
#include "MenuFunctions.h"

class MenuApp : public BaseApp {
public:
    MenuApp(Adafruit_SSD1306& disp, eepromData& state, MenuItem* main, int count);
    void init() override;
    void render() override;
    void onRotate(int dir) override;
    void onPress() override;

private:
    // ---- Menu States ----
    const MenuItem* currentMenuLevel;
    const MenuItem* mainMenu;
    int currentSelection;
    int currentMenuSize;
    int mainMenuSize;

    // ---- Menu Methods ----
    void performCurrentAction();
    const char* labelAt(int index);
    void reset();

    // Stored Menus
    static MenuItem mainMenuItems[3];
    static MenuItem runOpts[3];
    static MenuItem settingOpts[4];
    static MenuItem stageOpts[5];
};
