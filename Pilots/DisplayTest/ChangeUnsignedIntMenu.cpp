#include "MenuManager.h"
#include "ChangeUnsignedIntMenu.h"
#include "DrawingUtils.h"
#include "UpdateValueAction.h"
#include "ChangeUnsignedIntAction.h"

ChangeUnsignedIntMenu::ChangeUnsignedIntMenu(MenuManager& menuManager)
  : IMenu{menuManager}
  , valueItem(*this, nullptr, "0")
  , buttons{ButtonMenuItem(*this, new UpdateValueAction(menuManager, 0), "7"), ButtonMenuItem(*this, new UpdateValueAction(menuManager, 1), "8"), ButtonMenuItem(*this, new UpdateValueAction(menuManager, 2), "9"),
            ButtonMenuItem(*this, new UpdateValueAction(menuManager, 3), "4"), ButtonMenuItem(*this, new UpdateValueAction(menuManager, 4), "5"), ButtonMenuItem(*this, new UpdateValueAction(menuManager, 5), "6"),
            ButtonMenuItem(*this, new UpdateValueAction(menuManager, 6), "1"), ButtonMenuItem(*this, new UpdateValueAction(menuManager, 7), "2"), ButtonMenuItem(*this, new UpdateValueAction(menuManager, 8), "3"),
            ButtonMenuItem(*this, new UpdateValueAction(menuManager, 9), "<="), ButtonMenuItem(*this, new UpdateValueAction(menuManager, 10), "0"), ButtonMenuItem(*this, new ChangeUnsignedIntAction(menuManager), "OK")}
{}

void ChangeUnsignedIntMenu::draw()
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
    for (int i = 0; i < nRows; ++i)
    {
      for (int j = 0; j < nCols; ++j)
      {
        buttons[nCols * i + j].setPosition(x + j * dx, y + i * dy, width, height);
      }
    }
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

void ChangeUnsignedIntMenu::update(MenuControlCommand menuControlCommand)
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
      if (selectedButtonInd < nButtons - nCols)
      {
        buttons[selectedButtonInd].draw(menuManager, false);
        selectedButtonInd += nCols;
        buttons[selectedButtonInd].draw(menuManager, true);
      }
      break;
    }
    case MenuControlCommand::left:
    {
      if (selectedButtonInd % nCols > 0)
      {
        buttons[selectedButtonInd].draw(menuManager, false);
        --selectedButtonInd;
        buttons[selectedButtonInd].draw(menuManager, true);
      }
      break;
    }
    case MenuControlCommand::right:
    {
      if (selectedButtonInd % nCols < nCols - 1)
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

void ChangeUnsignedIntMenu::resetSelection()
{
  selectedButtonInd = 11;
}

String ChangeUnsignedIntMenu::getValue() const
{
  return valueItem.getValue();
}

String ChangeUnsignedIntMenu::getValue(int ind) const
{
  return buttons[ind].getValue();
}

void ChangeUnsignedIntMenu::setValue(const String& value)
{
  valueItem.setValue(value);
}

void ChangeUnsignedIntMenu::setSelectedItemValue(const String& value)
{
}

void ChangeUnsignedIntMenu::updateValue(const String& value)
{
  valueItem.update(menuManager, value);
}