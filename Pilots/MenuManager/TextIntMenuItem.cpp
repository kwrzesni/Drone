#include "TextIntMenuItem.h"
#include "MenuManager.h"
#include "DrawingUtils.h"
#include "Utils.h"

TextIntMenuItem::TextIntMenuItem(Action action, const String& text, int& value, int minValue, int maxValue)
  : IMenuItem(action), text{text}, value{value < minValue ? minValue : (value > maxValue ? maxValue : value)}, minValue{minValue}, maxValue{maxValue}
{}

void TextIntMenuItem::calculateSize(MenuManager& menuManager, int maxWidth)
{
  this->maxWidth = maxWidth;
  int textLength = getText().length();
  width = calculateItemTextWidth(menuManager, textLength, maxWidth);
  height = trimmedHeight = calculateItemTextHeight(menuManager, textLength, width);
  trimmed = false;
}

void TextIntMenuItem::draw(MenuManager& menuManager, bool isSelected)
{
  String text = getText();
  drawItemText(menuManager, x, y, width, trimmedHeight, text, isSelected, HorizontalAlignment::left, VerticalAlignment::up, true);
}

void TextIntMenuItem::redraw(MenuManager& menuManager, bool isSelected)
{
  menuManager.tft.fillRect(x, y, maxWidth, trimmedHeight, (uint16_t)menuManager.menuFormatSettings.backgroundColor);
  draw(menuManager, isSelected);
}

String TextIntMenuItem::getValue() const
{
  return String(value);
}

String TextIntMenuItem::getText() const
{
  return text + getValue();
}

bool TextIntMenuItem::setValue(MenuManager& menuManager, String value)
{
  this->value = value.toInt();
  this->value = this->value < minValue ? minValue : (this->value > maxValue ? maxValue : this->value);
  if (isMenuLayoutParameter(menuManager, (void*)&this->value))
  {
    ++menuManager.menuFormatSettings.menuVersion;
  }
  int previousHeight = height;
  int previousTimmedHeight = trimmedHeight;
  calculateSize(menuManager, maxWidth);
  if (trimmed)
  {
    if (height >= previousTimmedHeight)
    {
      height = trimmedHeight = previousTimmedHeight;
      return false;
    }
    else
    {
      trimmed = false;
      return true;
    }
  }
  else
  {
    return height != previousHeight;
  }
}

bool TextIntMenuItem::updateValue(MenuManager& menuManager, const String& value)
{
  return setValue(menuManager, value);
}

bool TextIntMenuItem::isValidValue(const String& value)
{
  return isInt(value);
}