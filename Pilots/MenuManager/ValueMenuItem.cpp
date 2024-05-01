#include "ValueMenuItem.h"
#include "MenuManager.h"
#include "DrawingUtils.h"
#include "Utils.h"

ValueMenuItem::ValueMenuItem(Action action, const String& value)
  : IMenuItem(action), value{value}
{
}

void ValueMenuItem::calculateSize(MenuManager& menuManager, int maxWidth)
{
  width = this->maxWidth = maxWidth;
  height = trimmedHeight = 2 * (menuManager.menuFormatSettings.itemBorderSize + menuManager.menuFormatSettings.itemsPaddingY) + menuManager.displaySettings.yTextScalar * menuManager.menuFormatSettings.itemTextSizeY;
  int maxTextLength = (width - 2 * (menuManager.menuFormatSettings.itemBorderSize + menuManager.menuFormatSettings.itemsPaddingX)) / (menuManager.displaySettings.xTextScalar * menuManager.menuFormatSettings.itemTextSizeX);
  if (value.length() > maxTextLength)
  {
    value = value.substring(0, maxTextLength);
  }
}

void ValueMenuItem::draw(MenuManager& menuManager, bool isSelected)
{
  Serial.println("Drawing value");
  drawItemBorder(menuManager, x, y, width, height, isSelected);
  menuManager.tft.fillRect(x + menuManager.menuFormatSettings.itemBorderSize, y + menuManager.menuFormatSettings.itemBorderSize, width - 2 * menuManager.menuFormatSettings.itemBorderSize, trimmedHeight - 2 * menuManager.menuFormatSettings.itemBorderSize, (uint16_t)menuManager.menuFormatSettings.itemBackgroundColor);
  drawItemText(menuManager, x + menuManager.menuFormatSettings.itemBorderSize + menuManager.menuFormatSettings.itemsPaddingX, y + menuManager.menuFormatSettings.itemBorderSize + menuManager.menuFormatSettings.itemsPaddingY, width - 2 * (menuManager.menuFormatSettings.itemBorderSize + menuManager.menuFormatSettings.itemsPaddingX), trimmedHeight - 2 * (menuManager.menuFormatSettings.itemBorderSize + menuManager.menuFormatSettings.itemsPaddingY), value, isSelected, HorizontalAlignment::left, VerticalAlignment::center, false);
}

void ValueMenuItem::redraw(MenuManager& menuManager, bool isSelected)
{
  draw(menuManager, isSelected);
}

String ValueMenuItem::getValue() const
{
  return value;
}

String ValueMenuItem::getText() const
{
  return getValue();
}

bool ValueMenuItem::setValue(MenuManager& menuManager, String value)
{
  int maxTextLength = (width - 2 * (menuManager.menuFormatSettings.itemBorderSize + menuManager.menuFormatSettings.itemsPaddingX)) / (menuManager.displaySettings.xTextScalar * menuManager.menuFormatSettings.itemTextSizeX);
  Serial.println("ValueMenuItem::setValue: " + value);
  if (value.length() > maxTextLength)
  {
    this->value = value.substring(0, maxTextLength);
  }
  else
  {
    this->value = value;
  }
  return false;
}

bool ValueMenuItem::updateValue(MenuManager& menuManager, const String& value)
{
  int maxTextLength = (width - 2 * (menuManager.menuFormatSettings.itemBorderSize + menuManager.menuFormatSettings.itemsPaddingX)) / (menuManager.displaySettings.xTextScalar * menuManager.menuFormatSettings.itemTextSizeX);
  Serial.println("ValueMenuItem::updateValue: " + value);
  int textLenght = this->value.length();
  if (value == "<=")
  {
    if (textLenght > 0)
    {
      this->value = this->value.substring(0, textLenght - 1);
    }
  }
  else
  {
    if (textLenght + value.length() <= maxTextLength)
    {
      this->value += value;
    }
    else if (textLenght < maxTextLength)
    {
      this->value += value.substring(0, maxTextLength - textLenght);
    }
  }
  return false;
}

bool ValueMenuItem::isValidValue(const String& value)
{
  return true;
}