#include "TextMenuItem.h"
#include "MenuManager.h"
#include "DrawingUtils.h"
#include "Utils.h"

TextMenuItem::TextMenuItem(Action action, const String& text)
  : IMenuItem(action), text(text)
{
}

void TextMenuItem::calculateSize(MenuManager& menuManager, int maxWidth)
{
  this->maxWidth = maxWidth;
  int textLength = getText().length();
  width = calculateItemTextWidth(menuManager, textLength, maxWidth);
  height = trimmedHeight = calculateItemTextHeight(menuManager, textLength, width);
  trimmed = false;
}

void TextMenuItem::draw(MenuManager& menuManager, bool isSelected)
{
  String text = getText();
  drawItemText(menuManager, x, y, width, trimmedHeight, text, isSelected, HorizontalAlignment::left, VerticalAlignment::up, true);
}

void TextMenuItem::redraw(MenuManager& menuManager, bool isSelected)
{
  menuManager.tft.fillRect(x, y, maxWidth, trimmedHeight, (uint16_t)menuManager.menuFormatSettings.backgroundColor);
  draw(menuManager, isSelected);
}

String TextMenuItem::getValue() const
{
  return getText();
}

String TextMenuItem::getText() const
{
  return text;
}

bool TextMenuItem::setValue(MenuManager& menuManager, String value)
{
  this->text = value;
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

bool TextMenuItem::updateValue(MenuManager& menuManager, const String& value)
{
  return setValue(menuManager, value);
}

bool TextMenuItem::isValidValue(const String& value)
{
  return true;
}