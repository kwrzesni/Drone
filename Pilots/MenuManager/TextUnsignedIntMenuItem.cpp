#include "TextUnsignedIntMenuItem.h"
#include "MenuManager.h"
#include "DrawingUtils.h"
#include "Utils.h"

TextUnsignedIntMenuItem::TextUnsignedIntMenuItem(Action action, const String& text, unsigned int& value, unsigned int minValue, unsigned int maxValue)
  : IMenuItem(action), text{text}, value{value < minValue ? minValue : (value > maxValue ? maxValue : value)}, minValue{minValue}, maxValue{maxValue}
{}

void TextUnsignedIntMenuItem::calculateSize(MenuManager& menuManager, int maxWidth)
{
  this->maxWidth = maxWidth;
  int textLength = getText().length();
  width = calculateItemTextWidth(menuManager, textLength, maxWidth);
  height = trimmedHeight = calculateItemTextHeight(menuManager, textLength, width);
  trimmed = false;
}

void TextUnsignedIntMenuItem::draw(MenuManager& menuManager, bool isSelected)
{
  String text = getText();
  drawItemText(menuManager, x, y, width, trimmedHeight, text, isSelected, HorizontalAlignment::left, VerticalAlignment::up, true);
}

void TextUnsignedIntMenuItem::redraw(MenuManager& menuManager, bool isSelected)
{
  menuManager.tft.fillRect(x, y, maxWidth, trimmedHeight, (uint16_t)menuManager.menuFormatSettings.backgroundColor);
  draw(menuManager, isSelected);
}

String TextUnsignedIntMenuItem::getValue() const
{
  return String(value);
}

String TextUnsignedIntMenuItem::getText() const
{
  return text + getValue();
}

bool TextUnsignedIntMenuItem::setValue(MenuManager& menuManager, String value)
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

bool TextUnsignedIntMenuItem::updateValue(MenuManager& menuManager, const String& value)
{
  return setValue(menuManager, value);
}

bool TextUnsignedIntMenuItem::isValidValue(const String& value)
{
  return isUnsignedInt(value);
}