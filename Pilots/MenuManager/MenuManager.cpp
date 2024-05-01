#include "MenuManager.h"
#include "TextMenuItem.h"
#include "TextColorMenuItem.h"
#include "TextUnsignedIntMenuItem.h"
#include "TextBoolMenuItem.h"
#include "Action.h"

bool isMenuLayoutParameter(const MenuManager& menuManager, const void* value)
{
  return value >= (void*)&menuManager.menuFormatSettings.headerHeight && value <= (void*)&menuManager.menuFormatSettings.multilineItemsText;
}

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
    new TextMenuItem({ActionType::none}, "A"), 
    new TextMenuItem({ActionType::none}, "B"), 
    new TextMenuItem({ActionType::none}, "C"),
    new TextMenuItem({ActionType::changeMenu, (void*)&formatingMenu}, "Formating")});

  formatingMenu.assignMenuItems({
    new TextColorMenuItem({ActionType::changeMenu, (void*)&colorMenu}, "background color: ", menuFormatSettings.backgroundColor),
    new TextColorMenuItem({ActionType::changeMenu, (void*)&colorMenu}, "header background color: ", menuFormatSettings.headerBackgroundColor),
    new TextColorMenuItem({ActionType::changeMenu, (void*)&colorMenu}, "header text color: ", menuFormatSettings.headerTextColor),
    new TextColorMenuItem({ActionType::changeMenu, (void*)&colorMenu}, "header border color: ", menuFormatSettings.headerBorderColor),
    new TextColorMenuItem({ActionType::changeMenu, (void*)&colorMenu}, "item background color: ", menuFormatSettings.itemBackgroundColor),
    new TextColorMenuItem({ActionType::changeMenu, (void*)&colorMenu}, "item border selection color: ", menuFormatSettings.itemBorderSelectionColor),
    new TextColorMenuItem({ActionType::changeMenu, (void*)&colorMenu}, "item text color: ", menuFormatSettings.itemTextColor),
    new TextColorMenuItem({ActionType::changeMenu, (void*)&colorMenu}, "item text selection color: ", menuFormatSettings.itemTextSelectionColor),
    new TextColorMenuItem({ActionType::changeMenu, (void*)&colorMenu}, "item border color: ", menuFormatSettings.itemBorderColor),
    new TextUnsignedIntMenuItem({ActionType::changeMenu, (void*)&changeUnsignedIntMenu}, "header height: ", menuFormatSettings.headerHeight, 0, 100),
    new TextUnsignedIntMenuItem({ActionType::changeMenu, (void*)&changeUnsignedIntMenu}, "header border size: ", menuFormatSettings.headerBorderSize, 0, 10),
    new TextUnsignedIntMenuItem({ActionType::changeMenu, (void*)&changeUnsignedIntMenu}, "header X text size: ", menuFormatSettings.headerTextSizeX, 1, 20),
    new TextUnsignedIntMenuItem({ActionType::changeMenu, (void*)&changeUnsignedIntMenu}, "header Y text size: ", menuFormatSettings.headerTextSizeY, 1, 20),
    new TextUnsignedIntMenuItem({ActionType::changeMenu, (void*)&changeUnsignedIntMenu}, "header border size: ", menuFormatSettings.itemBorderSize, 0, 10),
    new TextUnsignedIntMenuItem({ActionType::changeMenu, (void*)&changeUnsignedIntMenu}, "item X text size: ", menuFormatSettings.itemTextSizeX, 1, 10),
    new TextUnsignedIntMenuItem({ActionType::changeMenu, (void*)&changeUnsignedIntMenu}, "item Y text size: ", menuFormatSettings.itemTextSizeY, 1, 10),
    new TextUnsignedIntMenuItem({ActionType::changeMenu, (void*)&changeUnsignedIntMenu}, "header padding: ", menuFormatSettings.headerPadding, 0, 200),
    new TextUnsignedIntMenuItem({ActionType::changeMenu, (void*)&changeUnsignedIntMenu}, "items X padding: ", menuFormatSettings.itemTextSizeX, 0, 20),
    new TextUnsignedIntMenuItem({ActionType::changeMenu, (void*)&changeUnsignedIntMenu}, "items Y padding: ", menuFormatSettings.itemTextSizeY, 0, 20),
    new TextBoolMenuItem({ActionType::updateCurrentItemValue}, "is multiline: ", menuFormatSettings.multilineItemsText)});

  colorMenu.assignMenuItems({
    new TextMenuItem(ActionType::updateParentItemValue, colorToString(Color::black)),
    new TextMenuItem(ActionType::updateParentItemValue, colorToString(Color::navy)),
    new TextMenuItem(ActionType::updateParentItemValue, colorToString(Color::darkGreen)),
    new TextMenuItem(ActionType::updateParentItemValue, colorToString(Color::darkCyan)),
    new TextMenuItem(ActionType::updateParentItemValue, colorToString(Color::maroon)),
    new TextMenuItem(ActionType::updateParentItemValue, colorToString(Color::purple)),
    new TextMenuItem(ActionType::updateParentItemValue, colorToString(Color::olive)),
    new TextMenuItem(ActionType::updateParentItemValue, colorToString(Color::lightGrey)),
    new TextMenuItem(ActionType::updateParentItemValue, colorToString(Color::darkGrey)),
    new TextMenuItem(ActionType::updateParentItemValue, colorToString(Color::blue)),
    new TextMenuItem(ActionType::updateParentItemValue, colorToString(Color::green)),
    new TextMenuItem(ActionType::updateParentItemValue, colorToString(Color::cyan)),
    new TextMenuItem(ActionType::updateParentItemValue, colorToString(Color::red)),
    new TextMenuItem(ActionType::updateParentItemValue, colorToString(Color::magenta)),
    new TextMenuItem(ActionType::updateParentItemValue, colorToString(Color::yellow)),
    new TextMenuItem(ActionType::updateParentItemValue, colorToString(Color::white)),
    new TextMenuItem(ActionType::updateParentItemValue, colorToString(Color::orange)),
    new TextMenuItem(ActionType::updateParentItemValue, colorToString(Color::greenYellow))});
};

