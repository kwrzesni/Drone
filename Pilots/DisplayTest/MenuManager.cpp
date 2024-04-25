#include "MenuManager.h"
#include "TextMenuItem.h"
#include "TextValueMenuItem.h"
#include "TextColorMenuItem.h"
#include "TextUnsignedIntMenuItem.h"
#include "TextBoolMenuItem.h"
#include "ChangeMenuAction.h"
#include "ChangeColorAction.h"
#include "UpdateBoolAction.h"

MenuManager::MenuManager(ILI9488 tft)
  : tft{tft}
  , mainManu{*this, "Settings"}
  , formatingMenu{*this, "Formating"}
  , colorMenu{*this, "Colors"}
  , changeUnsignedIntMenu{*this}
  , changeIntMenu{*this}
  , changeFloatMenu{*this}
{
  mainManu.assignMenuItems({
    new TextMenuItem(mainManu, nullptr, "A"), 
    new TextMenuItem(mainManu, nullptr, "B"), 
    new TextMenuItem(mainManu, nullptr, "C"),
    new TextMenuItem(mainManu, new ChangeMenuAction(formatingMenu, *this), "Formating")});

  formatingMenu.assignMenuItems({
    new TextColorMenuItem(formatingMenu, new ChangeMenuAction(colorMenu, *this, 0), "background color: ", menuFormatSettings.backgroundColor),
    new TextColorMenuItem(formatingMenu, new ChangeMenuAction(colorMenu, *this, 1), "header background color: ", menuFormatSettings.headerBackgroundColor),
    new TextColorMenuItem(formatingMenu, new ChangeMenuAction(colorMenu, *this, 2), "header text color: ", menuFormatSettings.headerTextColor),
    new TextColorMenuItem(formatingMenu, new ChangeMenuAction(colorMenu, *this, 3), "header border color: ", menuFormatSettings.headerBorderColor),
    new TextColorMenuItem(formatingMenu, new ChangeMenuAction(colorMenu, *this, 4), "item background color: ", menuFormatSettings.itemBackgroundColor),
    new TextColorMenuItem(formatingMenu, new ChangeMenuAction(colorMenu, *this, 5), "item border selection color: ", menuFormatSettings.itemBorderSelectionColor),
    new TextColorMenuItem(formatingMenu, new ChangeMenuAction(colorMenu, *this, 6), "item text color: ", menuFormatSettings.itemTextColor),
    new TextColorMenuItem(formatingMenu, new ChangeMenuAction(colorMenu, *this, 7), "item text selection color: ", menuFormatSettings.itemTextSelectionColor),
    new TextColorMenuItem(formatingMenu, new ChangeMenuAction(colorMenu, *this, 8), "item border color: ", menuFormatSettings.itemBorderColor),
    new TextUnsignedIntMenuItem(formatingMenu, new ChangeMenuAction(changeUnsignedIntMenu, *this, 9), "header height: ", menuFormatSettings.headerHeight, 0, 100),
    new TextUnsignedIntMenuItem(formatingMenu, new ChangeMenuAction(changeUnsignedIntMenu, *this, 10), "header border size: ", menuFormatSettings.headerBorderSize, 0, 10),
    new TextUnsignedIntMenuItem(formatingMenu, new ChangeMenuAction(changeUnsignedIntMenu, *this, 11), "header X text size: ", menuFormatSettings.headerTextSizeX, 1, 20),
    new TextUnsignedIntMenuItem(formatingMenu, new ChangeMenuAction(changeUnsignedIntMenu, *this, 12), "header Y text size: ", menuFormatSettings.headerTextSizeY, 1, 20),
    new TextUnsignedIntMenuItem(formatingMenu, new ChangeMenuAction(changeUnsignedIntMenu, *this, 13), "header border size: ", menuFormatSettings.itemBorderSize, 0, 10),
    new TextUnsignedIntMenuItem(formatingMenu, new ChangeMenuAction(changeUnsignedIntMenu, *this, 14), "item X text size: ", menuFormatSettings.itemTextSizeX, 1, 10),
    new TextUnsignedIntMenuItem(formatingMenu, new ChangeMenuAction(changeUnsignedIntMenu, *this, 15), "item Y text size: ", menuFormatSettings.itemTextSizeY, 1, 10),
    new TextUnsignedIntMenuItem(formatingMenu, new ChangeMenuAction(changeUnsignedIntMenu, *this, 16), "header padding: ", menuFormatSettings.headerPadding, 0, 200),
    new TextUnsignedIntMenuItem(formatingMenu, new ChangeMenuAction(changeUnsignedIntMenu, *this, 17), "items X padding: ", menuFormatSettings.itemTextSizeX, 0, 20),
    new TextUnsignedIntMenuItem(formatingMenu, new ChangeMenuAction(changeUnsignedIntMenu, *this, 18), "items Y padding: ", menuFormatSettings.itemTextSizeY, 0, 20),
    new TextBoolMenuItem(formatingMenu, new UpdateBoolAction(*this), "is multiline: ", menuFormatSettings.isMultiLine)});

  colorMenu.assignMenuItems({
    new TextMenuItem(colorMenu, new ChangeColorAction(*this), colorToString(Color::black)),
    new TextMenuItem(colorMenu, new ChangeColorAction(*this), colorToString(Color::navy)),
    new TextMenuItem(colorMenu, new ChangeColorAction(*this), colorToString(Color::darkGreen)),
    new TextMenuItem(colorMenu, new ChangeColorAction(*this), colorToString(Color::darkCyan)),
    new TextMenuItem(colorMenu, new ChangeColorAction(*this), colorToString(Color::maroon)),
    new TextMenuItem(colorMenu, new ChangeColorAction(*this), colorToString(Color::purple)),
    new TextMenuItem(colorMenu, new ChangeColorAction(*this), colorToString(Color::olive)),
    new TextMenuItem(colorMenu, new ChangeColorAction(*this), colorToString(Color::lightGrey)),
    new TextMenuItem(colorMenu, new ChangeColorAction(*this), colorToString(Color::darkGrey)),
    new TextMenuItem(colorMenu, new ChangeColorAction(*this), colorToString(Color::blue)),
    new TextMenuItem(colorMenu, new ChangeColorAction(*this), colorToString(Color::green)),
    new TextMenuItem(colorMenu, new ChangeColorAction(*this), colorToString(Color::cyan)),
    new TextMenuItem(colorMenu, new ChangeColorAction(*this), colorToString(Color::red)),
    new TextMenuItem(colorMenu, new ChangeColorAction(*this), colorToString(Color::magenta)),
    new TextMenuItem(colorMenu, new ChangeColorAction(*this), colorToString(Color::yellow)),
    new TextMenuItem(colorMenu, new ChangeColorAction(*this), colorToString(Color::white)),
    new TextMenuItem(colorMenu, new ChangeColorAction(*this), colorToString(Color::orange)),
    new TextMenuItem(colorMenu, new ChangeColorAction(*this), colorToString(Color::greenYellow))});
};

void MenuManager::begin()
{
  tft.begin();
  tft.setRotation(1);

  currentMenu->draw();
}

void MenuManager::update(MenuControlCommand menuControlCommand)
{
  currentMenu->update(menuControlCommand);
}

void MenuManager::changeMenu(IMenu& targetMenu, bool selectionResetRequired)
{
  currentMenu = &targetMenu;
  if (selectionResetRequired)
  {
    currentMenu->resetSelection();
  }
  currentMenu->draw();
}