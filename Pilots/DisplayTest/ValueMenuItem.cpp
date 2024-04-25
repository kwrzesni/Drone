#include "ValueMenuItem.h"
#include "MenuManager.h"
#include "DrawingUtils.h"

ValueMenuItem::ValueMenuItem(IMenu& menu, IAction* action, const String& value)
  : IMenuItem(menu, action), value{value}
{}

void ValueMenuItem::draw(MenuManager& menuManager, bool isSelected)
{
  drawValue(menuManager, x, y, width, height, value, isSelected);
}

void ValueMenuItem::update(MenuManager& menuManager, const String& value)
{
  int textLenght = this->value.length();
  if (value == "<=")
  {
    if (textLenght > 0)
    {
      this->value = this->value.substring(0, textLenght - 1);
      updateDrawnValue(menuManager, x, y, width, height, this->value, textLenght);
    }
  }
  else
  {
    int maxTextLength = (width - 2 * (menuManager.menuFormatSettings.itemBorderSize + menuManager.menuFormatSettings.itemsPaddingX)) / (menuManager.displaySettings.xTextScalar * menuManager.menuFormatSettings.itemTextSizeX);
    if (textLenght + value.length() <= maxTextLength)
    {
      this->value += value;
      updateDrawnValue(menuManager, x, y, width, height, this->value, textLenght);
    }
  }
}

String ValueMenuItem::getValue() const
{
  return value.length() ? value : "0";
}

void ValueMenuItem::setValue(String value)
{
  this->value = value;
}

String ValueMenuItem::getText() const
{
  return value;
}