void MenuManager::begin()
{
  tft.begin();
  tft.setRotation(1);

  currentMenu->draw();
}

void MenuManager::update(MenuControlCommand menuControlCommand)
{
  switch (menuControlCommand)
  {
  case MenuControlCommand::left:
    currentMenu->onLeft();
    break;
  case MenuControlCommand::right:
    currentMenu->onRigth();
    break;
  case MenuControlCommand::up:
    currentMenu->onUp();
    break;
  case MenuControlCommand::down:
    currentMenu->onDown();
    break;
  case MenuControlCommand::ok:
    runAction(currentMenu->getAction());
    break;
  case MenuControlCommand::home:
    changeMenu(homeMenu, true);
    break;
  case MenuControlCommand::menu:
    changeMenu(mainManu, true);
    break;
  case MenuControlCommand::back:
  {
    IMenu* parent = currentMenu->getParent();
    if (parent)
    {
      changeMenu(*parent, false);
    }
    break;
  }
  }
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

void MenuManager::runAction(const Action& action)
{
  switch (action.type)
  {
  case ActionType::none:
    break;
  case ActionType::changeMenu:
    runChangeMenuAction(*(IMenu*)action.parameter);
    break;
  case ActionType::updateCurrentMenuValue:
    runUpdateCurrentMenuValue();
    break;
  case ActionType::updateCurrentItemValue:
    runUpdateCurrentItemValue();
    break;
  case ActionType::updateParentMenuValue:
    runUpdateParentMenuValue();
    break;
  case ActionType::updateParentItemValue:
    runUpdateParentItemValue();
    break;
  }
}

void MenuManager::runChangeMenuAction(IMenu& targetMenu)
{
  targetMenu.setParent(currentMenu);
  targetMenu.setValue(currentMenu->getItemValue());
  changeMenu(targetMenu, false);
}

void MenuManager::runUpdateCurrentMenuValue()
{
  bool layoutChange = currentMenu->updateValue(currentMenu->getItemValue());
  if (layoutChange || currentMenu->getMenuVersion() != menuFormatSettings.menuVersion)
  {
    currentMenu->draw();
  }
  else
  {
    // redraw
    currentMenu->draw();
  }
}

void MenuManager::runUpdateCurrentItemValue()
{
  bool layoutChange = currentMenu->updateItemValue(currentMenu->getItemValue());
  if (layoutChange || currentMenu->getMenuVersion() != menuFormatSettings.menuVersion)
  {
    currentMenu->draw();
  }
  else
  {
    // redraw
    currentMenu->draw();
  }
}

void MenuManager::runUpdateParentMenuValue()
{
  currentMenu->getParent()->setValue(currentMenu->getItemValue());
  changeMenu(*currentMenu->getParent(), false);
}

void MenuManager::runUpdateParentItemValue()
{
  currentMenu->getParent()->setItemValue(currentMenu->getValue());
  changeMenu(*currentMenu->getParent(), false);
}