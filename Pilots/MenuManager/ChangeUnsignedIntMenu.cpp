#include "MenuManager.h"
#include "ChangeUnsignedIntMenu.h"
#include "DrawingUtils.h"
#include "Action.h"
#include "Utils.h"

ChangeUnsignedIntMenu::ChangeUnsignedIntMenu(MenuManager& menuManager)
  : IMenu{menuManager}
  , valueItem({ActionType::none}, "0")
  , buttons{ButtonMenuItem({ActionType::updateCurrentMenuValue}, "7"), ButtonMenuItem({ActionType::updateCurrentMenuValue}, "8"), ButtonMenuItem({ActionType::updateCurrentMenuValue}, "9"),
            ButtonMenuItem({ActionType::updateCurrentMenuValue}, "4"), ButtonMenuItem({ActionType::updateCurrentMenuValue}, "5"), ButtonMenuItem({ActionType::updateCurrentMenuValue}, "6"),
            ButtonMenuItem({ActionType::updateCurrentMenuValue}, "1"), ButtonMenuItem({ActionType::updateCurrentMenuValue}, "2"), ButtonMenuItem({ActionType::updateCurrentMenuValue}, "3"),
            ButtonMenuItem({ActionType::updateCurrentMenuValue}, "<="), ButtonMenuItem({ActionType::updateCurrentMenuValue}, "0"), ButtonMenuItem({ActionType::updateParentItemValue}, "OK")}
{}

const Action& ChangeUnsignedIntMenu::getAction() const
{
  return buttons[selectedButtonInd].getAction();
}

void ChangeUnsignedIntMenu::onLeft()
{
  if (selectedButtonInd % nCols > 0)
  {
    buttons[selectedButtonInd].draw(menuManager, false);
    --selectedButtonInd;
    buttons[selectedButtonInd].draw(menuManager, true);
  }
}

void ChangeUnsignedIntMenu::onRigth()
{
  if (selectedButtonInd % nCols < nCols - 1)
  {
    buttons[selectedButtonInd].draw(menuManager, false);
    ++selectedButtonInd;
    buttons[selectedButtonInd].draw(menuManager, true);
  }
}

void ChangeUnsignedIntMenu::onUp()
{
  if (selectedButtonInd >= nCols)
  {
    buttons[selectedButtonInd].draw(menuManager, false);
    selectedButtonInd -= nCols;
    buttons[selectedButtonInd].draw(menuManager, true);
  }
}

void ChangeUnsignedIntMenu::onDown()
{
  if (selectedButtonInd < nButtons - nCols)
  {
    buttons[selectedButtonInd].draw(menuManager, false);
    selectedButtonInd += nCols;
    buttons[selectedButtonInd].draw(menuManager, true);
  }
}

void ChangeUnsignedIntMenu::draw()
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

void ChangeUnsignedIntMenu::resetSelection()
{
  selectedButtonInd = nButtons - 1;
}

String ChangeUnsignedIntMenu::getValue() const
{
  return valueItem.getValue(); 
}

String ChangeUnsignedIntMenu::getItemValue() const
{
  return buttons[selectedButtonInd].getValue(); 
}

bool ChangeUnsignedIntMenu::setValue(const String& value)
{
  return valueItem.setValue(menuManager, value); 
}

bool ChangeUnsignedIntMenu::setItemValue(const String& value)
{
  return buttons[selectedButtonInd].setValue(menuManager, value); 
}

bool ChangeUnsignedIntMenu::updateValue(const String& value)
{
  return valueItem.updateValue(menuManager, value); 
}

bool ChangeUnsignedIntMenu::updateItemValue(const String& value)
{
  return buttons[selectedButtonInd].updateValue(menuManager, value); 
}

bool ChangeUnsignedIntMenu::isValidValue(const String& value)
{
  if (value.length() == 0)
  {
    return true;
  }
  return isUnsignedInt(value);
}

bool ChangeUnsignedIntMenu::isValidItemValue(const String& value)
{
  return buttons[selectedButtonInd].isValidValue(value); 
}

void ChangeUnsignedIntMenu::showValueError()
{
  valueItem.draw(menuManager, true);
  valueItem.draw(menuManager, false);
}

void ChangeUnsignedIntMenu::updateLayout()
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
  for (int i = 0; i < nRows; ++i)
  {
    for (int j = 0; j < nCols; ++j)
    {
      buttons[nCols * i + j].setSize(width, height);
      buttons[nCols * i + j].setPosition(x + j * dx, y + i * dy);
    }
  }
}