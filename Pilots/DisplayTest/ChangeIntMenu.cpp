#include "MenuManager.h"
#include "ChangeIntMenu.h"
#include "DrawingUtils.h"
#include "UpdateValueAction.h"
#include "ChangeIntAction.h"

ChangeIntMenu::ChangeIntMenu(MenuManager& menuManager)
  : IMenu{menuManager}
  , valueItem(*this, nullptr, "0")
  , buttons{ButtonMenuItem(*this, new UpdateValueAction(menuManager, 0), "7"), ButtonMenuItem(*this, new UpdateValueAction(menuManager, 1), "8"), ButtonMenuItem(*this, new UpdateValueAction(menuManager, 2), "9"),
            ButtonMenuItem(*this, new UpdateValueAction(menuManager, 3), "4"), ButtonMenuItem(*this, new UpdateValueAction(menuManager, 4), "5"), ButtonMenuItem(*this, new UpdateValueAction(menuManager, 5), "6"),
            ButtonMenuItem(*this, new UpdateValueAction(menuManager, 6), "1"), ButtonMenuItem(*this, new UpdateValueAction(menuManager, 7), "2"), ButtonMenuItem(*this, new UpdateValueAction(menuManager, 8), "3"),
            ButtonMenuItem(*this, new UpdateValueAction(menuManager, 9), "-"), ButtonMenuItem(*this, new UpdateValueAction(menuManager, 10), "0"), ButtonMenuItem(*this, new UpdateValueAction(menuManager, 11), "<="),
            ButtonMenuItem(*this, new ChangeIntAction(menuManager), "OK")}
{}

void ChangeIntMenu::draw()
{
  if (menuVersion < menuManager.menuFormatSettings.menuVersion)
  { 
    int x = menuManager.menuFormatSettings.itemsPaddingX;
    int y = menuManager.menuFormatSettings.headerHeight + menuManager.menuFormatSettings.headerPadding;
    int dx = (menuManager.displaySettings.width - x) / nCols;
    int dy = (menuManager.displaySettings.height - y) / (nRows + 1);
    int width = dx - menuManager.menuFormatSettings.itemsPaddingX;
    int height = dy - menuManager.menuFormatSettings.itemsPaddingY;
    valueItem.setPosition(x, y, menuManager.displaySettings.width - 2 * menuManager.menuFormatSettings.itemsPaddingX, height);
    y += dy;
    for (int i = 0; i < nRows - 1; ++i)
    {
      for (int j = 0; j < nCols; ++j)
      {
        buttons[nCols * i + j].setPosition(x + j * dx, y + i * dy, width, height);
      }
    }
    buttons[12].setPosition(x, y + 4 * dy, menuManager.displaySettings.width - 2 * menuManager.menuFormatSettings.itemsPaddingX, height);
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

void ChangeIntMenu::update(MenuControlCommand menuControlCommand)
{
  switch (menuControlCommand)
  {
    case MenuControlCommand::up:
    {
      if (selectedButtonInd >= nCols)
      {
        buttons[selectedButtonInd].draw(menuManager, false);
        selectedButtonInd -= nCols;
        buttons[selectedButtonInd].draw(menuManager, true);
      }
      break;
    }
    case MenuControlCommand::down:
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
      break;
    }
    case MenuControlCommand::left:
    {
      if (selectedButtonInd % nCols > 0 && selectedButtonInd != 12)
      {
        buttons[selectedButtonInd].draw(menuManager, false);
        --selectedButtonInd;
        buttons[selectedButtonInd].draw(menuManager, true);
      }
      break;
    }
    case MenuControlCommand::right:
    {
      if (selectedButtonInd % nCols < nCols - 1 && selectedButtonInd != 12)
      {
        buttons[selectedButtonInd].draw(menuManager, false);
        ++selectedButtonInd;
        buttons[selectedButtonInd].draw(menuManager, true);
      }
      break;
    }
    case MenuControlCommand::ok:
    {
      buttons[selectedButtonInd].runAction();
      break;
    }
    case MenuControlCommand::back:
    {
      if (parent)
      {
        menuManager.changeMenu(*parent, false);
      }
      break;
    }
  }
}

void ChangeIntMenu::resetSelection()
{
  selectedButtonInd = 12;
}

String ChangeIntMenu::getValue() const
{
  return valueItem.getValue();
}

String ChangeIntMenu::getValue(int ind) const
{
  return buttons[ind].getValue();
}

void ChangeIntMenu::setValue(const String& value)
{
  valueItem.setValue(value);
}

void ChangeIntMenu::setSelectedItemValue(const String& value)
{
}

void ChangeIntMenu::updateValue(const String& value)
{
  valueItem.update(menuManager, value);
}

void ChangeIntMenu::showValueError()
{
  valueItem.draw(menuManager, true);
  valueItem.draw(menuManager, false);
}