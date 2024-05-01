#include "TextBoolMenuItem.h"
#include "MenuManager.h"
#include "DrawingUtils.h"
#include "Utils.h"

TextBoolMenuItem::TextBoolMenuItem(Action action, const String& text, bool& value)
  : IMenuItem(action), text{text}, value{value}
{}

void TextBoolMenuItem::calculateSize(MenuManager& menuManager, int maxWidth)
{
  this->maxWidth = maxWidth;
  int textLength = getText().length();
  width = calculateItemTextWidth(menuManager, textLength, maxWidth);
  height = trimmedHeight = calculateItemTextHeight(menuManager, textLength, width);
  trimmed = false;
}

void TextBoolMenuItem::draw(MenuManager& menuManager, bool isSelected)
{
  String text = getText();
  drawItemText(menuManager, x, y, width, trimmedHeight, text, isSelected, HorizontalAlignment::left, VerticalAlignment::up, true);
}

void TextBoolMenuItem::redraw(MenuManager& menuManager, bool isSelected)
{
  menuManager.tft.fillRect(x, y, maxWidth, trimmedHeight, (uint16_t)menuManager.menuFormatSettings.backgroundColor);
  draw(menuManager, isSelected);
}


String TextBoolMenuItem::getValue() const
{
  return value ? "true" : "false";
}

String TextBoolMenuItem::getText() const
{
  return text + getValue();
}

bool TextBoolMenuItem::setValue(MenuManager& menuManager, String value)
{
  if (value == "true")
  {
    this->value == true;
  }
  else if (value == "false")
  {
    this->value = false;
  }
  else
  {
    return false;
  }
  if (isMenuLayoutParameter(menuManager, &this->value))
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

bool TextBoolMenuItem::updateValue(MenuManager& menuManager, const String& value)
{
  if (value == "u")
  {
    return setValue(menuManager, this->value ? "false" : "true");
  }
  return false;
}

bool TextBoolMenuItem::isValidValue(const String& value)
{
  return value == "true" || value == "false";
}