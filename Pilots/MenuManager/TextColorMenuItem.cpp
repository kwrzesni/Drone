#include "TextColorMenuItem.h"
#include "MenuManager.h"
#include "DrawingUtils.h"
#include "Utils.h"

TextColorMenuItem::TextColorMenuItem(Action action, const String& text, Color& color)
  : IMenuItem(action), text{text}, color{color}
{}

void TextColorMenuItem::calculateSize(MenuManager& menuManager, int maxWidth)
{
  this->maxWidth = maxWidth;
  int textLength = getText().length();
  width = calculateItemTextWidth(menuManager, textLength, maxWidth);
  height = trimmedHeight = calculateItemTextHeight(menuManager, textLength, width);
  trimmed = false;
}

void TextColorMenuItem::draw(MenuManager& menuManager, bool isSelected)
{
  String text = getText();
  drawItemText(menuManager, x, y, width, trimmedHeight, text, isSelected, HorizontalAlignment::left, VerticalAlignment::up, true);
}

void TextColorMenuItem::redraw(MenuManager& menuManager, bool isSelected)
{
  menuManager.tft.fillRect(x, y, maxWidth, trimmedHeight, (uint16_t)menuManager.menuFormatSettings.backgroundColor);
  draw(menuManager, isSelected);
}

String TextColorMenuItem::getValue() const
{
  return colorToString(color);
}

String TextColorMenuItem::getText() const
{
  return text + getValue();
}

bool TextColorMenuItem::setValue(MenuManager& menuManager, String value)
{
  color = stringToColor(value);
  if (isMenuLayoutParameter(menuManager, &this->color))
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

bool TextColorMenuItem::updateValue(MenuManager& menuManager, const String& value)
{
  return setValue(menuManager, value);
}

bool TextColorMenuItem::isValidValue(const String& value)
{
  return stringToColor(value) != Color::none;
}