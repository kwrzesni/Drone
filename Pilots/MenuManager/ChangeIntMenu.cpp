#include "MenuManager.h"
#include "ChangeIntMenu.h"
#include "DrawingUtils.h"
#include "Action.h"
#include "Utils.h"

ChangeIntMenu::ChangeIntMenu(MenuManager& menuManager)
  : IMenu{menuManager}
  , valueItem({ActionType::none}, "0")
  , buttons{ButtonMenuItem({ActionType::updateCurrentMenuValue}, "7"), ButtonMenuItem({ActionType::updateCurrentMenuValue}, "8"), ButtonMenuItem({ActionType::updateCurrentMenuValue}, "9"),
            ButtonMenuItem({ActionType::updateCurrentMenuValue}, "4"), ButtonMenuItem({ActionType::updateCurrentMenuValue}, "5"), ButtonMenuItem({ActionType::updateCurrentMenuValue}, "6"),
            ButtonMenuItem({ActionType::updateCurrentMenuValue}, "1"), ButtonMenuItem({ActionType::updateCurrentMenuValue}, "2"), ButtonMenuItem({ActionType::updateCurrentMenuValue}, "3"),
            ButtonMenuItem({ActionType::updateCurrentMenuValue}, "-"), ButtonMenuItem({ActionType::updateCurrentMenuValue}, "0"), ButtonMenuItem({ActionType::updateCurrentMenuValue}, "<="),
            ButtonMenuItem({ActionType::updateParentItemValue}, "OK")}
{
}

const Action& ChangeIntMenu::getAction() const
{
  return buttons[selectedButtonInd].getAction();
}

void ChangeIntMenu::onLeft()
{
  if (selectedButtonInd % nCols > 0 && selectedButtonInd != 12)
  {
    buttons[selectedButtonInd].draw(menuManager, false);
    --selectedButtonInd;
    buttons[selectedButtonInd].draw(menuManager, true);
  }
}

void ChangeIntMenu::onRigth()
{
  if (selectedButtonInd % nCols < nCols - 1 && selectedButtonInd != 12)
  {
    buttons[selectedButtonInd].draw(menuManager, false);
    ++selectedButtonInd;
    buttons[selectedButtonInd].draw(menuManager, true);
  }
}

void ChangeIntMenu::onUp()
{
  if (selectedButtonInd >= nCols)
  {
    buttons[selectedButtonInd].draw(menuManager, false);
    selectedButtonInd -= nCols;
    buttons[selectedButtonInd].draw(menuManager, true);
  }
}

void ChangeIntMenu::onDown()
{
  if (selectedButtonInd < 12 && selectedButtonInd > 8)
  {
    buttons[selectedButtonInd].draw(menuManager, false);
    selectedButtonInd = 12;
    buttons[selectedButtonInd].draw(menuManager, true);
  }
  else if (selectedButtonInd < nButtons - nCols)
  {
    buttons[selectedButtonInd].draw(menuManager, false);
    selectedButtonInd += nCols;
    buttons[selectedButtonInd].draw(menuManager, true);
  }
}

void ChangeIntMenu::draw()
{
  if (menuVersion < menuManager.menuFormatSettings.menuVersion)
  { 
    updateLayout();
    menuVersion = menuManager.menuFormatSettings.menuVersion;
  }
  clearScreen(menuManager);
  drawHeader(menuManager, title);
  valueItem.draw(menuManager, false);
  for (int i = 0; i < nButtons; ++i)
  {
    buttons[i].draw(menuManager, i == selectedButtonInd);
  }
}

void ChangeIntMenu::resetSelection()
{
  selectedButtonInd = nButtons - 1;
}

String ChangeIntMenu::getValue() const
{
  return valueItem.getValue();
}

String ChangeIntMenu::getItemValue() const
{
  return buttons[selectedButtonInd].getValue();
}

bool ChangeIntMenu::setValue(const String& value)
{
  return valueItem.setValue(menuManager, value);
}

bool ChangeIntMenu::setItemValue(const String& value)
{
  return buttons[selectedButtonInd].setValue(menuManager, value);
}

bool ChangeIntMenu::updateValue(const String& value)
{
  return valueItem.updateValue(menuManager, value);
}

bool ChangeIntMenu::updateItemValue(const String& value)
{
  return buttons[selectedButtonInd].updateValue(menuManager, value);
}

bool ChangeIntMenu::isValidValue(const String& value)
{
  if (value.length() == 0)
  {
    return true;
  }
  isInt(value);
}

bool ChangeIntMenu::isValidItemValue(const String& value)
{
  return buttons[selectedButtonInd].isValidValue(value);
}

void ChangeIntMenu::showValueError()
{
  valueItem.draw(menuManager, true);
  valueItem.draw(menuManager, false);
}

void ChangeIntMenu::updateLayout()
{
  int x = menuManager.menuFormatSettings.itemsPaddingX;
  int y = menuManager.menuFormatSettings.headerHeight + menuManager.menuFormatSettings.headerPadding;
  int dx = (menuManager.displaySettings.width - x) / nCols;
  int dy = (menuManager.displaySettings.height - y) / (nRows + 1);
  int width = dx - menuManager.menuFormatSettings.itemsPaddingX;
  int height = dy - menuManager.menuFormatSettings.itemsPaddingY;
  valueItem.setSize(menuManager.displaySettings.width - 2 * menuManager.menuFormatSettings.itemsPaddingX, height);
  valueItem.setPosition(x, y);
  y += dy;
  for (int i = 0; i < nRows - 1; ++i)
  {
    for (int j = 0; j < nCols; ++j)
    {
      buttons[nCols * i + j].setSize(width, height);
      buttons[nCols * i + j].setPosition(x + j * dx, y + i * dy);
    }
  }
  buttons[12].setSize(menuManager.displaySettings.width - 2 * menuManager.menuFormatSettings.itemsPaddingX, height);
  buttons[12].setPosition(x, y + 4 * dy);
}