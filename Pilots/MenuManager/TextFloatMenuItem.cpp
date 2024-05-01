#include "TextFloatMenuItem.h"
#include "MenuManager.h"
#include "DrawingUtils.h"
#include "Utils.h"

TextFloatMenuItem::TextFloatMenuItem(Action action, const String& text, float& value, float minValue, float maxValue)
  : IMenuItem(action), text{text}, value{value < minValue ? minValue : (value > maxValue ? maxValue : value)}, minValue{minValue}, maxValue{maxValue}
{}

void TextFloatMenuItem::calculateSize(MenuManager& menuManager, int maxWidth)
{
  this->maxWidth = maxWidth;
  int textLength = getText().length();
  width = calculateItemTextWidth(menuManager, textLength, maxWidth);
  height = trimmedHeight = calculateItemTextHeight(menuManager, textLength, width);
  trimmed = false;
}

void TextFloatMenuItem::draw(MenuManager& menuManager, bool isSelected)
{
  String text = getText();
  drawItemText(menuManager, x, y, width, trimmedHeight, text, isSelected, HorizontalAlignment::left, VerticalAlignment::up, true);
}

void TextFloatMenuItem::redraw(MenuManager& menuManager, bool isSelected)
{
  menuManager.tft.fillRect(x, y, maxWidth, trimmedHeight, (uint16_t)menuManager.menuFormatSettings.backgroundColor);
  draw(menuManager, isSelected);
}

String TextFloatMenuItem::getValue() const
{
  return String(value);
}

String TextFloatMenuItem::getText() const
{
  return text + getValue();
}
  
bool TextFloatMenuItem::setValue(MenuManager& menuManager, String value)
{
  this->value = value.toFloat();
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

bool TextFloatMenuItem::updateValue(MenuManager& menuManager, const String& value)
{
  return setValue(menuManager, value);
}

bool TextFloatMenuItem::isValidValue(const String& value)
{
  return isFloat(value);
}