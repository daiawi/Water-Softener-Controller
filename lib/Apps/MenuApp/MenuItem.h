#pragma once

class Menu;

class MenuItem {
public:
    typedef void (*MenuFunction)(); // Define a type for function pointers

    // Constructor for action items
    MenuItem(const char* label, MenuFunction func);

    // Constructor for sub-menu items
    MenuItem(const char* label, MenuItem* subMenuItems, int subMenuCount);

    // Action Methods
    const char* getLabel() const;
    void execute() const;
    MenuItem* getSubMenu() const;
    int getSubMenuCount() const;

    // Identification Methods
    bool isAction() const;
    bool isSubMenu() const;

private:
    const char* label;          // Text label for the menu item
    MenuFunction func;          // Function pointer to be called (null if it's a sub-menu)
    MenuItem* subMenuItems;     // Pointer to a list of sub-menu items (null if it's an action)
    int subMenuCount;           // Number of items in the sub-menu
};

