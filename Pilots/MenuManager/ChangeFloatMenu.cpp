#include "MenuManager.h"
#include "ChangeFloatMenu.h"
#include "DrawingUtils.h"
#include "Action.h"
#include "Utils.h"

ChangeFloatMenu::ChangeFloatMenu(MenuManager& menuManager)
  : IMenu{menuManager}
  , valueItem({ActionType::none}, "0")
  , buttons{ButtonMenuItem({ActionType::updateCurrentMenuValue}, "7"), ButtonMenuItem({ActionType::updateCurrentMenuValue}, "8"), ButtonMenuItem({ActionType::updateCurrentMenuValue}, "9"),
            ButtonMenuItem({ActionType::updateCurrentMenuValue}, "4"), ButtonMenuItem({ActionType::updateCurrentMenuValue}, "5"), ButtonMenuItem({ActionType::updateCurrentMenuValue}, "6"),
            ButtonMenuItem({ActionType::updateCurrentMenuValue}, "1"), ButtonMenuItem({ActionType::updateCurrentMenuValue}, "2"), ButtonMenuItem({ActionType::updateCurrentMenuValue}, "3"),
            ButtonMenuItem({ActionType::updateCurrentMenuValue}, "-"), ButtonMenuItem({ActionType::updateCurrentMenuValue}, "0"), ButtonMenuItem({ActionType::updateCurrentMenuValue}, "."),
            ButtonMenuItem({ActionType::updateCurrentMenuValue}, "<="), ButtonMenuItem({ActionType::updateParentItemValue}, "OK")}
{}

const Action& ChangeFloatMenu::getAction() const
{
  return buttons[selectedButtonInd].getAction();
}

void ChangeFloatMenu::onLeft()
{
  if (selectedButtonInd % nCols > 0 && selectedButtonInd != 12)
  {
    buttons[selectedButtonInd].draw(menuManager, false);
    --selectedButtonInd;
    buttons[selectedButtonInd].draw(menuManager, true);
  }
}

void ChangeFloatMenu::onRigth()
{
  if (selectedButtonInd % nCols < nCols - 1 && selectedButtonInd != 13)
  {
    buttons[selectedButtonInd].draw(menuManager, false);
    ++selectedButtonInd;
    buttons[selectedButtonInd].draw(menuManager, true);
  }
}

void ChangeFloatMenu::onUp()
{
  if (selectedButtonInd >= nCols)
  {
    buttons[selectedButtonInd].draw(menuManager, false);
    selectedButtonInd -= nCols;
    buttons[selectedButtonInd].draw(menuManager, true);
  }
}

void ChangeFloatMenu::onDown()
{
  if (selectedButtonInd == 9)
  {
    buttons[selectedButtonInd].draw(menuManager, false);
    selectedButtonInd = 12;
    buttons[selectedButtonInd].draw(menuManager, true);
  }
  else if (selectedButtonInd == 10 || selectedButtonInd == 11)
  {
    buttons[selectedButtonInd].draw(menuManager, false);
    selectedButtonInd = 13;
    buttons[selectedButtonInd].draw(menuManager, true);
  }
  else if (selectedButtonInd < nButtons - nCols)
  {
    buttons[selectedButtonInd].draw(menuManager, false);
    selectedButtonInd += nCols;
    buttons[selectedButtonInd].draw(menuManager, true);
  }
}

void ChangeFloatMenu::draw()
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

void ChangeFloatMenu::resetSelection()
{
  selectedButtonInd = nButtons - 1;
}

String ChangeFloatMenu::getValue() const
{
  return valueItem.getValue();
}

String ChangeFloatMenu::getItemValue() const
{
  return buttons[selectedButtonInd].getValue();
}

bool ChangeFloatMenu::setValue(const String& value)
{
  return valueItem.setValue(menuManager, value);
}

bool ChangeFloatMenu::setItemValue(const String& value)
{
  return buttons[selectedButtonInd].setValue(menuManager, value);
}

bool ChangeFloatMenu::updateValue(const String& value)
{
  return valueItem.updateValue(menuManager, value);
}
  
bool ChangeFloatMenu::updateItemValue(const String& value)
{
  return buttons[selectedButtonInd].updateValue(menuManager, value);
}

bool ChangeFloatMenu::isValidValue(const String& value)
{
  return valueItem.isValidValue(value);
}

bool ChangeFloatMenu::isValidItemValue(const String& value)
{
  return buttons[selectedButtonInd].isValidValue(value);
}

void ChangeFloatMenu::showValueError()
{
  valueItem.draw(menuManager, true);
  valueItem.draw(menuManager, false);
}

void ChangeFloatMenu::updateLayout()
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
  y += 4 * dy;
  dx = (menuManager.displaySettings.width - x) / (nCols - 1);
  width = dx - menuManager.menuFormatSettings.itemsPaddingX;
  buttons[12].setSize(width, height);
  buttons[12].setPosition(x, y);
  buttons[13].setSize(width, height);
  buttons[13].setPosition(x + dx, y);
}