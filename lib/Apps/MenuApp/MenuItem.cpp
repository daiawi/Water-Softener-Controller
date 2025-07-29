#include "MenuItem.h"

// Constructor for action items
MenuItem::MenuItem(const char* label, MenuFunction func)
    : label(label), func(func), subMenuItems(nullptr), subMenuCount(0) {}

// Constructor for sub-menu items
MenuItem::MenuItem(const char* label, MenuItem* subMenuItems, int subMenuCount)
    : label(label), func(nullptr), subMenuItems(subMenuItems), subMenuCount(subMenuCount) {}

// Action Methods
const char* MenuItem::getLabel() const {
    return label;
}

void MenuItem::execute() const {
    if (func) func();
}

MenuItem* MenuItem::getSubMenu() const {
    return subMenuItems;
}

int MenuItem::getSubMenuCount() const {
    return subMenuCount;
}

// Identification Methods
bool MenuItem::isAction() const {
    return func != nullptr;
}

bool MenuItem::isSubMenu() const {
    return subMenuItems != nullptr;
}
